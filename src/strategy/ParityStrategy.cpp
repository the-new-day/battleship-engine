#include "ParityStrategy.hpp"

#include <algorithm>

namespace Battleship {

void ParityStrategy::MakeNextStrategicShot() {
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

void ParityStrategy::StartGame() {
    is_game_started_ = true;
    last_shot_point_ = {0, 0};
    last_strategic_shot_ = {0, 0};
}

} // namespace Battleship
