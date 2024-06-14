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
    m_controllerIsActive = false;  m_keyboardIsActive = false;  m_mouseIsActive = false;
    m_pWindow->winPos = { 10 ,10 }; m_pWindow->winSize = { 800, 500 }; m_pWindow->center = { m_pWindow->winSize.x / 2 , m_pWindow->winSize.y / 2 }; // !! valeur random de la fenêtre à changer quand on aura la bonne window !!
    for (int i = 0; i < XUSER_MAX_COUNT; i++)
    {
        m_controllerList.PushBack(nullptr);
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

    for (int i = 0; i < XUSER_MAX_COUNT; i++)
    {

        if (XInputGetState(i, &state) == ERROR_SUCCESS && m_controllerList.Get(i) == nullptr)
        {
            ControllerInput* pController = new ControllerInput(i);
            m_controllerList.Insert(i, pController);
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
    m_keyboardIsActive, m_mouseIsActive, m_controllerIsActive = false;

    for (int i = 0; i < XUSER_MAX_COUNT; i++)
    {

        if (m_controllerList[i] != nullptr)
        {
            m_controllerList[i]->UpdateControllerInput(m_controllerIsActive);
            m_controllerList[i]->UpdateJoySticksinput();
        }
    }

    m_keyboard.UpdateKeyboardInput(m_keyboardIsActive);
    m_mouse.UpdateMouseInput(m_pWindow, m_mouseIsActive);

}



KeyboardInput::KeyboardInput()
{
    for (int i = 0; i < 255; i++)
    {
        pListOfKeyboardKeys.PushBack(NONE);

    }
}



// <summary>
// Function to update the keyboard input state.
// This function checks the state of each keyboard key using GetAsyncKeyState.
// It updates the corresponding key state in the pListOfKeyboardKeys array.
// If a key is pressed, it sets the state to DOWN or PUSH if it was already DOWN.
// If a key is released, it sets the state to UP or NONE if it was already UP.
// </summary>
// <param name="isActive"> A boolean indicating whether the keyboard input is active. It is set to true if any key is pressed. </param>

void KeyboardInput::UpdateKeyboardInput(bool isActive)
{

    for (int i = 5; i < 255; i++)
    {

        if (GetAsyncKeyState(i) != 0)
        {
            isActive = true;

            switch (pListOfKeyboardKeys[i])
            {
            case NONE:
                pListOfKeyboardKeys[i] = DOWN;
                break;
            case UP:
                pListOfKeyboardKeys[i] = DOWN;
                break;
            case DOWN:
                pListOfKeyboardKeys[i] = PUSH;
                break;

            }
        }
        else
        {

            switch (pListOfKeyboardKeys[i])
            {
            case PUSH:
                pListOfKeyboardKeys[i] = UP;
                break;
            case UP:
                pListOfKeyboardKeys[i] = NONE;
                break;
            case DOWN:
                pListOfKeyboardKeys[i] = UP;
                break;

            }
        }
    }

}



// <summary>
// Function to check if a specific keyboard key is in the DOWN state.
// This function checks the state of a given virtual key in the pListOfKeyboardKeys array.
// If the state is DOWN, it returns true. Otherwise, it returns false.
// </summary>
// <param name="vKey"> The virtual key code to check. </param>
bool KeyboardInput::GetKeyDown(int vKey)
{
    if (pListOfKeyboardKeys[vKey] == DOWN)
    {
        return true;
    }
    return false;
}



// <summary>
// Function to check if a specific keyboard key is in the PUSH state.
// This function checks the state of a given virtual key in the pListOfKeyboardKeys array.
// If the state is PUSH, it returns true. Otherwise, it returns false.
// </summary>
// <param name="vKey> The virtual key code to check. It should be a valid virtual key code. </param>
bool KeyboardInput::GetKeyStay(int vKey)
{
    if (pListOfKeyboardKeys[vKey] == PUSH)
    {
        return true;
    }
    return false;
}


// <summary>
// Function to check if a specific keyboard key is in the UP state.
// This function checks the state of a given virtual key in the pListOfKeyboardKeys array.
// If the state is UP, it returns true. Otherwise, it returns false.
// </summary>
// <param name="vKey"> The virtual key code to check. It should be a valid virtual key code. </param>
bool KeyboardInput::GetKeyUp(int vKey)
{
    if (pListOfKeyboardKeys[vKey] == UP)
    {
        return true;
    }
    return false;
}


MouseInput::MouseInput()
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
// This function checks the state of each mouse button and updates the corresponding button state in the pMouseButtons array.
// It also updates the mouse position within the game window.
// </summary>
// <param name="pWinInfos"> A pointer to a GCWINDOW structure containing information about the game window. </param>
// <param name="isActive"> A boolean indicating whether the mouse input is active. It is set to true if any button is pressed or the mouse position changes. </param>
void MouseInput::UpdateMouseInput(const GCWINDOW* pWinInfos, bool isActive)
{

    POINT pointOnScreen;

    if (GetCursorPos(&pointOnScreen))
    {

        if (pointOnScreen.x <= pWinInfos->winPos.x)
        {
            pointOnScreen.x = (int)pWinInfos->winPos.x;
        }
        else if (pointOnScreen.x >= pWinInfos->winPos.x + pWinInfos->winSize.x)
        {
            pointOnScreen.x = (int)(pWinInfos->winPos.x + pWinInfos->winSize.x);
        }

        if (pointOnScreen.y <= pWinInfos->winPos.y)
        {
            pointOnScreen.y = (int)pWinInfos->winPos.y;
        }
        else if (pointOnScreen.y >= pWinInfos->winPos.y + pWinInfos->winSize.y)
        {
            pointOnScreen.y = (int)(pWinInfos->winPos.y + pWinInfos->winSize.y);
        }

        if (m_canLeaveWin == false)
        {
            SetCursorPos(pointOnScreen.x, pointOnScreen.y);
        }

        if (m_mousePos.x != pointOnScreen.x - pWinInfos->center.x || m_mousePos.y != pointOnScreen.y - pWinInfos->center.y) isActive = true;

        m_mousePos.x = pointOnScreen.x - pWinInfos->center.x;
        m_mousePos.y = pointOnScreen.y - pWinInfos->center.y;

    }

    for (int i = 0; i < 5; i++)
    {
        if (GetAsyncKeyState(i))
        {
            isActive = true;

            switch (m_pMouseButtons[i])
            {
            case NONE:
                m_pMouseButtons[i] = DOWN;
                break;
            case UP:
                m_pMouseButtons[i] = DOWN;
                break;
            case DOWN:
                m_pMouseButtons[i] = PUSH;
                break;

            }
        }
        else
        {

            switch (m_pMouseButtons[i])
            {
            case PUSH:
                m_pMouseButtons[i] = UP;
                break;
            case UP:
                m_pMouseButtons[i] = NONE;
                break;
            case DOWN:
                m_pMouseButtons[i] = UP;
                break;
            }
        }
    }


}



// <summary>
// Function to check if a specific mouse button is in the DOWN state.
// This function checks the state of a given mouse button in the pMouseButtons array.
// If the state is DOWN, it returns true. Otherwise, it returns false.
// </summary>
// <param name="mouseButton"> The mouse button code to check. It should be a valid mouse button code. From 1 to 5 </param>
bool MouseInput::GetMouseDown(int mouseButton)
{
    if (mouseButton > 5 || mouseButton < 1)
        return false;
    if (mouseButton > 2)
        mouseButton += 1;

    if (m_pMouseButtons[mouseButton] == DOWN)
    {
        return true;
    }
    return false;
}



// <summary>
// Function to check if a specific mouse button is in the PUSH state.
// This function checks the state of a given mouse button in the pMouseButtons array.
// If the state is PUSH, it returns true. Otherwise, it returns false.
// </summary>
// <param name="mouseButton"> The mouse button code to check. It should be a valid mouse button code. From 1 to 5  </param>
bool MouseInput::GetMouseStay(int mouseButton)
{
    if (mouseButton > 5 || mouseButton < 1)
        return false;

    if (mouseButton > 2)
        mouseButton += 1;

    if (m_pMouseButtons[mouseButton] == PUSH)
    {
        return true;
    }
    return false;
}


// <summary>
// Function to check if a specific mouse button is in the UP state.
// This function checks the state of a given mouse button in the pMouseButtons array.
// It returns true if the mouse button is in the UP state, false otherwise.
// </summary>
// <param name="mouseButton"> The mouse button code to check. It should be a valid mouse button. From 1 to 5 </param>
bool MouseInput::GetMouseUp(int mouseButton)
{
    if (mouseButton > 5 || mouseButton < 1)
        return false;
    if (mouseButton > 2)
        mouseButton += 1;
    if (m_pMouseButtons[mouseButton] == UP)
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
bool MouseInput::OnMouseHover(GCVEC2* objectPos, GCVEC2* objSize) {

    if (m_mousePos.x < objectPos->x)
        return false;
    if (m_mousePos.x > objectPos->x + objSize->x)
        return false;
    if (m_mousePos.y < objectPos->y)
        return false;
    if (m_mousePos.y > objectPos->y + objSize->y)
        return false;

    return true;
}



ControllerInput::ControllerInput()
{
    m_ID = -1;
    pControllersLeftAxis[0] = 0.0; pControllersLeftAxis[1] = 0.0;
    pControllersRightAxis[0] = 0.0; pControllersRightAxis[1] = 0.0;

    for (int j = 0; j < 16; j++)
    {
        pListofControllerKeys.PushBack(NONE);
    }
}

ControllerInput::ControllerInput(int id)
{
    m_ID = id;
    pControllersLeftAxis[0] = 0.0; pControllersLeftAxis[1] = 0.0;
    pControllersRightAxis[0] = 0.0; pControllersRightAxis[1] = 0.0;

    for (int j = 0; j < 16; j++)
    {
        pListofControllerKeys.PushBack(NONE);
    }
}


// <summary>
// Function to check if a specific controller button is in the DOWN state.
// This function checks the state of a given virtual button in the pListofControllerKeys array.
// If the state is DOWN, it returns true. Otherwise, it returns false.
// </summary>
// <param name="vButton"> The virtual button code to check. It should be a valid virtual button code. </param>
bool ControllerInput::GetControllerButtonDown(int vButton)
{
    int index = 0x5800;
    if (vButton > 0x05807)
        index += 8;

    if (pListofControllerKeys[vButton - index] == DOWN)
        return true;
    return false;
}



// <summary>
// Function to check if a specific controller button is in the PUSH state.
// This function checks the state of a given virtual button in the pListofControllerKeys array.
// If the state is PUSH, it returns true. Otherwise, it returns false.
// </summary>
// <param name="vButton"> The virtual button code to check. It should be a valid virtual button code. </param>
bool ControllerInput::GetControllerButtonStay(int vButton)
{
    int index = 0x5800;
    if (vButton > 0x05807)
        index += 8;

    if (pListofControllerKeys[vButton - index] == PUSH)
        return true;
    return false;
}



// <summary>
// Function to check if a specific controller button is in the UP state.
// This function checks the state of a given virtual button in the pListofControllerKeys array.
// If the state is UP, it returns true. Otherwise, it returns false.
// </summary>
// <param name="vButton"> The virtual button code to check. It should be a valid virtual button code. </param>
bool ControllerInput::GetControllerButtonUp(int vButton)
{
    int index = 0x5800;
    if (vButton > 0x05807)
        index += 8;

    if (pListofControllerKeys[vButton - index] == UP)
        return true;
    return false;
}



// <summary>
// Updates the state of the controller's buttons.
// This function checks for keystrokes from the controller and updates the state of the buttons.
// It also handles the transition between different button states (DOWN, UP, PUSH).
// </summary>
// <param name="isActive"> A boolean flag indicating whether the controller input is active. </param>
void ControllerInput::UpdateControllerInput(bool isActive)
{

    XINPUT_KEYSTROKE key;

    int j = 0;

    if (XInputGetKeystroke(m_ID, 0, &key) == ERROR_SUCCESS)
    {
        isActive = true;

        for (int i = 0; i < 16; i++)
        {
            j = 0x5800;

            if (i > 7)
                j = 0x5800 + 8;

            if (key.VirtualKey == j + i && key.Flags == 1)
            {
                switch (pListofControllerKeys[key.VirtualKey - j])
                {
                case NONE:
                    pListofControllerKeys[key.VirtualKey - j] = DOWN;
                    break;
                case UP:
                    pListofControllerKeys[key.VirtualKey - j] = DOWN;
                    break;
                case DOWN:
                    pListofControllerKeys[key.VirtualKey - j] = PUSH;
                    break;
                }
            }

            else if (key.VirtualKey == j + i)
            {
                switch (pListofControllerKeys[key.VirtualKey - j])
                {
                case PUSH:
                    pListofControllerKeys[key.VirtualKey - j] = UP;
                    break;
                case UP:
                    pListofControllerKeys[key.VirtualKey - j] = NONE;
                    break;
                case DOWN:
                    pListofControllerKeys[key.VirtualKey - j] = UP;
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
// The normalized values are stored in the pControllersLeftAxis and pControllersRightAxis arrays.
// </summary>
void ControllerInput::UpdateJoySticksinput()
{
    XINPUT_STATE state;

    int side[2] = { 1,1 };

    if (XInputGetState(m_ID, &state) == ERROR_SUCCESS)
    {

        float LX = state.Gamepad.sThumbLX;
        float LY = state.Gamepad.sThumbLY;


        if (LX <= -32767) LX = -32767;
        if (LY <= -32767) LY = -32767;

        float rLX = LX / 32767;  float rLY = LY / 32767;

        if (rLX < 0) side[0] = -1; if (rLY < 0) side[1] = -1;

        rLX = rLX * rLX;
        rLY = rLY * rLY;


        if (LX * side[0] + LY * side[1] < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        {
            rLX = 0.0f, rLY = 0.0f;
        }


        pControllersLeftAxis[0] = rLX; pControllersLeftAxis[1] = rLY;


        side[0] = 1; side[1] = 1;

        float RX = state.Gamepad.sThumbRX;
        float RY = state.Gamepad.sThumbRY;


        if (RX <= -32767) RX = -32767;
        if (RY <= -32767) RY = -32767;

        float rRX = RX / 32767;  float rRY = RY / 32767;

        if (rRX < 0) side[0] = -1; if (rRY < 0) side[1] = -1;

        rRX = rRX * rRX;
        rRY = rRY * rRY;


        if (RX * side[0] + RY * side[1] < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
        {
            rRX = 0.0, rRY = 0.0;
        }


        pControllersRightAxis[0] = rRX; pControllersRightAxis[1] = rRY;
    }
}