#pragma once
#define NOMINMAX

#include "pch.h"



enum GCKEYBOARD
{
    F1 = 112, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
    
    KEY1 = 49, AMPERSAND        = 49,
    KEY2 = 50, E_ACUTE          = 50, TILDE           = 50,
    KEY3 = 51, DOUBLE_QUOTATION = 51, HASHTAG         = 51,
    KEY4 = 52, SINGLE_QUOTATION = 52, LCURLY_BRACKET  = 52,
    KEY5 = 53, LPARENTHESE      = 53, LSQUARE_BRACKET = 53,
    KEY6 = 54, MINUS            = 54, VERTICAL_BAR    = 54,
    KEY7 = 55, E_GRAVE          = 55, GRAVE_ACCENT    = 55,
    KEY8 = 56, UNDERSCORE       = 56, BACKSLASH       = 56,
    KEY9 = 57, C_CEDILLA        = 57, //CIRCUMFLEX      = 57,
    KEY0 = 58, A_GRAVE          = 58, AT              = 58,
    
    A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    
    ESCAPE    = 27,  ESC      = 27,
    BACKSPACE = 8,
    TAB       = 9,
    ENTER     = 13,
    CAPS_LOCK = 20,
    SHIFT     = 16,
    LSHIFT    = 160,
    RSHIFT    = 161,
    CTRL      = 17,
    LCTRL     = 162,
    RCTRL     = 163,
    LWINDOW   = 91,
    RWINDOW   = 92,
    ALTS      = 18,
    LALT      = 164, ALT      = 164,
    RALT      = 165, ALTGR    = 165,
    SPACE     = 32,  SPACEBAR = 32,
    MENU      = 93,
    
    SCROLL_LOCK = 145,
    PAUSE       = 19,
    INSERT      = 45,
    HOME        = 36,
    PAGE_UP     = 33,
    /*DELETE      = 46,*/ DEL = 46,
    END         = 35,
    PAGE_DOWN   = 34,
    
    UP_ARROW    = 38, //UP    = 38,
    LEFT_ARROW  = 37, //LEFT  = 37,
    RIGHT_ARROW = 39, //RIGHT = 39,
    DOWN_ARROW  = 40, //DOWN  = 40,
    
    NUM_LOCK         = 144,
    NUMPAD_DIVIDE    = 111,
    NUMPAD_MULTIPLY  = 106,
    NUMPAD_SUBSTRACT = 109,
    NUMPAD_ADD       = 107,
    NUMPAD_PERIOD    = 110,
    NUMPAD0 = 96, NUMPAD1, NUMPAD2, NUMPAD3, NUMPAD4, NUMPAD5, NUMPAD6, NUMPAD7, NUMPAD8, NUMPAD9,
    
    SQUARE         = 222,
    DEGREE         = 219, RPARENTHESE = 219, RSQUARE_BRACKET = 219,
    PLUS           = 187, EQUALS      = 187, RCURLY_BRACKET  = 187,
    DIAERESIS      = 221, CIRCUMFLEX  = 221,
    POUND          = 186, DOLLAR      = 186, //CURRENCY        = 186,
    PERCENTAGE     = 192, U_GRAVE     = 192,
    MU             = 220, ASTERISK    = 220,
    ANGLE_BRACKETS = 226,
    INTERROGATION  = 188, COMMA       = 188,
    PERIOD         = 190, SEMICOLON   = 190,
    SLASH          = 191, COLON       = 191,
    SECTION        = 223, EXCLAMATION = 223,
    
    KEYIDCOUNT = 257
};

enum GCMOUSE
{
    LEFT     = 1,
    RIGHT    = 2,
    MIDDLE   = 4,
    BACKWARD = 5,
    FORWARD  = 6,
    
    MOUSEIDCOUNT = 7
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
    GCEventManager* m_pEventManager;
};

class GCKeyboardInputManager : public GCInputManager
{
    friend class GCInputManager;
    friend class GCInputSystem;

public:

    GCKeyboardInputManager();

    bool IsKeyPressed( int keyID );

    //////////////////////////////////////////////////
    /// @brief Return the state of the key.
    ///
    /// @param keyID key's ID in the keyState list. 
    //////////////////////////////////////////////////
    BYTE GetKeyState( int keyID ) { return m_keyState[ keyID ]; }

    bool GetKeyDown( int key );
    bool GetKeyStay( int key );
    bool GetKeyUp( int key );
    
    enum KeyboardState
    {
        NONE, // NOT PRESSED
        STAY, // STAY PRESSED
        UP,   // RELEASED
        DOWN, // PRESSED ONCE
        KEYSTATECOUNT
    };

    int GetIDSize() const override { return GCKEYBOARD::KEYIDCOUNT; };

    int GetStateSize() const override { return KeyboardState::KEYSTATECOUNT; };

    void RegisterForKeyEvents();
    
    //////////////////////////////////////////////////////////////
    /// @brief Bind a function to a specific key and key state.
    /// 
    /// @param keyId Key ID.
    /// @param state Key State.
    /// @param func The function can be generic.
    //////////////////////////////////////////////////////////////
    template<typename Func>
    void BindAction( int keyId, BYTE state, Func&& func )
    { callbacks[ state ][ keyId ] = [ func ](GCEvent&) { func(); }; }
    
    //////////////////////////////////////////////////////////////
    /// @brief Unbind a function based on key ID and key state.
    /// 
    /// @tparam Func 
    /// 
    /// @param keyId Key ID.
    /// @param state Key State.
    /// @param func Member function.
    /// 
    /// @note The function should be a member function.
    //////////////////////////////////////////////////////////////
    template<typename Func>
    void UnbindAction( int keyId, BYTE state, Func&& func )
    {
        auto callback = [func](GCEvent&) { func(); };
        auto& stateCallbacks = callbacks[state];

        auto it = std::find_if(
            stateCallbacks.begin(),
            stateCallbacks.end(),
            [&]( const std::function<void(GCEvent&)>& storedCallback) { return storedCallback.target_type() == callback.target_type(); }
        );

        if (it != stateCallbacks.end())
            stateCallbacks.erase( it );
    }
    
    //////////////////////////////////////////////////////////////
    /// @brief Unbind a function based on key ID and key state.
    /// 
    /// @param keyID Key ID.
    /// @param keyState Key State.
    /// 
    /// @note Unbind for lambda function.
    /// @note KeyID and KeyState can access by the class name.
    //////////////////////////////////////////////////////////////
    void UnbindAction( int keyID, BYTE keyState );

private:
    void SendEvent( int index, BYTE state );

    void OnKeyPressed( GCKeyPressedEvent& event );
    void OnKeyReleased( GCKeyReleasedEvent& event );

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

    bool IsKeyPressed( int keyID );

    BYTE GetKeyState( int keyID ) { return m_buttonState[ keyID ]; }

    bool GetKeyDown( int key );
    bool GetKeyStay( int key );
    bool GetKeyUp( int key );

    GCVEC2 GetMousePos() { return m_mousePos; }

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

    int GetIDSize() const override { return GCMOUSE::MOUSEIDCOUNT; };

    int GetStateSize() const override { return MouseState::MOUSESTATECOUNT; };
    std::vector<BYTE> m_buttonState;
    GCVEC2 m_mousePos;
    void SendEvent( int index, BYTE state );
    
};


class GCControllerInputManager : public GCInputManager
{

public: 

    GCControllerInputManager();
    GCControllerInputManager( int id );

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

    GCVEC2* GetControllerLeftJoyStick( int controllerID ) { return &m_controllersLeftAxis; }
    GCVEC2* GetControllerRightJoyStick( int controllerID ) { return &m_controllersRightAxis; }
    float GetControllerLeftAxisX( int controllerID ) { return m_controllersLeftAxis.x; }
    float GetControllerLeftAxisY( int controllerID ) { return m_controllersLeftAxis.y; }


    float GetControllerRightAxisX( int controllerID ) { return m_controllersRightAxis.x; }
    float GetControllerRightAxisY( int controllerID ) { return m_controllersRightAxis.y; }

    float GetControllerLeftTriggerState( int controllerID ) { return m_controllerTrigger.x; }
    float GetControllerRightTriggerState( int controllerID ) { return m_controllerTrigger.y; }

    bool GetControllerButtonDown( int vButton );
    bool GetControllerButtonStay( int vButton );
    bool GetControllerButtonUp( int vButton );

    GCVector<BYTE> m_buttonState;
private:



    void UpdateJoySticksinput();
    void UpdateControllerInput();
    void UpdateTriggers();

    void SendEvent( int index, BYTE state );

    int m_ID;
    GCVEC2 m_controllersLeftAxis;
    GCVEC2 m_controllersRightAxis;
    GCVEC2 m_controllerTrigger; // 0 - left, 1 - Right ;

    GCVector<int> m_updatedControllerKeys;


};


class GCInputSystem
{
    friend class GCGameManager;

public:
    void SetEventManager( GCEventManager* pEventMananger );

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



class GCINPUTS
{
friend class GCGameManager;

public:
    GCINPUTS() = delete;
    ~GCINPUTS() = delete;
    
    static bool GetKeyDown( GCKEYBOARD keyId );
    static bool GetKeyStay( GCKEYBOARD keyId );
    static bool GetKeyUp( GCKEYBOARD keyId );
    
    static bool GetKeyDown( GCMOUSE keyId );
    static bool GetKeyStay( GCMOUSE keyId );
    static bool GetKeyUp( GCMOUSE keyId );

    static GCVEC2 GetMousePos();

// private:
//     static void Update();

// private:
//     static GCInputSystem* s_pActiveInputSystem;

};