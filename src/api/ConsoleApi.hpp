#pragma once

#include "Api.hpp"

namespace Battleship {

class ConsoleApi : public Api {
public:
    ConsoleApi(Battleship& game) : Api(game) {}
    void Start() override;

private:
    bool ParseSet();
    bool ParseCreate();
    bool ParseGet();
    bool ParseShot();
};
    
} // namespace Battleship

