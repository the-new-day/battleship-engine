#pragma once

#include "field/Field.hpp"
#include "field/MatrixField.hpp"
#include "field/CompressedField.hpp"

#include <utility>
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
    Strategy(uint64_t field_width,
             uint64_t field_height,
             const std::map<ShipType, uint64_t>& ship_types,
             Field* field,
             Field* enemy_field);

    virtual ShotResult RecieveShot(uint64_t x, uint64_t y);
    virtual void SetLastShotResult(ShotResult result);
    virtual bool HasAliveShips() const;

    virtual FieldPoint MakeNextShot() = 0;

protected:
    uint64_t field_width_;
    uint64_t field_height_;
    std::map<ShipType, uint64_t> ship_types_;

    Field* field_;
    Field* enemy_field_;

    ShotResult last_shot_result_;
    FieldPoint last_shot_point_;

    // made only so that up to 4 * (2^64 - 1) ships can be stored
    // does not represent the number of ships of a particular type
    std::array<uint64_t, kShipTypesAmount> ships_count_;
    void DecreaseShipsAmount();
};

} // namespace Battleship
