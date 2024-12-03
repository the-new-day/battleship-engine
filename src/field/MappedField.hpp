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
    struct FieldPointHash {
        size_t operator()(const FieldPoint& point) const {
            size_t hash1 = std::hash<uint64_t>{}(point.x);
            size_t hash2 = std::hash<uint64_t>{}(point.y);
            return hash1
                   ^ (hash2 + 0x9e3779b9 + (hash1 << 6)
                   + (hash1 >> 2));
        }
    };
    
    std::unordered_set<FieldPoint, FieldPointHash> points_;
};
    
} // namespace Battleship
