#include "Battleship.hpp"
#include "field/MatrixField.hpp"
#include "field/CompressedDenseField.hpp"
#include "field/CompressedField.hpp"
#include "field/MappedField.hpp"

#include <iostream>

namespace Battleship {

// Max area of a field that can be stored in a simple binary matrix (30 MB)
const uint64_t kMaxMatrixFieldArea = 251'658'240;

const uint64_t kMaxProbabilityStrategySize = 1'000'000;

Battleship::Battleship() {
    for (uint8_t i = 1; i <= kMaxShipLength; ++i) {
        ships_count_[i] = 0;
    }
}

Battleship::~Battleship() {
    RefreshGame();
}

bool Battleship::Dump(const std::string& path) const {
    if (ship_handler_ == nullptr) {
        return false;
    }

    return ship_handler_->Dump(path);
}

bool Battleship::LoadFrom(const std::string& path) {
    if (is_game_running_) {
        return false;
    }

    RefreshGame();
    game_mode_ = GameMode::kSlave;
    ship_handler_ = new ShipHandler();

    if (!ship_handler_->LoadFromFile(path)) {
        status_ == BattleshipStatus::kWrongParameter;
        return false;
    }

    field_width_ = ship_handler_->GetFieldWidth();
    field_height_ = ship_handler_->GetFieldHeight();
    status_ = BattleshipStatus::kConfigurationDone;

    return true;
}

bool Battleship::IsPossibleToStart() const {
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
    || !field_height_.has_value()) {
        status_ = BattleshipStatus::kConfigurationNotSet;
        return;
    }

    bool have_ship = false;

    for (uint8_t i = 1; i <= kMaxShipLength; ++i) {
        if (ships_count_[i] > 0) {
            have_ship = true;
        }
    }

    if (!have_ship) {
        status_ = BattleshipStatus::kConfigurationNotSet;
        return;
    }

    status_ = is_game_running_ ? BattleshipStatus::kGameRunning : BattleshipStatus::kConfigurationDone;
}

bool Battleship::IsLose() const {
    return ship_handler_ != nullptr && !ship_handler_->HasAliveShips();
}

bool Battleship::IsWin() const {
    return strategy_ != nullptr && !strategy_->EnemyHasShips();
}

void Battleship::InitStrategy() {
    ordered_strategy_ = new OrderedStrategy(field_width_.value(), field_height_.value(), ships_count_, enemy_field_);
    parity_strategy_ = new ParityStrategy(field_width_.value(), field_height_.value(), ships_count_, enemy_field_);
    probability_strategy_ = new ProbabilityStrategy(field_width_.value(), field_height_.value(), ships_count_, enemy_field_);

    ChangeStrategy();
}

void Battleship::ChangeStrategy() {
    if (strategy_type_ == StrategyType::kOrdered) {
        strategy_ = ordered_strategy_;
    } else if (strategy_type_ == StrategyType::kParity) {
        strategy_ = parity_strategy_;
    } else if (strategy_type_ == StrategyType::kProbability) {
        strategy_ = probability_strategy_;
    } else {
        // if (static_cast<double>(field_width_.value()) / kMaxProbabilityStrategySize * field_height_.value() < 1) {
        //     strategy_ = probability_strategy_;
        //     return;
        // }

        strategy_ = parity_strategy_;
    }
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

    return strategy_->MakeNextShot();
}

std::optional<ShotResult> Battleship::RecieveShot(uint64_t x, uint64_t y) {
    if (strategy_ == nullptr) {
        return std::nullopt;
    }

    return ship_handler_->ProcessShot(x, y);
}

void Battleship::SetLastShotResult(ShotResult result) {
    if (strategy_ != nullptr) {
        strategy_->SetLastShotResult(result);
    }
}

bool Battleship::IsFinished() const {
    return is_game_finished_ 
    || ship_handler_ != nullptr && !ship_handler_->HasAliveShips() 
    || strategy_ != nullptr && !strategy_->EnemyHasShips();
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
    SetEnemyField();
    InitStrategy();
    SetShipHandler();

    if (!ship_handler_->PlaceShips()) {
        status_ = BattleshipStatus::kWrongParameter;
        return false;
    }

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

bool Battleship::SetShipsCount(uint8_t ship_size, uint64_t amount) {
    if (is_game_running_ || (game_mode_.has_value() && game_mode_.value() == GameMode::kMaster)) {
        return false;
    }

    ships_count_[ship_size] = amount;
    HandleErrors();
    return true;
}

uint64_t Battleship::GetShipsCount(uint8_t ship_size) const {
    if (ship_size == 0 || ship_size > kMaxShipLength) {
        return 0;
    }

    return ships_count_.at(ship_size);
}

void Battleship::RefreshGame() {
    delete ordered_strategy_;
    delete parity_strategy_;
    delete probability_strategy_;
    delete ship_handler_;
    delete enemy_field_;

    ordered_strategy_ = nullptr;
    probability_strategy_ = nullptr;
    parity_strategy_ = nullptr;
    ship_handler_ = nullptr;
    enemy_field_ = nullptr;
}

void Battleship::SetMasterConfig() {
    field_width_ = kMasterModeFieldWidth;
    field_height_ = kMasterModeFieldHeight;
    ships_count_ = kMasterModeShipsCount;
    HandleErrors();
}

void Battleship::SetShipHandler() {
    ship_handler_ = new ShipHandler(field_width_.value(), field_height_.value(), ships_count_);
}

void Battleship::SetEnemyField() {
    enemy_field_ = new CompressedField(field_width_.value(), field_height_.value());
}

} // namespace Battleship
