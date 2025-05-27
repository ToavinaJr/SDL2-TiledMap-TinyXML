#pragma once

#include "TileLayer.hpp"
#include <string>
#include <optional>

class TMXLoader {
public:
    static std::optional<TileLayer> loadFromFile(const std::string& tmxFilePath);
};
