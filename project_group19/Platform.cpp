#include "Platform.h"

using namespace std;
using namespace sf;

Platform::Platform(int x, int y) : x(x), y(y), isActive(true) {}
Platform::~Platform() {}
void Platform::update(float dy) { y -= dy; }
void Platform::update_x() {}
void Platform::deactivate() { isActive = false; }
bool Platform::isPlatformActive() const { return isActive; }
int Platform::getY() const { return y; }

RegularPlatform::RegularPlatform(int x, int y) : Platform(x, y) {}
void RegularPlatform::draw(sf::RenderWindow& app, sf::Sprite& regularSprite, sf::Sprite&, sf::Sprite&) {
    regularSprite.setPosition(x, y);
    app.draw(regularSprite);
}
bool RegularPlatform::checkCollision(int playerX, int playerY, float& dy) {
    if ((playerX + 50 > x) && (playerX + 20 < x + 68) &&
        (playerY + 70 > y) && (playerY + 70 < y + 14) && (dy > 0)) {
        dy = -10;
        return true;
    }
    return false;
}

MovingPlatform::MovingPlatform(int x, int y, float speedX) : Platform(x, y), speedX(speedX) {}
void MovingPlatform::update_x() {
    x += speedX;
    if (x < 0 || x > 400) speedX = -speedX;
}
void MovingPlatform::draw(sf::RenderWindow& app, sf::Sprite&, sf::Sprite& moveSprite, sf::Sprite&) {
    moveSprite.setPosition(x, y);
    app.draw(moveSprite);
}
bool MovingPlatform::checkCollision(int playerX, int playerY, float& dy) {
    return MovingPlatform::checkCollision(playerX, playerY, dy);
}

OneTimePlatform::OneTimePlatform(int x, int y) : Platform(x, y) {}
void OneTimePlatform::draw(sf::RenderWindow& app, sf::Sprite&, sf::Sprite&, sf::Sprite& onceSprite) {
    if (isActive) {
        onceSprite.setPosition(x, y);
        app.draw(onceSprite);
    }
}
bool OneTimePlatform::checkCollision(int playerX, int playerY, float& dy) {
    if (OneTimePlatform::checkCollision(playerX, playerY, dy) && isActive) {
        isActive = false;
        return true;
    }
    return false;
}
