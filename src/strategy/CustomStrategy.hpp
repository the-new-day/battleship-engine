#pragma once

#include "Strategy.hpp"
#include "field/Field.hpp"

namespace Battleship {

class CustomStrategy : public Strategy {
public:
    CustomStrategy(Field* field,
                   Field* enemy_field,
                   const std::map<ShipType, uint64_t>& ship_types);

    FieldPoint GetNextShot() const override;
    
private:
    void StartGame() override;
};

} // namespace Battleship

