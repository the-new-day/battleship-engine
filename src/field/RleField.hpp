#pragma once

#include "Field.hpp"

#include <vector>

namespace Battleship {

class RleField : public Field {
public:
    RleField(uint64_t width, uint64_t height);

    bool SetShip(uint64_t x, uint64_t y) override;
    bool HasShip(uint64_t x, uint64_t y) const override;

    void RemoveShip(uint64_t x, uint64_t y) override;

    void Clear() override;
    bool IsEmpty() const override;

private:
    struct RleRow {
        bool starts_with_zero = true;
        std::vector<uint64_t> data;
        // I was tempted to use std::list, but that would waste 16 extra bytes per node
        // and besides, in the battleship, reading bits is much more common than writing them
    };

    std::vector<RleRow> rows_;

    void SetBit(uint64_t x, uint64_t y, bool to_one);
    bool IsOneAt(uint64_t x, uint64_t y) const;
};
    
} // namespace Battleship
