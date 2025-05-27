#include "../headers/Entity.hpp"

Entity::Entity(int x, int y, int w, int h, SDL_Texture* texture, bool movable)
    : rect{ x, y, w, h },
      srcRect{ 0, 0, w, h },
      texture(texture),
      movable(movable) {}


Entity::Entity(int x, int y, int w, int h,
           int srcX, int srcY, int srcW, int srcH,
           SDL_Texture* texture, bool movable)
        : rect{ x, y, w, h }, srcRect{ srcX, srcY, srcW, srcH }, texture(texture), movable(movable) {}


void Entity::render(SDL_Renderer* renderer) {
    // Couleur temporaire : rouge (r=255, g=0, b=0, a=255)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);

    // Facultatif : contour noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

void Entity::setVelocity(float newVx, float newVy) {
    vx = newVx;
    vy = newVy;
}

void Entity::update(float deltaTime) {
    if (movable) {
        rect.x += static_cast<int>(vx * deltaTime);
        rect.y += static_cast<int>(vy * deltaTime);
    }
}


void Entity::move(int dx, int dy) {
    if (movable) {
        rect.x += dx;
        rect.y += dy;
    }
}

void Entity::setPosition(int x, int y) {
    rect.x = x;
    rect.y = y;
}

SDL_Rect Entity::getRect() const {
    return rect;
}

bool Entity::isMovable() const {
    return movable;
}
