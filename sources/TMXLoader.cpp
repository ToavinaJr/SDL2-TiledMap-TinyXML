#include "../headers/TMXLoader.hpp"

#include <algorithm>
#include <cctype>
#include <tinyxml2.h>
#include <filesystem>
#include <sstream>
#include <iostream>
#include <ranges>

namespace fs = std::filesystem;
using namespace tinyxml2;

std::optional<TileLayer> TMXLoader::loadFromFile(const std::string& tmxFilePath) {
    TileLayer layer;
    XMLDocument doc;

    if (doc.LoadFile(tmxFilePath.c_str()) != XML_SUCCESS) {
        std::cerr << "Erreur de chargement du fichier TMX : " << doc.ErrorStr() << '\n';
        return std::nullopt;
    }

    auto* map = doc.FirstChildElement("map");
    if (!map) return std::nullopt;

    map->QueryIntAttribute("width", &layer.mapWidth);
    map->QueryIntAttribute("height", &layer.mapHeight);
    map->QueryIntAttribute("tilewidth", &layer.mapTileWidth);
    map->QueryIntAttribute("tileheight", &layer.mapTileHeight);

    auto* tileset = map->FirstChildElement("tileset");
    if (!tileset) return std::nullopt;

    tileset->QueryIntAttribute("firstgid", &layer.firstGid);

    const char* source = tileset->Attribute("source");
    fs::path baseDir = fs::path(tmxFilePath).parent_path();

    if (source) {
        fs::path tsxPath = baseDir / source;
        XMLDocument tsxDoc;
        if (tsxDoc.LoadFile(tsxPath.c_str()) != XML_SUCCESS) return std::nullopt;

        auto* tsElem = tsxDoc.FirstChildElement("tileset");
        if (!tsElem) return std::nullopt;

        tsElem->QueryIntAttribute("tilewidth", &layer.tilesetTileWidth);
        tsElem->QueryIntAttribute("tileheight", &layer.tilesetTileHeight);

        auto* image = tsElem->FirstChildElement("image");
        if (!image || !image->Attribute("source")) return std::nullopt;

        layer.tilesetImagePath = (tsxPath.parent_path() / image->Attribute("source")).string();

        int imgWidth = 0;
        image->QueryIntAttribute("width", &imgWidth);
        if (layer.tilesetTileWidth > 0)
            layer.tilesetCols = imgWidth / layer.tilesetTileWidth;
    } else {
        tileset->QueryIntAttribute("tilewidth", &layer.tilesetTileWidth);
        tileset->QueryIntAttribute("tileheight", &layer.tilesetTileHeight);

        auto* image = tileset->FirstChildElement("image");
        if (!image || !image->Attribute("source")) return std::nullopt;

        layer.tilesetImagePath = (baseDir / image->Attribute("source")).string();

        int imgWidth = 0;
        image->QueryIntAttribute("width", &imgWidth);
        if (layer.tilesetTileWidth > 0)
            layer.tilesetCols = imgWidth / layer.tilesetTileWidth;
    }

    auto* layerElem = map->FirstChildElement("layer");
    if (!layerElem) return std::nullopt;

    auto* data = layerElem->FirstChildElement("data");
    if (!data || strcmp(data->Attribute("encoding"), "csv") != 0) return std::nullopt;

    std::stringstream ss(data->GetText());
    std::string item;

    while (std::getline(ss, item, ',')) {
        std::ranges::remove_if(item, [](char c){ return std::isspace(c); });
        try {
            if (!item.empty()) {
                int id = std::stoi(item);
                layer.tiles.push_back(id);
            }
        } catch (...) {
            return std::nullopt;
        }
    }

    if (layer.tiles.size() != static_cast<size_t>(layer.mapWidth * layer.mapHeight))
        return std::nullopt;

    return layer;
}
