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

bool GCWindow::InitializeWindow()
{
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
        NULL, NULL, NULL, NULL);

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
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SIZE:
        break;
    default:
        break;
    }
    return 0;
}
