#pragma once

#include "Strategy.hpp"
#include "field/CompressedField.hpp"

namespace Battleship {

class HuntingStrategy : public Strategy {
public:
    HuntingStrategy(uint64_t field_width,
                    uint64_t field_height, 
                    const std::map<uint8_t, uint64_t>& ship_types);

    FieldPoint GetNextShot() override;

protected:
    FieldPoint last_successful_hunt_shot_;
    FieldPoint last_strategic_shot_;

    CompressedField enemy_field_;

    std::vector<FieldPoint> target_cells_;
    std::vector<FieldPoint> potential_targets_;

    void MakeNextHuntingShot();

    virtual void MakeNextStrategicShot() = 0;
};
    
} // namespace Battleship

