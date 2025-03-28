#include "OrderedStrategy.hpp"

namespace Battleship {

void OrderedStrategy::StartGame() {
    last_shot_point_ = FieldPoint{0, 0};
}

FieldPoint OrderedStrategy::GetNextShot() {
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
