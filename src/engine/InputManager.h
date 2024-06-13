#pragma once
#include <windows.h>
#include "../Core/framework.h"


enum
{
    NONE,
    PUSH,
    UP,
    DOWN,
};



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

} GCWINDOWv;



class KeyboardInput
{
    friend class GCInputManager;

public:

    bool GetKeyDown(int vKey);
    bool GetKeyStay(int vKey);
    bool GetKeyUp(int vKey);




private:

    KeyboardInput();
    virtual ~KeyboardInput() {};
    BYTE pListOfKeyboardKeys[255];
    void UpdateKeyboardInput();
};


class MouseInput
{
    friend class GCInputManager;

public:
    /// \brief int from 1 to 5 : left button, right button, middle button, X button 1, Xbutton 2
    bool GetMouseDown(int mouseButton);
    /// \brief int from 1 to 5 : left button, right button, middle button, X button 1, Xbutton 2
    bool GetMouseStay(int mouseButton);
    /// \brief int from 1 to 5 : left button, right button, middle button, X button 1, Xbutton 2
    bool GetMouseUp(int mouseButton);

    bool OnMouseHover(Vector2 objectPos, Vector2 objSize);


    void SetLeavingWindows(bool canLeave);


    Vector2 GetMousePos();


private:

    MouseInput();
    virtual ~MouseInput() {};


    void UpdateMouseInput(const GCWINDOW* pWinInfos);
    bool m_canLeaveWin;
    Vector2 m_mousePos;

    BYTE pMouseButtons[5];

};


class ControllerInput
{
    friend class GCInputManager;

public:

    bool GetControllerButtonDown(int controllerId, int vButton);
    bool GetControllerButtonStay(int controllerId, int vButton);
    bool GetControllerButtonUp(int controllerId, int vButton);

    float GetControllerLeftAxisX(int controllerID);
    float GetControllerLeftAxisY(int controllerID);
    float GetControllerRightAxisX(int controllerID);
    float GetControllerRightAxisY(int controllerID);

private:

    ControllerInput();
    virtual ~ControllerInput() {};


    void UpdateJoySticksinput(int controllerID);
    void UpdateControllerInput(int controllerID);

    BYTE pListofControllerKeys[16];
    float pControllersLeftAxis[2];
    float pControllersRightAxis[2];
};





class GCInputManager
{

public:

    GCInputManager();
    virtual ~GCInputManager() {};

    void GetConnectedController();

    void UpdateInputs();

private:

    GCWINDOW* m_pWindow;
    GCVector<ControllerInput*> m_controllerList;
    KeyboardInput m_keyboard;
    MouseInput m_mouse;
};


