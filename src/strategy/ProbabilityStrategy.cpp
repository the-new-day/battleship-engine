#include "ProbabilityStrategy.hpp"

namespace Battleship {

ProbabilityStrategy::ProbabilityStrategy(
    uint64_t field_width, 
    uint64_t field_height,
    const std::map<uint8_t, uint64_t>& ship_types,
    Field* enemy_field)
    : Strategy(field_width, field_height, ship_types, enemy_field) {

}

FieldPoint ProbabilityStrategy::GetNextShot()
{
    return FieldPoint();
}

void ProbabilityStrategy::StartGame()
{
}

} // namespace Battleship

