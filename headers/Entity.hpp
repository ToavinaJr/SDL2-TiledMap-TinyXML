#pragma once

#include <SDL2/SDL.h>
#include <string>

class Entity {
public:
    Entity(int x, int y, int w, int h, SDL_Texture* texture, bool movable);
    Entity(int x, int y, int w, int h,
        int srcX, int srcY, int srcW, int srcH,
        SDL_Texture* texture, bool movable);
    void render(SDL_Renderer* renderer);
    void move(int dx, int dy);  // déplacement simple
    void setPosition(int x, int y);
    
    void update(float deltaTime);
    void setVelocity(float vx, float vy);

    SDL_Rect getRect() const;
    bool isMovable() const;
    float getSpeed() const { return speed; }
    void setSpeed(float newSpeed) { speed = newSpeed; }

private:
    SDL_Rect rect;
    SDL_Rect srcRect;
    SDL_Texture* texture;
    bool movable;
    float vx = 0.0f, vy = 0.0f;
    float speed = 100.0f;
};
