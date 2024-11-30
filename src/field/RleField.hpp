#pragma once

#include "Field.hpp"

#include <vector>

namespace Battleship {

class RleField : public Field {
public:
    RleField(uint64_t width, uint64_t height);

    bool SetShip(uint64_t x, uint64_t y) override;
    bool HasShip(uint64_t x, uint64_t y) const override;

    void RemoveShip(uint64_t x, uint64_t y) override;

    void Clear() override;

private:
    
};
    
} // namespace Battleship
