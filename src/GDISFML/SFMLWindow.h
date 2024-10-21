#pragma once

#ifdef SFML_MODE

#include "Window.h"

class RenderWindow2 : public sf::RenderWindow
{
public:
    RenderWindow2(HWND hwnd, App* appInstance) : sf::RenderWindow(hwnd) {
        m_appInstance = appInstance;
    }

    ~RenderWindow2() {};

    App* m_appInstance;
};

class SFMLWindow : public Window
{
public:
    SFMLWindow();
    ~SFMLWindow();

    void Initialize(const std::string& title, int width, int height, HINSTANCE& hInstance, App* appInstance) override;
    bool Run() override;

    LRESULT HandleEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    inline RenderWindow2* GetCustomSfmlWindow() { return m_customSfWindow; }

private:
    RenderWindow2* m_customSfWindow;
};

#endif
