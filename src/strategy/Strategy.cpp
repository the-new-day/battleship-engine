#include "Strategy.hpp"

#include <iostream>

namespace Battleship {

Strategy::Strategy(Field* field, 
                   Field* enemy_field,
                   const std::map<ShipType, uint64_t>& ship_types)
                   : field_(field)
                   , enemy_field_(enemy_field)
                   , ship_types_(ship_types)
                   , field_width_(field->GetWidth())
                   , field_height_(field->GetHeight()) {
    ships_count_[0] = ship_types.at(ShipType::kOne);
    ships_count_[1] = ship_types.at(ShipType::kTwo);
    ships_count_[2] = ship_types.at(ShipType::kThree);
    ships_count_[3] = ship_types.at(ShipType::kFour);
}

ShotResult Strategy::RecieveShot(uint64_t x, uint64_t y) {
    if (!is_game_started_) {
        StartGame();
    }

    if (!field_->HasShip(x, y)) {
        return ShotResult::kMiss;
    }

    ShotResult result = ShotResult::kHit;
    if ((x == field_width_ - 1 || !field_->HasShip(x + 1, y))
    && (y == field_height_ - 1 || !field_->HasShip(x, y + 1))
    && (x == 0 || !field_->HasShip(x - 1, y))
    && (y == 0 || !field_->HasShip(x, y - 1))) {
        DecreaseShipsAmount();
        result = ShotResult::kKill;
    }

    field_->RemoveShip(x, y);
    return result;
}

bool Strategy::HasAliveShips() const {
    for (size_t i = 0; i < kShipTypesAmount; ++i) {
        if (ships_count_[0] > 0) {
            return true;
        }
    }

    return false;
}

const std::map<ShipType, uint64_t>& Strategy::GetShipTypes() const {
    return ship_types_;
}

uint64_t Strategy::GetFieldWidth() const {
    return field_width_;
}

uint64_t Strategy::GetFieldHeight() const {
    return field_height_;
}

void Strategy::DecreaseShipsAmount() {
    for (size_t i = 0; i < kShipTypesAmount; ++i) {
        if (ships_count_[0] > 0) {
            --ships_count_[0];
            return;
        }
    }
}

void Strategy::SetLastShotCoords(uint64_t x, uint64_t y) {
    last_shot_point_.x = x;
    last_shot_point_.y = y;
}

void Strategy::SetLastShotCoords(FieldPoint point) {
    last_shot_point_ = point;
}

void Strategy::SetLastShotResult(ShotResult result) {
    last_shot_result_ = result;
}

ShotResult Strategy::GetLastShotResult() const {
    return last_shot_result_;
}

FieldPoint Strategy::GetLastShotPoint() const {
    return last_shot_point_;
}

} // namespace Battleship
