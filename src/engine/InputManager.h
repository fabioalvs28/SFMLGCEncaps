#pragma once
#define NOMINMAX
#include <windows.h>
#include <functional>
#include "../core/framework.h"
#include "EventManager.h"


class GCControllerInputManager;
class GCEventManager;

class GCInputManager
{

    friend class GCGameManager;

public:

    GCInputManager() = default;

    virtual void Update() {} ;

    virtual int GetStateSize() const = 0;
    virtual int GetIDSize() const = 0;
    
protected:
    void InitializeCallbacks();
    std::vector<std::vector<std::function<void(GCEvent&)>>> callbacks;
    GCEventManager* m_eventManager = nullptr;
};



class GCKeyboardInputManager : public GCInputManager
{
    friend class GCInputManager;

public:

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

    int GetIDSize() const override { return KeyboardID::KEYIDCOUNT; };

    int GetStateSize() const override { return KeyboardState::KEYSTATECOUNT; };

    void RegisterEvent(GCEventManager* eventmanager);


    template<typename Func>
    void SetKeyInputCallback(BYTE state, int keyId, Func&& func)
    {
        callbacks[state][keyId] = func;
    }

    void Update();
private:


    std::vector<BYTE> m_keyState;

    void SendEvent(int index, BYTE state);

    void OnKeyPressed(GCKeyPressedEvent& ev);
    void OnKeyReleased(GCKeyReleasedEvent& ev);
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

    int GetIDSize() const override { return MouseID::MOUSEIDCOUNT; };

    int GetStateSize() const override { return MouseState::MOUSESTATECOUNT; };
    

    GCVector<BYTE> m_buttonState;

    void SendEvent(int index, BYTE state);
    
};


class GCControllerManager : public GCInputManager
{

public: 

    GCControllerManager();
    void GetConnectedControllers();
    void Update();

    GCVector<GCControllerInputManager*> m_pControllerList;
};


class GCControllerInputManager : public GCControllerManager
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

    int GetIDSize() const override { return ControllerID::CONTROLLERIDCOUNT; };

    int GetStateSize() const override { return ControllerState::CONTROLLERSTATECOUNT; };

    void UpdateController();

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

    GCVector<BYTE> m_buttonState;
private:



    void UpdateJoySticksinput();
    void UpdateControllerInput();
    void UpdateTriggers();

    void SendEvent(int index, BYTE state);

    int m_ID;
    GCVEC2 m_pControllersLeftAxis;
    GCVEC2 m_pControllersRightAxis;
    GCVEC2 m_pControllerTrigger; // 0 - left, 1 - Right ;

    GCVector<int> m_updatedControllerKeys;


};
