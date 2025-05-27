#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <vector>

#include "../headers/TileLayer.hpp"
#include "../headers/TMXLoader.hpp"
#include "../headers/Renderer.hpp"
#include "../headers/Entity.hpp"

class Game {
public:
    Game(const char* MAP_FILE);
    ~Game();

    void run();

private:
    void render();
    void cleanup();

    int windowWidth;
    int windowHeight;
    
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TileLayer tileLayer;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> tilesetTexture{nullptr, SDL_DestroyTexture};
    
    std::vector<Entity> entities;
    SDL_Texture* entityTexture = nullptr;

    bool isRunning = false;
};
