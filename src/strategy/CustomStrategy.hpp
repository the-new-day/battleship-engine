#pragma once

#include "Strategy.hpp"
#include "field/Field.hpp"
#include "field/MatrixField.hpp"
#include "field/CompressedField.hpp"

namespace Battleship {

class CustomStrategy : public Strategy {
public:
    FieldPoint MakeNextShot() override;

private:

};

} // namespace Battleship

