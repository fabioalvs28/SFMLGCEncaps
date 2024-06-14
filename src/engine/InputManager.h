#pragma once
#include <windows.h>
#include "../core/framework.h"


enum
{
    NONE,
    PUSH,
    UP,
    DOWN,
};


struct GCWINDOW
{
    GCVEC2 winPos;
    GCVEC2 winSize;
    GCVEC2 center;

} GCWINDOWv;



class KeyboardInput
{
    friend class GCInputManager;

public:

    bool GetKeyDown( int vKey );
    bool GetKeyStay( int vKey );
    bool GetKeyUp( int vKey );




private:

    KeyboardInput();
    virtual ~KeyboardInput() {};
    GCVector<BYTE> pListOfKeyboardKeys;
    void UpdateKeyboardInput( bool isActive );
};


class MouseInput
{
    friend class GCInputManager;

public:
    
    bool GetMouseDown( int mouseButton );
    bool GetMouseStay( int mouseButton );
    bool GetMouseUp( int mouseButton );

    bool OnMouseHover( GCVEC2* objectPos, GCVEC2* objSize );


    void SetLeavingWindows( bool canLeave ) { m_canLeaveWin = canLeave;  }


    GCVEC2* GetMousePos() { return &m_mousePos; }


private:

    MouseInput();
    virtual ~MouseInput() {};


    void UpdateMouseInput( const GCWINDOW* pWinInfos, bool isActive );
    bool m_canLeaveWin;
    GCVEC2 m_mousePos;
    GCVector<BYTE> m_pMouseButtons;

};


class ControllerInput
{
    friend class GCInputManager;

public:

    bool GetControllerButtonDown( int vButton );
    bool GetControllerButtonStay( int vButton );
    bool GetControllerButtonUp( int vButton );

    float GetControllerLeftAxisX() { return pControllersLeftAxis[0]; }
    float GetControllerLeftAxisY() { return pControllersLeftAxis[1]; }
    float GetControllerRightAxisX() { return pControllersRightAxis[0]; }
    float GetControllerRightAxisY() { return pControllersRightAxis[1]; }

private:

    ControllerInput();
    ControllerInput( int id ); 
    virtual ~ControllerInput() {};


    void UpdateJoySticksinput();
    void UpdateControllerInput( bool isActive );

    int m_ID; 
    GCVector<BYTE> pListofControllerKeys;
    float pControllersLeftAxis[2];
    float pControllersRightAxis[2];
};



class GCInputManager
{

    friend class GCGameManager;

private:

    GCInputManager();
    virtual ~GCInputManager() {};

    void GetConnectedController();
    void UpdateInputs();
    
    bool GetKeyboardState() { return m_keyboardIsActive; }
    bool GetMouseState() { return m_mouseIsActive; }
    bool GetControllersState() { return m_controllerIsActive;  }


private:

    bool m_keyboardIsActive; 
    bool m_mouseIsActive; 
    bool m_controllerIsActive; 

    GCWINDOW* m_pWindow;
    GCVector<ControllerInput*> m_controllerList;
    KeyboardInput m_keyboard;
    MouseInput m_mouse;
};

 