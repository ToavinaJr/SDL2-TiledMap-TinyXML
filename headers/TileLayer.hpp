#pragma once

#include <string>
#include <vector>
#include <optional>
#include <SDL2/SDL.h>

class TileLayer {
public:
    int mapWidth = 0;
    int mapHeight = 0;
    int mapTileWidth = 0;
    int mapTileHeight = 0;

    int tilesetTileWidth = 0;
    int tilesetTileHeight = 0;
    int tilesetCols = 0;

    int firstGid = 0;
    std::string tilesetImagePath;
    std::vector<int> tiles;

    bool isValid() const;
};
