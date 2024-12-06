#pragma once

#include "Strategy.hpp"

namespace Battleship {

class ProbabilityStrategy : public Strategy {
public:
    ProbabilityStrategy(uint64_t field_width,
                        uint64_t field_height, 
                        const std::map<uint8_t, uint64_t>& ship_types);

    FieldPoint GetNextShot();

private:
    void StartGame() override;
};
    
} // namespace Battleship

