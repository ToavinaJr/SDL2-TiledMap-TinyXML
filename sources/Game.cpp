#include "Game.hpp"
#include <iostream>
#include <SDL2/SDL_image.h>

Game::Game(const char* MAP_FILE) {
    auto tileLayerOpt = TMXLoader::loadFromFile(MAP_FILE);
    
    if (!tileLayerOpt) {
        std::cerr << "Erreur chargement de la carte TMX (initial)\n";
        throw std::runtime_error("Erreur chargement TMX (initial)");
    }
    tileLayer = std::move(*tileLayerOpt);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Erreur SDL_Init : " << SDL_GetError() << '\n';
        throw std::runtime_error("Erreur SDL_Init");
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Erreur IMG_Init : " << IMG_GetError() << '\n';
        SDL_Quit();
        throw std::runtime_error("Erreur IMG_Init");
    }
    
    windowWidth = tileLayer.mapWidth * tileLayer.mapTileWidth;
    windowHeight = tileLayer.mapHeight * tileLayer.mapTileHeight;

    window = SDL_CreateWindow(
        "Carte TMX",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Erreur création fenêtre : " << SDL_GetError() << '\n';
        IMG_Quit();
        SDL_Quit();
        throw std::runtime_error("Erreur création fenêtre");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Erreur création renderer : " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        throw std::runtime_error("Erreur création renderer");
    }

    tilesetTexture.reset(Renderer::loadTileset(renderer, tileLayer.tilesetImagePath));
    if (!tilesetTexture) {
        std::cerr << "Erreur chargement tileset\n";
        throw std::runtime_error("Erreur chargement tileset");
    }
    
    entities.emplace_back(100, 100, 32, 32, nullptr, true);  // Pas besoin de texture ici

}

Game::~Game() {
    cleanup();
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 92, 148, 252, 255);
    SDL_RenderClear(renderer);
    Renderer::render(renderer, tilesetTexture.get(), tileLayer);

    for (auto& entity : entities) {
        entity.render(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::run() {
    isRunning = true;
    SDL_Event event;
    Uint32 lastTime = SDL_GetTicks();

    while (isRunning) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 500.0f;
        lastTime = currentTime;

        // Gestion événements clavier
        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        float vx = 0.0f, vy = 0.0f;

        if (keystates[SDL_SCANCODE_UP])    vy = -entities[0].getSpeed();
        if (keystates[SDL_SCANCODE_DOWN])  vy = entities[0].getSpeed();
        if (keystates[SDL_SCANCODE_LEFT])  vx = -entities[0].getSpeed();
        if (keystates[SDL_SCANCODE_RIGHT]) vx = entities[0].getSpeed();

        entities[0].setVelocity(vx, vy);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        for (auto& entity : entities) {
            entity.update(deltaTime);
        }

        render();
    }
}

void Game::cleanup() {
    if (entityTexture) {
        SDL_DestroyTexture(entityTexture);
        entityTexture = nullptr;
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    IMG_Quit();
    SDL_Quit();
}
#include "Game.hpp"
#include <iostream>
#include <SDL2/SDL_image.h>

Game::Game(const char* MAP_FILE) {
    auto tileLayerOpt = TMXLoader::loadFromFile(MAP_FILE);
    
    if (!tileLayerOpt) {
        std::cerr << "Erreur chargement de la carte TMX (initial)\n";
        throw std::runtime_error("Erreur chargement TMX (initial)");
    }
    tileLayer = std::move(*tileLayerOpt);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Erreur SDL_Init : " << SDL_GetError() << '\n';
        throw std::runtime_error("Erreur SDL_Init");
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Erreur IMG_Init : " << IMG_GetError() << '\n';
        SDL_Quit();
        throw std::runtime_error("Erreur IMG_Init");
    }
    
    windowWidth = tileLayer.mapWidth * tileLayer.mapTileWidth;
    windowHeight = tileLayer.mapHeight * tileLayer.mapTileHeight;

    window = SDL_CreateWindow(
        "Carte TMX",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Erreur création fenêtre : " << SDL_GetError() << '\n';
        IMG_Quit();
        SDL_Quit();
        throw std::runtime_error("Erreur création fenêtre");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Erreur création renderer : " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        throw std::runtime_error("Erreur création renderer");
    }

    tilesetTexture.reset(Renderer::loadTileset(renderer, tileLayer.tilesetImagePath));
    if (!tilesetTexture) {
        std::cerr << "Erreur chargement tileset\n";
        throw std::runtime_error("Erreur chargement tileset");
    }
    
    // Exemple d'entité à position (64, 64)
    SDL_Surface* entSurface = IMG_Load("res/tilesets/sprite-mario.jpeg");
    if (!entSurface) {
        std::cerr << "Erreur chargement image entité : " << IMG_GetError() << '\n';
        throw std::runtime_error("Erreur chargement image entité");
    }
    entityTexture = SDL_CreateTextureFromSurface(renderer, entSurface);
    SDL_FreeSurface(entSurface);

    if (!entityTexture) {
        std::cerr << "Erreur création texture entité : " << SDL_GetError() << '\n';
        throw std::runtime_error("Erreur texture entité");
    }

    entities.emplace_back(64, 64, 32, 32, entityTexture, true); // joueur mobile

}

Game::~Game() {
    cleanup();
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 92, 148, 252, 255);
    SDL_RenderClear(renderer);
    Renderer::render(renderer, tilesetTexture.get(), tileLayer);

    for (auto& entity : entities) {
        entity.render(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::run() {
    isRunning = true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP: entities[0].move(0, -tileLayer.mapTileHeight); break;
                    case SDLK_DOWN: entities[0].move(0, tileLayer.mapTileHeight); break;
                    case SDLK_LEFT: entities[0].move(-tileLayer.mapTileWidth, 0); break;
                    case SDLK_RIGHT: entities[0].move(tileLayer.mapTileWidth, 0); break;
                }
            }
        }

        render();
    }
}

void Game::cleanup() {
    if (entityTexture) {
        SDL_DestroyTexture(entityTexture);
        entityTexture = nullptr;
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    IMG_Quit();
    SDL_Quit();
}