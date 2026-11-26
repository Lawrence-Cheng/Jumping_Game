#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <ctime>
#include <fstream>
#include "Platform.h"
#include "GameObject.h"


using namespace sf;
using namespace std;

// window 打不開的話可以加上這幾行確保程式正常執行
//template <typename T, typename... Args>
//std::unique_ptr<T> make_unique(Args&&... args) {
//    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
//}


int main() {
    srand(time(0));

    RenderWindow app(VideoMode(400, 533), "Doodle Game!"); // 建立視窗
    app.setFramerateLimit(60); // 建立每秒更新次數

    Texture background, platform, platformMove, platformOnce, charactor, rocketTexture, holeTexture, endBoard, initialBoard; // 讀入圖片
    background.loadFromFile("sources/images/background.png");
    platform.loadFromFile("sources/images/Regular Floor.png");
    platformMove.loadFromFile("sources/images/Left-Right Shift Floor.png");
    platformOnce.loadFromFile("sources/images/Temporary Floor.png");
    charactor.loadFromFile("sources/images/doodle.png");
    rocketTexture.loadFromFile("sources/images/Jetpack.png");
    holeTexture.loadFromFile("sources/images/blackhole.png");
    endBoard.loadFromFile("sources/images/endBackground.jpg");
    initialBoard.loadFromFile("sources/images/Background.jpg");

    Sprite sBackground(background), sPlat(platform), sPlatMove(platformMove), sPlatOnce(platformOnce),  // 設定圖片變數
           sPers(charactor), sRocket(rocketTexture), sHole(holeTexture), sEndBoard(endBoard), iBoard(initialBoard);


    Font font; // 處理輸出字體
    font.loadFromFile("sources/arial.ttf");
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::Black);
    scoreText.setPosition(10, 10);

    int highScore = 0; // 歷史最高分的變數宣告

    ifstream inputFile("sources/highscore.txt"); // 開啟歷史最高分檔案
    if (inputFile.is_open()) {
        inputFile >> highScore; // 成功讀取最高分
        inputFile.close();
    } else {
        cout << "無法讀取歷史最高分，將以 0 為初始值。" << endl; // 檔案不存在或無法開啟的提示
    }

    int x = 100, y = 100, h = 200, counter = 0; // 建立基本遊戲變數
    float dx = 0, dy = 0;
    bool ifRocket = true, ifHole = true;

    vector<unique_ptr<Platform>> platforms; // 建立平台
    unique_ptr<GameObject> rocket = make_unique<Rocket>(rand() % 400, rand() % 533); // 建立火箭
    unique_ptr<GameObject> blackHole = make_unique<BlackHole>(rand() % 400, rand() % 533); // 建立黑洞

// 主迴圈中的更新與繪製部分
bool conti = false, end = false; // 建立遊戲狀態變數
int score = 0; // 建立遊戲分數
while (app.isOpen()) {
    Event e;

    while (app.pollEvent(e)) // 當執行程式被關閉時
    {
        if (e.type == Event::Closed)
            app.close();
    }

    if (conti == false) app.draw(iBoard); // 若尚未繼續執行，印出初始面板

    if (Keyboard::isKeyPressed(Keyboard::Space) && conti == false) // 若按下空白開始執行
    {
        score = 0;
        conti = true;
        counter = 0;
        x = 100; y = 100;
        for (int i = 0; i < 10; i++) { // 初始化平台，依照亂數隨機分配三種平台
            int plat_x = rand() % 400; // 亂數產生 x 座標
            int plat_y = i * (533 / 10); // 亂數產生 y 座標
            int randNum = rand() % 10;
            if (randNum < 2) {
                platforms.push_back(make_unique<OneTimePlatform>(plat_x, plat_y));
            } else if (randNum <= 6) {
                platforms.push_back(make_unique<RegularPlatform>(plat_x, plat_y));
            } else {
                float speedX = (rand() % 2 == 0 ? 1 : -1) * (rand() % 2 + 1);
                platforms.push_back(make_unique<MovingPlatform>(plat_x, plat_y, speedX));
            }
        }
    }

    if (conti == true) // 當遊戲繼續執行時
    {
        if (Keyboard::isKeyPressed(Keyboard::Right)) x += 3; // 左右鍵控制 x 座標
        if (Keyboard::isKeyPressed(Keyboard::Left)) x -= 3;
        if (x > 400) x = 0; // 如超出左右界則跑到另一邊
        if (x < 0) x = 400;

        dy += 0.2; // 內建上升速度
        y += dy;
        score += dy; // 增加分書

        for (auto& plat : platforms) { // 更新水平移動平台之座標
            plat->update_x();
        }

        if(y > 553) // 掉到畫面最下方
        {
            end = true; // 遊戲結束
            conti = false;
            platforms.clear(); // 清空平台
        }

        if (y < h) { // 更新畫面物件（平台、火箭、黑洞）位置
            for (auto& plat : platforms) {
                y = h;
                plat->update(dy);
            }
            rocket->update(dy);
            blackHole->update(dy);
        }

        if(rocket->getY() > 533) // 如火箭超出
            ifRocket = false;
        if(blackHole->getY() > 533)
            ifHole = false;
        // 更新樓梯，檢查並重置超出範圍的樓梯
        for (auto it = platforms.begin(); it != platforms.end();) {
            if ((*it)->getY() > 533) { // 當樓梯超出下邊界
                int plat_x = rand() % 400; // 隨機 X 座標
                int randNum = rand() % 10; // 隨機類型選擇

                // 根據亂數類型生成新樓梯
                if (randNum < 2) {
                    *it = make_unique<OneTimePlatform>(plat_x, 0);
                } else if (randNum <= 6) {
                    *it = make_unique<RegularPlatform>(plat_x, 0);
                } else {
                    float speedX = (rand() % 2 == 0 ? 1 : -1) * (rand() % 2 + 1);
                    *it = make_unique<MovingPlatform>(plat_x, 0, speedX);
                }

                if(rand() % 20 < 1 && ifRocket == false) // 隨機生成火箭
                {
                    rocket = make_unique<Rocket>(rand() % 400, 0);
                    ifRocket = true;
                }
                if(rand() % 7 < 1 && ifHole == false) // 隨機生成黑洞
                {
                    blackHole = make_unique<BlackHole>(rand() % 400, 0);
                    ifHole = true;
                }
            } 
            else {
                ++it; // 繼續處理下一個樓梯
            }
        }

        for (auto& plat : platforms) { //判斷是否碰到平台
            if (plat->checkCollision(x, y, dy)) break;
        }

        if (rocket->checkCollision(x, y)) { // 碰到火箭加速
            dy = -40; 
            counter = 0; // 將counter 重置，進入無敵狀態
        }

        if (blackHole->checkCollision(x, y) && counter > 300) {
            // 玩家掉進黑洞，結束遊戲
            end = true;
            conti = false;
            platforms.clear(); // 清空平台
        }

        sPers.setPosition(x, y); // 建立角色
        // 計分
        scoreText.setString("Score: " + std::to_string(std::abs(score) / 50)); // 建立計分板
        scoreText.setString("Score: " + to_string(abs(score) / 50) + 
                    " High Score: " + to_string(highScore));
        app.clear();
        app.draw(sBackground);
        for (auto& plat : platforms) {
            plat->draw(app, sPlat, sPlatMove, sPlatOnce);
        }
        if(ifRocket == true) rocket->draw(app, sRocket);
        if(ifHole == true) blackHole->draw(app, sHole);
        app.draw(sPers);
        app.draw(scoreText);
        counter++;
    }
    if (conti == false && end == true) {
    if (score / -50 > highScore) { // 若當前分數超過最高分
        highScore = score / -50; // 更新最高分
        ofstream outputFile("sources/highscore.txt"); // 開啟檔案進行儲存
        if (outputFile.is_open()) {
            outputFile << highScore; // 寫入新最高分
            outputFile.close();
        } else {
            cout << "無法儲存歷史最高分。" << endl; // 無法儲存的錯誤提示
        }
    }
    score = 0; // 重置當前分數
    app.draw(sEndBoard); // 顯示遊戲結束畫面
    }
    // 顯示螢幕
    app.display();

    }
    return 0;
}