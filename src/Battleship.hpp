#pragma once

#include "Command.hpp"
#include "field/Field.hpp"
#include "field/MatrixField.hpp"

#include "strategy/Strategy.hpp"
#include "strategy/OrderedStrategy.hpp"
#include "strategy/CustomStrategy.hpp"

#include <map>
#include <string>
#include <utility>

namespace Battleship {

enum class GameMode {
    kMaster,
    kSlave
};

enum class StrategyType {
    kCustom,
    kOrdered
};

struct BattleshipError {
    // TODO:
};

class Battleship {
public:
    Battleship(GameMode mode);
    ~Battleship();
    Battleship(const Battleship&) = delete;
    Battleship& operator=(const Battleship&) = delete;

    bool Start();
    bool Stop();

    bool SetFieldWidth(uint64_t width);
    bool SetFieldHeight(uint64_t height);

    uint64_t GetWidth() const;
    uint64_t GetHeight() const;

    void SetShipsCount(ShipType type, uint64_t amount);
    uint64_t GetShipsCount(ShipType type);

    void SetStrategy(StrategyType strategy);

    std::pair<uint64_t, uint64_t> GetNextShot() const;
    ShotResult GetShot(uint64_t x, uint64_t y);

    bool IsFinished() const;
    bool IsWin() const;
    bool IsLose() const;

    bool Dump(const std::string& path) const;
    bool LoadFrom(const std::string& path);

    bool IsPossibleToStart() const;

    bool HasError() const;
    BattleshipError GetError() const;

private:
    //std::map<std::string, Command*> commands_; // TODO: separate Battleship class and the commands
    bool is_game_running_ = false;
    bool is_game_finished_ = false;
    bool did_lose_ = false;
    bool did_win_ = false;

    Field* field_ = nullptr;
    bool is_field_good_ = false;

    GameMode game_mode_;

    std::map<ShipType, uint64_t> ships_count_;
    bool has_ships_ = false;

    StrategyType strategy_ = StrategyType::kCustom;

    uint64_t field_width_ = 0;
    uint64_t field_height_ = 0;

    BattleshipError error_;
};

} // namespace Battleship
