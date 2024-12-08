#pragma once

#include "Field.hpp"

#include <unordered_set>

namespace Battleship {

class MappedField : public Field {
public:
    using Field::Field;

    void SetBit(uint64_t x, uint64_t y) override;
    bool IsOneAt(uint64_t x, uint64_t y) const override;

    void RemoveBit(uint64_t x, uint64_t y) override;

    void Clear() override;

private:
    std::unordered_set<FieldPoint, FieldPointHash> points_;
};
    
} // namespace Battleship
