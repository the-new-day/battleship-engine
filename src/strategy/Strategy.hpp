#pragma once

#include "field/CompressedField.hpp"
#include "ShipHandler.hpp"

#include <map>

namespace Battleship {

class Strategy {
public:
    Strategy(uint64_t field_width,
             uint64_t field_height,
             const std::map<uint8_t, uint64_t>& ship_types,
             Field* enemy_field);

    virtual FieldPoint GetNextShot() = 0;
    FieldPoint MakeNextShot();

    void SetLastShotCoords(uint64_t x, uint64_t y);
    void SetLastShotResult(ShotResult result);

    bool EnemyHasShips() const;

protected:
    uint64_t field_width_;
    uint64_t field_height_;
    std::map<uint8_t, uint64_t> enemy_ships_count_;

    bool is_game_started_ = false;

    Field* enemy_field_;

    ShotResult last_shot_result_;
    FieldPoint last_shot_point_;

    virtual void StartGame() = 0;
};

} // namespace Battleship
