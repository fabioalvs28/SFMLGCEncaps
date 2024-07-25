#pragma once

class Window
{
public:
    Window(HINSTANCE hInstance);
    Window(const Window& rhs) = delete;
    Window& operator=(const Window& rhs) = delete;
    ~Window();
    static Window* GetApp();

    HINSTANCE AppInst()const;
    HWND      MainWnd()const;
    float     AspectRatio()const;


    bool IsKeyDown(int keyCode) const {
        // Implémentation spécifique à votre système d'entrée
        // Exemple avec la Windows API :
        return (GetAsyncKeyState(keyCode) & 0x8000) != 0;
    };

    bool Get4xMsaaState()const;
    void Set4xMsaaState(bool value);

    bool Run(GCRenderContext* pRender);

    virtual bool Initialize();
    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    int GetClientWidth();
    int GetClientHeight();
    HWND GetHMainWnd();

private:
    void OnResize();

    bool InitMainWindow();

    void CalculateFrameStats();

private:

    static Window* mApp;
    bool      m4xMsaaState = false;    // 4X MSAA enabled
    UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA
    int m_ClientWidth = 1920;
    int m_ClientHeight = 1080;
    HWND      m_hMainWnd = nullptr; // main window handle
    HINSTANCE mhAppInst = nullptr; // application instance handle
    bool      mAppPaused = false;  // is the application paused?
    bool      mMinimized = false;  // is the application minimized?
    bool      mMaximized = false;  // is the application maximized?
    bool      mResizing = false;   // are the resize bars being dragged?
    bool      mFullscreenState = false;// fullscreen enabled

    // Set true to use 4X MSAA (§4.1.8).  The default is false.

    // Used to keep track of the “delta-time” and game time (§4.4).
    Timer mTimer;
    IDXGIFactory4* mdxgiFactory;
    // Derived class should set these in derived constructor to customize starting values.
    std::wstring mMainWndCaption = L"Rendu";
    //GCRender* m_pRender;
};