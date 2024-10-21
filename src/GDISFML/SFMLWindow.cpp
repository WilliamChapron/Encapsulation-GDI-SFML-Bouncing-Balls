#include "pch.h"

#ifdef SFML_MODE

#include "SFMLWindow.h"
#include "SFMLSurface.h"

#include "App.h"


SFMLWindow::SFMLWindow()
{
}

SFMLWindow::~SFMLWindow()
{
    delete m_customSfWindow;
}

void SFMLWindow::Initialize(const std::string& title, int width, int height, HINSTANCE& hInstance, App* appInstance) {
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

    m_customSfWindow = new RenderWindow2(m_hwnd, appInstance);
    RenderWindow2* pRW = (RenderWindow2*)GetWindowLongPtr(m_hwnd, GWLP_USERDATA);
    pRW->m_appInstance = appInstance;

    m_width = width;
    m_height = height;
}

LRESULT SFMLWindow::HandleEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            break;
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

bool SFMLWindow::Run() {
    MSG msg = {};

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
   
    RenderWindow2* renderWindow = reinterpret_cast<RenderWindow2*>(GetWindowLongPtr(m_hwnd, GWLP_USERDATA));

    if (renderWindow && renderWindow->m_appInstance) {
        renderWindow->m_appInstance->ExecuteDrawCommands(renderWindow->m_appInstance->GetSurface());
    }

    return true;
}
#endif


