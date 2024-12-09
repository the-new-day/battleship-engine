#pragma once

#include "field/Field.hpp"

#include <unordered_set>
#include <map>
#include <cstdint>
#include <string>
#include <fstream>

namespace Battleship {

// Max area of a field to be stored in a simple binary matrix (30 MB)
const uint64_t kMaxMatrixFieldArea = 251'658'240;

enum class ShotResult {
    kMiss,
    kHit,
    kKill
};

const size_t kShipTypesAmount = 4;
const size_t kMaxShipLength = 4;

class ShipHandler {
public:
    ShipHandler(uint64_t field_width, uint64_t field_height, const std::map<uint8_t, uint64_t>& ships_count);
    ShipHandler() = default;

    uint64_t GetFieldWidth() const;
    uint64_t GetFieldHeight() const;

    bool LoadFromFile(const std::string& filename);
    bool Dump(const std::string& filename) const;

    ShotResult ProcessShot(uint64_t x, uint64_t y);
    bool HasAliveShips() const;

    bool PlaceShips();
    bool PlaceShipsLinear();
    bool PlaceShipsRandomly();

    uint64_t GetShipsCount(uint8_t ship_size) const;

private:
    uint64_t field_width_;
    uint64_t field_height_;

    std::map<uint8_t, uint64_t> ships_count_;
    std::unordered_set<FieldPoint, FieldPointHash> hit_points_;

    Field* field_;

    bool IsHitFatal(uint64_t x, uint64_t y, const std::vector<FieldPoint>& ship_cells) const;
    bool IsShipAt(uint64_t x, uint64_t y) const;

    void FindShipCells(uint64_t x, uint64_t y, std::vector<FieldPoint>& cells) const;

    void PlaceShip(uint64_t x, uint64_t y, uint8_t ship_size, bool is_horizontal);
    bool IsPossibleToPlaceShip(uint64_t x, uint64_t y, uint8_t ship_size, bool is_horizontal) const;

    void SetField();

    void DumpHorizontalShips(std::ofstream& file) const;
    void DumpVerticalShips(std::ofstream& file) const;
};
    
} // namespace Battleship
