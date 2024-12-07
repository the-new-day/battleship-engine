#include "ProbabilityStrategy.hpp"

#include <iostream>

namespace Battleship {

ProbabilityStrategy::ProbabilityStrategy(
    uint64_t field_width, 
    uint64_t field_height,
    const std::map<uint8_t, uint64_t>& ship_types,
    Field* enemy_field)
    : HuntingStrategy(field_width, field_height, ship_types, enemy_field) {
    probability_map_.resize(field_height);
    for (auto& row : probability_map_) {
        row.resize(field_width_);
    }
    
    CalculateFullMap();

    // for (uint64_t y = 0; y < field_height_; ++y) {
    //     for (uint64_t x = 0; x < field_width_; ++x) {
    //         std::cout << probability_map_[y][x] << ' ';
    //     }

    //     std::cout << '\n';
    // }
}

void ProbabilityStrategy::MakeNextStrategicShot() {
    CalculateFullMap();
    last_strategic_shot_ = GetMaxProbabilityPoint();
}

void ProbabilityStrategy::CalculateFullMap() {
    for (auto& row : probability_map_) {
        std::fill(row.begin(), row.end(), 0);
    }

    for (const auto& [size, amount] : enemy_ships_count_) {
        if (amount > 0) {
            CalculateFullMapForShip(size);
        }
    }
}

FieldPoint ProbabilityStrategy::GetMaxProbabilityPoint() const {
    FieldPoint max_point;
    uint64_t max_probability = probability_map_[0][0];

    for (uint64_t y = 0; y < field_height_; ++y) {
        for (uint64_t x = 0; x < field_width_; ++x) {
            if (probability_map_[y][x] > max_probability) {
                max_point = {x, y};
                max_probability = probability_map_[y][x];
            }
        }
    }

    return max_point;
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

void ProbabilityStrategy::RecalculateMapForShip(uint64_t x, uint64_t y, uint8_t ship_size) {

}

void ProbabilityStrategy::StartGame() {
    is_game_started_ = true;
    last_strategic_shot_ = last_shot_point_ = GetMaxProbabilityPoint();
    
}

} // namespace Battleship

