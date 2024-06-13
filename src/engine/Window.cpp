#include "pch.h"
#include "Window.h"


GCWindow::GCWindow(HINSTANCE hInstance, int nCmdShow, GCEventSystem& eventSystem)
    : m_hInstance(hInstance), m_eventSystem(eventSystem), m_hwnd(nullptr)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = m_className;

    RegisterClass(&wc);
}

GCWindow::~GCWindow()
{
    if (m_hwnd)
    {
        DestroyWindow(m_hwnd);
    }
}

bool GCWindow::Create(LPCWSTR windowName, int width, int height)
{
    m_hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        m_className,                    // Window class
        windowName,                     // Window text
        WS_OVERLAPPEDWINDOW,            // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr,                        // Parent window
        nullptr,                        // Menu
        m_hInstance,                    // Instance handle
        this                            // Additional application data
    );

    return m_hwnd != nullptr;
}

void GCWindow::Show(int nCmdShow)
{
    ShowWindow(m_hwnd, nCmdShow);
}

void GCWindow::PollEvents()
{
    MSG msg = { 0 };
    while (msg.message !=  WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg); 
        }
    }
}

LRESULT CALLBACK GCWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    GCWindow* window = nullptr;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        window = (GCWindow*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);

        window->m_hwnd = hwnd;
    }
    else
    {
        window = (GCWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (window)
    {
        return window->HandleMessage(uMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT GCWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
    {
        UINT width = LOWORD(lParam);
        UINT height = HIWORD(lParam);
        GCWindowResizeEvent event(width, height);
        
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        GCMouseButtonPressed event(x, y, GCMouseButton::Left);
        std::cout << "Left clicked" << std::endl;
        return 0;
    }
    case WM_KEYDOWN:
        //TODO: Implement InputManager

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
}

