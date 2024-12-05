#pragma once

#include "field/Field.hpp"

#include <unordered_set>
#include <map>
#include <cstdint>
#include <string>
#include <fstream>

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
    ShipHandler() = default;

    uint64_t GetFieldWidth() const;
    uint64_t GetFieldHeight() const;

    bool LoadFromFile(const std::string& filename);
    bool Dump(const std::string& filename) const;

    ShotResult ProcessShot(uint64_t x, uint64_t y);
    bool HasAliveShips() const;

    bool PlaceShips();

    uint64_t GetShipsCount(ShipType ship_type) const;

private:
    uint64_t field_width_;
    uint64_t field_height_;

    std::map<ShipType, uint64_t> ships_count_;
    std::unordered_set<FieldPoint, FieldPointHash> hit_points_;

    uint64_t ship_placement_seed;

    // TODO: what if file gets deleted
    bool was_loaded_from_file = false;
    std::string filename;

    Field* field_;

    bool IsHitFatal(uint64_t x, uint64_t y, const std::vector<FieldPoint>& ship_cells) const;
    bool IsShipAt(uint64_t x, uint64_t y) const;

    void FindShipCells(uint64_t x, uint64_t y, std::vector<FieldPoint>& cells) const;

    void PlaceShip(uint64_t x, uint64_t y, ShipType type, bool is_horizontal);
    bool IsPossibleToPlaceShip(uint64_t x, uint64_t y, ShipType type, bool is_horizontal) const;

    uint8_t GetShipSize(ShipType type) const;

    bool PlaceShipsLinear();

    void SetField();

    void DumpHorizontalShips(std::ofstream& file) const;
    void DumpVerticalShips(std::ofstream& file) const;
};
    
} // namespace Battleship

