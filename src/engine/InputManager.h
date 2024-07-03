#pragma once
#include <windows.h>
#include <functional>
#include "../core/framework.h"
#include "EventSystem.h"

/// ControllerKeys


//////////////////////////////////////////////
/// Mouse + KeyBoard.  
// Mouse.
#define GC_LEFT_CLICK 0x01
#define GC_RIGHT_CLICK 0x02
#define GC_MIDDLE_CLICK 0x04
#define GC_THUMB1_CLICK 0x05
#define GC_THUMB2_CLICK 006
// Keyboard. Letter and Num : 'A' , '0'.
#define GC_BACKSPACE 0x08
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


class GCControllerInputManager;
class GCKeyboardInputManager;
class GCMouseInputManager;


class GCInputManager
{
    GCVector<GCVector<std::function<void(GCEvent&)>>> callbacks;

    friend class GCGameManager;

public:
    GCInputManager();
    void Init();
    void Update();

    
    GCVector<GCControllerInputManager*> m_controllerList;
    GCKeyboardInputManager* m_pKeyboardManager;
    GCMouseInputManager* m_pMouseManager;

private:




    virtual int GetStateSize() = 0;
    virtual int GetIDSize() = 0;
    void GetConnectedControllers();
};



class GCKeyboardInputManager : public GCInputManager
{
    friend class GCInputManager;

public :

    GCKeyboardInputManager();

    bool IsKeyPressed(int keyID);

    BYTE GetKeyState(int keyID) { return m_keyState[keyID]; }

    bool GetKeyDown(int key);
    bool GetKeyStay(int key);
    bool GetKeyUp(int key);
    
    enum KeyboardID
    {
        TAB = 9,
        ENTER = 13,
        SHIFT = 16,
        CTRL,
        ALT,
        CAPS = 20,
        ESCAPE = 27,
        SPACE = 32,
        LEFT_ARROW = 37,
        UP_ARROW,
        RIGHT_ARROW,
        DOWN_ARROW,
        KEY0 = 48,
        KEY1,
        KEY2,
        KEY3,
        KEY4,
        KEY5,
        KEY6,
        KEY7,
        KEY8,
        KEY9,
        A = 65,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
        EQUAL = 187, 
        CLOSED_PARENTHESIS = 219,
        SQUARED = 222,

        KEYIDCOUNT
    };
    

    enum KeyboardState
    {
        NONE, // NOT PRESSED
        STAY, // STAY PRESSED
        UP,   // RELEASED
        DOWN, // PRESSED ONCE
        KEYSTATECOUNT
    };

private:


    void Update();

    int GetIDSize() override
    {
        return KeyboardID::KEYIDCOUNT;
    };

    int GetStateSize() override
    {
        return KeyboardState::KEYSTATECOUNT;
    };

    GCVector<BYTE> m_keyState;

    void SendEvent(int index, BYTE state);

};


class GCMouseInputManager : public GCInputManager
{
    friend class GCInputManager;

public: 
    GCMouseInputManager();

    bool IsKeyPressed(int keyID);

    BYTE GetKeyState(int keyID) { return m_buttonState[keyID]; }

    bool GetKeyDown(int key);
    bool GetKeyStay(int key);
    bool GetKeyUp(int key);


    enum MouseID
    {
        LEFT = 1,
        RIGHT,
        MIDDLE = 4,
        THUMB_1,
        THUMB_2,
        MOUSEIDCOUNT
    };

    enum MouseState
    {
        NONE, // NOT PRESSED
        STAY, // STAY PRESSED
        UP,   // RELEASED
        DOWN, // PRESSED ONCE
        MOUSESTATECOUNT
    };



private:

    void Update();

    int GetIDSize() override
    {
        return MouseID::MOUSEIDCOUNT;
    };

    int GetStateSize() override
    {
        return MouseState::MOUSESTATECOUNT;
    };

    GCVector<BYTE> m_buttonState;

    void SendEvent(int index, BYTE state);
    
};

class GCControllerInputManager : public GCInputManager
{

public: 

    GCControllerInputManager();
    GCControllerInputManager(int id);


    enum ControllerID
    {
        A,B,X,Y,
        RSHOULDER, LSHOULDER, 
        LTRIGGER, RTRIGGER, 
        CROSS_UP,CROSSDOWN,CROSSLEFT,CROSSRIGHT,
        START, BACK,
        LJOYSTICK, RJOYSTICK,
        CONTROLLERIDCOUNT
    };

    enum ControllerState
    {
        NONE, // NOT PRESSED
        STAY, // STAY PRESSED
        UP,   // RELEASED
        DOWN, // PRESSED ONCE
        CONTROLLERSTATECOUNT
    };

public:

    int GetIDSize() override
    {
        return ControllerID::CONTROLLERIDCOUNT;
    };

    int GetStateSize() override
    {
        return ControllerState::CONTROLLERSTATECOUNT;
    };



    void Update();

    GCVEC2* GetControllerLeftJoyStick(int controllerID) { return &m_pControllersLeftAxis; }
    GCVEC2* GetControllerRightJoyStick(int controllerID) { return &m_pControllersRightAxis; }
    float GetControllerLeftAxisX(int controllerID) { return m_pControllersLeftAxis.x; }
    float GetControllerLeftAxisY(int controllerID) { return m_pControllersLeftAxis.y; }


    float GetControllerRightAxisX(int controllerID) { return m_pControllersRightAxis.x; }
    float GetControllerRightAxisY(int controllerID) { return m_pControllersRightAxis.y; }

    float GetControllerLeftTriggerState(int controllerID) { return m_pControllerTrigger.x; }
    float GetControllerRightTriggerState(int controllerID) { return m_pControllerTrigger.y; }

    bool GetControllerButtonDown(int vButton);
    bool GetControllerButtonStay(int vButton);
    bool GetControllerButtonUp(int vButton);

private:



    void UpdateJoySticksinput();
    void UpdateControllerInput();
    void UpdateTriggers();

    void SendEvent(int index, BYTE state);

    int m_ID;
    GCVector<BYTE> m_buttonState;
    GCVEC2 m_pControllersLeftAxis;
    GCVEC2 m_pControllersRightAxis;
    GCVEC2 m_pControllerTrigger; // 0 - left, 1 - Right ;

    GCVector<int> m_updatedControllerKeys;


};
