#include "ConsoleApi.hpp"
#include "utils.hpp"

#include <iostream>

namespace Battleship {

void ConsoleApi::Start() {
    std::string command;

    // TODO: refactor this...

    while (std::cin >> command) {
        if (command == "ping") {
            std::cout << "pong";
        } else if (command == "exit") {
            std::cout << "ok\n";
            return;
        } else if (command == "start") {
            if (is_game_created_ && game_.Start()) {
                std::cout << "ok";
                is_game_running_ = true;
            } else {
                std::cout << "failed";
            }
        } else if (command == "stop") {
            if (!is_game_running_ || !game_.Stop()) {
                std::cout << "failed";
            } else {
                std::cout << "ok";
                is_game_running_ = false;
            }
        } else if (command == "shot") {
            if (is_game_running_) {
                auto point = game_.MakeNextShot().value();
                std::cout << point.x << ' ' << point.y;
            } else {
                std::cout << "failed";
            }
        } else if (command == "finished") {
            std::cout << (game_.IsFinished() ? "yes" : "no");
        } else if (command == "win") {
            std::cout << (game_.IsWin() ? "yes" : "no");
        } else if (command == "lose") {
            std::cout << (game_.IsLose() ? "yes" : "no");
        } else if (command == "set") {
            std::cout << (ParseSet() ? "ok" : "failed");
        } else if (command == "get") {
            if (!ParseGet()) {
                std::cout << "failed";
            }
        } else if (command == "create") {
            std::cout << (ParseCreate() ? "ok" : "failed");
        }

        std::cout << '\n';
    }
}

bool ConsoleApi::ParseSet() {
    std::string parameter;
    std::string value_str;

    std::cin >> parameter;
    std::cin >> value_str;

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
        ShipType ship_type;

        if (value_str == "1") {
            ship_type = ShipType::kOne;
        } else if (value_str == "2") {
            ship_type = ShipType::kTwo;
        } else if (value_str == "3") {
            ship_type = ShipType::kThree;
        } else if (value_str == "4") {
            ship_type = ShipType::kFour;
        } else {
            return false;
        }

        std::cin >> value_str;
        std::optional<uint64_t> amount = ParseNumber<uint64_t>(value_str);
        if (!amount.has_value()) {
            return false;
        }

        return game_.SetShipsCount(ship_type, amount.value());
    } else if (parameter == "strategy") {
        if (value_str == "custom") {
            game_.SetStrategy(StrategyType::kCustom);
        } else if (value_str == "ordered") {
            game_.SetStrategy(StrategyType::kOrdered);
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
    }

    return true;
}

bool ConsoleApi::ParseCreate() {
    std::string value_str;
    std::cin >> value_str;

    if (value_str == "slave") {
        game_.SetGameMode(GameMode::kSlave);
    } else if (value_str == "master") {
        game_.SetGameMode(GameMode::kMaster);
    } else {
        return false;
    }

    is_game_created_ = true;
    return true;
}

bool ConsoleApi::ParseGet() {
    std::string parameter;
    std::cin >> parameter;

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
        ShipType ship_type;
        std::string value_str;
        std::cin >> value_str;

        if (value_str == "1") {
            ship_type = ShipType::kOne;
        } else if (value_str == "2") {
            ship_type = ShipType::kTwo;
        } else if (value_str == "3") {
            ship_type = ShipType::kThree;
        } else if (value_str == "4") {
            ship_type = ShipType::kFour;
        } else {
            return false;
        }

        std::cout << game_.GetShipsCount(ship_type);
    }

    return true;
}

bool ConsoleApi::ParseShot() {
    return false;
}

} // namespace Battleship
