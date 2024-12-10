#include "ProbabilityStrategy.hpp"

namespace Battleship {

ProbabilityStrategy::ProbabilityStrategy(
    uint64_t field_width, 
    uint64_t field_height, 
    const std::map<uint8_t, uint64_t>& ship_types)
    : HuntingStrategy(field_width, field_height, ship_types) {
    for (const auto& [size, amount] : ship_types) {
        real_enemy_ships_count_[size - 1] = amount;
    }
}

bool ProbabilityStrategy::IsPossibleToPlaceShip(uint64_t x, uint64_t y, uint8_t ship_size, bool is_horizontal) const {
    if (is_horizontal && x + ship_size > field_width_
    || !is_horizontal && y + ship_size > field_height_) {
        return false;
    }

    if (is_horizontal) {
        for (uint64_t i = (x == 0 ? 0 : x - 1); i <= x + ship_size; ++i) {
            if (enemy_field_->IsOneAt(i, y + 1) || enemy_field_->IsOneAt(i, y) || enemy_field_->IsOneAt(i, y - 1)) {
                return false;
            }
        }

        return true;
    }

    for (uint64_t i = (y == 0 ? 0 : y - 1); i <= y + ship_size; ++i) {
        if (enemy_field_->IsOneAt(x + 1, i) || enemy_field_->IsOneAt(x, i) || enemy_field_->IsOneAt(x - 1, i)) {
            return false;
        }
    }

    return true;
}

void ProbabilityStrategy::CalculateFullMap() {
    for (uint8_t i = 0; i < real_enemy_ships_count_.size(); ++i) {
        if (real_enemy_ships_count_[i] > 0) {
            CalculateFullMapForShip(i + 1);
        }
    }
}

void ProbabilityStrategy::CalculateFullMapForShip(uint8_t ship_size) {
    for (uint64_t y = 0; y < field_height_; ++y) {
        for (uint64_t x = 0; x < field_width_; ++x) {
            if (IsPossibleToPlaceShip(x, y, ship_size, true)) {
                for (uint8_t i = 0; i < ship_size; ++i) {
                    probability_map_[y][x + i] += 1;
                }
            }
            
            if (IsPossibleToPlaceShip(x, y, ship_size, false)) {
                for (uint8_t i = 0; i < ship_size; ++i) {
                    probability_map_[y + i][x] += 1;
                }
            }
        }
    }
}

void ProbabilityStrategy::StartGame() {
    SetEnemyField();
    
    probability_map_.resize(field_height_);
    for (auto& row : probability_map_) {
        row.resize(field_width_);
    }

    CalculateFullMap();
    last_shot_point_ = GetMostLikelyPoint();
}

void ProbabilityStrategy::RecalculateMap(uint64_t x, uint64_t y) {
    for (uint8_t i = 0; i < real_enemy_ships_count_.size(); ++i) {
        if (real_enemy_ships_count_[i] > 0) {
            RecalculateMapForShip(x, y, i + 1);
        }
    }
}

void ProbabilityStrategy::RecalculateMapForShip(uint64_t x, uint64_t y, uint8_t ship_size) {
    for (uint64_t i = (x > ship_size ? x - ship_size : 0); i <= x; ++i) {
        if (i < field_width_ - ship_size && !IsPossibleToPlaceShip(i, y, ship_size, true)) {
            for (uint8_t j = 0; j < ship_size; ++j) {
                if (probability_map_[y][i + j] > 0) {
                    probability_map_[y][i + j] -= 1;
                }
            }
        }
    }

    for (uint64_t i = (y > ship_size ? y - ship_size : 0); i <= y; ++i) {
        if (i < field_height_ - ship_size && !IsPossibleToPlaceShip(x, i, ship_size, true)) {
            for (uint8_t j = 0; j < ship_size; ++j) {
                if (probability_map_[i + j][x] > 0) {
                    probability_map_[i + j][x] -= 1;
                }
            }
        }
    }
}

void ProbabilityStrategy::UpdateEnemyField() {
    for (const auto& cell : target_cells_) {
        enemy_field_->SetBit(cell.x, cell.y);

        for (uint64_t y = (cell.y > 0 ? cell.y - 1 : 0 ); 
             y <= (cell.y < field_height_ - 1 ? cell.y + 1 : field_height_ - 1);
             ++y) {
            for (uint64_t x = (cell.x > 0 ? cell.x - 1 : 0 ); 
                 x <= (cell.x < field_width_ - 1 ? cell.x + 1 : field_width_ - 1); 
                 ++x) {
                probability_map_[y][x] = 0;
            }
        }
    }
}

FieldPoint ProbabilityStrategy::GetMostLikelyPoint() const {
    FieldPoint result{0, 0};
    uint64_t max_probability = probability_map_[0][0];

    for (uint64_t y = 0; y < field_height_; ++y) {
        for (uint64_t x = 0; x < field_width_; ++x) {
            if (probability_map_[y][x] > max_probability) {
                max_probability = probability_map_[y][x];
                result = {x, y};
            }
        }
    }
    
    return result;
}

void ProbabilityStrategy::MakeNextStrategicShot() {
    HandleLastShot();
    last_strategic_shot_ = GetMostLikelyPoint();
}

void ProbabilityStrategy::HandleLastShot() {
    if (last_shot_result_ == ShotResult::kKill) {
        UpdateEnemyField();

        for (const auto& cell : target_cells_) {
            RecalculateMap(cell.x, cell.y);
        }
    } else {
        enemy_field_->SetBit(last_shot_point_.x, last_shot_point_.y);
        probability_map_[last_shot_point_.y][last_shot_point_.x] = 0;
        RecalculateMap(last_shot_point_.x, last_shot_point_.y);
    }
}

} // namespace Battleship
