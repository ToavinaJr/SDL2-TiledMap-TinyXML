#include "Game.hpp"
#include <iostream>

constexpr char MAP_FILE[] = "res/map/MAP.tmx";

int main() {
    try {
        Game game(MAP_FILE);
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Erreur fatale: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
