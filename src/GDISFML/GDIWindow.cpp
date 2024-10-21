#include "pch.h"

#include "App.h"

#ifdef GDI_MODE

#include "GDIWindow.h"

#include "RenderInterface.h"

#include "GDISurface.h"
#include "Texture.h"



GDIWindow::GDIWindow() : Window()
{
}

GDIWindow::~GDIWindow()
{
}

void GDIWindow::Initialize(const std::string& title, int width, int height, HINSTANCE& hInstance, App* appInstance) {
    const wchar_t CLASS_NAME[] = L"GDI Window Class";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    m_hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        std::wstring(title.begin(), title.end()).c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL,
        NULL,
        hInstance,
        nullptr
    );

    m_width = width;
    m_height = height;

    SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(appInstance));
    ShowWindow(m_hwnd, SW_SHOWDEFAULT);
}

LRESULT GDIWindow::HandleEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        App* app = reinterpret_cast<App*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (app != nullptr) {
            app->ExecuteDrawCommands(app->GetSurface());
        }

        EndPaint(hwnd, &ps);  
        return 0;
    }
        

    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#endif