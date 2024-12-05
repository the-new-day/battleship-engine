#pragma once

#include "field/CompressedField.hpp"
#include "ShipHandler.hpp"

#include <cstdint>
#include <map>
#include <array>
#include <unordered_set>
#include <ctime>

namespace Battleship {

class Strategy {
public:
    Strategy(uint64_t field_width, uint64_t field_height, const std::map<ShipType, uint64_t>& ship_types);

    virtual FieldPoint GetNextShot() = 0;
    FieldPoint MakeNextShot();

    uint64_t GetFieldWidth() const;
    uint64_t GetFieldHeight() const;

    void SetLastShotCoords(uint64_t x, uint64_t y);
    void SetLastShotCoords(FieldPoint point);
    void SetLastShotResult(ShotResult result);

    ShotResult GetLastShotResult() const;
    FieldPoint GetLastShotPoint() const;

    bool EnemyHasShips() const;

protected:
    uint64_t field_width_;
    uint64_t field_height_;
    std::map<ShipType, uint64_t> enemy_ships_count_;

    bool is_game_started_ = false;

    CompressedField enemy_field_;

    ShotResult last_shot_result_;
    FieldPoint last_shot_point_;

    virtual void StartGame() = 0;
};

} // namespace Battleship
