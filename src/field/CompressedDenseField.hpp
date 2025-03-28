#pragma once

#include "Field.hpp"
#include "RleBlock.hpp"

#include <vector>

namespace Battleship {

class CompressedDenseField : public Field {
public:
    CompressedDenseField(uint64_t width, uint64_t height);

    void SetBit(uint64_t x, uint64_t y) override;
    bool IsOneAt(uint64_t x, uint64_t y) const override;

    void RemoveBit(uint64_t x, uint64_t y) override;

    void Clear() override;

private:
    std::vector<RleBlock> rows_;
};
    
} // namespace Battleship
