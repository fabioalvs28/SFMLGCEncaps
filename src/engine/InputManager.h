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

    void GetConnectedController();

    void UpdateInputs();

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
    bool GetMouseUp(int mouseButton);
    bool OnMouseHover(Vector2 objectPos, Vector2 objSize);


    bool GetControllerButtonDown(int controllerId, int vButton);
    bool GetControllerButtonStay(int controllerId, int vButton);
    bool GetControllerButtonUp(int controllerId, int vButton);

    void SetLeavingWindows(bool canLeave);

    float GetControllerLeftAxisX(int controllerID);
    float GetControllerLeftAxisY(int controllerID);
    float GetControllerRightAxisX(int controllerID);
    float GetControllerRightAxisY(int controllerID);

private:


    void UpdateJoySticksinput(int controllerID);
    bool pConnectedController[4];
    void UpdateControllerInput(int controllerID);
    void UpdateKeyboardInput();
    bool m_canLeaveWin;
    BYTE pListOfKeyboardKeys[255];
    BYTE pListofControllerKeys[4][16];
    float pControllersLeftAxis[4][2];
    float pControllersRightAxis[4][2];

    enum
    {
        NONE,
        PUSH,
        UP,
        DOWN,
    };
    Vector2 m_mousePos;

};

