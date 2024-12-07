#pragma once

#include <cstdint>
#include <vector>

namespace Battleship {

class RleBlock {
public:
    RleBlock(uint64_t width, uint64_t height);

    bool IsOneAt(uint64_t x, uint64_t y) const;
    void SetBitAt(uint64_t x, uint64_t y, bool to_one);
    void Clear();

    bool IsEmpty() const;

private:
    uint64_t width_;
    uint64_t height_;
    bool starts_with_zero_ = true;

    std::vector<uint64_t> data_;

    bool IsInBounds(uint64_t x, uint64_t y) const;
};

} // namespace Battleship
