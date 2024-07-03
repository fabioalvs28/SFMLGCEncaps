#pragma once
#include <windows.h>
#include "../core/framework.h"
#include "EventManager.h"

/// ControllerKeys
#define GC_CONTROLLER_A                        0x5800
#define GC_CONTROLLER_B                        0x5801
#define GC_CONTROLLER_X                        0x5802
#define GC_CONTROLLER_Y                        0x5803
#define GC_CONTROLLER_RSHOULDER                0x5804
#define GC_CONTROLLER_LSHOULDER                0x5805
#define GC_CONTROLLER_LTRIGGER                 0x5806
#define GC_CONTROLLER_RTRIGGER                 0x5807

#define GC_CONTROLLER_CROSS_UP                 0x5810
#define GC_CONTROLLER_CROSS_DOWN               0x5811
#define GC_CONTROLLER_CROSS_LEFT               0x5812
#define GC_CONTROLLER_CROSS_RIGHT              0x5813
#define GC_CONTROLLER_START                    0x5814
#define GC_CONTROLLER_BACK                     0x5815
#define GC_CONTROLLER_LJOYSTICK_PRESS          0x5816
#define GC_CONTROLLER_RJOYSTICK_PRESS          0x5817

//////////////////////////////////////////////

/// Mouse + KeyBoard.  
// Mouse.
#define GC_LEFT_CLICK 0x01
#define GC_RIGHT_CLICK 0x02
#define GC_MIDDLE_CLICK 0x04
#define GC_THUMB1_CLICK 0x05
#define GC_THUMB2_CLICK 006
// Keyboard. Letter and Num : 'A' , '0'.
#define GC_BAKCSPACE 0x08
#define GC_TAB 0x09
#define GC_ENTER 0x0D 
#define GC_SHIFT 0x10
#define GC_CTRL 0x11
#define GC_ALT 0x12
#define GC_CAPS 0x14
#define GC_ESCAPE 0x1B
#define GC_SPACE 0x20
#define GC_PAGE_UP 0x21;
#define GC_PAGE_DOWN 0x22
#define GC_LEFT 0x25
#define GC_UP 0x26
#define GC_RIGHT 0x27
#define GC_DOWN 0x28
#define GC_INSERT 0x2D
#define GC_DEL 0x2F
#define GC_NUMPAD_0 0x60
#define GC_NUMPAD_1 0x61
#define GC_NUMPAD_2 0x62
#define GC_NUMPAD_3 0x63
#define GC_NUMPAD_4 0x64
#define GC_NUMPAD_5 0x65
#define GC_NUMPAD_6 0x66
#define GC_NUMPAD_7 0x67
#define GC_NUMPAD_8 0x68
#define GC_NUMPAD_9 0x69
#define GC_LSHIFT 0xA0
#define GC_RSHIFT 0xA1
#define GC_LCTRL 0xA2
#define GC_RCTRL 0xA3
#define GC_LALT 0xA4
#define GC_RALT 0xA5

class GCEventManager;

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



class GCMouseInput 
{

    friend class GCInputManager;
    
private:

    GCMouseInput();
    virtual ~GCMouseInput() {};

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

    void AddtoControllerListUpdate(int index); 
    
    int m_ID;
    GCVector<BYTE> m_pListofControllerKeys;
    GCVEC2 m_pControllersLeftAxis;
    GCVEC2 m_pControllersRightAxis;
    GCVEC2 m_pControllerTrigger; // 0 - left, 1 - Right ;

    GCVector<int> m_updatedControllerKeys;


};


class GCInputManager
{

    friend class GCGameManager;
    GCEventManager* m_eventManager = nullptr;
public:

    GCInputManager();
    GCInputManager(GCEventManager*);
    ~GCInputManager() {};

    void GetConnectedController();

    void UpdateInputs();

    //bool IsKeyPressed();
    bool IsKeyPressed(int keyID);

    //bool IsControllerPressed(int controllerID); 
    bool IsControllerKeyPressed(int controllerID, int button);

    //GCVector<int>* GetControllereUpdatekeys(int controllerID);

    //GCVector<int>* GetUpdatedKeys() { return &m_updatedKeys; }

    void AddToUpdateList(int index, BYTE state);

    void OnEvent(GCEvent& ev);

    BYTE GetKeyState(int keyID) { return m_keyState[keyID];}

    bool GetKeyDown(int key);
    bool GetKeyStay(int key);
    bool GetKeyUp(int key);
    
    bool GetControllerButtonDown(int key);
    bool GetControllerButtonStay(int key);
    bool GetControllerButtonUp(int key);


    bool GetControllerButtonDown( int controllerID, int vButton ) { return m_controllerList[controllerID]->GetControllerButtonDown(vButton); }
    bool GetControllerButtonStay( int controllerID, int vButton ) { return m_controllerList[controllerID]->GetControllerButtonStay(vButton); }
    bool GetControllerButtonUp( int controllerID, int vButton ) { return m_controllerList[controllerID]->GetControllerButtonUp(vButton); }

    GCVEC2* GetControllerLeftJoyStick(int controllerID) { return &m_controllerList[controllerID]->m_pControllersLeftAxis; }
    GCVEC2* GetControllerRightJoyStick(int controllerID) { return &m_controllerList[controllerID]->m_pControllersRightAxis; }
    float GetControllerLeftAxisX( int controllerID ) { return m_controllerList[controllerID]->m_pControllersLeftAxis.x; }
    float GetControllerLeftAxisY( int controllerID ) { return m_controllerList[controllerID]->m_pControllersLeftAxis.y; }
    
    
    float GetControllerRightAxisX( int controllerID ) { return m_controllerList[controllerID]->m_pControllersRightAxis.x; }
    float GetControllerRightAxisY( int controllerID ) { return m_controllerList[controllerID]->m_pControllersRightAxis.y; }

    float GetControllerLeftTriggerState( int controllerID ) { return m_controllerList[controllerID]->m_pControllerTrigger.x; }
    float GetControllerRightTriggerState( int controllerID ) { return m_controllerList[controllerID]->m_pControllerTrigger.y; }


private:

    GCVEC2 m_mousePos;
    WinTest* m_pWindow; // Remplacer avec window finale

    GCVector<GCControllerInput*> m_controllerList;
    GCVector<int> m_updatedKeys;
    GCVector<BYTE> m_keyState;
};
