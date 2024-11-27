#pragma once

#include "Command.hpp"

#include <map>
#include <string>
#include <utility>

namespace Battleship {

enum class PlayerMode {
    kMaster,
    kSlave
};

enum class ShipType {
    kOne,
    kTwo,
    kThree,
    kFour
};

enum class Strategy {
    kOrdered,
    kCustom
};

enum class ShotResult {
    kMiss,
    kHit,
    kKill
};

struct BattleshipError {
    // TODO:
};

class Battleship {
public:
    Battleship(PlayerMode mode);
    ~Battleship();

    bool Start();
    bool Stop();

    bool SetFieldWidth(uint64_t width);
    bool SetFieldHeight(uint64_t height);

    uint64_t GetWidth() const;
    uint64_t GetHeight() const;

    void SetShipsCount(ShipType type, uint64_t amount);
    uint64_t GetShipsCount(ShipType type);

    void SetStrategy(Strategy strategy);

    std::pair<uint64_t, uint64_t> GetNextShot() const;
    ShotResult GetShot(uint64_t x, uint64_t y);

    bool IsFinished() const;
    bool IsWin() const;
    bool IsLose() const;

    bool Dump(const std::string& path) const;
    bool LoadFrom(const std::string& path);

    bool CanStartGame() const;

    bool HasError() const;
    BattleshipError GetError() const;

private:
    //std::map<std::string, Command*> commands_;
    bool is_game_running_ = false;
    bool is_game_finished_ = false;

    PlayerMode player_mode_;

    std::map<ShipType, uint64_t> ships_count_;
    bool has_ships_ = false;

    Strategy strategy = Strategy::kCustom;

    uint64_t field_width_ = 0;
    uint64_t field_height_ = 0;

    BattleshipError error_;
};

} // namespace Battleship

