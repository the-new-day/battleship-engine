#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

namespace Battleship {

struct FieldPoint {
    uint64_t x;
    uint64_t y;
};

class Field {
public:
    Field(uint64_t width, uint64_t height) : width_(width), height_(height) {};

    virtual bool SetShip(uint64_t x, uint64_t y) = 0;
    virtual void RemoveShip(uint64_t x, uint64_t y) = 0;

    virtual bool HasShip(uint64_t x, uint64_t y) const = 0;
    bool IsInBounds(uint64_t x, uint64_t y) const;

    uint64_t GetWidth() const;
    uint64_t GetHeight() const;

    virtual void Clear() = 0;

protected:
    uint64_t width_;
    uint64_t height_;
};

} // namespace Battleship

