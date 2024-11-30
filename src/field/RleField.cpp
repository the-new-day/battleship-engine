#include "RleField.hpp"

namespace Battleship {

RleField::RleField(uint64_t width, uint64_t height) : Field(width, height) {
    
}

bool RleField::SetShip(uint64_t x, uint64_t y) {
    return false;
}

bool RleField::HasShip(uint64_t x, uint64_t y) const {
    return false;
}

void RleField::RemoveShip(uint64_t x, uint64_t y) {
    if (!IsInBounds(x, y)) {
        return;
    }

    
}

void RleField::Clear() {
    
}

} // namespace Battleship
