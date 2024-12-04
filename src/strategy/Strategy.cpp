#include "Strategy.hpp"

#include <iostream>
#include <algorithm>

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

FieldPoint Strategy::MakeNextShot() {
    if (!is_game_started_) {
        StartGame();
        return last_shot_point_;
    }

    FieldPoint point = GetNextShot();
    last_shot_point_ = point;
    return last_shot_point_;
}

ShotResult Strategy::RecieveShot(uint64_t x, uint64_t y) {
    if (!is_game_started_) {
        StartGame();
    }

    if (!field_->HasShip(x, y)) {
        return ShotResult::kMiss;
    }

    ShotResult result = ShotResult::kHit;

    std::vector<FieldPoint> ship_cells;
    ship_cells.reserve(kShipMaxLength);

    FindShipCells(x, y, ship_cells);
    
    if (IsHitFatal(x, y, ship_cells)) {
        for (const FieldPoint& cell : ship_cells) {
            hit_points_.erase(cell);
        }

        DecreaseShipsAmount();
        result = ShotResult::kKill;
    } else {
        hit_points_.insert({x, y});
    }

    field_->RemoveShip(x, y);
    return result;
}

bool Strategy::IsHitFatal(uint64_t x, uint64_t y, const std::vector<FieldPoint>& ship_cells) const {
    if (ship_cells.empty()) {
        return false;
    }

    for (const FieldPoint& cell : ship_cells) {
        if ((cell.x != x || cell.y != y) && field_->HasShip(cell.x, cell.y)) {
            return false;
        }
    }

    return true;
}

bool Strategy::IsShipAt(uint64_t x, uint64_t y) const {
    return hit_points_.contains({x, y}) || field_->HasShip(x, y);
}

void Strategy::FindShipCells(uint64_t x, uint64_t y, std::vector<FieldPoint>& cells) const {
    if (!IsShipAt(x, y) || std::count(cells.begin(), cells.end(), FieldPoint{x, y}) != 0) {
        return;
    }

    cells.emplace_back(x, y);

    if (x < field_width_) FindShipCells(x + 1, y, cells);
    if (y < field_height_) FindShipCells(x, y + 1, cells);
    if (x > 0) FindShipCells(x - 1, y, cells);
    if (y > 0) FindShipCells(x, y - 1, cells);
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

const std::map<ShipType, uint64_t>& Strategy::GetShipTypes() const {
    return ship_types_;
}

uint64_t Strategy::GetFieldWidth() const {
    return field_width_;
}

uint64_t Strategy::GetFieldHeight() const {
    return field_height_;
}

bool Strategy::HasAliveShips() const {
    for (size_t i = 0; i < kShipTypesAmount; ++i) {
        if (ships_count_[i] > 0) {
            return true;
        }
    }

    return false;
}

void Strategy::DecreaseShipsAmount() {
    for (size_t i = 0; i < kShipTypesAmount; ++i) {
        if (ships_count_[i] > 0) {
            --ships_count_[i];
            return;
        }
    }
}

} // namespace Battleship
