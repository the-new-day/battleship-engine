#pragma once

#include "field/Field.hpp"

#include <cstdint>
#include <map>
#include <array>

namespace Battleship {

enum class ShotResult {
    kMiss,
    kHit,
    kKill
};

enum class ShipType {
    kOne = 1,
    kTwo = 2,
    kThree = 3,
    kFour = 4
};

const size_t kShipTypesAmount = 4;

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

protected:
    uint64_t field_width_;
    uint64_t field_height_;
    std::map<ShipType, uint64_t> ship_types_;

    bool is_game_started_ = false;

    Field* field_;
    Field* enemy_field_;

    ShotResult last_shot_result_;
    FieldPoint last_shot_point_;

    std::array<uint64_t, kShipTypesAmount> ships_count_;
    void DecreaseShipsAmount();

    virtual void StartGame() = 0;
};

} // namespace Battleship
