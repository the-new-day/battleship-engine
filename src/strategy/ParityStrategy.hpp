#pragma once

#include "HuntingStrategy.hpp"

namespace Battleship {

class ParityStrategy : public HuntingStrategy {
public:
    using HuntingStrategy::HuntingStrategy;
    
    FieldPoint GetNextShot() override;

private:
    FieldPoint last_strategic_shot_;
    FieldPoint last_successful_hunt_shot_;

    std::vector<FieldPoint> target_cells_;
    std::vector<FieldPoint> potential_targets_;

    bool was_parity_changed_ = false;

    void StartGame() override;

    void MakeNextStrategicShot();
    void MakeNextHuntingShot();

    void UpdateSafeZone();

    FieldPoint ChooseNextStrategicShot(FieldPoint last_shot);
};

} // namespace Battleship

