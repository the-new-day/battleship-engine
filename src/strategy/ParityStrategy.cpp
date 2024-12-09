#include "ParityStrategy.hpp"

#include <algorithm>

namespace Battleship {

ParityStrategy::ParityStrategy(
    uint64_t field_width, 
    uint64_t field_height, 
    const std::map<uint8_t, uint64_t>& ship_types)
    : HuntingStrategy(field_width, field_height, ship_types) {}

void ParityStrategy::MakeNextStrategicShot() {
    if (last_shot_result_ == ShotResult::kKill) {
        UpdateSafeZone();
    }

    FieldPoint next_shot = ChooseNextStrategicShot(last_strategic_shot_);

    while (enemy_field_->IsOneAt(next_shot.x, next_shot.y)) {
        if (last_strategic_shot_ == next_shot) {
            return;
        }

        next_shot = ChooseNextStrategicShot(next_shot);
    }

    last_strategic_shot_ = next_shot;
}

FieldPoint ParityStrategy::ChooseNextStrategicShot(FieldPoint last_shot) {
    FieldPoint next_shot = last_shot;

    if (next_shot.x + 2 >= field_width_) {
        if (next_shot.y < field_height_ - 1) {
            ++next_shot.y;
            if (was_parity_changed_) {
                next_shot.x = next_shot.y % 2 + 1;
            } else {
                next_shot.x = next_shot.y % 2;
            }
        } else {
            next_shot.y = 0;
            next_shot.x = (field_width_ == 1) ? 0 : 1;
            was_parity_changed_ = true;
        }
    } else {
        next_shot.x += 2;
    }

    return next_shot;
}

void ParityStrategy::UpdateSafeZone() {
    bool is_horizontal = (target_cells_.size() > 1 && target_cells_[0].y == target_cells_[1].y);

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

void ParityStrategy::StartGame() {
    last_shot_point_ = {0, 0};
    last_strategic_shot_ = {0, 0};
    SetEnemyField();
}

} // namespace Battleship
