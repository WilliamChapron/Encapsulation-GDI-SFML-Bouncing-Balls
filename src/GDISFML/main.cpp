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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd) {
    #ifdef _DEBUG
        AllocConsole();
        FILE* pFile;
        freopen_s(&pFile, "CONOUT$", "w", stdout);
        freopen_s(&pFile, "CONOUT$", "w", stderr);
    #endif


    App* app = new App();
    app->Initialize("Hello World", 800, 600, hInstance);

    // Res init
    HWND hwnd = app->GetWindow()->GetHWND();
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    int width = clientRect.right - clientRect.left;
    int height = clientRect.bottom - clientRect.top;

    app->LoadTexture("..\\..\\..\\src\\textures\\mario.bmp", hwnd); // #IMPORTANT replace "mario" by "magenta", in GDI Version it will draw transparent the pure magenta pixels of the texture

    std::string fontPath = "..\\..\\..\\font\\arial.ttf";
    app->LoadFont(fontPath);

    // Text init
    std::wstring currentFPSText = L"" + std::to_wstring(app->GetCurrentFPS());
    Text* currentFPSTextObject = app->CreateText(app->GetFontById(0), currentFPSText);
    std::wstring targetMSText = L"" + std::to_wstring(app->GetTargetMS());
    Text* targetMSTextObject = app->CreateText(app->GetFontById(0), targetMSText);
    std::wstring targetFPSText = L"" + std::to_wstring(app->GetTargetFPS());
    Text* targetFPSTextObject = app->CreateText(app->GetFontById(0), targetFPSText);
    std::wstring deltaTimeText = L"" + std::to_wstring(app->GetDeltaTime());
    Text* deltaTimeTextObject = app->CreateText(app->GetFontById(0), deltaTimeText);
    std::wstring totalBallsText = L"";
    Text* totalBallsTextObject = app->CreateText(app->GetFontById(0), totalBallsText);
    std::wstring currentBallsText = L"";
    Text* currentBallsTextObject = app->CreateText(app->GetFontById(0), currentBallsText);

    unsigned int currentFPSColor[3] = { 255, 255, 255 }; 
    unsigned int targetFPSColor[3] = { 173, 216, 230 }; 
    unsigned int targetMSColor[3] = { 144, 238, 144 }; 
    unsigned int deltaTimeColor[3] = { 255, 228, 196 }; 
    unsigned int totalBallsColor[3] = { 255, 255, 224 }; 
    unsigned int currentBallsColor[3] = { 255, 182, 193 }; 

    DrawCommand currentFPSCommand;
    currentFPSCommand.x = 30.0f;
    currentFPSCommand.y = 110.0f;
    currentFPSCommand.type = DrawType::Text;
    currentFPSCommand.text = currentFPSTextObject;
    currentFPSCommand.color[0] = currentFPSColor[0];
    currentFPSCommand.color[1] = currentFPSColor[1];
    currentFPSCommand.color[2] = currentFPSColor[2];

    DrawCommand targetFPSCommand;
    targetFPSCommand.x = 30.0f;
    targetFPSCommand.y = 150.0f;
    targetFPSCommand.type = DrawType::Text;
    targetFPSCommand.text = targetFPSTextObject;
    targetFPSCommand.color[0] = targetFPSColor[0];
    targetFPSCommand.color[1] = targetFPSColor[1];
    targetFPSCommand.color[2] = targetFPSColor[2];

    DrawCommand targetMSCommand;
    targetMSCommand.x = 30.0f;
    targetMSCommand.y = 190.0f;
    targetMSCommand.type = DrawType::Text;
    targetMSCommand.text = targetMSTextObject;
    targetMSCommand.color[0] = targetMSColor[0];
    targetMSCommand.color[1] = targetMSColor[1];
    targetMSCommand.color[2] = targetMSColor[2];

    DrawCommand deltaTimeCommand;
    deltaTimeCommand.x = 30.0f;
    deltaTimeCommand.y = 230.0f;
    deltaTimeCommand.type = DrawType::Text;
    deltaTimeCommand.text = deltaTimeTextObject;
    deltaTimeCommand.color[0] = deltaTimeColor[0];
    deltaTimeCommand.color[1] = deltaTimeColor[1];
    deltaTimeCommand.color[2] = deltaTimeColor[2];

    DrawCommand totalBallsCommand;
    totalBallsCommand.x = 30.0f;
    totalBallsCommand.y = 270.0f;
    totalBallsCommand.type = DrawType::Text;
    totalBallsCommand.text = totalBallsTextObject;
    totalBallsCommand.color[0] = totalBallsColor[0];
    totalBallsCommand.color[1] = totalBallsColor[1];
    totalBallsCommand.color[2] = totalBallsColor[2];

    DrawCommand currentBallsCommand;
    currentBallsCommand.x = 30.0f;
    currentBallsCommand.y = 310.0f;
    currentBallsCommand.type = DrawType::Text;
    currentBallsCommand.text = currentBallsTextObject;
    currentBallsCommand.color[0] = currentBallsColor[0];
    currentBallsCommand.color[1] = currentBallsColor[1];
    currentBallsCommand.color[2] = currentBallsColor[2];

    // Init bouncing gameplay system
    BouncingBall ball;
    ball.BallAppear(width, height);

    auto lastBallAppearTime = std::chrono::steady_clock::now();
    const int ballAppearInterval = 150;

    // Gameplay loop
    while (true) {
        // Run render API (this manages the fps control itself)
        if (!app->Run()) {
            break;
        }

        // Key input for change fps
        if (GetAsyncKeyState('1') & 0x8000) {
            app->SetFPS(30);
        }
        if (GetAsyncKeyState('2') & 0x8000) {
            app->SetFPS(60);
        }
        if (GetAsyncKeyState('3') & 0x8000) {
            app->SetFPS(120);
        }
        if (GetAsyncKeyState('4') & 0x8000) {
            app->SetFPS(10);
        }

        // Manage appear time interval
        auto currentTime = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastBallAppearTime).count() >= ballAppearInterval) {
            ball.BallAppear(width, height);
            lastBallAppearTime = currentTime;
        }

        ball.BallMoving(width, height);

        // For each ball, add draw command object
        std::vector<Ball> circleShapes = ball.GetAllBalls();
        for (const auto& circleShape : circleShapes) {
            DrawCommand command;
            command.texture = app->GetTextureById(0);
            command.x = circleShape.x;
            command.y = circleShape.y;
            command.radius = circleShape.radius;
            command.type = DrawType::Circle;
            app->AddDrawCommand(command);
        }

        // Update texts values & command
        std::wstring updatedFPSText = L"Current FPS : " + std::to_wstring(app->GetCurrentFPS());
        currentFPSTextObject->SetText(updatedFPSText);
        std::wstring updatedTargetFPSText = L"FPS Limit : " + std::to_wstring(app->GetTargetFPS());
        targetFPSTextObject->SetText(updatedTargetFPSText);
        std::wstring updatedTargetMSText = L"Target MS : " + std::to_wstring(app->GetTargetMS());
        targetMSTextObject->SetText(updatedTargetMSText);
        std::wstring updatedDeltaTimeText = L"Delta Time : " + std::to_wstring(app->GetDeltaTime());
        deltaTimeTextObject->SetText(updatedDeltaTimeText);
        std::wstring updatedTotalBallsText = L"Total Balls : " + std::to_wstring(ball.GetTotalBallsGenerated());
        totalBallsTextObject->SetText(updatedTotalBallsText);
        std::wstring updatedCurrentBallsText = L"Balls On Screen : " + std::to_wstring(ball.GetCurrentBallsOnScreen());
        currentBallsTextObject->SetText(updatedCurrentBallsText);

        app->AddDrawCommand(currentFPSCommand);
        app->AddDrawCommand(targetFPSCommand);
        app->AddDrawCommand(targetMSCommand);
        app->AddDrawCommand(deltaTimeCommand);
        app->AddDrawCommand(totalBallsCommand);
        app->AddDrawCommand(currentBallsCommand);
    }

    OutputDebugStringA("End app");

    #ifdef _DEBUG
        _CrtDumpMemoryLeaks();
    #endif

    return 0;
}
