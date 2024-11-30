#include "OrderedStrategy.hpp"

#include <iostream>

namespace Battleship {

FieldPoint OrderedStrategy::MakeNextShot() {
    if (!is_game_started_) {
        StartGame();
        return last_shot_point_;
    }

    FieldPoint point = GetNextShot();
    last_shot_point_ = point;
    return last_shot_point_;
}

void OrderedStrategy::StartGame() {
    is_game_started_ = true;
    last_shot_point_ = FieldPoint{0, 0};
}

FieldPoint OrderedStrategy::GetNextShot() const {
    FieldPoint point = last_shot_point_;
    if (last_shot_point_.x != field_width_ - 1) {
        point.x = last_shot_point_.x + 1;
        return point;
    }

    point.x = 0;
    if (last_shot_point_.y == field_height_ - 1) {
        point.y = 0;
    } else {
        point.y = last_shot_point_.y + 1;
    }

    return point;
}

} // namespace Battleship
