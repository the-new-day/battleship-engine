#include "RleBlocksField.hpp"

namespace Battleship {

RleBlocksField::RleBlocksField(uint64_t width, uint64_t height) : Field(width, height) {
    
}

bool RleBlocksField::SetShip(uint64_t x, uint64_t y) {
    return false;
}

bool RleBlocksField::HasShip(uint64_t x, uint64_t y) const {
    return false;
}

void RleBlocksField::RemoveShip(uint64_t x, uint64_t y) {
    if (!IsInBounds(x, y)) {
        return;
    }

    
}

void RleBlocksField::Clear() {
    
}

} // namespace Battleship
