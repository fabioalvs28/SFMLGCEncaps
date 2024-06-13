#include "pch.h"
#include "InputManager.h"
#include <xinput.h>
#include <math.h>
#pragma comment(lib,"xinput.lib")

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689

GCInputManager::GCInputManager()
{
    for (int i = 0; i < XUSER_MAX_COUNT; i++)
    {
        m_controllerList.PushBack(nullptr);
    }
}

KeyboardInput::KeyboardInput()
{
    for (int i = 0; i < 255; i++)
    {
        pListOfKeyboardKeys[i] = NONE;

    }
}


ControllerInput::ControllerInput()
{

    pControllersLeftAxis[0] = 0.0; pControllersLeftAxis[1] = 0.0;
    pControllersRightAxis[0] = 0.0; pControllersRightAxis[1] = 0.0;

    for (int j = 0; j < 16; j++)
    {
        pListofControllerKeys[j] = NONE;
    }
}




void GCInputManager::GetConnectedController()
{

    XINPUT_STATE state;

    for (int i = 0; i < XUSER_MAX_COUNT; i++)
    {

        if (XInputGetState(i, &state) == ERROR_SUCCESS && m_controllerList.Get(i) == nullptr)
        {
            ControllerInput* pController = new ControllerInput();
            m_controllerList.Insert(i, pController);
        }
    }
}


void KeyboardInput::UpdateKeyboardInput()
{

    bool keyState;

    for (int i = 0; i < 255; i++)
    {
        keyState = GetAsyncKeyState(i);

        if (keyState == true)
        {

            switch (pListOfKeyboardKeys[i])
            {
            case NONE:
                pListOfKeyboardKeys[i] = DOWN;
                break;
            case PUSH:
                pListOfKeyboardKeys[i] = PUSH;
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



void MouseInput::UpdateMouseInput(const GCWINDOW* pWinInfos)
{

    POINT pointOnScreen;

    if (GetCursorPos(&pointOnScreen))
    {

        if (pointOnScreen.x <= pWinInfos->winPos.x)
        {
            pointOnScreen.x = pWinInfos->winPos.x;
        }
        else if (pointOnScreen.x >= pWinInfos->winPos.x + pWinInfos->winSize.x)
        {
            pointOnScreen.x = pWinInfos->winPos.x + pWinInfos->winSize.x;
        }

        if (pointOnScreen.y <= pWinInfos->winPos.y)
        {
            pointOnScreen.y = pWinInfos->winPos.y;
        }
        else if (pointOnScreen.y >= pWinInfos->winPos.y + pWinInfos->winSize.y)
        {
            pointOnScreen.y = pWinInfos->winPos.y + pWinInfos->winSize.y;
        }

        if (m_canLeaveWin == false)
        {
            SetCursorPos(pointOnScreen.x, pointOnScreen.y);
        }


        m_mousePos.x = pointOnScreen.x - pWinInfos->center.x;
        m_mousePos.y = pointOnScreen.y - pWinInfos->center.y;

    }

    for (int i = 0; i < 5; i++)
    {
        if ( GetAsyncKeyState(i) )
        {
            switch (pMouseButtons[i])
            {
                case NONE:
                    pMouseButtons[i] = DOWN;
                    break;
                case UP:
                    pMouseButtons[i] = DOWN;
                    break;
                case DOWN:
                    pMouseButtons[i] = PUSH;
                    break;

            }
        }
        else
        {

            switch (pMouseButtons[i])
            {
                case PUSH:
                    pMouseButtons[i] = UP;
                    break;
                case UP:
                    pMouseButtons[i] = NONE;
                    break;
                case DOWN:
                    pMouseButtons[i] = UP;
                    break;
            }
        }
    }


}


Vector2 MouseInput::GetMousePos()
{
    return m_mousePos;
}


bool KeyboardInput::GetKeyDown(int vKey)
{
    if (pListOfKeyboardKeys[vKey] == DOWN)
    {
        return true;
    }
    return false;
}

bool KeyboardInput::GetKeyStay(int vKey)
{
    if (pListOfKeyboardKeys[vKey] == PUSH)
    {
        return true;
    }
    return false;
}

bool KeyboardInput::GetKeyUp(int vKey)
{
    if (pListOfKeyboardKeys[vKey] == UP)
    {
        return true;
    }
    return false;
}


bool MouseInput::GetMouseDown(int mouseButton)
{
    if (mouseButton > 2)
        mouseButton += 1;
    if (pMouseButtons[mouseButton] == DOWN)
    {
        return true;
    }
    return false;
}
bool MouseInput::GetMouseStay(int mouseButton)
{
    if (mouseButton > 2)
        mouseButton += 1;
    if (pMouseButtons[mouseButton] == PUSH)
    {
        return true;
    }
    return false;
}
bool MouseInput::GetMouseUp(int mouseButton)
{
    if (mouseButton > 2)
        mouseButton += 1;
    if (pMouseButtons[mouseButton] == UP)
    {
        return true;
    }
    return false;
}


bool ControllerInput::GetControllerButtonDown(int controllerID, int vButton)
{
    int index = 0x5800;
    if (vButton > 0x05807)
        index += 8;

    if (pListofControllerKeys[vButton - index] == DOWN)
        return true;
    return false;
}


bool ControllerInput::GetControllerButtonStay(int controllerID, int vButton)
{
    int index = 0x5800;
    if (vButton > 0x05807)
        index += 8;

    if (pListofControllerKeys[vButton - index] == PUSH)
        return true;
    return false;
}
bool ControllerInput::GetControllerButtonUp(int controllerID, int vButton)
{
    int index = 0x5800;
    if (vButton > 0x05807)
        index += 8;

    if (pListofControllerKeys[vButton - index] == UP)
        return true;
    return false;
}

bool MouseInput::OnMouseHover(Vector2 objectPos, Vector2 objSize) {

    if (m_mousePos.x < objectPos.x)
        return false;
    if (m_mousePos.x > objectPos.x + objSize.x)
        return false;
    if (m_mousePos.y < objectPos.y)
        return false;
    if (m_mousePos.y > objectPos.y + objSize.y)
        return false;

    return true;
}

void MouseInput::SetLeavingWindows(bool canLeave) {
    m_canLeaveWin = canLeave;
}



void GCInputManager::UpdateInputs()
{

    for (int i = 0; i < XUSER_MAX_COUNT; i++)
    {

        if (m_controllerList[i] != nullptr)
        {
            m_controllerList[i]->UpdateControllerInput(i);
            m_controllerList[i]->UpdateJoySticksinput(i);
        }
    }

    m_keyboard.UpdateKeyboardInput();
    m_mouse.UpdateMouseInput(m_pWindow);
}

void ControllerInput::UpdateControllerInput(int controllerID)
{

    XINPUT_KEYSTROKE key;

    int j = 0;
    if (XInputGetKeystroke(controllerID, 0, &key) == ERROR_SUCCESS)
    {
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

float ControllerInput::GetControllerLeftAxisX(int controllerID)
{
    return pControllersLeftAxis[0];
}

float ControllerInput::GetControllerLeftAxisY(int controllerID)
{
    return pControllersLeftAxis[1];
}


float ControllerInput::GetControllerRightAxisX(int controllerID)
{
    return pControllersRightAxis[0];
}

float ControllerInput::GetControllerRightAxisY(int controllerID)
{
    return pControllersRightAxis[1];
}


void ControllerInput::UpdateJoySticksinput(int controllerID)
{
    XINPUT_STATE state;

    int side[2] = { 1,1 };

    if (XInputGetState(controllerID, &state) == ERROR_SUCCESS)
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