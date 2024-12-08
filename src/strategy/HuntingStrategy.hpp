#pragma once

#include "Strategy.hpp"

namespace Battleship {

class HuntingStrategy : public Strategy {
public:
    HuntingStrategy(uint64_t field_width,
                    uint64_t field_height, 
                    const std::map<uint8_t, uint64_t>& ship_types,
                    Field* enemy_field);

    FieldPoint GetNextShot() override;

protected:
    FieldPoint last_successful_hunt_shot_;
    FieldPoint last_strategic_shot_;

    std::vector<FieldPoint> target_cells_;
    std::vector<FieldPoint> potential_targets_;

    void MakeNextHuntingShot();

    virtual void MakeNextStrategicShot() = 0;
};
    
} // namespace Battleship

