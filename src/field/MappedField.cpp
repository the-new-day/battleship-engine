#include "MappedField.hpp"

namespace Battleship {

MappedField::MappedField(uint64_t width, uint64_t height) : Field(width, height) {}

bool MappedField::SetShip(uint64_t x, uint64_t y) {
    if (!IsInBounds(x, y)) {
        return false;
    }

    points_.insert({x, y});
    return true;
}

bool MappedField::HasShip(uint64_t x, uint64_t y) const {
    return points_.contains({x, y});
}

void MappedField::RemoveShip(uint64_t x, uint64_t y) {
    points_.erase({x, y});
}

void MappedField::Clear() {
    points_.clear();
}

bool MappedField::IsEmpty() const {
    return points_.empty();
}

} // namespace Battleship
