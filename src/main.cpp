#include "api/ConsoleApi.hpp"
#include "Battleship.hpp"

#include "field/CompressedField.hpp"

#include <iostream>

int main() {
    Battleship::Battleship game;
    Battleship::ConsoleApi api(game);

    // Battleship::RleBlock block(10, 1);

    // block.SetBitAt(0, 0, 1);
    // block.SetBitAt(1, 0, 1);
    // block.SetBitAt(3, 0, 1);
    // block.SetBitAt(9, 0, 1);

    // for (int i = 0; i < 10; ++i) {
    //     for (int j = 0; j < 10; ++j) {
    //         std::cout << block.IsOneAt(j, 0);
    //     }
    //     std::cout << '\n';
    // }

    // for (int j = 0; j < 10; ++j) {
    //     std::cout << block.IsOneAt(j, 0);
    // }

    api.Start();
    return EXIT_SUCCESS;
}
