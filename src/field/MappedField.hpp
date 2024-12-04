#pragma once

#include "Field.hpp"

#include <vector>
#include <utility>
#include <unordered_set>

namespace Battleship {

class MappedField : public Field {
public:
    MappedField(uint64_t width, uint64_t height);

    void SetShip(uint64_t x, uint64_t y) override;
    bool HasShip(uint64_t x, uint64_t y) const override;

    void RemoveShip(uint64_t x, uint64_t y) override;

    void Clear() override;

private:
    std::unordered_set<FieldPoint, FieldPointHash> points_;
};
    
} // namespace Battleship
