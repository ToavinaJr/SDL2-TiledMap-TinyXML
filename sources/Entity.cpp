#include "../headers/Entity.hpp"

Entity::Entity(int x, int y, int w, int h, SDL_Texture* texture, bool movable)
    : rect{ x, y, w, h }, texture(texture), movable(movable) {}

void Entity::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
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
