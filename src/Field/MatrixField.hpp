#pragma once

#include "Field.hpp"

#include <vector>

namespace Battleship {

class MartixField : public Field {
public:
    MartixField(uint64_t width, uint64_t height);

    bool SetShip(uint64_t x, uint64_t y, uint8_t ship_size, bool is_horizontal = false) override;
    bool HasShip(uint64_t x, uint64_t y) const override;
    bool IsPossibleToPlaceShip(uint64_t x, uint64_t y, uint8_t ship_size, bool is_horizontal = false) const override;

    bool IsInBounds(uint64_t x, uint64_t y) const override;

private:
    std::vector<std::vector<bool>> matrix_;
    uint64_t width_;
    uint64_t height_;
};
    
} // namespace Battleship

