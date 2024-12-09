#pragma once

#include "HuntingStrategy.hpp"
#include "field/CompressedField.hpp"

#include <vector>

namespace Battleship {

class ProbabilityStrategy : public HuntingStrategy {
public:
    ProbabilityStrategy(uint64_t field_width,
                        uint64_t field_height, 
                        const std::map<uint8_t, uint64_t>& ship_types);

private:
    std::vector<std::vector<uint64_t>> probability_map_;
    CompressedField enemy_field_;

    bool IsPossibleToPlaceShip(uint64_t x, uint64_t y, uint8_t ship_size, bool is_horizontal) const;

    void CalculateFullMap();
    void CalculateFullMapForShip(uint8_t ship_size);

    void RecalculateMap(uint64_t x, uint64_t y);
    void RecalculateMapForShip(uint64_t x, uint64_t y, uint8_t ship_size);

    void UpdateEnemyField();

    FieldPoint GetMostLikelyPoint() const;

    void MakeNextStrategicShot() override;
    void StartGame() override;
};
    
} // namespace Battleship

