#include "ParityStrategy.hpp"

#include <algorithm>

#include <iostream>

namespace Battleship {

ParityStrategy::ParityStrategy(
    uint64_t field_width, 
    uint64_t field_height, 
    const std::map<uint8_t, uint64_t>& ship_types,
    Field* enemy_field)
    : Strategy(field_width, field_height, ship_types, enemy_field) {
    target_cells_.reserve(kShipMaxLength);
    potential_targets_.reserve(4);
}

FieldPoint Battleship::ParityStrategy::GetNextShot() {
    if (last_shot_result_ == ShotResult::kHit) {
        target_cells_.push_back(last_shot_point_);
        MakeNextHuntingShot();
    } else {
        target_cells_.clear();
        MakeNextStrategicShot();
        last_shot_point_ = last_strategic_shot_;
    }

    return last_shot_point_;
}

void ParityStrategy::MakeNextStrategicShot() {
    FieldPoint next_shot = ChooseNextStrategicShot();

    while (enemy_field_->IsOneAt(next_shot.x, next_shot.y)) {
        if (last_strategic_shot_ == next_shot) {
            return;
        }

        next_shot = ChooseNextStrategicShot();
    }

    last_strategic_shot_ = next_shot;
}

FieldPoint ParityStrategy::ChooseNextStrategicShot() const {
    FieldPoint next_shot = last_strategic_shot_;

    if (next_shot.x + 2 >= field_width_) {
        if (next_shot.y < field_height_ - 1) {
            ++next_shot.y;
            next_shot.x = next_shot.y % 2;
        } else {
            next_shot.y = 0;
            next_shot.x = (field_width_ == 1) ? 0 : 1;
        }
    } else {
        next_shot.x += 2;
    }

    return next_shot;
}

void ParityStrategy::MakeNextHuntingShot() {
    if (target_cells_.size() == 1) {
        if (last_shot_point_.x > 0) {
            potential_targets_.push_back({last_shot_point_.x - 1, last_shot_point_.y});
        }

        if (last_shot_point_.y > 0) {
            potential_targets_.push_back({last_shot_point_.x, last_shot_point_.y - 1});
        }

        if (last_shot_point_.x < field_width_ - 1) {
            potential_targets_.push_back({last_shot_point_.x + 1, last_shot_point_.y});
        }

        if (last_shot_point_.y < field_height_ - 1) {
            potential_targets_.push_back({last_shot_point_.x, last_shot_point_.y + 1});
        }
    } else {
        potential_targets_.clear();

        if (target_cells_[0].y == target_cells_[1].y) {
            auto cmp = [](const FieldPoint& p1, const FieldPoint& p2) { return p1.x < p2.x; };

            FieldPoint leftmost_cell = *std::min_element(target_cells_.begin(), target_cells_.end(), cmp);
            FieldPoint rightmost_cell = *std::max_element(target_cells_.begin(), target_cells_.end(), cmp);

            if (leftmost_cell.x > 0) {
                potential_targets_.push_back({leftmost_cell.x - 1, leftmost_cell.y});
            }

            if (rightmost_cell.x < field_width_ - 1) {
                potential_targets_.push_back({rightmost_cell.x + 1, rightmost_cell.y});
            }
        } else {
            auto cmp = [](const FieldPoint& p1, const FieldPoint& p2) { return p1.y < p2.y; };

            FieldPoint top_cell = *std::max_element(target_cells_.begin(), target_cells_.end(), cmp);
            FieldPoint bottom_cell = *std::min_element(target_cells_.begin(), target_cells_.end(), cmp);

            if (top_cell.y > 0) {
                potential_targets_.push_back({top_cell.x, top_cell.y - 1});
            }

            if (bottom_cell.x < field_height_ - 1) {
                potential_targets_.push_back({bottom_cell.x, bottom_cell.y + 1});
            }
        }
    }

    last_shot_point_ = potential_targets_.back();
    potential_targets_.pop_back();
}

void ParityStrategy::StartGame() {
    is_game_started_ = true;
    last_shot_point_ = {0, 0};
    last_strategic_shot_ = {0, 0};
}

} // namespace Battleship
