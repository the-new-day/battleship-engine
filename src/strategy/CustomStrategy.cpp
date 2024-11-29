#include "CustomStrategy.hpp"

namespace Battleship {

FieldPoint CustomStrategy::GetNextShot() const {
    return last_shot_point_;
}

FieldPoint CustomStrategy::MakeNextShot() {
    return last_shot_point_;
}

} // namespace Battleship
