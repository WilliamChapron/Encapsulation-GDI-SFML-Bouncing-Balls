#include "pch.h"
#include "App.h"
#include "Window.h"
#include "Texture.h"
#include "Surface.h"
#include "Font.h"
#include "Text.h"

#ifdef GDI_MODE
    #include "GDIWindow.h"
    #include "GDIRenderer.h"
    #include "GDITexture.h"
    #include "GDIFont.h"
    #include "GDISurface.h"
#elif SFML_MODE
    #include "SFMLWindow.h"
    #include "SFMLRenderer.h"
    #include "SFMLTexture.h"
    #include "SFMLFont.h"
    #include "SFMLSurface.h"
#endif

App::App()
    : m_window(nullptr),
    m_renderer(nullptr),
    m_renderTarget(nullptr),
    m_targetFPS(60.0f),
    m_targetMS(1000.0f / 60.0f),
    m_currentFPS(60.0f),
    m_previousTime(GetTickCount()),
    m_deltaTime(0.0f),
    m_isReadyToRender(false)
{
}

App::~App() {
    for (Texture* texture : m_textures) {
        delete texture;
    }
    m_textures.clear();
    for (Font* font : m_fonts) {
        delete font;
    }
    m_fonts.clear();
    for (Text* text : m_texts) {
        delete text;
    }
    m_fonts.clear();

    delete m_window;
    delete m_renderer;
    delete m_renderTarget;
}

void App::Initialize(const std::string& title, int width, int height, HINSTANCE& hInstance) {
#ifdef GDI_MODE
    m_window = new GDIWindow();
    m_window->Initialize(title, width, height, hInstance, this);
    m_renderer = new GDIRenderer();
    HDC hdc = GetDC(m_window->GetHWND());
    m_renderTarget = new GDISurface(hdc, width, height);
#elif SFML_MODE
    m_window = new SFMLWindow();
    m_window->Initialize(title, width, height, hInstance, this);
    m_renderer = new SFMLRenderer();
    SFMLWindow* sfmlWindow = static_cast<SFMLWindow*>(m_window);
    sf::RenderWindow* window = static_cast<sf::RenderWindow*>(sfmlWindow->GetCustomSfmlWindow());
    m_renderTarget = new SFMLSurface(window);
#endif
}

bool App::Run() {
    if (!m_window->Run()) {
        return false;
    }
    CheckFrameRate();
    return true;
}

void App::LoadTexture(const char* fileName, HWND hwnd) {
#ifdef GDI_MODE
    HDC hdc = GetDC(hwnd);
    Texture* texture = new GDITexture();
    texture->Load(fileName, hdc);
    m_textures.push_back(texture);
#elif SFML_MODE
    Texture* texture = new SFMLTexture();
    texture->Load(fileName);
    m_textures.push_back(texture);
#endif
}

void App::LoadFont(std::string& fontName) {
#ifdef GDI_MODE
    Font* font = new GDIFont();
    font->Load(fontName);
    m_fonts.push_back(font);
#elif SFML_MODE
    Font* font = new SFMLFont();
    font->Load(fontName);
    m_fonts.push_back(font);
#endif
}

Text* App::CreateText(Font* font, std::wstring& text) {
#ifdef GDI_MODE
    GDIFont* gdiFont = static_cast<GDIFont*>(font);
    Text* newText = new GDIText(gdiFont, text);
    m_texts.push_back(newText);
    return newText;
#elif SFML_MODE
    SFMLFont* sfmlFont = static_cast<SFMLFont*>(font);
    Text* newText = new SFMLText(sfmlFont, text);
    m_texts.push_back(newText);
    return newText;
#endif
}

Texture* App::GetTextureById(int id) {
    if (id >= 0 && id < static_cast<int>(m_textures.size())) {
        return m_textures[id];
    }
    return nullptr;
}

Font* App::GetFontById(int id) {
    if (id >= 0 && id < static_cast<int>(m_fonts.size())) {
        return m_fonts[id];
    }
    return nullptr;
}

void App::AddDrawCommand(DrawCommand& command) {
    m_commandList.push_back(command);
}

void App::ExecuteDrawCommands(Surface* surface) {
    m_renderer->PreDraw(surface);
    for (auto& command : m_commandList) {
        if (command.type == DrawType::Text) {
            m_renderer->RenderText(command);
            continue;
        }
        m_renderer->DrawObject(command);
    }
    if (m_isReadyToRender) {
        m_renderer->PostDraw();
    }
    m_commandList.clear();
}

void App::SetFPS(int fps) {
    m_targetMS = 1000.0f / fps;
    m_targetFPS = fps;
}

void App::CheckFrameRate() {
    DWORD currentTime = GetTickCount64();
    m_deltaTime = currentTime - m_previousTime;

    if (m_deltaTime >= m_targetMS) {
        m_previousTime = currentTime;
        m_isReadyToRender = true;
        m_frameCounter = 0;
        m_currentFPS = 1000.0f / m_deltaTime;
    }
    else {
        m_isReadyToRender = false;
        m_frameCounter++;
    }
}
