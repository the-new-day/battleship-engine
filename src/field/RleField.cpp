#include "RleField.hpp"

#include <iostream>

namespace Battleship {

RleField::RleField(uint64_t width, uint64_t height) : Field(width, height) {
    rows_.resize(height);
    Clear();
}

bool RleField::SetShip(uint64_t x, uint64_t y) {
    if (!IsInBounds(x, y)) {
        return false;
    }

    SetBit(x, y, 1);
    return true;
}

bool RleField::HasShip(uint64_t x, uint64_t y) const {
    return IsInBounds(x, y) && IsOneAt(x, y);
}

void RleField::RemoveShip(uint64_t x, uint64_t y) {
    if (!IsInBounds(x, y)) {
        return;
    }

    SetBit(x, y, 0);
}

void RleField::Clear() {
    for (auto& row : rows_) {
        row.data.resize(1);
        row.starts_with_zero = true;
        row.data[0] = width_;
    }
}

bool RleField::IsEmpty() const {
    return false;
}

void RleField::SetBit(uint64_t x, uint64_t y, bool to_one) {
    RleRow& row = rows_[y];
    uint64_t skipped_bits = 0;
    bool is_odd_block = false;

    for (size_t i = 0; i < row.data.size(); ++i) {
        const uint64_t block_size = row.data[i];
        is_odd_block = !is_odd_block;

        if (skipped_bits + block_size <= x) {
            skipped_bits += block_size;
            continue;
        }

        if (row.starts_with_zero && is_odd_block && !to_one
        || row.starts_with_zero && !is_odd_block && to_one
        || !row.starts_with_zero && is_odd_block && to_one
        || !row.starts_with_zero && !is_odd_block && !to_one) {
            return;
        }

        if (block_size == 1) {
            if (row.data.size() == 1) {
                row.starts_with_zero = !to_one;
                return;
            }

            if (i == 0) {
                row.starts_with_zero = !to_one;
                row.data.erase(row.data.begin());
                ++row.data[0];
                return;
            }

            if (i == row.data.size() - 1) {
                row.data.erase(std::prev(row.data.end()));
                ++row.data.back();
                return;
            }
            
            row.data[i - 1] += 1 + row.data[i + 1];
            row.data.erase(std::next(row.data.begin(), i), std::next(row.data.begin(), i + 2));
            return;
        }

        uint64_t first_new_block_size = x - skipped_bits;

        if (first_new_block_size == 0) {
            if (i == 0) {
                row.data.insert(row.data.begin(), 1);
                --row.data[1];
                row.starts_with_zero = !to_one;
                return;
            }

            ++row.data[i - 1];
            --row.data[i];
            return;
        }

        row.data[i] = 1;
        row.data.insert(std::next(row.data.begin(), i), first_new_block_size);
        row.data.insert(std::next(row.data.begin(), i + 2), block_size - first_new_block_size - 1);

        if (x == 8) {
            for (size_t i = 0; i < row.data.size(); ++i) {
                std::cout << row.data[i] << ' ';
            }
            std::cout << '\n';
        }
        
        return;
    }
}

bool RleField::IsOneAt(uint64_t x, uint64_t y) const {
    const RleRow& row = rows_[y];
    size_t skipped_bits = 0;
    bool is_odd_block = true;

    for (const uint64_t& block_size : row.data) {
        if (skipped_bits + block_size <= x) {
            skipped_bits += block_size;
            is_odd_block = !is_odd_block;
            continue;
        }

        return row.starts_with_zero ? !is_odd_block : is_odd_block;
    }

    return false;
}

} // namespace Battleship
