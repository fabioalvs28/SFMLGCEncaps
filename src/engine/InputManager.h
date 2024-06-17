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


typedef struct WinTest
{
    GCVEC2 winPos;
    GCVEC2 winSize;
    GCVEC2 center;

} WinTest;



class GCKeyboardInput 
{
    friend class GCInputManager;

private: 

    GCKeyboardInput();
    virtual ~GCKeyboardInput() {};

    bool GetKeyDown( int vKey );
    bool GetKeyStay( int vKey );
    bool GetKeyUp( int vKey );


    GCVector<BYTE> m_pListOfKeyboardKeys;
    void UpdateKeyboardInput();
};


class GCMouseInput
{

    friend class GCInputManager;

    

        
private:

    GCMouseInput();
    virtual ~GCMouseInput() {};

    bool GetMouseDown( int mouseButton );
    bool GetMouseStay( int mouseButton);
    bool GetMouseUp( int mouseButton );

    bool OnMouseHover( GCVEC2* objectPos, GCVEC2* objSize );
    void SetLeavingWindows( bool canLeave ) { m_canLeaveWin = canLeave; }
    GCVEC2 GetMousePos() { return m_mousePos; }
    void UpdateMouseInput( const WinTest* pWinInfos );

    bool m_canLeaveWin;
    GCVEC2 m_mousePos;
    GCVector<BYTE> m_pMouseButtons;

};


class GCControllerInput
{
        
    friend class GCInputManager;

    
private:
    GCControllerInput();
    GCControllerInput( int id );
    virtual ~GCControllerInput() {};

    bool GetControllerButtonDown( int vButton );
    bool GetControllerButtonStay( int vButton );
    bool GetControllerButtonUp( int vButton );

    void UpdateJoySticksinput();
    void UpdateControllerInput();
    void UpdateTriggers();
    
    int m_ID;
    GCVector<BYTE> m_pListofControllerKeys;
    float m_pControllersLeftAxis[2];
    float m_pControllersRightAxis[2];
    float m_pControllerTrigger[2]; // 0 - left, 1 - Right ;
};


class GCInputManager
{

    friend class GCGameManager;

private:

    GCInputManager();
    ~GCInputManager() {};

    void GetConnectedController();

    void UpdateInputs();

    GCVEC2 GetMousePos() { return m_mouse.m_mousePos; }

    BYTE GetMouseButtonState( int button ) { return m_mouse.m_pMouseButtons[button]; }
    BYTE GetKeyboardKeyState( int key ) { return m_keyboard.m_pListOfKeyboardKeys[key]; }
    BYTE GetControllerButtonState( int controllerID, int key ) { return m_controllerList[controllerID]->m_pListofControllerKeys[key]; }

    bool GetControllerButtonDown( int controllerID, int vButton ) { return m_controllerList[controllerID]->GetControllerButtonDown(vButton); }
    bool GetControllerButtonStay( int controllerID, int vButton ) { return m_controllerList[controllerID]->GetControllerButtonStay(vButton); }
    bool GetControllerButtonUp( int controllerID, int vButton ) { return m_controllerList[controllerID]->GetControllerButtonUp(vButton); }

    float GetControllerLeftAxisX( int controllerID ) { return m_controllerList[controllerID]->m_pControllersLeftAxis[0]; }
    float GetControllerLeftAxisY( int controllerID ) { return m_controllerList[controllerID]->m_pControllersLeftAxis[1]; }
    float GetControllerRightAxisX( int controllerID ) { return m_controllerList[controllerID]->m_pControllersRightAxis[0]; }
    float GetControllerRightAxisY( int controllerID ) { return m_controllerList[controllerID]->m_pControllersRightAxis[1]; }

    float GetControllerLeftTriggerState( int controllerID ) { return m_controllerList[controllerID]->m_pControllerTrigger[0]; }
    float GetControllerRightTriggerState( int controllerID ) { return m_controllerList[controllerID]->m_pControllerTrigger[1]; }



private:

    bool m_checkController; 

    WinTest* m_pWindow;
    GCVector<GCControllerInput*> m_controllerList;
    GCKeyboardInput m_keyboard;
    GCMouseInput m_mouse;
};
