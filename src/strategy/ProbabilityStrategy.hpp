#pragma once

#include "HuntingStrategy.hpp"

namespace Battleship {

class ProbabilityStrategy : public HuntingStrategy {
public:
    ProbabilityStrategy(uint64_t field_width,
                        uint64_t field_height, 
                        const std::map<uint8_t, uint64_t>& ship_types,
                        Field* enemy_field);

private:
    std::vector<std::vector<uint64_t>> probability_map_;

    void MakeNextStrategicShot() override;

    void CalculateFullMap();
    void RecalculateMap(uint64_t x, uint64_t y);

    FieldPoint GetMaxProbabilityPoint() const;

    void CalculateFullMapForShip(uint8_t ship_size);
    void RecalculateMapForShip(uint64_t x, uint64_t y, uint8_t ship_size);

    bool IsPossibleToPlaceShip(uint64_t x, uint64_t y, uint8_t ship_size, bool is_horizontal) const;

    void StartGame() override;
};
    
} // namespace Battleship

