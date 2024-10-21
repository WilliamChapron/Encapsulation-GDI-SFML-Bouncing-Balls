#include "pch.h"
#include "Window.h"
#include "App.h"
#include "SFMLWindow.h"
#include "GDIWindow.h"
#include "RenderInterface.h"
#include "Texture.h"

Window::Window() {}

Window::~Window() {}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    #ifdef SFML_MODE
        RenderWindow2* renderWindow = reinterpret_cast<RenderWindow2*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (renderWindow == nullptr || renderWindow->m_appInstance == nullptr) {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
        return renderWindow->m_appInstance->GetWindow()->HandleEvent(hwnd, uMsg, wParam, lParam);
    #elif GDI_MODE
        App* app = reinterpret_cast<App*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (app != nullptr) {
            return app->GetWindow()->HandleEvent(hwnd, uMsg, wParam, lParam);
        }
    #endif
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool Window::Run() {
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    App* app = reinterpret_cast<App*>(GetWindowLongPtr(m_hwnd, GWLP_USERDATA));
    // Throw paint event
    RedrawWindow(m_hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_INTERNALPAINT);
    return true;
}
