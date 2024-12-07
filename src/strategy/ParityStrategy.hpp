#pragma once

#include "Strategy.hpp"

namespace Battleship {

class ParityStrategy : public Strategy {
public:
    ParityStrategy(uint64_t field_width,
                   uint64_t field_height, 
                   const std::map<uint8_t, uint64_t>& ship_types,
                   Field* enemy_field);

    FieldPoint GetNextShot();

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

