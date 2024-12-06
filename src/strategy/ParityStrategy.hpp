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

    std::vector<FieldPoint> current_target_cells_;
    bool is_new_target_ = true;
    bool is_target_active_ = false;
    std::optional<FieldPoint> failed_target_shot_;
    bool is_second_failed_shot_ = false;

    void StartGame() override;

    void MakeNextStrategicShot();
    void MakeNextHuntingShot();

    uint8_t GetPointParity(FieldPoint point);

    void UpdateEnemyShips();

    bool WasVisitedInStrategy(FieldPoint point) const;
};

} // namespace Battleship

