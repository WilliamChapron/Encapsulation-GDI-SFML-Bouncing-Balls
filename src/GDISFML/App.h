#pragma once

class Window;
class RenderInterface;
class Surface;
class Font;
class Text;
class Texture;
class BitmapTexture;

#include "./CommonStructures.h"

class App {
public:
    App();
    ~App();

    // Start window & Renderer
    void Initialize(const std::string& title, int width, int height, HINSTANCE& hInstance);

    // Perform window events, and specific libraries draw event or logic, this is not blocking
    bool Run();

    // Each frame, resfresh time vars & check if we can render
    void CheckFrameRate();
    void SetFPS(int fps);

    inline Window* GetWindow() { return m_window; }
    inline RenderInterface* GetRenderer() { return m_renderer; }
    inline Surface* GetSurface() { return m_renderTarget; }

    void LoadTexture(const char* fileName, HWND hwnd);
    void LoadFont(std::string& fontName);
    Texture* GetTextureById(int id);
    Font* GetFontById(int id);

    Text* CreateText(Font* font, std::wstring& text);

    // Allow add DrawCommand to App command list
    void AddDrawCommand(DrawCommand& command);
    // Each render call, render all objects and text, it will execute post draw only if render is not blocked by fps
    void ExecuteDrawCommands(Surface* surface);

    bool m_isReadyToRender;

    // Fps 
    inline float GetTargetFPS() const {return m_targetFPS;}
    inline float GetTargetMS() const { return m_targetMS; }
    inline float GetCurrentFPS() const { return m_currentFPS; }

    inline DWORD GetPreviousTime() const {return m_previousTime;}
    inline float GetDeltaTime() const {return m_deltaTime;}
    inline int GetFrameCounter() const {return m_frameCounter;}

private:
    Window* m_window;
    RenderInterface* m_renderer;
    Surface* m_renderTarget;

    // Resources
    std::vector<Texture*> m_textures;
    std::vector<Font*> m_fonts;

    // Command list
    std::vector<DrawCommand> m_commandList;

    // Text elements
    std::vector<Text*> m_texts;

    // Frame rate
    float m_targetMS;
    float m_targetFPS;
    float m_currentFPS;

    // Time variables
    DWORD m_previousTime;
    float m_deltaTime;
    int m_frameCounter = 0;
};

