#pragma once

#include "HuntingStrategy.hpp"
#include "field/CompressedField.hpp"

namespace Battleship {

class ParityStrategy : public HuntingStrategy {
public:
    ParityStrategy(uint64_t field_width,
                   uint64_t field_height, 
                   const std::map<uint8_t, uint64_t>& ship_types);

private:
    bool was_parity_changed_ = false;
    CompressedField enemy_field_;

    void StartGame() override;

    void MakeNextStrategicShot() override;
    FieldPoint ChooseNextStrategicShot(FieldPoint last_shot);

    void UpdateSafeZone();
};

} // namespace Battleship

