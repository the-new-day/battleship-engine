#pragma once

#include <cstdint>
#include <vector>
#include <cstddef>

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
    // I was tempted to use std::list, but that would waste 16 extra bytes per node
    // and besides, in the battleship, reading bits is much more common than writing them

    bool IsInBounds(uint64_t x, uint64_t y) const;
};

} // namespace Battleship
