#include "MatrixField.hpp"

namespace Battleship {

MartixField::MartixField(uint64_t width, uint64_t height) : Field(width, height) {
    matrix_.reserve(height);
    for (auto& row : matrix_) {
        row.reserve(width);
    }
}

bool MartixField::SetShip(uint64_t x, uint64_t y) {
    if (!IsInBounds(x, y)) {
        return false;
    }

    matrix_[y][x] = true;
    return true;
}

bool MartixField::HasShip(uint64_t x, uint64_t y) const {
    if (!IsInBounds(x, y)) {
        return false;
    }

    return matrix_[y][x];
}

bool MartixField::IsInBounds(uint64_t x, uint64_t y) const {
    return x < width_ && y < height_;
}

void MartixField::RemoveShip(uint64_t x, uint64_t y) {
    if (!IsInBounds(x, y)) {
        return;
    }

    matrix_[y][x] = false;
}

} // namespace Battleship
