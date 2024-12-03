#include "Battleship.hpp"
#include "field/MatrixField.hpp"
#include "field/CompressedDenseField.hpp"
#include "field/CompressedField.hpp"
#include "field/MappedField.hpp"

#include <iostream>

namespace Battleship {

// Max area of a field that can be stored in a simple binary matrix (30 MB)
const uint64_t kMaxMatrixFieldArea = 251'658'240;

Battleship::Battleship() {
    ship_types_[ShipType::kOne] = 0;
    ship_types_[ShipType::kTwo] = 0;
    ship_types_[ShipType::kThree] = 0;
    ship_types_[ShipType::kFour] = 0;
}

Battleship::~Battleship() {
    RefreshGame();
}

bool Battleship::IsPossibleToStart() const {
    // TODO: check if possible to place ships
    return game_mode_.has_value() && status_ == BattleshipStatus::kConfigurationDone;
}

bool Battleship::HasError() const {
    return status_ != BattleshipStatus::kConfigurationDone
        && status_ != BattleshipStatus::kGameRunning;
}

BattleshipStatus Battleship::GetStatus() const {
    return status_;
}

void Battleship::HandleErrors() {
    if (!game_mode_.has_value() 
    || !field_width_.has_value() 
    || !field_height_.has_value() 
    || (ship_types_.at(ShipType::kOne) == 0
    && ship_types_.at(ShipType::kTwo) == 0
    && ship_types_.at(ShipType::kThree) == 0
    && ship_types_.at(ShipType::kFour) == 0)) {
        status_ = BattleshipStatus::kConfigurationNotSet;
        return;
    }

    status_ = is_game_running_ ? BattleshipStatus::kGameRunning : BattleshipStatus::kConfigurationDone;
}

bool Battleship::IsLose() const {
    return is_game_finished_ && !strategy_->HasAliveShips();
}

bool Battleship::IsWin() const {
    return is_game_finished_ && !EnemyHasShips();
}

void Battleship::CreateField() {
    if (static_cast<double>(field_width_.value()) / kMaxMatrixFieldArea * field_height_.value() < 1) {
        field_ = new MartixField(field_width_.value(), field_height_.value());
        enemy_field_ = new MartixField(field_width_.value(), field_height_.value());
        return;
    }

    double density = (static_cast<double>(ship_types_.at(ShipType::kOne)) / field_width_.value())
                   + (static_cast<double>(ship_types_.at(ShipType::kTwo)) / field_width_.value())
                   + (static_cast<double>(ship_types_.at(ShipType::kThree)) / field_width_.value())
                   + (static_cast<double>(ship_types_.at(ShipType::kFour)) / field_width_.value());

    density /= field_height_.value();

    if (density < 0.25) {
        field_ = new MappedField(field_width_.value(), field_height_.value());
    } else if (density < 0.75) {
        field_ = new CompressedField(field_width_.value(), field_height_.value());
    } else {
        field_ = new CompressedDenseField(field_width_.value(), field_height_.value());
    }

    enemy_field_ = new CompressedField(field_width_.value(), field_height_.value());
}

void Battleship::InitStrategy() {
    ordered_strategy_ = new OrderedStrategy(field_, enemy_field_, ship_types_);
    custom_strategy_ = new CustomStrategy(field_, enemy_field_, ship_types_);

    if (strategy_type_ == StrategyType::kOrdered) {
        strategy_ = ordered_strategy_;
        return;
    }

    strategy_ = custom_strategy_;
}

void Battleship::ChangeStrategy() {
    if (strategy_type_ == StrategyType::kOrdered) {
        strategy_ = ordered_strategy_;
    } else {
        strategy_ = custom_strategy_;
    }

    strategy_->SetLastShotCoords(last_shot_point_);
    strategy_->SetLastShotResult(last_shot_result_);
}

void Battleship::DecreaseEnemyShipsAmount() {
    for (size_t i = 0; i < kShipTypesAmount; ++i) {
        if (enemy_ships_count_[0] > 0) {
            --enemy_ships_count_[0];
            return;
        }
    }
}

bool Battleship::EnemyHasShips() const {
    for (size_t i = 0; i < kShipTypesAmount; ++i) {
        if (enemy_ships_count_[0] > 0) {
            return true;
        }
    }

    return false;
}

void Battleship::SetStrategy(StrategyType strategy_type) {
    strategy_type_ = strategy_type;
    HandleErrors();

    if (is_game_running_) {
        ChangeStrategy();
    }
}

std::optional<FieldPoint> Battleship::MakeNextShot() {
    if (strategy_ == nullptr) {
        return std::nullopt;
    }

    last_shot_point_ = strategy_->MakeNextShot();
    return last_shot_point_;
}

std::optional<ShotResult> Battleship::RecieveShot(uint64_t x, uint64_t y) {
    if (strategy_ == nullptr) {
        return std::nullopt;
    }

    last_shot_result_ = strategy_->RecieveShot(x, y);
    return last_shot_result_;
}

void Battleship::SetLastShotResult(ShotResult result) {
    if (strategy_ != nullptr) {
        strategy_->SetLastShotResult(result);
    }

    last_shot_result_ = result;

    if (result == ShotResult::kKill) {
        DecreaseEnemyShipsAmount();
    }
}

bool Battleship::IsFinished() const {
    return is_game_finished_;
}

bool Battleship::SetFieldHeight(uint64_t height) {
    if (!game_mode_.has_value() || game_mode_.value() == GameMode::kMaster) {
        return false;
    }
    
    if (height == 0) {
        status_ = BattleshipStatus::kWrongParameter;
        return false;
    }

    if (is_game_running_) {
        return false;
    }

    field_height_ = height;
    HandleErrors();
    return true;
}

bool Battleship::SetFieldWidth(uint64_t width) {
    if (!game_mode_.has_value() || game_mode_.value() == GameMode::kMaster) {
        return false;
    }

    if (width == 0) {
        status_ = BattleshipStatus::kWrongParameter;
        return false;
    }

    if (is_game_running_) {
        return false;
    }

    field_width_ = width;
    HandleErrors();
    return true;
}

bool Battleship::SetGameMode(GameMode mode) {
    if (game_mode_.has_value()) {
        return false;
    }

    game_mode_ = mode;
    HandleErrors();
    
    if (mode == GameMode::kMaster) {
        SetMasterConfig();
    }

    return true;
}

bool Battleship::Start() {
    if (!IsPossibleToStart() || is_game_running_) {
        return false;
    }

    if (game_mode_ == GameMode::kMaster) {
        SetMasterConfig();
    }
    
    RefreshGame();
    CreateField();
    InitStrategy();

    enemy_ships_count_[0] = ship_types_.at(ShipType::kOne);
    enemy_ships_count_[1] = ship_types_.at(ShipType::kTwo);
    enemy_ships_count_[2] = ship_types_.at(ShipType::kThree);
    enemy_ships_count_[3] = ship_types_.at(ShipType::kFour);

    is_game_running_ = true;
    is_game_finished_ = false;
    status_ = BattleshipStatus::kGameRunning;
    return true;
}

bool Battleship::Stop() {
    if (!is_game_running_) {
        return false;
    }

    is_game_running_ = false;
    is_game_finished_ = true;
    status_ = BattleshipStatus::kConfigurationDone;
    return true;
}

std::optional<uint64_t> Battleship::GetWidth() const {
    return field_width_;
}

std::optional<uint64_t> Battleship::GetHeight() const {
    return field_height_;
}

bool Battleship::SetShipsCount(ShipType type, uint64_t amount) {
    if (is_game_running_ || (game_mode_.has_value() && game_mode_.value() == GameMode::kMaster)) {
        return false;
    }

    ship_types_[type] = amount;
    HandleErrors();
    return true;
}

uint64_t Battleship::GetShipsCount(ShipType type) const {
    if (!ship_types_.contains(type)) {
        return 0;
    }

    return ship_types_.at(type);
}

void Battleship::RefreshGame() {
    delete field_;
    delete enemy_field_;
    delete ordered_strategy_;
    delete custom_strategy_;
}

void Battleship::SetMasterConfig() {
    field_width_ = kMasterModeFieldWidth;
    field_height_ = kMasterModeFieldHeight;
    ship_types_ = kMasterModeShipsCount;
    HandleErrors();
}

} // namespace Battleship
