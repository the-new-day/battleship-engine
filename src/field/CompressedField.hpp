#pragma once

#include "Field.hpp"
#include "RleBlock.hpp"

#include <vector>
#include <unordered_map>

namespace Battleship {

class CompressedField : public Field {
public:
    CompressedField(uint64_t width, uint64_t height) : Field(width, height) {};

    void SetShip(uint64_t x, uint64_t y) override;
    bool HasShip(uint64_t x, uint64_t y) const override;

    void RemoveShip(uint64_t x, uint64_t y) override;

    void Clear() override;

private:
    static const uint32_t kRleBlockSize = 512;
    
    struct BlockIndexHash {
        size_t operator()(const FieldPoint& point) const {
            size_t hash1 = std::hash<uint64_t>{}(point.x);
            size_t hash2 = std::hash<uint64_t>{}(point.x);
            return hash1
                   ^ (hash2 + 0x9e3779b9 + (hash1 << 6)
                   + (hash1 >> 2));
        }
    };

    std::unordered_map<FieldPoint, RleBlock, BlockIndexHash> blocks_;

    FieldPoint GetBlockIndex(uint64_t x, uint64_t y) const;
    void SetBit(uint64_t x, uint64_t y, bool to_one);
    void AddBlock(uint64_t x, uint64_t y);
    bool IsOneAt(uint64_t x, uint64_t y) const;
};
    
} // namespace Battleship
