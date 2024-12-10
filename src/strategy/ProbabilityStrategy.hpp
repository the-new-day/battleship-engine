#pragma once

#include "HuntingStrategy.hpp"

#include <vector>
#include <array>

namespace Battleship {

class ProbabilityStrategy : public HuntingStrategy {
public:
    ProbabilityStrategy(uint64_t field_width,
                        uint64_t field_height, 
                        const std::map<uint8_t, uint64_t>& ship_types);

private:
    std::vector<std::vector<uint64_t>> probability_map_;
    std::array<uint64_t, kMaxShipLength> real_enemy_ships_count_;

    bool IsPossibleToPlaceShip(uint64_t x, uint64_t y, uint8_t ship_size, bool is_horizontal) const;

    void CalculateFullMap();
    void CalculateFullMapForShip(uint8_t ship_size);

    void RecalculateMap(uint64_t x, uint64_t y);
    void RecalculateMapForShip(uint64_t x, uint64_t y, uint8_t ship_size);

    void UpdateEnemyField();

    FieldPoint GetMostLikelyPoint() const;

    void MakeNextStrategicShot() override;
    void StartGame() override;

    void HandleLastShot();
};
    
} // namespace Battleship

