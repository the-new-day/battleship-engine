#pragma once

#include "field/Field.hpp"

#include <unordered_set>
#include <map>
#include <cstdint>

namespace Battleship {

enum class ShotResult {
    kMiss,
    kHit,
    kKill
};

enum class ShipType : uint8_t {
    kOne = 1,
    kTwo = 2,
    kThree = 3,
    kFour = 4
};

const size_t kShipTypesAmount = 4;
const size_t kShipMaxLength = 4;

class ShipHandler {
public:
    ShipHandler(uint64_t field_width, uint64_t field_height, const std::map<ShipType, uint64_t>& ships_count);

    ShotResult ProcessShot(uint64_t x, uint64_t y);
    bool HasAliveShips() const;

    bool PlaceShips();

private:
    uint64_t field_width_;
    uint64_t field_height_;

    std::map<ShipType, uint64_t> ships_count_;
    std::unordered_set<FieldPoint, FieldPointHash> hit_points_;

    uint64_t ship_placement_seed;

    Field* field_;

    bool IsHitFatal(uint64_t x, uint64_t y, const std::vector<FieldPoint>& ship_cells) const;
    bool IsShipAt(uint64_t x, uint64_t y) const;

    void FindShipCells(uint64_t x, uint64_t y, std::vector<FieldPoint>& cells) const;

    void PlaceShip(uint64_t x, uint64_t y, ShipType type, bool is_horizontal);
    bool IsPossibleToPlaceShip(uint64_t x, uint64_t y, ShipType type, bool is_horizontal) const;

    uint8_t GetShipSize(ShipType type) const;

    bool PlaceShipsLinear();
};
    
} // namespace Battleship

