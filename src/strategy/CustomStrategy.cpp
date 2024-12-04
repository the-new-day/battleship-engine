#include "CustomStrategy.hpp"

#include <iostream>

namespace Battleship {

CustomStrategy::CustomStrategy(Field* field,
                               Field* enemy_field,
                               const std::map<ShipType, uint64_t>& ship_types) 
                               : Strategy(field, enemy_field, ship_types) {
}

FieldPoint CustomStrategy::GetNextShot() const {
    return last_shot_point_;
}

void CustomStrategy::StartGame() {
    is_game_started_ = true;
    last_shot_point_ = FieldPoint{0, 0};
}

} // namespace Battleship
