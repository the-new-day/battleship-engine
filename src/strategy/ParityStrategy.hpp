#pragma once

#include "Strategy.hpp"

namespace Battleship {

class ParityStrategy : public Strategy {
public:
    ParityStrategy(uint64_t field_width,
                   uint64_t field_height, 
                   const std::map<ShipType, uint64_t>& ship_types);

    FieldPoint GetNextShot();

private:
    void StartGame();

    void MakeNextStrategicShot();
    void MakeNextHuntingShot();
};

} // namespace Battleship

