#pragma once
#include <windows.h>
#include "../core/framework.h"
#include "EventSystem.h"


//////////////////////////////////////////////

// 1 - 255 : Mouse + KeyBoard.  


#define CONTROLLER_BUTTON_A                        0x5800
#define CONTROLLER_BUTTON_B                        0x5801
#define CONTROLLER_BUTTON_X                        0x5802
#define CONTROLLER_BUTTON_Y                        0x5803
#define CONTROLLER_BUTTON_RSHOULDER                0x5804
#define CONTROLLER_BUTTON_LSHOULDER                0x5805
#define CONTROLLER_BUTTON_LTRIGGER                 0x5806
#define CONTROLLER_BUTTON_RTRIGGER                 0x5807

#define CONTROLLER_BUTTON_CROSS_UP                 0x5810
#define CONTROLLER_BUTTON_CROSS_DOWN               0x5811
#define CONTROLLER_BUTTON_CROSS_LEFT               0x5812
#define CONTROLLER_BUTTON_CROSS_RIGHT              0x5813
#define CONTROLLER_BUTTON_START                    0x5814
#define CONTROLLER_BUTTON_BACK                     0x5815
#define CONTROLLER_BUTTON_LJOYSTICK_PRESS          0x5816
#define CONTROLLER_BUTTON_RJOYSTICK_PRESS          0x5817

//////////////////////////////////////////////

//VK_SHIFT




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

    void AddtoControllerListUpdate(int index); 
    
    int m_ID;
    GCVector<BYTE> m_pListofControllerKeys;
    float m_pControllersLeftAxis[2];
    float m_pControllersRightAxis[2];
    float m_pControllerTrigger[2]; // 0 - left, 1 - Right ;

    GCVector<int> m_updatedControllerKeys;

};


class GCInputManager
{

    friend class GCGameManager;

public:

    GCInputManager();
    ~GCInputManager() {};

    void GetConnectedController();

    void UpdateInputs();

    bool IsKeyPressed();
    bool IsKeyPressed(int keyID);

    bool IsControllerPressed(int controllerID); 
    bool IsControllerKeyPressed(int controllerID, int button);

    GCVector<int>* GetControllereUpdatekeys(int controllerID);

    GCVector<int>* GetUpdatedKeys() { return &m_updatedKeys; }

    void AddToUpdateList(int index, BYTE state);




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
    GCVEC2 m_mousePos;
    WinTest* m_pWindow;
    GCVector<GCControllerInput*> m_controllerList;
    GCVector<int> m_updatedKeys;
    GCVector<BYTE> m_keyState;

    GCEventSystem* m_eventSystem;
};
