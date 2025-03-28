#include "CompressedField.hpp"

namespace Battleship {

void CompressedField::SetBit(uint64_t x, uint64_t y) {
    if (!IsInBounds(x, y)) {
        return;
    }

    SetBit(x, y, 1);
}

bool CompressedField::IsOneAt(uint64_t x, uint64_t y) const {
    if (!IsInBounds(x, y)) {
        return false;
    }
    
    FieldPoint block_index = GetBlockIndex(x, y);
    if (!blocks_.contains(block_index)) {
        return false;
    }

    x %= kRleBlockSize;
    y %= kRleBlockSize;

    return blocks_.at(block_index).IsOneAt(x, y);
}

void CompressedField::RemoveBit(uint64_t x, uint64_t y) {
    if (!IsInBounds(x, y)) {
        return;
    }

    SetBit(x, y, 0);
}

void CompressedField::Clear() {
    blocks_.clear();
}

FieldPoint CompressedField::GetBlockIndex(uint64_t x, uint64_t y) const {
    return {x / kRleBlockSize, y / kRleBlockSize};
}

void CompressedField::SetBit(uint64_t x, uint64_t y, bool to_one) {
    FieldPoint block_index = GetBlockIndex(x, y);
    if (!blocks_.contains(block_index) && !to_one) {
        return;
    }

    if (!blocks_.contains(block_index)) {
        AddBlock(x, y);
    }
    
    RleBlock& block = blocks_.at(block_index);

    x %= kRleBlockSize;
    y %= kRleBlockSize;

    block.SetBitAt(x, y, to_one);

    if (block.IsEmpty()) {
        blocks_.erase(block_index);
    }
}

void CompressedField::AddBlock(uint64_t x, uint64_t y) {
    blocks_.emplace(GetBlockIndex(x, y), RleBlock(kRleBlockSize, kRleBlockSize));
}

} // namespace Battleship
