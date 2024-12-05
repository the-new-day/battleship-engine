#pragma once

#include "Api.hpp"

#include <map>
#include <functional>
#include <string_view>

namespace Battleship {

class ConsoleApi : public Api {
public:
    ConsoleApi(Battleship& game);
    void Start() override;

private:
    bool is_game_created_ = false;
    bool is_game_running_ = false;
    
    bool ParseSet(std::string_view cmd);
    bool ParseCreate(std::string_view cmd);
    bool ParseGet(std::string_view cmd);

    bool HandleShot(std::string_view cmd);
    bool HandleStart(std::string_view cmd);
    bool HandleStop(std::string_view cmd);

    bool HandleLoad(std::string_view cmd);
    bool HandleDump(std::string_view cmd);

    std::map<std::string_view, std::function<bool(std::string_view)>> commands;
};
    
} // namespace Battleship
