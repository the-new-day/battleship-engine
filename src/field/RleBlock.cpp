#include "RleBlock.hpp"

#include <iostream>

namespace Battleship {

RleBlock::RleBlock(uint64_t width, uint64_t height) : width_(width), height_(height) {
    Clear();
}

bool RleBlock::IsOneAt(uint64_t x, uint64_t y) const {
    if (!IsInBounds(x, y)) {
        return false;
    }

    size_t skipped_bits = 0;
    bool is_odd_block = true;

    x = y * width_ + x;

    for (const uint64_t& block_size : data_) {
        if (skipped_bits + block_size <= x) {
            skipped_bits += block_size;
            is_odd_block = !is_odd_block;
            continue;
        }

        return starts_with_zero_ ? !is_odd_block : is_odd_block;
    }

    return false;
}

void RleBlock::SetBitAt(uint64_t x, uint64_t y, bool to_one) {
    if (!IsInBounds(x, y)) {
        return;
    }

    uint64_t skipped_bits = 0;
    bool is_odd_block = false;

    x = y * width_ + x;

    for (size_t i = 0; i < data_.size(); ++i) {
        const uint64_t block_size = data_[i];
        is_odd_block = !is_odd_block;

        if (skipped_bits + block_size <= x) {
            skipped_bits += block_size;
            continue;
        }

        if (starts_with_zero_ && is_odd_block && !to_one
        || starts_with_zero_ && !is_odd_block && to_one
        || !starts_with_zero_ && is_odd_block && to_one
        || !starts_with_zero_ && !is_odd_block && !to_one) {
            return;
        }

        if (block_size == 1) {
            if (data_.size() == 1) {
                starts_with_zero_ = !to_one;
                return;
            }

            if (i == 0) {
                starts_with_zero_ = !to_one;
                data_.erase(data_.begin());
                ++data_[0];
                return;
            }

            if (i == data_.size() - 1) {
                data_.erase(std::prev(data_.end()));
                ++data_.back();
                return;
            }
            
            data_[i - 1] += 1 + data_[i + 1];
            data_.erase(std::next(data_.begin(), i), std::next(data_.begin(), i + 2));
            return;
        }

        uint64_t first_new_block_size = x - skipped_bits;

        if (first_new_block_size == 0) {
            if (i == 0) {
                data_.insert(data_.begin(), 1);
                --data_[1];
                starts_with_zero_ = !to_one;
                return;
            }

            ++data_[i - 1];
            --data_[i];
            return;
        }

        data_[i] = 1;
        data_.insert(std::next(data_.begin(), i), first_new_block_size);
        data_.insert(std::next(data_.begin(), i + 2), block_size - first_new_block_size - 1);
        
        return;
    }
}

void RleBlock::Clear() {
    data_.resize(1);
    starts_with_zero_ = true;
    data_[0] = width_ * height_;
}

bool RleBlock::IsEmpty() const {
    return data_.size() == 1 && starts_with_zero_;
}

bool RleBlock::IsInBounds(uint64_t x, uint64_t y) const {
    return x < width_ && y < height_;
}

} // namespace Battleship

