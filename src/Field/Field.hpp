#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

namespace Battleship {

class Field {
public:
    virtual bool SetShip(uint64_t x, uint64_t y, uint8_t ship_size, bool is_horizontal = false) = 0;
    virtual bool HasShip(uint64_t x, uint64_t y) const = 0;

    virtual bool IsPossibleToPlaceShip(uint64_t x, uint64_t y, uint8_t ship_size, bool is_horizontal = false) const = 0;

    virtual bool IsInBounds(uint64_t x, uint64_t y) const = 0;
};

} // namespace Battleship

