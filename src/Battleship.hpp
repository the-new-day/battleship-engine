#pragma once

#include "field/Field.hpp"

#include "strategy/Strategy.hpp"
#include "strategy/OrderedStrategy.hpp"
#include "strategy/CustomStrategy.hpp"

#include <map>
#include <string>
#include <utility>
#include <optional>

namespace Battleship {

enum class GameMode {
    kMaster,
    kSlave
};

enum class StrategyType {
    kCustom,
    kOrdered
};

enum class BattleshipStatus {
    kConfigurationNotSet,
    kWrongParameter,
    kConfigurationDone,
    kGameRunning
};

const uint64_t kMasterModeFieldWidth = 10;
const uint64_t kMasterModeFieldHeight = 10;

const std::map<ShipType, uint64_t> kMasterModeShipsCount = {
    {ShipType::kOne, 4},
    {ShipType::kTwo, 3},
    {ShipType::kThree, 2},
    {ShipType::kFour, 1}
};

class Battleship {
public:
    Battleship();
    ~Battleship();
    Battleship(const Battleship&) = delete;
    Battleship& operator=(const Battleship&) = delete;

    bool SetGameMode(GameMode mode);

    bool Start();
    bool Stop();

    bool SetFieldWidth(uint64_t width);
    bool SetFieldHeight(uint64_t height);

    std::optional<uint64_t> GetWidth() const;
    std::optional<uint64_t> GetHeight() const;

    bool SetShipsCount(ShipType type, uint64_t amount);
    uint64_t GetShipsCount(ShipType type) const;

    void SetStrategy(StrategyType strategy_type);

    std::optional<FieldPoint> MakeNextShot();
    std::optional<ShotResult> RecieveShot(uint64_t x, uint64_t y);

    void SetLastShotResult(ShotResult result);

    bool IsFinished() const;
    bool IsWin() const;
    bool IsLose() const;

    bool Dump(const std::string& path) const;
    bool LoadFrom(const std::string& path);

    bool IsPossibleToStart() const;

    bool HasError() const;
    BattleshipStatus GetStatus() const;

private:
    bool is_game_running_ = false;
    bool is_game_finished_ = false;

    Field* field_ = nullptr;
    Field* enemy_field_ = nullptr;

    std::optional<GameMode> game_mode_;

    std::map<ShipType, uint64_t> ship_types_;

    StrategyType strategy_type_ = StrategyType::kCustom;

    Strategy* strategy_ = nullptr;
    OrderedStrategy* ordered_strategy_ = nullptr;
    CustomStrategy* custom_strategy_ = nullptr;

    std::optional<uint64_t> field_width_;
    std::optional<uint64_t> field_height_;

    BattleshipStatus status_ = BattleshipStatus::kConfigurationNotSet;

    ShotResult last_shot_result_;
    FieldPoint last_shot_point_;

    // made only so that up to 4 * (2^64 - 1) ships can be stored
    // does not represent the number of ships of a particular type
    std::array<uint64_t, kShipTypesAmount> enemy_ships_count_;

    void HandleErrors();

    void CreateField();
    void InitStrategy();
    void ChangeStrategy();
    
    void DecreaseEnemyShipsAmount();
    bool EnemyHasShips() const;

    void RefreshGame();

    void SetMasterConfig();
};

} // namespace Battleship
