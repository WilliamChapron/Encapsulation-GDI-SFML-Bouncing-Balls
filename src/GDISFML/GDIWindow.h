#pragma once

#include "Window.h"

struct WinData;
struct WinGDIData;

class GDIWindow : public Window {
public:
    GDIWindow();
    ~GDIWindow();

    void Initialize(const std::string& title, int width, int height, HINSTANCE& hInstance, App* appInstance) override;

    LRESULT HandleEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};


