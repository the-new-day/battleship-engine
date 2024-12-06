#include "ParityStrategy.hpp"

#include <algorithm>

namespace Battleship {

ParityStrategy::ParityStrategy(
    uint64_t field_width, 
    uint64_t field_height, 
    const std::map<uint8_t, uint64_t>& ship_types,
    Field* enemy_field)
    : Strategy(field_width, field_height, ship_types, enemy_field) {
    current_target_cells_.reserve(kShipMaxLength);
}

FieldPoint Battleship::ParityStrategy::GetNextShot() {
    UpdateEnemyShips();

    if (last_shot_result_ == ShotResult::kHit || is_target_active_) {
        is_target_active_ = true;
        MakeNextHuntingShot();
    } else if (last_shot_result_ == ShotResult::kMiss && is_target_active_) {
        if (failed_target_shot_.has_value()) {
            is_second_failed_shot_ = true;
        }

        failed_target_shot_ = last_shot_point_;
    } else {
        MakeNextStrategicShot();
        last_shot_point_ = last_strategic_shot_;
    }

    return last_shot_point_;
}

void ParityStrategy::MakeNextStrategicShot() {
    FieldPoint next_shot = last_strategic_shot_;

    while (enemy_field_->IsOneAt(last_strategic_shot_.x, last_strategic_shot_.y)) {
        next_shot.x = (last_strategic_shot_.x + 2) % field_width_;

        if (last_strategic_shot_.x + 2 > field_width_) {
            next_shot.y = (next_shot.y == field_height_ - 1) ? 0 : next_shot.y;
        }

        if (last_strategic_shot_ == next_shot) {
            ++last_strategic_shot_.x;
            return;
        }
    }

    last_strategic_shot_ = next_shot;
}

void ParityStrategy::MakeNextHuntingShot() {
    if (current_target_cells_.size() == 0) {
        return;
    }

    if (current_target_cells_.size() == 1) {
        const FieldPoint& cell = current_target_cells_[0];

        if (enemy_field_->IsOneAt(cell.x - 1, cell.y) && enemy_field_->IsOneAt(cell.x + 1, cell.y)) {
            last_shot_point_ = {cell.x, cell.y + (cell.y == 0 ? 1 : -1)};
        } else if (enemy_field_->IsOneAt(cell.x, cell.y + 1) && enemy_field_->IsOneAt(cell.x, cell.y - 1)) {
            last_shot_point_ = {cell.x + (cell.x == 0 ? 1 : -1), cell.y};
        } else if (enemy_field_->IsOneAt(cell.x - 1, cell.y)) {
            last_shot_point_ = {cell.x + 1, cell.y};
        } else if (enemy_field_->IsOneAt(cell.x + 1, cell.y)) {
            last_shot_point_ = {cell.x - 1, cell.y};
        } else if (enemy_field_->IsOneAt(cell.x, cell.y - 1)) {
            last_shot_point_ = {cell.x, cell.y + 1};
        } else if (enemy_field_->IsOneAt(cell.x, cell.y + 1)) {
            last_shot_point_ = {cell.x, cell.y - 1};
        } else {
            last_shot_point_ = {cell.x + (cell.x == 0 ? 1 : -1), cell.y + (cell.y == 0 ? 1 : -1)};
        }

        return;
    }

    bool is_horizontal = (current_target_cells_[1].y == current_target_cells_[0].y);

    if (is_horizontal) {
        FieldPoint leftmost_cell = current_target_cells_[0];
        FieldPoint rightmost_cell = current_target_cells_[0];
        auto cmp = [](const FieldPoint& p1, const FieldPoint& p2){ return p1.x < p2.x; };

        leftmost_cell = *std::min_element(
            current_target_cells_.begin(),
            current_target_cells_.end(), 
            cmp);

        rightmost_cell = *std::max_element(
            current_target_cells_.begin(),
            current_target_cells_.end(), 
            cmp);

        if (enemy_field_->IsOneAt(leftmost_cell.x - 1, leftmost_cell.y)) {
            ++rightmost_cell.x;
            last_shot_point_ = rightmost_cell;
        } else {
            --leftmost_cell.x;
            last_shot_point_ = leftmost_cell;
        }

        return;
    }

    FieldPoint top_cell = current_target_cells_[0];
    FieldPoint bottom_cell = current_target_cells_[0];

    auto cmp = [](const FieldPoint& p1, const FieldPoint& p2){ return p1.y < p2.y; };

    bottom_cell = *std::min_element(
        current_target_cells_.begin(),
        current_target_cells_.end(), 
        cmp);

    top_cell = *std::max_element(
        current_target_cells_.begin(),
        current_target_cells_.end(), 
        cmp);

    if (enemy_field_->IsOneAt(top_cell.x, top_cell.y + 1)) {
        --bottom_cell.y;
        last_shot_point_ = bottom_cell;
    } else {
        ++top_cell.y;
        last_shot_point_ = top_cell;
    }
}

void ParityStrategy::UpdateEnemyShips() {
    if (last_shot_result_ == ShotResult::kHit) {
        if (is_new_target_) {
            current_target_cells_.clear();
            is_new_target_ = false;
        }

        enemy_field_->SetBit(last_shot_point_.x, last_shot_point_.y);
        current_target_cells_.emplace_back(last_shot_point_.x, last_shot_point_.y);
    } else if (last_shot_result_ == ShotResult::kKill) {
        is_new_target_ = true;
        is_target_active_ = false;
        failed_target_shot_ = std::nullopt;

        if (current_target_cells_.size() == 1) {
            enemy_field_->SetBit(current_target_cells_[0].x, current_target_cells_[0].y);
            --enemy_ships_count_[1];
            return;
        }

        bool is_horizontal = (current_target_cells_[1].y == current_target_cells_[0].y);

        for (size_t i = 0; i < current_target_cells_.size(); ++i) {
            const FieldPoint& cell = current_target_cells_[i];
            enemy_field_->SetBit(cell.x, cell.y);

            if (is_horizontal) {
                enemy_field_->SetBit(cell.x, cell.y + 1);
                if (cell.y > 0) enemy_field_->SetBit(cell.x, cell.y - 1);
            } else {
                enemy_field_->SetBit(cell.x + 1, cell.y);
                if (cell.x > 0) enemy_field_->SetBit(cell.x - 1, cell.y);
            }
        }

        --enemy_ships_count_[current_target_cells_.size()];
    }
}

uint8_t ParityStrategy::GetPointParity(FieldPoint point) {
    return (point.x % kShipMaxLength + point.y % kShipMaxLength) % kShipMaxLength;
}
void ParityStrategy::StartGame() {
    is_game_started_ = true;
    last_shot_point_ = {0, 0};
    last_strategic_shot_ = {0, 0};
}

} // namespace Battleship
