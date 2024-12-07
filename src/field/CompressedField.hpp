#pragma once

#include "Field.hpp"
#include "RleBlock.hpp"

#include <unordered_map>

namespace Battleship {

class CompressedField : public Field {
public:
    using Field::Field;

    void SetBit(uint64_t x, uint64_t y) override;
    bool IsOneAt(uint64_t x, uint64_t y) const override;

    void RemoveBit(uint64_t x, uint64_t y) override;

    void Clear() override;

private:
    static const uint32_t kRleBlockSize = 512;

    std::unordered_map<FieldPoint, RleBlock, FieldPointHash> blocks_;

    FieldPoint GetBlockIndex(uint64_t x, uint64_t y) const;
    void SetBit(uint64_t x, uint64_t y, bool to_one);
    void AddBlock(uint64_t x, uint64_t y);
};
    
} // namespace Battleship
