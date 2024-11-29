#pragma once

#include "Strategy.hpp"
#include "field/Field.hpp"
#include "field/MatrixField.hpp"
#include "field/CompressedField.hpp"

namespace Battleship {

class CustomStrategy : public Strategy {
public:
    CustomStrategy(Field* field,
                   Field* enemy_field,
                   const std::map<ShipType, uint64_t>& ship_types) 
                   : Strategy(field, enemy_field, ship_types) {};

    FieldPoint GetNextShot() const override;
    FieldPoint MakeNextShot() override;
    
private:

};

} // namespace Battleship

