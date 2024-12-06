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

    std::vector<FieldPoint> target_cells_;
    std::vector<FieldPoint> potential_targets_;

    void StartGame() override;

    void MakeNextStrategicShot();
    void MakeNextHuntingShot();

    FieldPoint ChooseNextStrategicShot() const;
};

} // namespace Battleship

