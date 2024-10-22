#include "pch.h"
#include "App.h"
#include "Window.h"
#include "GDIWindow.h"
#include "Texture.h"
#include "Text.h"
#include "BouncingBall.h"
#include "./CommonStructures.h"

#ifdef _DEBUG
#define DEBUG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <chrono>
#include <iostream>
#include <thread>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <functional> 

#include <SFML/Graphics.hpp>
#include <map>
#include <functional>
#include <iostream>
#include <chrono>
#include <thread>
#include <filesystem>

using std::string;

class Enemy;

std::vector<Enemy> enemyObjects;
sf::RenderWindow* window = nullptr;

constexpr int G_RELOAD_TIME = 3;

class CustomCursor {
public:
    CustomCursor() {
        cursorShape.setRadius(30);
        cursorShape.setFillColor(sf::Color::Cyan); 
    }

    void Update(const sf::Vector2i& mousePosition) {
        cursorShape.setPosition(static_cast<float>(mousePosition.x) - 30, static_cast<float>(mousePosition.y) - 30);
    }

    void Draw(sf::RenderWindow& window) {
        window.draw(cursorShape); 
    }

private:
    sf::CircleShape cursorShape; 
};


enum class SuperShotgunState {
    BarrelFull,
    BarrelEmpty,
    Reloading,
    //Escape,
    //Close
};

class SuperShotgun;

class Timer {
public:
    void Start() {
        startTime = std::chrono::steady_clock::now();
    }

    bool CheckEndTimer(double seconds) {
        currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = currentTime - startTime;
        return elapsed.count() >= seconds;
    }

private:
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point currentTime;
};

class Enemy {
public:
    Enemy(float x, float y) {
        enemyShape.setPosition(x, y);
        enemyShape.setRadius(20.0f); 
        enemyShape.setFillColor(sf::Color::Green); 
        direction = sf::Vector2f(1.0f, 1.0f); 
        speed = 100.0f; 
        timer.Start();
    }

    bool IsHit(const sf::Vector2i& mousePosition) {
        float enemyX = enemyShape.getPosition().x + enemyShape.getRadius();
        float enemyY = enemyShape.getPosition().y + enemyShape.getRadius();

        float distanceX = abs(mousePosition.x - enemyShape.getPosition().x);
        float distanceY = abs(mousePosition.x - enemyShape.getPosition().x);

        if (distanceX <= enemyShape.getRadius() && distanceY <= enemyShape.getRadius()) {
            return true;
        }
        return false;
    }

    void Update(float deltaTime) {
        enemyShape.move(direction * speed * deltaTime);


        if (enemyShape.getPosition().x <= 0 || enemyShape.getPosition().x >= 650 - enemyShape.getRadius() * 2) {
            direction.x = -direction.x; 
        }
        if (enemyShape.getPosition().y <= 0 || enemyShape.getPosition().y >= 400 - enemyShape.getRadius() * 2) {
            direction.y = -direction.y; 
        }

        int random_value = std::rand() % 360;
        //std::cout << "Random value on [0, " << RAND_MAX << "]: " << random_value << '\n';

        if (timer.CheckEndTimer(0.5)) {
            //std::cout << "Change dir";

            float angle = static_cast<float>(random_value);
            direction.x = std::cos(angle * 3.14159f / 180.0f);
            direction.y = std::sin(angle * 3.14159f / 180.0f);

            timer.Start();
        }


    }

    void Draw(sf::RenderWindow& window) {
        window.draw(enemyShape); 
    }

private:
    sf::CircleShape enemyShape; 
    sf::Vector2f direction;     
    float speed;      

    Timer timer;
};


class Action {
public:
    Action(SuperShotgun* super_shotgun) : mSuperShotgun(super_shotgun) {}

    virtual void Start(sf::Event& input) = 0;
    virtual void Update(sf::Event& input) = 0;

protected:
    SuperShotgun* mSuperShotgun;
    Timer timer;
};




class SuperShotgun {
public:
    //
    int m_ammoCount = 2;

    int rpm = 120;

    //
    SuperShotgunState state;
    std::map<SuperShotgunState, Action*> actionMap;
    Action* currentAction = nullptr; 

    void SetActionMap(std::map<SuperShotgunState, Action*>& action) {
        this->actionMap = action;
    }

    void SetState(SuperShotgunState new_state) {
        state = new_state;
        currentAction = actionMap[state];  
        if (currentAction) {
            currentAction->Start(inputEvent);  
        }
    }

    void Update(sf::Event& input) {
        inputEvent = input;
        if (currentAction) {
            currentAction->Update(input);
        }
    }

    std::string GetSuperShotgunStateString(SuperShotgunState state) {
        switch (state) {
        case SuperShotgunState::BarrelFull:
            return "Barrel Full";
        case SuperShotgunState::BarrelEmpty:
            return "Barrel Empty";
        case SuperShotgunState::Reloading:
            return "Reloading";
        default:
            return "Unknown State";
        }
    }

private:
    sf::Event inputEvent;
};


class BarrelEmpty_Action : public Action {
public:
    BarrelEmpty_Action(SuperShotgun* super_shotgun) : Action(super_shotgun) {}

    void Start(sf::Event& input) override {
        std::cout << "SuperShotgun is empty.\n";
    }

    void Update(sf::Event& input) override {
        if (input.key.code == sf::Keyboard::R) {
            mSuperShotgun->SetState(SuperShotgunState::Reloading);
        }
    }
};


class Shoot_Action : public Action {
public:
    Shoot_Action(SuperShotgun* super_shotgun) : Action(super_shotgun) {}

    void Start(sf::Event& input) override {
        std::cout << "SuperShotgun fired.\n";
    }

    void Update(sf::Event& input) override {
        if (input.mouseButton.button == sf::Mouse::Left) {
            if (timer.CheckEndTimer(60.0f / mSuperShotgun->rpm) && mSuperShotgun->m_ammoCount > 0) {
                mSuperShotgun->m_ammoCount -= 1;
                timer.Start(); 

                for (Enemy& enemy : enemyObjects) {
                    sf::Vector2i pos = sf::Mouse::getPosition(*window);
                    if (enemy.IsHit(pos)) {
                        std::cout << "Enemy hit!\n";
                        // #TODO delete enemy 
                        break; 
                    }
                }

                if (mSuperShotgun->m_ammoCount == 0) {
                    mSuperShotgun->SetState(SuperShotgunState::BarrelEmpty);
                }
            }
        }
    }
};


class Reload_Action : public Action {
public:
    Reload_Action(SuperShotgun* super_shotgun) : Action(super_shotgun) {}

    int m_reloadTime = 3;

    void Start(sf::Event& input) override {
        timer.Start();
        std::cout << "Reloading...\n";
    }

    void Update(sf::Event& input) override {
        if (timer.CheckEndTimer(m_reloadTime)) {

            // #TODO allow reloading in BARREL FULL STATE IF ammo count est inferieur a Max ammo 
            mSuperShotgun->SetState(SuperShotgunState::BarrelFull);
            mSuperShotgun->m_ammoCount = 2;
        }
    }

};

class Character {
public:
    SuperShotgun* m_superShotgun = nullptr;
    int m_maxHealth = 100;
    float m_currentHealth;

    Character() {
        m_currentHealth = 100;
        m_superShotgun = new SuperShotgun();
    }




};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd) {

    window = new sf::RenderWindow(sf::VideoMode(650, 400), "SuperShotgun Visualizer");

    AllocConsole();
    FILE* pFile;
    freopen_s(&pFile, "CONOUT$", "w", stdout);
    freopen_s(&pFile, "CONOUT$", "w", stderr);

    string barrel_empty_path = "C:\\Users\\wchapron\\source\\BarrelEmpty.png";
    string barrel_full_path = "C:\\Users\\wchapron\\source\\BarrelFull.png";
    string clicking_path = "C:\\Users\\wchapron\\source\\Clicking.png";
    string reloading_path = "C:\\Users\\wchapron\\source\\Reloading.png";
    sf::Texture barrel_empty_texture;
    sf::Texture barrel_full_texture;
    sf::Texture clicking_texture;
    sf::Texture reloading_texture;

    if (!barrel_empty_texture.loadFromFile(barrel_empty_path))
    {
        std::cerr << "Failed to load barrel_empty_texture" << std::endl;
    }
    if (!barrel_full_texture.loadFromFile(barrel_full_path))
    {
        std::cerr << "Failed to load barrel_full_texture" << std::endl;
    }
    if (!clicking_texture.loadFromFile(clicking_path))
    {
        std::cerr << "Failed to load clicking_texture" << std::endl;
    }
    if (!reloading_texture.loadFromFile(reloading_path))
    {
        std::cerr << "Failed to load reloading_texture" << std::endl;
    }

    sf::Sprite barrel_empty_sprite(barrel_empty_texture);
    sf::Sprite barrel_full_sprite(barrel_full_texture);
    sf::Sprite clicking_sprite(clicking_texture);
    sf::Sprite reloading_sprite(reloading_texture);

    barrel_empty_sprite.setScale(0.52f, 0.56f);
    barrel_full_sprite.setScale(0.52f, 0.56f);
    clicking_sprite.setScale(0.52f, 0.56f);
    reloading_sprite.setScale(0.52f, 0.56f);

    barrel_empty_sprite.setPosition(0, 0);
    barrel_full_sprite.setPosition(0, 0);
    clicking_sprite.setPosition(0, 0);
    reloading_sprite.setPosition(0, 0);

    sf::Font font;
    if (!font.loadFromFile("C:\\Users\\wchapron\\source\\Hack-Regular.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
    }


    // State text
    sf::Text stateText;
    stateText.setFont(font);
    stateText.setCharacterSize(36);
    stateText.setFillColor(sf::Color::White);
    stateText.setPosition(10, 10);

    Character* character = new Character();

    SuperShotgun* superShotgun = character->m_superShotgun;

    BarrelEmpty_Action* barrelEmptyAction = new BarrelEmpty_Action(superShotgun);
    Shoot_Action* shootAction = new Shoot_Action(superShotgun);
    Reload_Action* reloadAction = new Reload_Action(superShotgun);

    std::map<SuperShotgunState, Action*> actionMap = {
        { SuperShotgunState::BarrelEmpty, barrelEmptyAction },
        { SuperShotgunState::BarrelFull, shootAction },
        { SuperShotgunState::Reloading, reloadAction }
    };

    superShotgun->SetActionMap(actionMap);
    superShotgun->SetState(SuperShotgunState::BarrelFull);

    //
    sf::RectangleShape healthBar;
    sf::RectangleShape border;

    healthBar.setSize(sf::Vector2f(202, 22));
    healthBar.setPosition(300, 10);
    healthBar.setFillColor(sf::Color::Red);


    border.setSize(sf::Vector2f(202, 22)); 
    border.setPosition(300, 10);
    border.setFillColor(sf::Color::Transparent); 
    border.setOutlineThickness(2); 
    border.setOutlineColor(sf::Color::White); 

    // Ammo
    sf::Text ammoText;
    ammoText.setFont(font);
    ammoText.setCharacterSize(24);
    ammoText.setFillColor(sf::Color::White);
    ammoText.setPosition(400, 40); 

    ammoText.setString("Munitions: " + std::to_string(2));


    // Enemy

    std::srand(static_cast<unsigned>(std::time(0))); 


    enemyObjects.push_back(Enemy(100, 100));
    enemyObjects.push_back(Enemy(200, 200)); 

    CustomCursor customCursor;

    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }


        }

        superShotgun->Update(event);

        stateText.setString(superShotgun->GetSuperShotgunStateString(superShotgun->state));

        window->clear();

        switch (superShotgun->state)
        {
        case SuperShotgunState::BarrelFull:
            window->draw(barrel_full_sprite);
            break;
        case SuperShotgunState::BarrelEmpty:
            window->draw(barrel_empty_sprite);
            break;
        case SuperShotgunState::Reloading:
            window->draw(reloading_sprite);
            break;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            character->m_currentHealth -= 0.001  ; 


            float healthPercentage = static_cast<float>(character->m_currentHealth) / 100.0f; 
            float healthBarWidth = healthPercentage * 200.0f; 
            std::cout << "Current Health: " << character->m_currentHealth << std::endl;

            healthBar.setSize(sf::Vector2f(healthBarWidth, 20)); 
            healthBar.setPosition(300, 10);
            healthBar.setFillColor(sf::Color::Red);

        }


        ammoText.setString("Munitions: " + std::to_string(superShotgun->m_ammoCount));

        // Enemy
        static sf::Clock clock;
        float deltaTime = clock.restart().asSeconds();

        for (Enemy& enemy : enemyObjects) {
            enemy.Update(deltaTime);
            enemy.Draw(*window);
        }


        sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

        customCursor.Update(mousePosition);
        customCursor.Draw(*window);



        window->draw(healthBar);
        window->draw(stateText);
        window->draw(border);
        window->draw(ammoText);
        window->display();

        
    }

    return 0;
}



//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd) {
//    #ifdef _DEBUG
//        AllocConsole();
//        FILE* pFile;
//        freopen_s(&pFile, "CONOUT$", "w", stdout);
//        freopen_s(&pFile, "CONOUT$", "w", stderr);
//    #endif
//
//
//    App* app = new App();
//    app->Initialize("Hello World", 800, 600, hInstance);
//
//    // Res init
//    HWND hwnd = app->GetWindow()->GetHWND();
//    RECT clientRect;
//    GetClientRect(hwnd, &clientRect);
//    int width = clientRect.right - clientRect.left;
//    int height = clientRect.bottom - clientRect.top;
//
//    app->LoadTexture("..\\..\\..\\src\\textures\\mario.bmp", hwnd); // #IMPORTANT replace "mario" by "magenta", in GDI Version it will draw transparent the pure magenta pixels of the texture
//
//    std::string fontPath = "..\\..\\..\\font\\arial.ttf";
//    app->LoadFont(fontPath);
//
//    // Text init
//    std::wstring currentFPSText = L"" + std::to_wstring(app->GetCurrentFPS());
//    Text* currentFPSTextObject = app->CreateText(app->GetFontById(0), currentFPSText);
//    std::wstring targetMSText = L"" + std::to_wstring(app->GetTargetMS());
//    Text* targetMSTextObject = app->CreateText(app->GetFontById(0), targetMSText);
//    std::wstring targetFPSText = L"" + std::to_wstring(app->GetTargetFPS());
//    Text* targetFPSTextObject = app->CreateText(app->GetFontById(0), targetFPSText);
//    std::wstring deltaTimeText = L"" + std::to_wstring(app->GetDeltaTime());
//    Text* deltaTimeTextObject = app->CreateText(app->GetFontById(0), deltaTimeText);
//    std::wstring totalBallsText = L"";
//    Text* totalBallsTextObject = app->CreateText(app->GetFontById(0), totalBallsText);
//    std::wstring currentBallsText = L"";
//    Text* currentBallsTextObject = app->CreateText(app->GetFontById(0), currentBallsText);
//
//    unsigned int currentFPSColor[3] = { 255, 255, 255 }; 
//    unsigned int targetFPSColor[3] = { 173, 216, 230 }; 
//    unsigned int targetMSColor[3] = { 144, 238, 144 }; 
//    unsigned int deltaTimeColor[3] = { 255, 228, 196 }; 
//    unsigned int totalBallsColor[3] = { 255, 255, 224 }; 
//    unsigned int currentBallsColor[3] = { 255, 182, 193 }; 
//
//    DrawCommand currentFPSCommand;
//    currentFPSCommand.x = 30.0f;
//    currentFPSCommand.y = 110.0f;
//    currentFPSCommand.type = DrawType::Text;
//    currentFPSCommand.text = currentFPSTextObject;
//    currentFPSCommand.color[0] = currentFPSColor[0];
//    currentFPSCommand.color[1] = currentFPSColor[1];
//    currentFPSCommand.color[2] = currentFPSColor[2];
//
//    DrawCommand targetFPSCommand;
//    targetFPSCommand.x = 30.0f;
//    targetFPSCommand.y = 150.0f;
//    targetFPSCommand.type = DrawType::Text;
//    targetFPSCommand.text = targetFPSTextObject;
//    targetFPSCommand.color[0] = targetFPSColor[0];
//    targetFPSCommand.color[1] = targetFPSColor[1];
//    targetFPSCommand.color[2] = targetFPSColor[2];
//
//    DrawCommand targetMSCommand;
//    targetMSCommand.x = 30.0f;
//    targetMSCommand.y = 190.0f;
//    targetMSCommand.type = DrawType::Text;
//    targetMSCommand.text = targetMSTextObject;
//    targetMSCommand.color[0] = targetMSColor[0];
//    targetMSCommand.color[1] = targetMSColor[1];
//    targetMSCommand.color[2] = targetMSColor[2];
//
//    DrawCommand deltaTimeCommand;
//    deltaTimeCommand.x = 30.0f;
//    deltaTimeCommand.y = 230.0f;
//    deltaTimeCommand.type = DrawType::Text;
//    deltaTimeCommand.text = deltaTimeTextObject;
//    deltaTimeCommand.color[0] = deltaTimeColor[0];
//    deltaTimeCommand.color[1] = deltaTimeColor[1];
//    deltaTimeCommand.color[2] = deltaTimeColor[2];
//
//    DrawCommand totalBallsCommand;
//    totalBallsCommand.x = 30.0f;
//    totalBallsCommand.y = 270.0f;
//    totalBallsCommand.type = DrawType::Text;
//    totalBallsCommand.text = totalBallsTextObject;
//    totalBallsCommand.color[0] = totalBallsColor[0];
//    totalBallsCommand.color[1] = totalBallsColor[1];
//    totalBallsCommand.color[2] = totalBallsColor[2];
//
//    DrawCommand currentBallsCommand;
//    currentBallsCommand.x = 30.0f;
//    currentBallsCommand.y = 310.0f;
//    currentBallsCommand.type = DrawType::Text;
//    currentBallsCommand.text = currentBallsTextObject;
//    currentBallsCommand.color[0] = currentBallsColor[0];
//    currentBallsCommand.color[1] = currentBallsColor[1];
//    currentBallsCommand.color[2] = currentBallsColor[2];
//
//    // Init bouncing gameplay system
//    BouncingBall ball;
//    ball.BallAppear(width, height);
//
//    auto lastBallAppearTime = std::chrono::steady_clock::now();
//    const int ballAppearInterval = 150;
//
//    // Gameplay loop
//    while (true) {
//        // Run render API (this manages the fps control itself)
//        if (!app->Run()) {
//            break;
//        }
//
//        // Key input for change fps
//        if (GetAsyncKeyState('1') & 0x8000) {
//            app->SetFPS(30);
//        }
//        if (GetAsyncKeyState('2') & 0x8000) {
//            app->SetFPS(60);
//        }
//        if (GetAsyncKeyState('3') & 0x8000) {
//            app->SetFPS(120);
//        }
//        if (GetAsyncKeyState('4') & 0x8000) {
//            app->SetFPS(10);
//        }
//
//        // Manage appear time interval
//        auto currentTime = std::chrono::steady_clock::now();
//        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastBallAppearTime).count() >= ballAppearInterval) {
//            ball.BallAppear(width, height);
//            lastBallAppearTime = currentTime;
//        }
//
//        ball.BallMoving(width, height);
//
//        // For each ball, add draw command object
//        std::vector<Ball> circleShapes = ball.GetAllBalls();
//        for (const auto& circleShape : circleShapes) {
//            DrawCommand command;
//            command.texture = app->GetTextureById(0);
//            command.x = circleShape.x;
//            command.y = circleShape.y;
//            command.radius = circleShape.radius;
//            command.type = DrawType::Circle;
//            app->AddDrawCommand(command);
//        }
//
//        // Update texts values & command
//        std::wstring updatedFPSText = L"Current FPS : " + std::to_wstring(app->GetCurrentFPS());
//        currentFPSTextObject->SetText(updatedFPSText);
//        std::wstring updatedTargetFPSText = L"FPS Limit : " + std::to_wstring(app->GetTargetFPS());
//        targetFPSTextObject->SetText(updatedTargetFPSText);
//        std::wstring updatedTargetMSText = L"Target MS : " + std::to_wstring(app->GetTargetMS());
//        targetMSTextObject->SetText(updatedTargetMSText);
//        std::wstring updatedDeltaTimeText = L"Delta Time : " + std::to_wstring(app->GetDeltaTime());
//        deltaTimeTextObject->SetText(updatedDeltaTimeText);
//        std::wstring updatedTotalBallsText = L"Total Balls : " + std::to_wstring(ball.GetTotalBallsGenerated());
//        totalBallsTextObject->SetText(updatedTotalBallsText);
//        std::wstring updatedCurrentBallsText = L"Balls On Screen : " + std::to_wstring(ball.GetCurrentBallsOnScreen());
//        currentBallsTextObject->SetText(updatedCurrentBallsText);
//
//        app->AddDrawCommand(currentFPSCommand);
//        app->AddDrawCommand(targetFPSCommand);
//        app->AddDrawCommand(targetMSCommand);
//        app->AddDrawCommand(deltaTimeCommand);
//        app->AddDrawCommand(totalBallsCommand);
//        app->AddDrawCommand(currentBallsCommand);
//    }
//
//    OutputDebugStringA("End app");
//
//    #ifdef _DEBUG
//        _CrtDumpMemoryLeaks();
//    #endif
//
//    return 0;
//}
