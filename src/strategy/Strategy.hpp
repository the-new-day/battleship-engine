#pragma once

#include "field/Field.hpp"

#include <cstdint>
#include <map>
#include <array>
#include <unordered_set>
#include <ctime>

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

class Strategy {
public:
    Strategy(Field* field,
             Field* enemy_field,
             const std::map<ShipType, uint64_t>& ship_types);

    virtual FieldPoint GetNextShot() const = 0;
    FieldPoint MakeNextShot();

    ShotResult RecieveShot(uint64_t x, uint64_t y);
    bool HasAliveShips() const;

    const std::map<ShipType, uint64_t>& GetShipTypes() const;
    uint64_t GetFieldWidth() const;
    uint64_t GetFieldHeight() const;

    void SetLastShotCoords(uint64_t x, uint64_t y);
    void SetLastShotCoords(FieldPoint point);
    void SetLastShotResult(ShotResult result);

    ShotResult GetLastShotResult() const;
    FieldPoint GetLastShotPoint() const;

    bool PlaceShips();

protected:
    uint64_t field_width_;
    uint64_t field_height_;
    std::map<ShipType, uint64_t> ship_types_;

    uint64_t ship_placement_seed = 0;

    std::unordered_set<FieldPoint, FieldPointHash> hit_points_;

    bool is_game_started_ = false;

    Field* field_;
    Field* enemy_field_;

    ShotResult last_shot_result_;
    FieldPoint last_shot_point_;

    std::array<uint64_t, kShipTypesAmount> ships_count_;
    void DecreaseShipsAmount();

    virtual void StartGame() = 0;

    bool IsHitFatal(uint64_t x, uint64_t y, const std::vector<FieldPoint>& ship_cells) const;
    bool IsShipAt(uint64_t x, uint64_t y) const;

    void FindShipCells(uint64_t x, uint64_t y, std::vector<FieldPoint>& cells) const;

    void PlaceShip(uint64_t x, uint64_t y, ShipType type, bool is_horizontal);
    bool IsPossibleToPlaceShip(uint64_t x, uint64_t y, ShipType type, bool is_horizontal) const;

    uint8_t GetShipSize(ShipType type) const;

    bool IsNeighbourhoodFree(uint64_t x, uint64_t y) const;
    bool PlaceShipsLinear();
};

} // namespace Battleship
