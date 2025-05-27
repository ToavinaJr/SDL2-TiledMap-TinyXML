#include "../headers/TileLayer.hpp"

bool TileLayer::isValid() const {
    return mapWidth > 0 && mapHeight > 0 && !tilesetImagePath.empty() && !tiles.empty();
}
