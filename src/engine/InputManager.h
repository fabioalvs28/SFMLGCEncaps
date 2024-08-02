#pragma once
#define NOMINMAX

#include "pch.h"

namespace KEYBOARD
{
enum Keys
{
    ESCAPE = 27, ESC = 27,

    F1 = 112, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,

    SCROLL_LOCK = 145,
    PAUSE = 19,

    SQUARE = 222,
    KEY1 = 49, AMPERSAND = 49,
    KEY2 = 50, E_ACUTE = 50, TILDE = 50,
    KEY3 = 51, DOUBLE_QUOTATION = 51, HASHTAG = 51,
    KEY4 = 52, SINGLE_QUOTATION = 52, LCURLY_BRACKET = 52,
    KEY5 = 53, LPARENTHESE = 53, LSQUARE_BRACKET = 53,
    KEY6 = 54, MINUS = 54, VERTICAL_BAR = 54,
    KEY7 = 55, E_GRAVE = 55, GRAVE_ACCENT = 55,
    KEY8 = 56, UNDERSCORE = 56, BACKSLASH = 56,
    KEY9 = 57, C_CEDILLA = 57,//CIRCUMFLEX      = 57,
    KEY0 = 58, A_GRAVE = 58, AT = 58,
    DEGREE = 219, RPARENTHESE = 219, RSQUARE_BRACKET = 219,
    PLUS = 187, EQUALS = 187, RCURLY_BRACKET = 187,
    DIAERESIS = 221, CIRCUMFLEX = 221,
    POUND = 186, DOLLAR = 186,// CURRENCY = 186,
    PERCENTAGE = 192, U_GRAVE = 192,
    MU = 220, ASTERISK = 220,
    INTERROGATION = 188, COMMA = 188,
    PERIOD = 190, SEMICOLON = 190,
    SLASH = 191, COLON = 191,
    SECTION = 223, EXCLAMATION = 223,

    A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    BACKSPACE = 8,
    ENTER = 13,
    TAB = 9,
    CAPS_LOCK = 20,
    SPACE = 32, SPACEBAR = 32,
    MENU = 93,
    ANGLE_BRACKETS = 226,

    CTRL = 17,
    LCTRL = 162,
    RCTRL = 163,

    SHIFT = 16,
    LSHIFT = 160,
    RSHIFT = 161,

    ALTS = 18,
    LALT = 164, ALT = 164,
    RALT = 165, ALTGR = 165,

    LWINDOW = 91,
    RWINDOW = 92,

    INSERT = 45,
    DELETE = 46, DEL = 46,
    HOME = 36,
    END = 35,
    PAGE_UP = 33,
    PAGE_DOWN = 34,

    UP_ARROW = 38,// UP    = 38,
    LEFT_ARROW = 37,// LEFT  = 37,
    RIGHT_ARROW = 39,// RIGHT = 39,
    DOWN_ARROW = 40,// DOWN  = 40,

    NUM_LOCK = 144,
    NUMPAD_DIVIDE = 111,
    NUMPAD_MULTIPLY = 106,
    NUMPAD_SUBSTRACT = 109,
    NUMPAD_ADD = 107,
    NUMPAD_PERIOD = 110,
    NUMPAD0 = 96, NUMPAD1, NUMPAD2, NUMPAD3, NUMPAD4, NUMPAD5, NUMPAD6, NUMPAD7, NUMPAD8, NUMPAD9,

    KEYIDCOUNT = 257
};

bool GetKeyDown(Keys keyId);

bool GetKeyUp(Keys keyId);

bool GetKeyStay(Keys keyId);

};

namespace MOUSE
{
enum Buttons
{
    LEFT = 1,
    RIGHT,
    MIDDLE = 4,
    THUMB_1,
    THUMB_2,
    MOUSEIDCOUNT
};

bool GetMouseKeyDown(Buttons keyId);

bool GetMouseKeyUp(Buttons keyId);

bool GetMouseKeyStay(Buttons keyId);
};

class GCInputManager
{
friend class GCGameManager;

public:

    GCInputManager() = default;

    virtual void Update() {};

    virtual int GetStateSize() const = 0;
    virtual int GetIDSize() const = 0;
    
protected:
    void InitializeCallbacks();
    std::vector<std::vector<std::function<void(GCEvent&)>>> callbacks;
    GCEventManager* m_eventManager;
};

class GCKeyboardInputManager : public GCInputManager
{
    friend class GCInputManager;
    friend class GCInputSystem;

public:

    GCKeyboardInputManager();


    bool IsKeyPressed(int keyID);

    //////////////////////////////////////////////////
    /// @brief Return the state of the key
    ///
    /// @param keyID key's ID in the keyState list. 
    //////////////////////////////////////////////////
    BYTE GetKeyState(int keyID) { return m_keyState[keyID]; }

    bool GetKeyDown(int key);
    bool GetKeyStay(int key);
    bool GetKeyUp(int key);
    
    
    
    enum KeyboardState
    {
        NONE, // NOT PRESSED
        STAY, // STAY PRESSED
        UP,   // RELEASED
        DOWN, // PRESSED ONCE
        KEYSTATECOUNT
    };

    int GetIDSize() const override { return KEYBOARD::KEYIDCOUNT; };

    int GetStateSize() const override { return KeyboardState::KEYSTATECOUNT; };

    void RegisterForKeyEvents();

    /// <summary>
    /// Bind a function to a specific key and key state
    /// </summary>
    /// <param name="keyId">Key ID</param>
    /// <param name="state">Key State</param>
    /// <param name="func">The function can be generic</param>
    template<typename Func>
    void BindAction(int keyId, BYTE state, Func&& func)
    {
        auto callback = [func](GCEvent&) { func(); };
        callbacks[state][keyId] = callback;
    }

    /// <summary>
    /// Unbind a function based on key ID and key state
    /// the function should be a member function
    /// </summary>
    /// <param name="keyId">Key ID</param>
    /// <param name="state">Key State</param>
    /// <param name="func">member function</param>
    template<typename Func>
    void UnbindAction(int keyId, BYTE state, Func&& func)
    {
        auto callback = [func](GCEvent&) { func(); };
        auto& stateCallbacks = callbacks[state];

        auto it = std::find_if(stateCallbacks.begin(), stateCallbacks.end(),
            [&](const std::function<void(GCEvent&)>& storedCallback) 
            {
                return storedCallback.target_type() == callback.target_type();
            });

        if (it != stateCallbacks.end()) {
            stateCallbacks.erase(it);
        }
    }

    /// <summary>
    /// Unbind a function based on key ID and key state
    /// Unbind for lambda function
    /// KeyID and KeyState can access by the class name
    /// </summary>
    /// <param name="keyID">Key ID</param>
    /// <param name="keyState">Key State</param>
    void UnbindAction(int keyID, BYTE keyState);

private:
    void SendEvent(int index, BYTE state);

    void OnKeyPressed(GCKeyPressedEvent& ev);
    void OnKeyReleased(GCKeyReleasedEvent& ev);

    void Update();
private:
    std::vector<BYTE> m_keyState;
};

class GCMouseInputManager : public GCInputManager
{
    friend class GCInputManager;
    friend class GCInputSystem;

public: 
    GCMouseInputManager();

    bool IsKeyPressed(int keyID);

    BYTE GetKeyState(int keyID) { return m_buttonState[keyID]; }

    bool GetKeyDown(int key);
    bool GetKeyStay(int key);
    bool GetKeyUp(int key);

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

    int GetIDSize() const override { return MOUSE::MOUSEIDCOUNT; };

    int GetStateSize() const override { return MouseState::MOUSESTATECOUNT; };

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


class GCInputSystem
{
    friend class GCGameManager;

public:
    void SetEventManager(GCEventManager* eventMananger);

public:
    GCKeyboardInputManager* m_pKeyboard;
    GCMouseInputManager* m_pMouse;
    std::vector<GCControllerInputManager*> m_pControllerList;

protected:
    GCInputSystem();
    ~GCInputSystem() {}

    void GetConnectedControllers();
    void Update();

};