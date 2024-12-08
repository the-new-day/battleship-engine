#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

namespace Battleship {

struct FieldPoint {
    uint64_t x;
    uint64_t y;

    bool operator==(const FieldPoint& other) const {
        return x == other.x && y == other.y;
    }
};

struct FieldPointHash {
    size_t operator()(const FieldPoint& point) const {
        size_t hash1 = std::hash<uint64_t>{}(point.x);
        size_t hash2 = std::hash<uint64_t>{}(point.y);
        return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
    }
};

class Field {
public:
    Field(uint64_t width, uint64_t height) : width_(width), height_(height) {};

    virtual void SetBit(uint64_t x, uint64_t y) = 0;
    virtual void RemoveBit(uint64_t x, uint64_t y) = 0;
    virtual bool IsOneAt(uint64_t x, uint64_t y) const = 0;
    virtual void Clear() = 0;

    bool IsInBounds(uint64_t x, uint64_t y) const;

    uint64_t GetWidth() const;
    uint64_t GetHeight() const;

protected:
    uint64_t width_;
    uint64_t height_;
};

} // namespace Battleship
