#pragma once
#include <windows.h>
#include "../core/framework.h"


enum GCKeyState
{
    NONE, // NOT PRESSED
    PUSH, // STAY PRESSED
    UP,   // RELEASED
    DOWN, // PRESSED ONCE
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
    void UpdateKeyboardInput();
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


    void UpdateMouseInput( const GCWINDOW* pWinInfos);
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
    void UpdateControllerInput();

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


    // <summary>
    // This function returns the state of the given mouse button.
    // </summary>
    // <param kname="button"> The button to check. It should be a valid index in the m_pMouseButtons array. </param>
    // <return> The state of the given mouse button. It can be one of the following: NONE, PUSH, UP, DOWN. </return>
    BYTE GetMouseButtonState(int button) { return m_mouse.m_pMouseButtons[button]; }


    // <summary>
    // This function returns the state of the given keyboard key.
    // </summary>
    // <param kname="key"> The key to check. It should be a valid index in the pListOfKeyboardKeys array. </param>
    // <return> The state of the given keyboard key. It can be one of the following: NONE, PUSH, UP, DOWN. </return>
    BYTE GetKeyboardKeyState(int key) { return m_keyboard.pListOfKeyboardKeys[key]; }


    //  <summary>
    //  This function returns the state of the given controller button.
    //  </summary> 
    //  <param name="controllerID"> The ID of the controller to check. </param>
    //  <param name="button> The button to check. </param>
    //  <return> The state of the given controller button. It can be one of the following: NONE, PUSH, UP, DOWN. </return>
    BYTE GetControllerButtonState(int controllerID, int key) { return m_controllerList[controllerID]->pListofControllerKeys[key]; }




    //  <summary>
    //  This function checks if the given controller button is down this frame.
    //  </summary>
    //  <param name="controllerID"> The ID of the controller to check. </param>
    //  <param name="vButton"> The button to check. </param>
    bool GetControllerButtonDown(int controllerID, int vButton) { return m_controllerList[controllerID]->GetControllerButtonDown(vButton); }


    //  This function checks if the given controller button is push this frame.
    //  </summary>
    //  <param name="controllerID"> The ID of the controller to check. </param>
    //  <param name="vButton"> The button to check. </param>
    bool GetControllerButtonStay(int controllerID, int vButton) { return m_controllerList[controllerID]->GetControllerButtonStay(vButton); }


    //  This function checks if the given controller button is up this frame.
    //  </summary>
    //  <param name="controllerID"> The ID of the controller to check. </param>
    //  <param name="vButton"> The button to check. </param>
    bool GetControllerButtonUp(int controllerID, int vButton) { return m_controllerList[controllerID]->GetControllerButtonUp(vButton); }






private:


    GCWINDOW* m_pWindow;
    GCVector<ControllerInput*> m_controllerList;
    KeyboardInput m_keyboard;
    MouseInput m_mouse;
};

