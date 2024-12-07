#pragma once

#include "Strategy.hpp"
#include "field/Field.hpp"

namespace Battleship {

class OrderedStrategy : public Strategy {
public:
    using Strategy::Strategy;
             
    FieldPoint GetNextShot() override;

private:
    void StartGame() override;
};

} // namespace Battleship
