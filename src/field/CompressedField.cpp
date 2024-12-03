#include "CompressedField.hpp"

namespace Battleship {

void CompressedField::SetShip(uint64_t x, uint64_t y) {
    if (!IsInBounds(x, y)) {
        return;
    }

    SetBit(x, y, 1);
}

bool CompressedField::HasShip(uint64_t x, uint64_t y) const {
    return IsInBounds(x, y) && IsOneAt(x, y);
}

void CompressedField::RemoveShip(uint64_t x, uint64_t y) {
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

    AddBlock(x, y);
    
    RleBlock& block = blocks_.at(block_index);

    x %= kRleBlockSize;
    y %= kRleBlockSize;

    block.SetBitAt(x, y, to_one);

    if (block.IsEmpty()) {
        blocks_.erase(block_index);
    }
}

void CompressedField::AddBlock(uint64_t x, uint64_t y) {
    FieldPoint block_index = GetBlockIndex(x, y);
    if (blocks_.contains(block_index)) {
        return;
    }

    blocks_.emplace(block_index, RleBlock(kRleBlockSize, kRleBlockSize));
}

bool CompressedField::IsOneAt(uint64_t x, uint64_t y) const {
    FieldPoint block_index = GetBlockIndex(x, y);
    if (!blocks_.contains(block_index)) {
        return false;
    }

    x %= kRleBlockSize;
    y %= kRleBlockSize;

    return blocks_.at(block_index).IsOneAt(x, y);
}

} // namespace Battleship
