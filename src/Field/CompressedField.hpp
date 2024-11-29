#pragma once

#include "Field.hpp"

#include <vector>

namespace Battleship {

class CompressedField : public Field {
public:
    CompressedField(uint64_t width, uint64_t height);

    bool SetShip(uint64_t x, uint64_t y) override;
    bool HasShip(uint64_t x, uint64_t y) const override;

    bool IsInBounds(uint64_t x, uint64_t y) const override;
    void RemoveShip(uint64_t x, uint64_t y) override;

private:
    uint64_t width_;
    uint64_t height_;
};
    
} // namespace Battleship
