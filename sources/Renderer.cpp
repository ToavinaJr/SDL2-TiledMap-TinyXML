#include "../headers/Renderer.hpp"
#include <SDL2/SDL_image.h>
#include <iostream>

SDL_Texture* Renderer::loadTileset(SDL_Renderer* renderer, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Erreur chargement tileset : " << IMG_GetError() << '\n';
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Erreur création texture : " << SDL_GetError() << '\n';
    }
    return texture;
}

void Renderer::render(SDL_Renderer* renderer, SDL_Texture* tileset, const TileLayer& layer) {
    if (!tileset || !layer.isValid()) return;

    for (int y = 0; y < layer.mapHeight; ++y) {
        for (int x = 0; x < layer.mapWidth; ++x) {
            int index = y * layer.mapWidth + x;
            int tileID = layer.tiles[index];


            int actualID = tileID - layer.firstGid;
            if (actualID < 0 || layer.tilesetCols == 0) continue;

            SDL_Rect src = {
                .x = (actualID % layer.tilesetCols) * layer.tilesetTileWidth,
                .y = (actualID / layer.tilesetCols) * layer.tilesetTileHeight,
                .w = layer.tilesetTileWidth,
                .h = layer.tilesetTileHeight
            };

            SDL_Rect dst = {
                .x = x * layer.mapTileWidth,
                .y = y * layer.mapTileHeight,
                .w = layer.mapTileWidth,
                .h = layer.mapTileHeight
            };

            SDL_RenderCopy(renderer, tileset, &src, &dst);
        }
    }
}
