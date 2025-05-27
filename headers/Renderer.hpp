#pragma once

#include "TileLayer.hpp"
#include <string>
#include <SDL2/SDL.h>

class Renderer {
public:
    static SDL_Texture* loadTileset(SDL_Renderer* renderer, const std::string& path);
    static void render(SDL_Renderer* renderer, SDL_Texture* tileset, const TileLayer& layer);
};
