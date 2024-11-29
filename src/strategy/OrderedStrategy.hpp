#pragma once

#include "Strategy.hpp"
#include "field/Field.hpp"
#include "field/MatrixField.hpp"
#include "field/CompressedField.hpp"

namespace Battleship {

class OrderedStrategy : public Strategy {
public:
    FieldPoint MakeNextShot() override;
};

} // namespace Battleship

