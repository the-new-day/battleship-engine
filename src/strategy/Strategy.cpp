#include "Strategy.hpp"
#include "field/Field.hpp"
#include "field/CompressedField.hpp"

#include <iostream>
#include <algorithm>
#include <random>

namespace Battleship {

Strategy::Strategy(uint64_t field_width, 
                   uint64_t field_height,
                   const std::map<ShipType, uint64_t>& ship_types)
                   : enemy_ships_count_(ship_types)
                   , field_width_(field_width)
                   , field_height_(field_height)
                   , enemy_field_(CompressedField(field_width, field_height)) {
}

FieldPoint Strategy::MakeNextShot() {
    if (!is_game_started_) {
        StartGame();
        return last_shot_point_;
    }

    FieldPoint point = GetNextShot();
    last_shot_point_ = point;
    return last_shot_point_;
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

uint64_t Strategy::GetFieldWidth() const {
    return field_width_;
}

uint64_t Strategy::GetFieldHeight() const {
    return field_height_;
}

} // namespace Battleship
