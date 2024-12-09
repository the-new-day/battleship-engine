#pragma once

#include "field/Field.hpp"

#include "strategy/Strategy.hpp"
#include "strategy/OrderedStrategy.hpp"
#include "strategy/ParityStrategy.hpp"
#include "strategy/ProbabilityStrategy.hpp"

#include "ShipHandler.hpp"

#include <map>
#include <string>
#include <optional>

namespace Battleship {

enum class GameMode {
    kMaster,
    kSlave
};

enum class StrategyType {
    kCustom,
    kOrdered,
    kParity,
    kProbability
};

enum class BattleshipStatus {
    kConfigurationNotSet,
    kWrongParameter,
    kConfigurationDone,
    kGameRunning
};

const uint64_t kMasterModeFieldWidth = 10;
const uint64_t kMasterModeFieldHeight = 10;

const std::map<uint8_t, uint64_t> kMasterModeShipsCount = {
    {1, 4},
    {2, 3},
    {3, 2},
    {4, 1}
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

    bool SetShipsCount(uint8_t ship_size, uint64_t amount);
    uint64_t GetShipsCount(uint8_t ship_size) const;

    void SetStrategy(StrategyType strategy_type);

    std::optional<FieldPoint> MakeNextShot();
    std::optional<ShotResult> ProcessShot(uint64_t x, uint64_t y);

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

    bool was_loaded_from_file_ = false;

    ShipHandler* ship_handler_ = nullptr;

    std::optional<GameMode> game_mode_;

    std::map<uint8_t, uint64_t> ships_count_;

    StrategyType strategy_type_ = StrategyType::kCustom;

    Strategy* strategy_ = nullptr;

    OrderedStrategy* ordered_strategy_ = nullptr;
    ParityStrategy* parity_strategy_ = nullptr;
    ProbabilityStrategy* probability_strategy_ = nullptr;

    std::optional<uint64_t> field_width_;
    std::optional<uint64_t> field_height_;

    BattleshipStatus status_ = BattleshipStatus::kConfigurationNotSet;

    void HandleErrors();

    void InitStrategy();
    void ChangeStrategy();
    void SetShipHandler();
    void RefreshGame();

    void SetMasterConfig();
};

} // namespace Battleship
