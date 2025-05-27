#pragma once

#include <SDL2/SDL.h>
#include <string>

class Entity {
public:
    Entity(int x, int y, int w, int h, SDL_Texture* texture, bool movable = false);

    void render(SDL_Renderer* renderer);
    void move(int dx, int dy);  // déplacement simple
    void setPosition(int x, int y);

    SDL_Rect getRect() const;
    bool isMovable() const;

private:
    SDL_Rect rect;
    SDL_Texture* texture;
    bool movable;
};
