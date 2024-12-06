#pragma once

#include "Field.hpp"

#include <vector>

namespace Battleship {

class MartixField : public Field {
public:
    MartixField(uint64_t width, uint64_t height);

    void SetBit(uint64_t x, uint64_t y) override;
    bool IsOneAt(uint64_t x, uint64_t y) const override;

    void RemoveBit(uint64_t x, uint64_t y) override;

    void Clear() override;

private:
    std::vector<std::vector<bool>> matrix_;
};
    
} // namespace Battleship
