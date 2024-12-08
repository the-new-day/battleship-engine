#pragma once

#include "Battleship.hpp"

namespace Battleship {

class Api {
public:
    Api(Battleship& game) : game_(game) {}
    virtual void Start() = 0;

protected:
    Battleship& game_;
};
    
} // namespace Battleship
