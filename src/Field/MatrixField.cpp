#include "MatrixField.hpp"

namespace Battleship {

MartixField::MartixField(uint64_t width, uint64_t height) : width_(width), height_(height) {
    matrix_.reserve(height);
    for (auto& row : matrix_) {
        row.reserve(width);
    }
}

bool MartixField::SetShip(uint64_t x, uint64_t y, uint8_t ship_size, bool is_horizontal) {
    if (!IsPossibleToPlaceShip(x, y, ship_size, is_horizontal)) {
        return false;
    }

    if (is_horizontal) {
        for (size_t i = x; i < x + ship_size; ++i) {
            matrix_[y][i] = true;
        }

        return true;
    }

    for (size_t i = y; i > y - ship_size; --i) {
        matrix_[i][x] = true;
    }

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

bool MartixField::IsPossibleToPlaceShip(uint64_t x, uint64_t y, uint8_t ship_size, bool is_horizontal) const {
    if (!IsInBounds(x, y)) {
        return false;
    }

    if (is_horizontal) {
        if (IsInBounds(x - 1, y) && matrix_[y][x - 1] == 1 
        || IsInBounds(x + ship_size + 1, y) && matrix_[y][x + ship_size + 1]) {
            return false;
        }

        for (size_t i = x; i < ship_size; ++i) {
            if (!IsInBounds(i, y)
            || matrix_[y][i] == 1
            || IsInBounds(i, y + 1) && matrix_[y + 1][i] == 1
            || IsInBounds(i, y - 1) && matrix_[y - 1][i] == 1) {
                return false;
            }
        }

        return true;
    }

    if (IsInBounds(x, y + 1) && matrix_[y + 1][x] == 1 
    || IsInBounds(x, y + ship_size + 1) && matrix_[y + ship_size + 1][x]) {
        return false;
    }

    for (size_t i = y; i < ship_size; ++i) {
        if (!IsInBounds(x, i)
        || matrix_[i][x] == 1
        || IsInBounds(x + 1, i) && matrix_[i][x + 1] == 1
        || IsInBounds(x - 1, i) && matrix_[i][x - 1] == 1) {
            return false;
        }
    }

    return true;
}

} // namespace Battleship
