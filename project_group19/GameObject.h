#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// 通用基類：遊戲物件
class GameObject {
protected:
    int x, y; // 座標

public:
    GameObject(int x, int y) : x(x), y(y) {} // constructor
    virtual ~GameObject() {} // destructor
    virtual void update(float dy) { y -= dy; } // 更新垂直座標
    virtual void draw(RenderWindow& app, Sprite& sprite) = 0; // 將建立物件圖像
    virtual bool checkCollision(int playerX, int playerY) = 0; // 判斷玩家是否有碰到
    int getY() { return y;} // 取得該物件垂直座標
};

// 特殊物件：火箭
class Rocket : public GameObject {
public:
    Rocket(int x, int y) : GameObject(x, y) {}

    void draw(RenderWindow& app, Sprite& sprite) override { // 建立火箭圖像
        sprite.setPosition(x, y);
        app.draw(sprite);
    }

    bool checkCollision(int playerX, int playerY) override { // 判斷玩家是否有碰到火箭
        if ((playerX + 50 > x) && (playerX + 20 < x + 40) &&
            (playerY + 70 > y) && (playerY + 70 < y + 60)) {
            return true;
        }
        return false;
    }
};

// 特殊物件：黑洞
class BlackHole : public GameObject {
public:
    BlackHole(int x, int y) : GameObject(x, y) {}

    void draw(RenderWindow& app, Sprite& sprite) override { // 建立黑洞圖像
        sprite.setPosition(x, y);
        app.draw(sprite);
    }

    bool checkCollision(int playerX, int playerY) override {  // 判斷是否有碰到黑洞
        if ((playerX + 50 > x) && (playerX + 20 < x + 80) &&
            (playerY + 70 > y) && (playerY + 70 < y + 70)) {
            return true;
        }
        return false;
    }
};
