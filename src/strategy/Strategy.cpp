#include "Strategy.hpp"

namespace Battleship {

Strategy::Strategy(
    uint64_t field_width, 
    uint64_t field_height,
    const std::map<uint8_t, uint64_t>& ship_types)
    : field_width_(field_width)
    , field_height_(field_height) {
    for (uint8_t i = 0; i < kMaxShipLength; ++i) {
        enemy_ships_count_[i] = ship_types.at(i + 1);
    }
}

Strategy::~Strategy() {
    delete enemy_field_;
}

FieldPoint Strategy::MakeNextShot() {
    if (!is_game_started_) {
        StartGame();
        is_game_started_ = true;
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

void Strategy::SetLastShotResult(ShotResult result) {
    last_shot_result_ = result;

    if (result == ShotResult::kKill) {
        for (auto& amount : enemy_ships_count_) {
            if (amount > 0) {
                --amount;
                break;
            }
        }
    }
}

bool Strategy::EnemyHasShips() const {
    for (const auto& amount : enemy_ships_count_) {
        if (amount > 0) {
            return true;
        }
    }

    return false;
}

void Strategy::SetEnemyField() {
    if (static_cast<double>(field_width_) / kMaxMatrixFieldArea * field_height_ <= 1) {
        enemy_field_ = new MartixField(field_width_, field_height_);
        return;
    }

    double density = 0;

    for (uint8_t i = 0; i < kMaxShipLength; ++i) {
        density += static_cast<double>(enemy_ships_count_[i]) / field_width_;
    }

    density /= field_height_;

    if (density < 0.1) {
        enemy_field_ = new MappedField(field_width_, field_height_);
    } else if (density < 0.75) {
        enemy_field_ = new CompressedField(field_width_, field_height_);
    } else {
        enemy_field_ = new CompressedDenseField(field_width_, field_height_);
    }
}

} // namespace Battleship
