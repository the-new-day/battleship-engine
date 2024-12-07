#include "HuntingStrategy.hpp"

#include <algorithm>

namespace Battleship {
HuntingStrategy::HuntingStrategy(
    uint64_t field_width, 
    uint64_t field_height, 
    const std::map<uint8_t, uint64_t>& ship_types, 
    Field *enemy_field)
    : Strategy(field_width, field_height, ship_types, enemy_field) {
    target_cells_.reserve(kMaxShipLength);
    potential_targets_.reserve(4);
}

void HuntingStrategy::MakeNextHuntingShot() {
    if (target_cells_.size() == 1) {
        if (last_successful_hunt_shot_.x > 0
        && !enemy_field_->IsOneAt(last_successful_hunt_shot_.x - 1, last_successful_hunt_shot_.y)) {
            potential_targets_.push_back({last_successful_hunt_shot_.x - 1, last_successful_hunt_shot_.y});
        }

        if (last_successful_hunt_shot_.x < field_width_ - 1 
        && !enemy_field_->IsOneAt(last_successful_hunt_shot_.x + 1, last_successful_hunt_shot_.y)) {
            potential_targets_.push_back({last_successful_hunt_shot_.x + 1, last_successful_hunt_shot_.y});
        }

        if (last_successful_hunt_shot_.y > 0
        && !enemy_field_->IsOneAt(last_successful_hunt_shot_.x, last_successful_hunt_shot_.y - 1)) {
            potential_targets_.push_back({last_successful_hunt_shot_.x, last_successful_hunt_shot_.y - 1});
        }

        if (last_successful_hunt_shot_.y < field_height_ - 1 
        && !enemy_field_->IsOneAt(last_successful_hunt_shot_.x, last_successful_hunt_shot_.y + 1)) {
            potential_targets_.push_back({last_successful_hunt_shot_.x, last_successful_hunt_shot_.y + 1});
        }
    } else {
        if (target_cells_.size() == 2) {
            potential_targets_.clear();
        }

        if (target_cells_[0].y == target_cells_[1].y) {
            auto cmp = [](const FieldPoint& p1, const FieldPoint& p2) { return p1.x < p2.x; };

            FieldPoint leftmost_cell = *std::min_element(target_cells_.begin(), target_cells_.end(), cmp);
            FieldPoint rightmost_cell = *std::max_element(target_cells_.begin(), target_cells_.end(), cmp);

            if (!enemy_field_->IsOneAt(leftmost_cell.x - 1, leftmost_cell.y)) {
                potential_targets_.push_back({leftmost_cell.x - 1, leftmost_cell.y});
            }

            if (rightmost_cell.y < field_width_ - 1 && !enemy_field_->IsOneAt(rightmost_cell.x + 1, rightmost_cell.y)) {
                potential_targets_.push_back({rightmost_cell.x + 1, rightmost_cell.y});
            }
        } else {
            auto cmp = [](const FieldPoint& p1, const FieldPoint& p2) { return p1.y < p2.y; };

            FieldPoint top_cell = *std::min_element(target_cells_.begin(), target_cells_.end(), cmp);
            FieldPoint bottom_cell = *std::max_element(target_cells_.begin(), target_cells_.end(), cmp);

            if (!enemy_field_->IsOneAt(top_cell.x, top_cell.y - 1)) {
                potential_targets_.push_back({top_cell.x, top_cell.y - 1});
            }

            if (bottom_cell.y < field_height_ - 1 && !enemy_field_->IsOneAt(bottom_cell.x, bottom_cell.y + 1)) {
                potential_targets_.push_back({bottom_cell.x, bottom_cell.y + 1});
            }
        }
    }

    last_shot_point_ = potential_targets_.back();
    potential_targets_.pop_back();
}

void HuntingStrategy::UpdateSafeZone() {
    if (target_cells_.size() == 1) {
        const FieldPoint& cell = target_cells_[0];
        for (uint64_t x = cell.x - (cell.x == 0 ? 0 : 1); x <= cell.x + (cell.x == field_width_ - 1 ? 0 : 1); ++x) {
            for (uint64_t y = cell.y - (cell.y == 0 ? 0 : 1); y <= cell.y + (cell.y == field_height_ - 1 ? 0 : 1); ++y) {
                enemy_field_->SetBit(x, y);
            }
        }

        return;
    }

    bool is_horizontal = target_cells_[0].y == target_cells_[1].y;

    std::sort(
        target_cells_.begin(), 
        target_cells_.end(),
        [is_horizontal](const FieldPoint& p1, const FieldPoint& p2) { 
            return is_horizontal && p1.x < p2.x || !is_horizontal && p1.y < p2.y;
        });

    for (uint64_t x = (target_cells_[0].x == 0 ? 0 : target_cells_[0].x - 1); 
         x <= (target_cells_.back().x == field_width_ - 1 ? field_width_ - 1 : target_cells_.back().x + 1);
         ++x) {
        for (uint64_t y = (target_cells_[0].y == 0 ? 0 : target_cells_[0].y - 1); 
             y <= (target_cells_.back().y == field_height_ - 1 ? field_height_ - 1 : target_cells_.back().y + 1);
             ++y) {
            enemy_field_->SetBit(x, y);
        }
    }
}

} // namespace Battleshi[]
