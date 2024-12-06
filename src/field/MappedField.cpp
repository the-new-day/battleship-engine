#include "MappedField.hpp"

namespace Battleship {

MappedField::MappedField(uint64_t width, uint64_t height) : Field(width, height) {}

void MappedField::SetBit(uint64_t x, uint64_t y) {
    if (!IsInBounds(x, y)) {
        return;
    }

    points_.insert({x, y});
}

bool MappedField::IsOneAt(uint64_t x, uint64_t y) const {
    return points_.contains({x, y});
}

void MappedField::RemoveBit(uint64_t x, uint64_t y) {
    points_.erase({x, y});
}

void MappedField::Clear() {
    points_.clear();
}

} // namespace Battleship
