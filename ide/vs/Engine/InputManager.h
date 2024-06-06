#pragma once
#include <windows.h>



typedef struct CGVECTOR2
{
    int x;
    int y;
} Vector2;



struct GCWINDOW
{
    Vector2 winPos;
    Vector2 winSize;
    Vector2 center;

};

class GCInputManager
{

public:

    GCInputManager();
    virtual ~GCInputManager() {};

    void UpdateKeyInput();
    void UpdateMousePos(const GCWINDOW* pWinInfos);

    Vector2 GetMousePos();

    bool GetKeyDown(int vKey);
    bool GetKeyStay(int vKey);
    bool GetKeyUp(int vKey);

    /// \brief int from 1 to 5 : left button, right button, middle button, X button 1, Xbutton 2
    bool GetMouseDown(int mouseButton);
    /// \brief int from 1 to 5 : left button, right button, middle button, X button 1, Xbutton 2
    bool GetMouseStay(int mouseButton);
    /// \brief int from 1 to 5 : left button, right button, middle button, X button 1, Xbutton 2
    bool getMouseUp(int mouseButton);
    bool OnMouseHover(Vector2 objectPos, Vector2 objSize);

    void SetLeavingWindows(bool canLeave);

private:
    void InitKeysTable();
    bool m_canLeaveWin;
    BYTE pListOfKeys[255];
    enum
    {
        NONE,
        PUSH,
        UP,
        DOWN,
    };
    Vector2 m_mousePos;

};
