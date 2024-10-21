#pragma once

class App;

class Window
{
public:
    Window();
    virtual ~Window();

    virtual void Initialize(const std::string& title, int width, int height, HINSTANCE& hInstance, App* appInstance) = 0;
    virtual bool Run();

    inline HWND GetHWND() { return m_hwnd; }

protected:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    virtual LRESULT HandleEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HWND m_hwnd;
    HINSTANCE m_hinstance;
    int m_width;
    int m_height;
};