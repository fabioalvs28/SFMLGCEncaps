#include "pch.h"
#include "InputManager.h"
#include <xinput.h>
#include <math.h>
#pragma comment(lib,"xinput.lib")
#include "../core/framework.h"



#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689


GCInputManager::GCInputManager()
{
    callbacks = GCVector<GCVector<std::function<void(GCEvent&)>>>(GetStateSize());
    for (int i = 0; i < callbacks.GetSize(); ++i)
    {
        callbacks[i] = GCVector<std::function<void(GCEvent&)>>(GetIDSize());
    }

}

void GCInputManager::Init()
{
    m_pKeyboardManager = new GCKeyboardInputManager();
    m_pMouseManager = new GCMouseInputManager();
    for (int i = 0; i < XUSER_MAX_COUNT; i++)
    {
        m_controllerList.PushBack(nullptr);
    }
}

////////////////////////////////////////////////////
/// @brief Function to get connected controllers.
////////////////////////////////////////////////////
void GCInputManager::GetConnectedControllers()
{

    XINPUT_STATE state;

    for ( int i = 0; i < XUSER_MAX_COUNT; i++ )
    {

        if ( XInputGetState( i, &state ) == ERROR_SUCCESS && m_controllerList.Get( i ) == nullptr )
        {
            GCControllerInputManager* pController = new GCControllerInputManager( i );
            m_controllerList.Insert( i, pController );
        }
    }
}

/////////////////////////////////////////////////////////
/// @brief Updates all input devices and their states.
/// 
/// @note Must be called every gameloop.
/////////////////////////////////////////////////////////
void GCInputManager::Update()
{
    GetConnectedControllers();
    m_pKeyboardManager->Update();
    m_pMouseManager->Update();

    for ( int i = 0; i < XUSER_MAX_COUNT; i++ )
    {
        if ( m_controllerList[i] != nullptr )
        {
            m_controllerList[i]->Update();
        }
    }   
}

void GCKeyboardInputManager::SendEvent(int index, BYTE state)
{
    m_keyState[index] = state;
}


GCKeyboardInputManager::GCKeyboardInputManager()
{
    for (int i = 0; i < 255; i++)
    {
        m_keyState.PushBack(GCKeyboardInputManager::NONE);
    }
}

void GCKeyboardInputManager::Update()
{
    for (int i = 0; i < KeyboardID::KEYIDCOUNT; i++)
    {

        if (GetAsyncKeyState(i) != 0)
        {

            switch (m_keyState[i])
            {
            case GCKeyboardInputManager::NONE :
                SendEvent(i, GCKeyboardInputManager::DOWN);
                break;
            case GCKeyboardInputManager::STAY:
                SendEvent(i, GCKeyboardInputManager::STAY);
                break;
            case GCKeyboardInputManager::UP:
                SendEvent(i, GCKeyboardInputManager::DOWN);
                break;
            case GCKeyboardInputManager::DOWN:
                SendEvent(i, GCKeyboardInputManager::STAY);
                break;

            }
        }
        else if (m_keyState[i] != GCKeyboardInputManager::NONE)
        {

            switch (m_keyState[i])
            {
            case GCKeyboardInputManager::STAY:
                SendEvent(i, GCKeyboardInputManager::UP);
                break;
            case GCKeyboardInputManager::UP:
                SendEvent(i, GCKeyboardInputManager::NONE);
                break;
            case GCKeyboardInputManager::DOWN:
                SendEvent(i, GCKeyboardInputManager::UP);
                break;

            }
        }
    }

}


////////////////////////////////////////////////////////////////////////////////////
/// @brief Return true if the given key have been pressed or relased in the frame
/// 
/// @param keyID : key's index in the list. 
////////////////////////////////////////////////////////////////////////////////////
bool GCKeyboardInputManager::IsKeyPressed(int keyID)
{
    if (m_keyState[keyID] != GCKeyboardInputManager::NONE)
        return true;
    return false;
}


///////////////////////////////////////////////////////
/// @brief Return true if the given key is DOWN  
/// 
/// @param key key's index in the list. 
///////////////////////////////////////////////////////
bool GCKeyboardInputManager::GetKeyDown(int key)
{
    if (m_keyState[key] == GCKeyboardInputManager::DOWN)
    {
        return true;
    }
    return false;
}

///////////////////////////////////////////////////////
/// @brief Return true if the given key is PUSH
/// 
/// @param key key's index in the list. 
///////////////////////////////////////////////////////
bool GCKeyboardInputManager::GetKeyStay(int key)
{
    if (m_keyState[key] == GCKeyboardInputManager::STAY)
    {
        return true;
    }
    return false;
}

/////////////////////////////////////////////////////
/// @brief Return true if the given key is UP
/// 
/// @param key key's index in the list. 
/////////////////////////////////////////////////////
bool GCKeyboardInputManager::GetKeyUp(int key)
{
    if (m_keyState[key] == GCKeyboardInputManager::UP)
    {
        return true;
    }
    return false;
}




GCMouseInputManager::GCMouseInputManager()
{
    for (int i = 0 ; i < MouseID::MOUSEIDCOUNT; i++)
    {
        m_buttonState.PushBack(GCMouseInputManager::NONE);
    }
}

bool GCMouseInputManager::IsKeyPressed(int keyID)
{
    if (m_buttonState[keyID] != GCMouseInputManager::NONE)
        return true;
    return false;
}


bool GCMouseInputManager::GetKeyDown(int key)
{
    if (m_buttonState[key] == GCMouseInputManager::DOWN)
    {
        return true;
    }
    return false;
}

///////////////////////////////////////////////////////
/// @brief Return true if the given key is PUSH
/// 
/// @param key key's index in the list. 
///////////////////////////////////////////////////////
bool GCMouseInputManager::GetKeyStay(int key)
{
    if (m_buttonState[key] == GCMouseInputManager::STAY)
    {
        return true;
    }
    return false;
}

/////////////////////////////////////////////////////
/// @brief Return true if the given key is UP
/// 
/// @param key key's index in the list. 
/////////////////////////////////////////////////////
bool GCMouseInputManager::GetKeyUp(int key)
{
    if (m_buttonState[key] == GCMouseInputManager::UP)
    {
        return true;
    }
    return false;
}

void GCMouseInputManager::SendEvent(int index, BYTE state)
{
   m_buttonState[index] = state;
}


void GCMouseInputManager::Update()
{
    for (int i = 0; i < MouseID::MOUSEIDCOUNT; i++)
    {

        if (GetAsyncKeyState(i) != 0)
        {

            switch (m_buttonState[i])
            {
            case GCMouseInputManager::NONE:
                SendEvent(i, GCMouseInputManager::DOWN);
                break;
            case GCMouseInputManager::STAY:
                SendEvent(i, GCMouseInputManager::STAY);
                break;
            case GCMouseInputManager::UP:
                SendEvent(i, GCMouseInputManager::DOWN);
                break;
            case GCMouseInputManager::DOWN:
                SendEvent(i, GCMouseInputManager::STAY);
                break;

            }
        }
        else if (m_buttonState[i] != NONE)
        {

            switch (m_buttonState[i])
            {
            case GCMouseInputManager::STAY:
                SendEvent(i, GCMouseInputManager::UP);
                break;
            case GCMouseInputManager::UP:
                SendEvent(i, GCMouseInputManager::NONE);
                break;
            case GCMouseInputManager::DOWN:
                SendEvent(i, GCMouseInputManager::UP);
                break;

            }
        }
    }
}

GCControllerInputManager::GCControllerInputManager()
{
    m_ID = -1;
    m_pControllersLeftAxis.x = 0.0; m_pControllersLeftAxis.y = 0.0;
    m_pControllersRightAxis.x = 0.0; m_pControllersRightAxis.y = 0.0;
    m_pControllerTrigger.x = 0.0; m_pControllerTrigger.y = 0.0;

    for (int j = 0; j < ControllerID::CONTROLLERIDCOUNT; j++)
    {
        m_buttonState.PushBack(GCControllerInputManager::NONE);
    }
}


GCControllerInputManager::GCControllerInputManager(int id)
{
    m_ID = id;
    m_pControllersLeftAxis.x = 0.0; m_pControllersLeftAxis.y = 0.0;
    m_pControllersRightAxis.x = 0.0; m_pControllersRightAxis.y = 0.0;
    m_pControllerTrigger.x = 0.0; m_pControllerTrigger.y = 0.0;
    for (int j = 0; j < ControllerID::CONTROLLERIDCOUNT; j++)
    {
        m_buttonState.PushBack(GCControllerInputManager::NONE);
    }
}

////////////////////////////////////////////////////////////////
/// @brief Return true if the given controller button is DOWN  
/// 
/// @param vButton button's index in the list. 
////////////////////////////////////////////////////////////////
bool GCControllerInputManager::GetControllerButtonDown(int vButton)
{
    int index = 0x5800;
    if ( vButton > 0x05807 )
        index += 8;

    if ( m_buttonState[vButton - index] == GCControllerInputManager::DOWN )
        return true;
    return false;
}

/////////////////////////////////////////////////////////////////
/// @brief Return true if the given controller button is PUSH  
/// 
/// @param vButton button's index in the list. 
/////////////////////////////////////////////////////////////////
bool GCControllerInputManager::GetControllerButtonStay(int vButton)
{
    int index = 0x5800;
    if ( vButton > 0x05807 )
        index += 8;

    if ( m_buttonState[vButton - index] == GCControllerInputManager::STAY )
        return true;
    return false;
}

//////////////////////////////////////////////////////////////
/// @brief Return true if the given controller button is UP
/// 
/// @param vButton button's index in the list. 
//////////////////////////////////////////////////////////////
bool GCControllerInputManager::GetControllerButtonUp(int vButton)
{
    int index = 0x5800;
    if ( vButton > 0x05807 )
        index += 8;

    if ( m_buttonState[vButton - index] == GCControllerInputManager::UP )
        return true;
    return false;
}

void GCControllerInputManager::Update()
{
    UpdateControllerInput();
    UpdateJoySticksinput();
    UpdateTriggers();
}

void GCControllerInputManager::SendEvent(int index, BYTE state)
{
    m_buttonState[index] = state;
}

///////////////////////////////////////////////////////////////////////////////
/// @brief Updates the state of the controller's buttons.
/// 
/// @note Calls the function to update joysticks and triggers' analog state
///////////////////////////////////////////////////////////////////////////////
void GCControllerInputManager::UpdateControllerInput()
{
    XINPUT_KEYSTROKE key;

    int j = 0;

    if (XInputGetKeystroke(m_ID, 0, &key) == ERROR_SUCCESS)
    {

        for ( int i = 0; i < 16; i++ )
        {
            j = 0x5800;

            if ( i > 7 )
                j = 0x5800 + 8;

            if ( key.VirtualKey == j + i && key.Flags == 1 )
            {
                switch ( m_buttonState[key.VirtualKey - j] )
                {
                case GCControllerInputManager::NONE:
                    SendEvent(key.VirtualKey - j, GCControllerInputManager::DOWN);
                    break;
                case GCControllerInputManager::UP:
                    SendEvent(key.VirtualKey - j, GCControllerInputManager::DOWN);
                    break;
                case GCControllerInputManager::DOWN:
                    SendEvent(key.VirtualKey - j, GCControllerInputManager::STAY);
                    break;
                }
            }

            else if ( key.VirtualKey == j + i )
            {
                switch ( m_buttonState[key.VirtualKey - j] )
                {
                case GCControllerInputManager::STAY:
                    SendEvent(key.VirtualKey - j, GCControllerInputManager::UP);
                    break;
                case GCControllerInputManager::UP:
                    SendEvent(key.VirtualKey - j, GCControllerInputManager::NONE);
                    break;
                case GCControllerInputManager::DOWN:
                    SendEvent(key.VirtualKey - j, GCControllerInputManager::UP);
                    break;

                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Updates the state of the controller's joysticks.
/// 
/// @note This function retrieves the state of the controller's left and right joystick axes 
/// and normalizes the values to a range between -1.0 and 1.0.
/// 
///////////////////////////////////////////////////////////////////////////////////////////////

void GCControllerInputManager::UpdateJoySticksinput()
{
    XINPUT_STATE state;

    int side[2] = { 1,1 };

    if ( XInputGetState( m_ID, &state ) == ERROR_SUCCESS )
    {

        float LX = state.Gamepad.sThumbLX;
        float LY = state.Gamepad.sThumbLY;


        if (LX <= -32767) LX = -32767;
        if (LY <= -32767) LY = -32767;

        float rLX = LX / 32767;  float rLY = LY / 32767;

        if ( rLX < 0 ) side[0] = -1; if ( rLY < 0 ) side[1] = -1;

        rLX = rLX * rLX;
        rLY = rLY * rLY;


        if ( LX * side[0] + LY * side[1] < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE )
        {
            rLX = 0.0f, rLY = 0.0f;
        }




        m_pControllersLeftAxis.x = rLX; m_pControllersLeftAxis.y = rLY;


        side[0] = 1; side[1] = 1;

        float RX = state.Gamepad.sThumbRX;
        float RY = state.Gamepad.sThumbRY;


        if ( RX <= -32767 ) RX = -32767;
        if ( RY <= -32767 ) RY = -32767;

        float rRX = RX / 32767;  float rRY = RY / 32767;

        if ( rRX < 0 ) side[0] = -1; if ( rRY < 0 ) side[1] = -1;

        rRX = rRX * rRX;
        rRY = rRY * rRY;


        if ( RX * side[0] + RY * side[1] < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE )
        {
            rRX = 0.0, rRY = 0.0;
        }


        m_pControllersRightAxis.x = rRX; m_pControllersRightAxis.y = rRY;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @brief Updates the state of the controller's triggers.
/// 
/// @note This function retrieves the state of the controller's left and right triggers
/// and normalizes the values to a range between 0.0 and 1.0.
/// 
//////////////////////////////////////////////////////////////////////////////////////////
void GCControllerInputManager::UpdateTriggers()
{
    XINPUT_STATE state; 
    if ( XInputGetState( m_ID, &state ) == ERROR_SUCCESS )
    {
        float lTriggerState = state.Gamepad.bLeftTrigger;
        float rTriggerState = state.Gamepad.bRightTrigger;

        lTriggerState /= 255;  rTriggerState /= 255;
         

        m_pControllerTrigger.x = lTriggerState; m_pControllerTrigger.y = rTriggerState; 
    }
}
