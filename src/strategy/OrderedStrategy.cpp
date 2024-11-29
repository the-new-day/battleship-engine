#include "OrderedStrategy.hpp"

namespace Battleship {

FieldPoint OrderedStrategy::MakeNextShot() {
    if (last_shot_point_.x == field_width_ - 1) {
        last_shot_point_.x = 0;
    } else {
        ++last_shot_point_.x;
    }

    if (last_shot_point_.y == field_height_ - 1) {
        last_shot_point_.y = 0;
    } else {
        ++last_shot_point_.y;
    }

    return last_shot_point_;
}

} // namespace Battleship
