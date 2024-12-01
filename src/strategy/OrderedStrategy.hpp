#pragma once

#include "Strategy.hpp"
#include "field/Field.hpp"

namespace Battleship {

class OrderedStrategy : public Strategy {
public:
    OrderedStrategy(Field* field,
                    Field* enemy_field,
                    const std::map<ShipType, uint64_t>& ship_types) 
                    : Strategy(field, enemy_field, ship_types) {};
             
    FieldPoint GetNextShot() const override;

private:
    void StartGame() override;
};

} // namespace Battleship

