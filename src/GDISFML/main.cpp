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

using std::string;

#define COOLPURPLE \
    CLITERAL(Color) { 153, 0, 0, 255 }  // cool Purple

constexpr int G_RELOAD_TIME = 3;

//enum class SuperShotgunState
//{
//    BarrelFull,
//    BarrelEmpty,
//    Reloading,
//    Escape,
//    Close,
//};
//
//typedef struct DoubleBarrel
//{
//    SuperShotgunState state;
//
//    std::map<SuperShotgunState, std::function<void(sf::Event&)>> m_actions;
//
//    void SetActions(std::map<SuperShotgunState, std::function<void(sf::Event&)>>& actionsData) {
//        m_actions = actionsData;
//    }
//
//    void ReadCommand(sf::Event& input) {
//        std::cout << (int)state;
//        if (m_actions.find(state) != m_actions.end()) {
//            m_actions[state](input); 
//        }
//        //std::cout << (int)state;
//    }
//
//
//
//    void click()
//    {
//        this->state = SuperShotgunState::BarrelEmpty;
//    }
//
//    void reload()
//    {
//        this->state = SuperShotgunState::Reloading;
//        std::thread start_thread(&DoubleBarrel::_reloadCounter, this);
//        start_thread.detach();
//    }
//
//    SuperShotgunState getState()
//    {
//        return this->state;
//    }
//
//    string getStateStr()
//    {
//        switch (this->state)
//        {
//        case SuperShotgunState::BarrelFull:
//            return "BarrelFull";
//        case SuperShotgunState::BarrelEmpty:
//            return "BarrelEmpty";
//        case SuperShotgunState::Reloading:
//            return "Reloading";
//        default:
//            return "something went horribly wrong";
//        }
//    }
//
//private:
//
//    void _reloadCounter()
//    {
//        std::this_thread::sleep_for(std::chrono::seconds(G_RELOAD_TIME));
//        this->state = SuperShotgunState::BarrelFull;
//    }
//
//} DoubleBarrel;
//
//
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd) {
//
//    // Récupérer le chemin courant
//    std::filesystem::path currentPath = std::filesystem::current_path();
//
//    // Allouer une nouvelle console
//    AllocConsole();
//
//    // Rediriger stdout et stderr vers la console
//    FILE* pFile;
//    freopen_s(&pFile, "CONOUT$", "w", stdout);
//    freopen_s(&pFile, "CONOUT$", "w", stderr);
//
//    // Afficher le chemin courant
//    std::cout << "Current Directory: " << currentPath << std::endl;
//
//    // Libérer les ressources et fermer la console
//
//    DoubleBarrel double_barrel{ .state = SuperShotgunState::BarrelFull };
//
//    sf::RenderWindow window(sf::VideoMode(650, 400), "GamingCampus - SuperShotgunVisualizer");
//
//    string barrel_empty_path = "C:\\Users\\wchapron\\source\\BarrelEmpty.png";
//    string barrel_full_path = "C:\\Users\\wchapron\\source\\BarrelFull.png";
//    string clicking_path = "C:\\Users\\wchapron\\source\\Clicking.png";
//    string reloading_path = "C:\\Users\\wchapron\\source\\Reloading.png";
//    sf::Texture barrel_empty_texture;
//    sf::Texture barrel_full_texture;
//    sf::Texture clicking_texture;
//    sf::Texture reloading_texture;
//
//    if (!barrel_empty_texture.loadFromFile(barrel_empty_path))
//    {
//        std::cerr << "Failed to load barrel_empty_texture" << std::endl;
//    }
//    if (!barrel_full_texture.loadFromFile(barrel_full_path))
//    {
//        std::cerr << "Failed to load barrel_full_texture" << std::endl;
//    }
//    if (!clicking_texture.loadFromFile(clicking_path))
//    {
//        std::cerr << "Failed to load clicking_texture" << std::endl;
//    }
//    if (!reloading_texture.loadFromFile(reloading_path))
//    {
//        std::cerr << "Failed to load reloading_texture" << std::endl;
//    }
//
//    sf::Sprite barrel_empty_sprite(barrel_empty_texture);
//    sf::Sprite barrel_full_sprite(barrel_full_texture);
//    sf::Sprite clicking_sprite(clicking_texture);
//    sf::Sprite reloading_sprite(reloading_texture);
//
//    barrel_empty_sprite.setScale(0.52f, 0.56f);
//    barrel_full_sprite.setScale(0.52f, 0.56f);
//    clicking_sprite.setScale(0.52f, 0.56f);
//    reloading_sprite.setScale(0.52f, 0.56f);
//
//    barrel_empty_sprite.setPosition(0, 0);
//    barrel_full_sprite.setPosition(0, 0);
//    clicking_sprite.setPosition(0, 0);
//    reloading_sprite.setPosition(0, 0);
//
//    sf::Font font;
//    if (!font.loadFromFile("C:\\Users\\wchapron\\source\\Hack-Regular.ttf")) 
//    {
//        std::cerr << "Failed to load font" << std::endl;
//    }
//
//    sf::Text stateText;
//    stateText.setFont(font);
//    stateText.setCharacterSize(36);
//    stateText.setFillColor(sf::Color::White);
//    stateText.setPosition(10, 10);
//
//    // Link actions to event 
//
//    std::map<SuperShotgunState, std::function<void(sf::Event&)>> actions;
//
//    actions[SuperShotgunState::BarrelFull] = { [&double_barrel](sf::Event& event) {
//        if (event.mouseButton.button == sf::Mouse::Left) {
//            double_barrel.click(); 
//        }
//    } };
//
//    actions[SuperShotgunState::BarrelEmpty] = { [&double_barrel](sf::Event& event) {
//        if (event.key.code == sf::Keyboard::R) {
//            double_barrel.reload(); 
//        }
//    } };
//
//    actions[SuperShotgunState::Close] = { [&double_barrel, &window](sf::Event& event) {
//        window.close();
//    } };
//
//    actions[SuperShotgunState::Escape] = { [&double_barrel, &window](sf::Event& event) {
//        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
//            window.close();
//        }
//    } };
//
//    double_barrel.SetActions(actions);
//
//    // loop :
//    // - print state each second.
//    // - get user input: Click/Reload/Quit
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//            {
//                double_barrel.state = SuperShotgunState::Close;
//                double_barrel.ReadCommand(event);
//            }
//            if (event.type == sf::Event::MouseButtonPressed &&
//                event.mouseButton.button == sf::Mouse::Left)
//            {
//                double_barrel.ReadCommand(event);
//            }
//            if (event.type == sf::Event::KeyPressed &&
//                event.key.code == sf::Keyboard::R)
//            {
//                double_barrel.ReadCommand(event);
//            }
//            if (event.type == sf::Event::KeyPressed &&
//                event.key.code == sf::Keyboard::Escape)
//            {
//                double_barrel.state = SuperShotgunState::Escape;
//                double_barrel.ReadCommand(event);
//            }
//        }
//
//        stateText.setString(double_barrel.getStateStr());
//
//        switch (double_barrel.getState())
//        {
//        case SuperShotgunState::BarrelFull:
//            window.draw(barrel_full_sprite);
//            break;
//        case SuperShotgunState::BarrelEmpty:
//            window.draw(barrel_empty_sprite);
//            break;
//        case SuperShotgunState::Reloading:
//            window.draw(reloading_sprite);
//            break;
//        }
//
//        window.draw(stateText);
//        window.display();
//    }
//}

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
