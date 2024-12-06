#pragma once

#include "Strategy.hpp"
#include "field/Field.hpp"

namespace Battleship {

class OrderedStrategy : public Strategy {
public:
    OrderedStrategy(uint64_t field_width, uint64_t field_height, const std::map<uint8_t, uint64_t>& ship_types) 
                    : Strategy(field_width, field_height, ship_types) {};
             
    FieldPoint GetNextShot() override;

private:
    void StartGame() override;
};

} // namespace Battleship
