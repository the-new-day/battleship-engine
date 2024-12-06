#include "MatrixField.hpp"

#include <iostream>

namespace Battleship {

MartixField::MartixField(uint64_t width, uint64_t height) : Field(width, height) {
    matrix_.resize(height);
    for (auto& row : matrix_) {
        row.resize(width);
    }
}

void MartixField::SetBit(uint64_t x, uint64_t y) {
    if (!IsInBounds(x, y)) {
        return;
    }

    matrix_[y][x] = true;
}

bool MartixField::IsOneAt(uint64_t x, uint64_t y) const {
    if (!IsInBounds(x, y)) {
        return false;
    }

    return matrix_[y][x];
}

void MartixField::RemoveBit(uint64_t x, uint64_t y) {
    if (!IsInBounds(x, y)) {
        return;
    }

    matrix_[y][x] = false;
}

void MartixField::Clear() {
    for (auto& row : matrix_) {
        row.clear();
    }
}

} // namespace Battleship
