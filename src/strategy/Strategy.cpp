#include "Strategy.hpp"

#include <iostream>
#include <algorithm>
#include <random>

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

bool Strategy::PlaceShips() {
    std::random_device rd;
    ship_placement_seed = rd();
    
    std::mt19937 mt(ship_placement_seed);
    std::uniform_int_distribution<> dist_x(0, field_width_ - 1);
    std::uniform_int_distribution<> dist_y(0, field_height_ - 1);
    std::uniform_int_distribution<> dist_orientation(0, 1);

    for (uint8_t ship_size = kShipMaxLength; ship_size > 0; --ship_size) {
        ShipType ship_type = ShipType(ship_size);
        uint64_t amount = ship_types_[ship_type];

        while (amount > 0) {
            uint64_t x;
            uint64_t y;
            bool is_horizontal;

            bool is_placed = false;

            for (uint64_t attempt = 0; 
            !is_placed && static_cast<double>(attempt) / field_width_ < field_height_;
            ++attempt) {
                x = dist_x(mt);
                y = dist_y(mt);
                is_horizontal = (dist_orientation(mt) == 0);

                if (IsPossibleToPlaceShip(x, y, ship_type, is_horizontal)) {
                    PlaceShip(x, y, ship_type, is_horizontal);
                    is_placed = true;
                }
            }

            if (!is_placed) {
                field_->Clear();
                return PlaceShipsLinear();
            }

            --amount;
        }
    }

    for (uint64_t y = 0; y < field_height_; ++y) {
        for (uint64_t x = 0; x < field_width_; ++x) {
            std::cout << field_->HasShip(x, y);
        }

        std::cout << '\n';
    }

    return true;
}

bool Strategy::PlaceShipsLinear() {
    bool is_horizontal = false;

    for (uint8_t ship_size = kShipMaxLength; ship_size > 0; --ship_size) {
        ShipType ship_type = ShipType(ship_size);
        uint64_t amount = ship_types_[ship_type];

        while (amount > 0) {
            bool is_placed = false;

            for (uint64_t i = 0; i < field_width_ && !is_placed; ++i) {
                for (uint64_t j = 0; j < field_height_ && !is_placed; ++j) {
                    if (IsPossibleToPlaceShip(i, j, ship_type, is_horizontal)) {
                        PlaceShip(i, j, ship_type, is_horizontal);
                        is_placed = true;
                    }
                }
            }

            if (!is_placed) {
                return false;
            }

            --amount;
            is_horizontal = !is_horizontal;
        }
    }

    return true;
}

void Strategy::PlaceShip(uint64_t x, uint64_t y, ShipType type, bool is_horizontal) {
    uint8_t ship_size = GetShipSize(type);

    for (uint8_t i = 0; i < ship_size; ++i) {
        uint64_t next_x = x + (is_horizontal ? i : 0);
        uint64_t next_y = y + (is_horizontal ? 0 : i);

        field_->SetShip(next_x, next_y);
    }
}

bool Strategy::IsPossibleToPlaceShip(uint64_t x, uint64_t y, ShipType type, bool is_horizontal) const {
    uint8_t ship_size = GetShipSize(type);

    if (is_horizontal && x + ship_size > field_width_
    || !is_horizontal && y + ship_size > field_height_) {
        return false;
    }

    if (is_horizontal) {
        for (uint64_t i = (x == 0 ? 0 : x - 1); i <= x + ship_size; ++i) {
            if (field_->HasShip(i, y + 1) || field_->HasShip(i, y) || field_->HasShip(i, y - 1)) {
                return false;
            }
        }

        return true;
    }

    for (uint64_t i = (y == 0 ? 0 : y - 1); i <= y + ship_size; ++i) {
        if (field_->HasShip(x + 1, i) || field_->HasShip(x, i) || field_->HasShip(x - 1, i)) {
            return false;
        }
    }

    return true;
}

uint8_t Strategy::GetShipSize(ShipType type) const {
    return static_cast<uint8_t>(type);
}

bool Strategy::IsNeighbourhoodFree(uint64_t x, uint64_t y) const {
    for (uint64_t i = x - 1; i <= x + 1; ++i) {
        for (uint64_t j = y - 1; j <= y + 1; ++j) {
            if (field_->HasShip(x + 1, y)) {
                return false;
            }
        }
    }

    return true;
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
