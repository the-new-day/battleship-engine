#pragma once

#include "Field.hpp"

#include <vector>

namespace Battleship {

class MartixField : public Field {
public:
    MartixField(uint64_t width, uint64_t height);

    void SetShip(uint64_t x, uint64_t y) override;
    bool HasShip(uint64_t x, uint64_t y) const override;

    void RemoveShip(uint64_t x, uint64_t y) override;

    void Clear() override;
    bool IsEmpty() const override;

private:
    std::vector<std::vector<bool>> matrix_;
};
    
} // namespace Battleship
