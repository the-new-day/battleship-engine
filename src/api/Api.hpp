#pragma once

#include "Battleship.hpp"

#include <string>

namespace Battleship {

class Api {
public:
    Api(Battleship& game) : game_(game) {}
    virtual void Start() = 0;

protected:
    Battleship& game_;
    bool is_game_created_ = false;
    bool is_game_running_ = false;
};
    
} // namespace Battleship

