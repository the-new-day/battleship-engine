#pragma once

#include "field/MatrixField.hpp"
#include "field/CompressedDenseField.hpp"
#include "field/CompressedField.hpp"
#include "field/MappedField.hpp"

#include "ShipHandler.hpp"

#include <array>

namespace Battleship {

class Strategy {
public:
    Strategy(uint64_t field_width,
             uint64_t field_height,
             const std::map<uint8_t, uint64_t>& ship_types);

    virtual ~Strategy();
    Strategy(const Strategy& other) = delete;
    Strategy& operator=(const Strategy& other) = delete;

    virtual FieldPoint GetNextShot() = 0;
    FieldPoint MakeNextShot();

    void SetLastShotCoords(uint64_t x, uint64_t y);
    void SetLastShotResult(ShotResult result);

    bool EnemyHasShips() const;

protected:
    const uint64_t field_width_;
    const uint64_t field_height_;
    std::array<uint64_t, kMaxShipLength> enemy_ships_count_;

    ShotResult last_shot_result_;
    FieldPoint last_shot_point_;

    Field* enemy_field_ = nullptr;

    virtual void StartGame() = 0;

    void SetEnemyField();

private:
    bool is_game_started_ = false;
};

} // namespace Battleship
