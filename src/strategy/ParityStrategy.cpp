#include "ParityStrategy.hpp"

namespace Battleship {

ParityStrategy::ParityStrategy(
    uint64_t field_width, 
    uint64_t field_height, 
    const std::map<ShipType, uint64_t>& ship_types)
    : Strategy(field_width, field_height, ship_types) {
}

FieldPoint Battleship::ParityStrategy::GetNextShot() {
    return FieldPoint();
}

void ParityStrategy::StartGame() {
    is_game_started_ = true;
    last_shot_point_ = {0, 0};

    // last_strategy_shot_ = {0, 0};
    // current_square_ = {0, 0};
    // square_size_ = 4;
}

} // namespace Battleship
