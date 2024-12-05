#pragma once

#include "Strategy.hpp"
#include "field/Field.hpp"

namespace Battleship {

class CustomStrategy : public Strategy {
public:
    CustomStrategy(uint64_t field_width, uint64_t field_height, const std::map<ShipType, uint64_t>& ship_types) 
                    : Strategy(field_width, field_height, ship_types) {};

    FieldPoint GetNextShot() const override;
    
private:
    void StartGame() override;
};

} // namespace Battleship

