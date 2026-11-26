#include <SFML/Graphics.hpp>
#include <memory>

using namespace std;
using namespace sf;

// 抽象基類：平台
class Platform {
protected:
    int x, y; // 座標
    bool isActive; // 開平台是否是啟用的

public:
    Platform(int x, int y) : x(x), y(y), isActive(true) {} // constructor
    virtual ~Platform() {} // destructor
    virtual void update(float dy) { y -= dy; } // 更新平台的 y 座標
    virtual void update_x() {} // 更新平台的 x 座標
    virtual void draw(RenderWindow& app, Sprite& regularSprite, Sprite& moveSprite, Sprite& onceSprite) = 0; // 繪製平台
    virtual bool checkCollision(int playerX, int playerY, float& dy) = 0; // 判斷是否碰到平台
    void deactivate() { isActive = false; } // 將平台改為不啟用
    bool isPlatformActive() const { return isActive; } // 取得該平台是否啟用的資訊
    int getY() const { return y; } // 取的該平台的 y 座標
};

// 普通平台
class RegularPlatform : public Platform {
public:
    RegularPlatform(int x, int y) : Platform(x, y) {}

    void draw(RenderWindow& app, Sprite& regularSprite, Sprite&, Sprite&) override { // 繪製普通平台的圖
        regularSprite.setPosition(x, y);
        app.draw(regularSprite);
    }

    bool checkCollision(int playerX, int playerY, float& dy) override { // 判斷普通平台是否被碰到。是的話則瞬間彈跳
        if ((playerX + 50 > x) && (playerX + 20 < x + 68) &&
            (playerY + 70 > y) && (playerY + 70 < y + 14) && (dy > 0)) {
            dy = -10;
            return true;
        }
        return false;
    }
};

// 移動平台
class MovingPlatform : public Platform {
    float speedX; // 紀錄移動平台的移動速度與方向

public:
    MovingPlatform(int x, int y, float speedX) 
        : Platform(x, y), speedX(speedX) {}

    void update(float dy) override {
        y -= dy;
    }

    void update_x() override { // 更新移動平台的 x 座標
        x += speedX;
        if (x < 0 || x > 400) speedX = -speedX;
    }

    void draw(RenderWindow& app, Sprite&, Sprite& moveSprite, Sprite&) override { // 繪製移動平台的圖
        moveSprite.setPosition(x, y);
        app.draw(moveSprite);
    }

    bool checkCollision(int playerX, int playerY, float& dy) override { // 判斷普通平台是否被碰到。是的話則瞬間彈跳
        if ((playerX + 50 > x) && (playerX + 20 < x + 68) &&
            (playerY + 70 > y) && (playerY + 70 < y + 14) && (dy > 0)) {
            dy = -10;
            return true;
        }
        return false;
    }
};

// 一次性平台
class OneTimePlatform : public Platform {
public:
    OneTimePlatform(int x, int y) : Platform(x, y) {}

    void draw(RenderWindow& app, Sprite&, Sprite&, Sprite& onceSprite) override { // 繪製一次性平台的圖
        if (isActive) {
            onceSprite.setPosition(x, y);
            app.draw(onceSprite);
        }
    }

    bool checkCollision(int playerX, int playerY, float& dy) override { // 判斷普通平台是否被碰到。是的話則瞬間彈跳且把是否啟用改為否
        if ((playerX + 50 > x) && (playerX + 20 < x + 68) &&
            (playerY + 70 > y) && (playerY + 70 < y + 14) && (dy > 0) && isActive == true) {
            dy = -10;
            isActive = false;
            return true;
        }
        return false;
    }
};