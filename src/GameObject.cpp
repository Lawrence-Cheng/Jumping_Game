#include "GameObject.h"

using namespace std;
using namespace sf;

GameObject::GameObject(int x, int y) : x(x), y(y) {}
GameObject::~GameObject() {}
void GameObject::update(float dy) { y -= dy; }
void draw(RenderWindow& app, Sprite& sprite) = 0;
virtual bool checkCollision(int playerX, int playerY) = 0;
int GameObject::getY() { return y; }

Rocket::Rocket(int x, int y) : GameObject(x, y) {}
Rocket::~Rocket() {}
void Rocket::draw(sf::RenderWindow& app, sf::Sprite& sprite) {
    sprite.setPosition(x, y);
    app.draw(sprite);
}
bool Rocket::checkCollision(int playerX, int playerY) {
    return (playerX + 50 > x) && (playerX + 20 < x + 40) &&
           (playerY + 70 > y) && (playerY + 70 < y + 60);
}

BlackHole::BlackHole(int x, int y) : GameObject(x, y) {}
BlackHole~BlackHole() {}
void BlackHole::draw(sf::RenderWindow& app, sf::Sprite& sprite) {
    sprite.setPosition(x, y);
    app.draw(sprite);
}
bool BlackHole::checkCollision(int playerX, int playerY) {
    return (playerX + 50 > x) && (playerX + 20 < x + 80) &&
           (playerY + 70 > y) && (playerY + 70 < y + 70);
}

