#include "OrderedStrategy.hpp"

namespace Battleship {

FieldPoint OrderedStrategy::MakeNextShot() {
    FieldPoint point = GetNextShot();
    last_shot_point_ = point;
    return last_shot_point_;
}

FieldPoint OrderedStrategy::GetNextShot() const {
    FieldPoint point;
    if (last_shot_point_.x == field_width_ - 1) {
        point.x = 0;
    } else {
        point.x = last_shot_point_.x + 1;
    }

    if (last_shot_point_.y == field_height_ - 1) {
        point.y = 0;
    } else {
        point.y = last_shot_point_.y + 1;
    }

    return point;
}

} // namespace Battleship
