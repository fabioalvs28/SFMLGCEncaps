#include "pch.h"
#include "GCWindow.h"

GCWindow::GCWindow(const GCWindowProperties& properties)
{
    m_properties = properties;
    m_windowCallback = nullptr;
}

void GCWindow::OnUpdate()
{
}

void GCWindow::PollEvents()
{
    MSG msg;
    while (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool GCWindow::Initialize(WindowCloseCallback closeCallback)
{
    m_windowCloseCallback = closeCallback;

    WNDCLASSEX wc
    {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = OnEvent,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hIcon = LoadIcon(NULL, IDI_APPLICATION),
        .hCursor = LoadCursor(NULL, IDC_ARROW),
        .hbrBackground = (HBRUSH)GetStockObject(HOLLOW_BRUSH),
        .lpszClassName = m_properties.Title,
        .hIconSm = LoadIcon(NULL, IDI_APPLICATION)
    };
    
    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    m_hWnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        m_properties.Title,
        m_properties.Title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, m_properties.Width, m_properties.Height,
        NULL, NULL, GetModuleHandle(NULL), this);

    if (m_hWnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

    return true;
}

void GCWindow::DestroyWindow()
{
}

LRESULT GCWindow::OnEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    GCWindow* window = nullptr;

    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = reinterpret_cast<GCWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)window);

        window->m_hWnd = hWnd;
    }
    else
    {
        window = reinterpret_cast<GCWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (window)
    {
        return window->HandleEvent(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam); 
}

LRESULT GCWindow::HandleEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    case WM_SIZE:
    {
        GCWindowResizeEvent ev(LOWORD(lParam), HIWORD(lParam));
        m_windowCallback(ev);
        break;
    }
    case WM_MOUSEMOVE:
    {
        GCMouseMoveEvent ev(LOWORD(lParam), HIWORD(lParam));
        m_windowCallback(ev);
        break;
    }
    default:
        break;
    }
    return DefWindowProc(m_hWnd, msg, wParam, lParam);
}
