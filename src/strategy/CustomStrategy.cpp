#include "CustomStrategy.hpp"

#include <iostream>

namespace Battleship {

FieldPoint CustomStrategy::GetNextShot() const {
    return last_shot_point_;
}

void CustomStrategy::StartGame() {
    is_game_started_ = true;
    last_shot_point_ = FieldPoint{0, 0};
}

} // namespace Battleship
