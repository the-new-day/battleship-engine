#include "CompressedField.hpp"

namespace Battleship {

CompressedField::CompressedField(uint64_t width, uint64_t height) : Field(width, height) {
    
}

bool CompressedField::SetShip(uint64_t x, uint64_t y) {
    return false;
}

bool CompressedField::HasShip(uint64_t x, uint64_t y) const {
    return false;
}

void CompressedField::RemoveShip(uint64_t x, uint64_t y) {
    if (!IsInBounds(x, y)) {
        return;
    }

    
}

void CompressedField::Clear() {
    
}

} // namespace Battleship
