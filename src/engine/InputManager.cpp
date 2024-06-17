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

    m_pWindow->winPos = { 10 ,10 }; m_pWindow->winSize = { 800, 500 }; m_pWindow->center = { m_pWindow->winSize.x / 2 , m_pWindow->winSize.y / 2 }; // !! valeur random de la fenêtre à changer quand on aura la bonne window !!
    m_checkController = true;
    for ( int i = 0; i < XUSER_MAX_COUNT; i++ )
    {
        m_controllerList.PushBack( nullptr );
    }
}


// <summary>
// Function to get connected controllers.
// This function checks for connected controllers using XInputGetState.
// If a controller is connected and not already in the controller list,
// it creates a new ControllerInput object for that controller and adds it to the list.
// </summary>
void GCInputManager::GetConnectedController()
{

    XINPUT_STATE state;

    for ( int i = 0; i < XUSER_MAX_COUNT; i++ )
    {

        if ( XInputGetState( i, &state ) == ERROR_SUCCESS && m_controllerList.Get( i ) == nullptr )
        {
            GCControllerInput* pController = new GCControllerInput( i );
            m_controllerList.Insert( i, pController );
        }
    }
}


// <summary>
// Updates all input devices and their states.
// This function iterates through all connected controllers, updates their input states,
// and calls the respective update functions for the keyboard and mouse.
// It also sets the boolean flags for keyboard, mouse, and controller activity.
// </summary>
void GCInputManager::UpdateInputs()
{
    m_updatedKeys.Clear();

    for (int i = 0; i < 255; i++)
    {

        if (GetAsyncKeyState(i) != 0)
        {

            switch (m_keyState[i])
            {
            case NONE:
                AddToUpdateList(i, DOWN);
                break;
            case PUSH:
                AddToUpdateList(i, PUSH);
                break;
            case UP:
                AddToUpdateList(i, DOWN);
                break;
            case DOWN:
                AddToUpdateList(i, PUSH);
                break;

            }
        }
        else
        {

            switch (m_keyState[i])
            {
            case PUSH:
                AddToUpdateList(i, UP);
                break;
            case UP:
                m_keyState[i] = NONE;
                break;
            case DOWN:
                AddToUpdateList(i, UP);
                break;

            }
        }
    }


    if ( m_checkController == false ) return; 
    for ( int i = 0; i < XUSER_MAX_COUNT; i++ )
    {
        if ( m_controllerList[i] != nullptr )
        {
            m_controllerList[i]->m_updatedControllerKeys.Clear();
            m_controllerList[i]->UpdateControllerInput();
            m_controllerList[i]->UpdateJoySticksinput();
            m_controllerList[i]->UpdateTriggers();
            for (int j = 0; j < m_controllerList[i]->m_updatedControllerKeys.GetSize(); j++)
            {
                m_updatedKeys.PushBack(m_controllerList[i]->m_updatedControllerKeys[j]);
            }
        }
    }   
}

void GCInputManager::AddToUpdateList(int index, BYTE state)
{
    m_keyState[index] = state;
    m_updatedKeys.PushBack(index);
}






GCMouseInput::GCMouseInput()
{
    m_canLeaveWin = true;

    for (int i = 0; i < 6; i++)
    {
        m_pMouseButtons.PushBack(NONE);
    }

    m_mousePos = { 0,0 };
}


// <summary>
// Function to update the mouse input state.
// This function checks the state of each mouse button and updates the corresponding button state in the m_pMouseButtons array.
// It also updates the mouse position within the game window.
// </summary>
// <param name="pWinInfos"> A pointer to a WinTest structure containing information about the game window. </param>
void GCMouseInput::UpdateMouseInput(const WinTest* pWinInfos)
{

    POINT pointOnScreen;

    if ( GetCursorPos(&pointOnScreen) )
    {

        if ( pointOnScreen.x <= pWinInfos->winPos.x )
        {
            pointOnScreen.x = ( int ) pWinInfos->winPos.x;
        }
        else if ( pointOnScreen.x >= pWinInfos->winPos.x + pWinInfos->winSize.x )
        {
            pointOnScreen.x = ( int ) ( pWinInfos->winPos.x + pWinInfos->winSize.x );
        }

        if ( pointOnScreen.y <= pWinInfos->winPos.y )
        {
            pointOnScreen.y = ( int ) pWinInfos->winPos.y;
        }
        else if ( pointOnScreen.y >= pWinInfos->winPos.y + pWinInfos->winSize.y )
        {
            pointOnScreen.y = ( int ) ( pWinInfos->winPos.y + pWinInfos->winSize.y );
        }

        if ( m_canLeaveWin == false )
        {
            SetCursorPos( pointOnScreen.x, pointOnScreen.y );
        }


        m_mousePos.x = pointOnScreen.x - pWinInfos->center.x;
        m_mousePos.y = pointOnScreen.y - pWinInfos->center.y;

    }

}


// <summary>
// Function to check if a specific mouse button is in the DOWN state.
// This function checks the state of a given mouse button in the m_pMouseButtons array.
// If the state is DOWN, it returns true. Otherwise, it returns false.
// </summary>
// <param name="mouseButton"> The mouse button code to check. It should be a valid mouse button code. From 1 to 5 </param>
bool GCMouseInput::GetMouseDown(int mouseButton)
{
    if ( mouseButton > 5 || mouseButton < 1 )
        return false;
    if ( mouseButton > 2 )
        mouseButton += 1;

    if ( m_pMouseButtons[mouseButton] == DOWN )
    {
        return true;
    }
    return false;
}


// <summary>
// Function to check if a specific mouse button is in the PUSH state.
// This function checks the state of a given mouse button in the m_pMouseButtons array.
// If the state is PUSH, it returns true. Otherwise, it returns false.
// </summary>
// <param name="mouseButton"> The mouse button code to check. It should be a valid mouse button code. From 1 to 5  </param>
bool GCMouseInput::GetMouseStay(int mouseButton)
{
    if ( mouseButton > 5 || mouseButton < 1 )
        return false;

    if ( mouseButton > 2 )
        mouseButton += 1;

    if ( m_pMouseButtons[mouseButton] == PUSH )
    {
        return true;
    }
    return false;
}


// <summary>
// Function to check if a specific mouse button is in the UP state.
// This function checks the state of a given mouse button in the m_pMouseButtons array.
// It returns true if the mouse button is in the UP state, false otherwise.
// </summary>
// <param name="mouseButton"> The mouse button code to check. It should be a valid mouse button. From 1 to 5 </param>
bool GCMouseInput::GetMouseUp(int mouseButton)
{
    if ( mouseButton > 5 || mouseButton < 1 )
        return false;
    if ( mouseButton > 2 )
        mouseButton += 1;
    if ( m_pMouseButtons[mouseButton] == UP )
    {
        return true;
    }
    return false;
}


// <summary>
// Function to check if the mouse cursor is within a specified object's boundaries.
// This function checks if the mouse cursor's position is within the boundaries of a given object.
// The object's position and size are provided as parameters.
// </summary>
// <param name="objectPos"> A pointer to a GCVEC2 structure representing the object's position. </param>
// <param name="objSize"> A pointer to a GCVEC2 structure representing the object's size. </param>
bool GCMouseInput::OnMouseHover(GCVEC2* objectPos, GCVEC2* objSize) {

    if ( m_mousePos.x < objectPos->x )
        return false;
    if ( m_mousePos.x > objectPos->x + objSize->x )
        return false;
    if ( m_mousePos.y < objectPos->y )
        return false;
    if ( m_mousePos.y > objectPos->y + objSize->y )
        return false;

    return true;
}


GCControllerInput::GCControllerInput()
{
    m_ID = -1;
    m_pControllersLeftAxis[0] = 0.0; m_pControllersLeftAxis[1] = 0.0;
    m_pControllersRightAxis[0] = 0.0; m_pControllersRightAxis[1] = 0.0;
    m_pControllerTrigger[0] = 0.0; m_pControllerTrigger[1] = 0.0; 

    for (int j = 0; j < 16; j++)
    {
        m_pListofControllerKeys.PushBack(NONE);
    }
}

GCControllerInput::GCControllerInput(int id)
{
    m_ID = id;
    m_pControllersLeftAxis[0] = 0.0; m_pControllersLeftAxis[1] = 0.0;
    m_pControllersRightAxis[0] = 0.0; m_pControllersRightAxis[1] = 0.0;
    m_pControllerTrigger[0] = 0.0; m_pControllerTrigger[1] = 0.0; 
    for (int j = 0; j < 16; j++)
    {
        m_pListofControllerKeys.PushBack(NONE);
    }
}


// <summary>
// Function to check if a specific controller button is in the DOWN state.
// This function checks the state of a given virtual button in the m_pListofControllerKeys array.
// If the state is DOWN, it returns true. Otherwise, it returns false.
// </summary>
// <param name="vButton"> The virtual button code to check. It should be a valid virtual button code. </param>
bool GCControllerInput::GetControllerButtonDown(int vButton)
{
    int index = 0x5800;
    if ( vButton > 0x05807 )
        index += 8;

    if ( m_pListofControllerKeys[vButton - index] == DOWN )
        return true;
    return false;
}


// <summary>
// Function to check if a specific controller button is in the PUSH state.
// This function checks the state of a given virtual button in the m_pListofControllerKeys array.
// If the state is PUSH, it returns true. Otherwise, it returns false.
// </summary>
// <param name="vButton"> The virtual button code to check. It should be a valid virtual button code. </param>
bool GCControllerInput::GetControllerButtonStay(int vButton)
{
    int index = 0x5800;
    if ( vButton > 0x05807 )
        index += 8;

    if ( m_pListofControllerKeys[vButton - index] == PUSH )
        return true;
    return false;
}


// <summary>
// Function to check if a specific controller button is in the UP state.
// This function checks the state of a given virtual button in the m_pListofControllerKeys array.
// If the state is UP, it returns true. Otherwise, it returns false.
// </summary>
// <param name="vButton"> The virtual button code to check. It should be a valid virtual button code. </param>
bool GCControllerInput::GetControllerButtonUp(int vButton)
{
    int index = 0x5800;
    if ( vButton > 0x05807 )
        index += 8;

    if ( m_pListofControllerKeys[vButton - index] == UP )
        return true;
    return false;
}


// <summary>
// Updates the state of the controller's buttons.
// This function checks for keystrokes from the controller and updates the state of the buttons.
// It also handles the transition between different button states (DOWN, UP, PUSH).
// </summary>
void GCControllerInput::UpdateControllerInput()
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
                switch ( m_pListofControllerKeys[key.VirtualKey - j] )
                {
                case NONE:
                    m_pListofControllerKeys[key.VirtualKey - j] = DOWN;
                    break;
                case UP:
                    m_pListofControllerKeys[key.VirtualKey - j] = DOWN;
                    break;
                case DOWN:
                    m_pListofControllerKeys[key.VirtualKey - j] = PUSH;
                    break;
                }
            }

            else if ( key.VirtualKey == j + i )
            {
                switch ( m_pListofControllerKeys[key.VirtualKey - j] )
                {
                case PUSH:
                    m_pListofControllerKeys[key.VirtualKey - j] = UP;
                    break;
                case UP:
                    m_pListofControllerKeys[key.VirtualKey - j] = NONE;
                    break;
                case DOWN:
                    m_pListofControllerKeys[key.VirtualKey - j] = UP;
                    break;

                }
            }
        }
    }
}


// <summary>
// Updates the state of the controller's joystick axes.
// This function retrieves the state of the controller's left and right joystick axes
// and normalizes the values to a range between -1.0 and 1.0.
// The normalized values are stored in the m_pControllersLeftAxis and m_pControllersRightAxis arrays.
// </summary>
void GCControllerInput::UpdateJoySticksinput()
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


        if (rLX != 0.0 || rLY != 0.0 ) AddtoControllerListUpdate(16);


        m_pControllersLeftAxis[0] = rLX; m_pControllersLeftAxis[1] = rLY;


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

        if (rRX != 0.0 || rRY != 0.0) AddtoControllerListUpdate(17);

        m_pControllersRightAxis[0] = rRX; m_pControllersRightAxis[1] = rRY;
    }
}


// <summary>
// Updates the state of the controller's triggers.
// This function retrieves the state of the controller's left and right trigger
// and normalizes the values to a range between 0.0 and 1.0.
// The normalized values are stored in the m_pControllerTrigger arrays.
// </summary>
void GCControllerInput::UpdateTriggers()
{
    XINPUT_STATE state; 
    if ( XInputGetState( m_ID, &state ) == ERROR_SUCCESS )
    {
        float lTriggerState = state.Gamepad.bLeftTrigger;
        float rTriggerState = state.Gamepad.bRightTrigger;

        lTriggerState /= 255;  rTriggerState /= 255;
        
        if (lTriggerState != 0.0f) AddtoControllerListUpdate(18); 
        if (rTriggerState != 0.0f) AddtoControllerListUpdate(19); 

        m_pControllerTrigger[0] = lTriggerState; m_pControllerTrigger[1] = rTriggerState; 
    }
}


void GCControllerInput::AddtoControllerListUpdate(int index)
{
    int keyindex; 
    keyindex = 1000 + (100 * m_ID); 
    keyindex += index; 
    m_updatedControllerKeys.PushBack(keyindex); 
}