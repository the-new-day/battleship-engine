#include <iostream>
#include <string>

#include "api/Api.hpp"
#include "api/ConsoleApi.hpp"

int main() {
    Battleship::Battleship game;
    Battleship::ConsoleApi api(game);

    api.Start();
    return EXIT_SUCCESS;
}
