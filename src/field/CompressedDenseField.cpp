#include "CompressedDenseField.hpp"

namespace Battleship {

CompressedDenseField::CompressedDenseField(uint64_t width, uint64_t height) : Field(width, height) {
    rows_.reserve(height);

    for (size_t i = 0; i < height; ++i) {
        rows_.emplace_back(width, 1);
    }

    Clear();
}

void CompressedDenseField::SetBit(uint64_t x, uint64_t y) {
    if (!IsInBounds(x, y)) {
        return;
    }

    rows_[y].SetBitAt(x, 0, 1);
}

bool CompressedDenseField::IsOneAt(uint64_t x, uint64_t y) const {
    return IsInBounds(x, y) && rows_[y].IsOneAt(x, 0);
}

void CompressedDenseField::RemoveBit(uint64_t x, uint64_t y) {
    if (!IsInBounds(x, y)) {
        return;
    }

    rows_[y].SetBitAt(x, 0, 0);
}

void CompressedDenseField::Clear() {
    for (auto& row : rows_) {
        row.Clear();
    }
}

} // namespace Battleship
