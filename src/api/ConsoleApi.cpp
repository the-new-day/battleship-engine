#include "ConsoleApi.hpp"
#include "utils.hpp"

#include <iostream>

namespace Battleship {

ConsoleApi::ConsoleApi(Battleship& game) : Api(game) {
    commands = {
        {"ping", [this](std::string_view){ std::cout << "pong"; return true; }},
        {"exit", [this](std::string_view){ std::cout << "ok"; return false; }},
        {"start", [this](std::string_view cmd){ return HandleStart(cmd); }},
        {"stop", [this](std::string_view cmd){ return HandleStop(cmd); }},
        {"shot", [this](std::string_view cmd){ return HandleShot(cmd); }},
        {"finished", [this](std::string_view) { std::cout << (game_.IsFinished() ? "yes" : "no"); return true; }},
        {"win", [this](std::string_view) { std::cout << (game_.IsWin() ? "yes" : "no"); return true; }},
        {"lose", [this](std::string_view) { std::cout << (game_.IsLose() ? "yes" : "no"); return true; }},
        {"set", [this](std::string_view cmd) { return ParseSet(cmd); }},
        {"get", [this](std::string_view cmd) { return ParseGet(cmd); }},
        {"create", [this](std::string_view cmd) { return ParseCreate(cmd); }},
        {"load", [this](std::string_view cmd) { return HandleLoad(cmd); }},
        {"dump", [this](std::string_view cmd) { return HandleDump(cmd); }}
    };
}

void ConsoleApi::Start() {
    std::string command;

    while (std::getline(std::cin, command)) {
        std::string_view command_name(command);
        command_name = command_name.substr(0, command_name.find(' '));

        if (!commands.contains(command_name)) {
            std::cout << "unknown command\n";
            continue;
        }

        if (!commands.at(command_name)(command)) {
            if (command_name == "exit") {
                break;
            }

            std::cout << "failed";
        }

        std::cout << '\n';
    }

    std::cout << '\n';
}

bool ConsoleApi::ParseSet(std::string_view cmd) {
    if (!is_game_created_) {
        return false;
    }

    cmd = cmd.substr(cmd.find(' ') + 1);
    std::string_view parameter = cmd.substr(0, cmd.find(' '));
    std::string_view value_str = cmd.substr(parameter.length() + 1);

    if (parameter == "width") {
        std::optional<uint64_t> value = ParseNumber<uint64_t>(value_str);
        if (!value.has_value() || !game_.SetFieldWidth(value.value())) {
            return false;
        }
    } else if (parameter == "height") {
        std::optional<uint64_t> value = ParseNumber<uint64_t>(value_str);
        if (!value.has_value() || !game_.SetFieldHeight(value.value())) {
            return false;
        }
    } else if (parameter == "count") {
        std::string_view ship_type_str = value_str.substr(0, value_str.find(' '));
        auto ship_size = ParseNumber<uint8_t>(ship_type_str);

        if (!ship_size.has_value() || ship_size == 0 || ship_size > kMaxShipLength) {
            return false;
        }
        
        value_str = value_str.substr(value_str.find(' ') + 1);
        std::optional<uint64_t> amount = ParseNumber<uint64_t>(value_str);
        
        if (!amount.has_value() || !game_.SetShipsCount(ship_size.value(), amount.value())) {
            return false;
        }
    } else if (parameter == "strategy") {
        if (value_str == "custom") {
            game_.SetStrategy(StrategyType::kCustom);
        } else if (value_str == "ordered") {
            game_.SetStrategy(StrategyType::kOrdered);
        } else if (value_str == "parity") {
            game_.SetStrategy(StrategyType::kParity);
        } else if (value_str == "probability") {
            game_.SetStrategy(StrategyType::kProbability);
        } else {
            return false;
        }
    } else if (parameter == "result") {
        if (value_str == "miss") {
            game_.SetLastShotResult(ShotResult::kMiss);
        } else if (value_str == "kill") {
            game_.SetLastShotResult(ShotResult::kKill);
        } else if (value_str == "hit") {
            game_.SetLastShotResult(ShotResult::kHit);
        } else {
            return false;
        }
    } else {
        return false;
    }

    std::cout << "ok";
    return true;
}

bool ConsoleApi::ParseCreate(std::string_view cmd) {
    if (is_game_created_ && !game_.IsFinished()) {
        return false;
    }

    std::string_view strategy = cmd.substr(cmd.find(' ') + 1);

    if (strategy == "slave") {
        game_.SetGameMode(GameMode::kSlave);
    } else if (strategy == "master") {
        game_.SetGameMode(GameMode::kMaster);
    } else {
        return false;
    }

    is_game_created_ = true;
    std::cout << "ok";
    return true;
}

bool ConsoleApi::ParseGet(std::string_view cmd) {
    if (!is_game_created_) {
        return false;
    }
    
    cmd = cmd.substr(cmd.find(' ') + 1);
    std::string_view parameter = cmd.substr(0, cmd.find(' '));

    if (parameter == "width") {
        auto width = game_.GetWidth();
        if (!width.has_value()) {
            return false;
        }

        std::cout << width.value();
    } else if (parameter == "height") {
        auto height = game_.GetHeight();
        if (!height.has_value()) {
            return false;
        }

        std::cout << height.value();
    } else if (parameter == "count") {
        std::string_view value_str = cmd.substr(parameter.length() + 1);
        auto ship_size = ParseNumber<uint8_t>(value_str);

        if (!ship_size.has_value() || ship_size == 0 || ship_size > kMaxShipLength) {
            return false;
        }

        std::cout << game_.GetShipsCount(ship_size.value());
    } else {
        return false;
    }

    return true;
}

bool ConsoleApi::HandleShot(std::string_view cmd) {
    if (!is_game_running_) {
        return false;
    }

    if (cmd == "shot") {
        auto point = game_.MakeNextShot();
        if (!point.has_value()) {
            return false;
        }

        std::cout << point.value().x << ' ' << point.value().y;
        return true;
    }

    cmd = cmd.substr(5);
    if (std::count(cmd.begin(), cmd.end(), ' ') != 1) {
        return false;
    }

    std::string_view x_str = cmd.substr(0, cmd.find(' '));
    std::string_view y_str = cmd.substr(cmd.find(' ') + 1);

    auto x = ParseNumber<uint64_t>(x_str);
    auto y = ParseNumber<uint64_t>(y_str);

    if (!x.has_value() || !y.has_value()) {
        return false;
    }

    ShotResult result = game_.RecieveShot(x.value(), y.value()).value();

    if (result == ShotResult::kMiss) {
        std::cout << "miss";
    } else if (result == ShotResult::kKill) {
        std::cout << "kill";
    } else if (result == ShotResult::kHit) {
        std::cout << "hit";
    }

    return true;
}

bool ConsoleApi::HandleStart(std::string_view cmd) {
    if (cmd != "start" || !game_.Start()) {
        return false;
    }

    std::cout << "ok";
    is_game_running_ = true;
    return true;
}

bool ConsoleApi::HandleStop(std::string_view cmd) {
    if (cmd != "stop" || !game_.Stop()) {
        return false;
    }
    
    std::cout << "ok";
    is_game_running_ = false;
    return true;
}

bool ConsoleApi::HandleLoad(std::string_view cmd) {
    if (game_.LoadFrom(std::string(cmd.substr(5)))) {
        std::cout << "ok";
        is_game_created_ = true;
        return true;
    }

    return false;
}

bool ConsoleApi::HandleDump(std::string_view cmd) {
    if (game_.Dump(std::string(cmd.substr(5)))) {
        std::cout << "ok";
        return true;
    }

    return false;
}

} // namespace Battleship
