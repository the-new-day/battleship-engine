#pragma once

#include "HuntingStrategy.hpp"

namespace Battleship {

class ParityStrategy : public HuntingStrategy {
public:
    using HuntingStrategy::HuntingStrategy;

private:
    bool was_parity_changed_ = false;

    void StartGame() override;

    void MakeNextStrategicShot() override;
    FieldPoint ChooseNextStrategicShot(FieldPoint last_shot);

    void UpdateSafeZone();
};

} // namespace Battleship

