#include "InputManager.h"
#include <xinput.h>
#include <math.h>
#pragma comment(lib,"xinput.lib")

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689

GCInputManager::GCInputManager()
{
    m_canLeaveWin = true;

    for (int i = 0; i < 255; i++)
    {
        pListOfKeyboardKeys[i] = NONE;

    }

    for (int i = 0; i < 4; i++)
    {
        pControllersLeftAxis[i][0] = 0.0; pControllersLeftAxis[i][1] = 0.0;
        pControllersRightAxis[i][0] = 0.0; pControllersRightAxis[i][1] = 0.0;

        for (int j = 0; j < 16; j++)
        {
            pListofControllerKeys[i][j] = NONE;
        }
    }

};


void GCInputManager::GetConnectedController()
{

    XINPUT_STATE state;

    for (int i = 0; i < XUSER_MAX_COUNT; i++)
    {

        if (XInputGetState(i, &state) == ERROR_SUCCESS)
        {
            pConnectedController[i] = true;
        }
        else
        {
            pConnectedController[i] = false;
        }
    }
}

void GCInputManager::UpdateKeyboardInput()
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



void GCInputManager::UpdateMousePos(const GCWINDOW* pWinInfos)
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


}


Vector2 GCInputManager::GetMousePos()
{
    return m_mousePos;
}


bool GCInputManager::GetKeyDown(int vKey)
{
    if (pListOfKeyboardKeys[vKey] == DOWN)
    {
        return true;
    }
    return false;
}
bool GCInputManager::GetKeyStay(int vKey)
{
    if (pListOfKeyboardKeys[vKey] == PUSH)
    {
        return true;
    }
    return false;
}
bool GCInputManager::GetKeyUp(int vKey)
{
    if (pListOfKeyboardKeys[vKey] == UP)
    {
        return true;
    }
    return false;
}


bool GCInputManager::GetMouseDown(int mouseButton)
{
    if (mouseButton > 2)
        mouseButton += 1;
    if (pListOfKeyboardKeys[mouseButton] == DOWN)
    {
        return true;
    }
    return false;
}
bool GCInputManager::GetMouseStay(int mouseButton)
{
    if (mouseButton > 2)
        mouseButton += 1;
    if (pListOfKeyboardKeys[mouseButton] == PUSH)
    {
        return true;
    }
    return false;
}
bool GCInputManager::GetMouseUp(int mouseButton)
{
    if (mouseButton > 2)
        mouseButton += 1;
    if (pListOfKeyboardKeys[mouseButton] == UP)
    {
        return true;
    }
    return false;
}


bool GCInputManager::GetControllerButtonDown(int controllerID, int vButton)
{
    int index = 0x5800;
    if (vButton > 0x05807)
        index += 8;

    if (pListofControllerKeys[controllerID][vButton - index] == DOWN)
        return true;
    return false;
}
bool GCInputManager::GetControllerButtonStay(int controllerID, int vButton)
{
    int index = 0x5800;
    if (vButton > 0x05807)
        index += 8;

    if (pListofControllerKeys[controllerID][vButton - index] == PUSH)
        return true;
    return false;
}
bool GCInputManager::GetControllerButtonUp(int controllerID, int vButton)
{
    int index = 0x5800;
    if (vButton > 0x05807)
        index += 8;

    if (pListofControllerKeys[controllerID][vButton - index] == UP)
        return true;
    return false;
}

bool GCInputManager::OnMouseHover(Vector2 objectPos, Vector2 objSize) {

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

void GCInputManager::SetLeavingWindows(bool canLeave) {
    m_canLeaveWin = canLeave;
}



void GCInputManager::UpdateInputs()
{
    for (int i = 0; i < XUSER_MAX_COUNT; i++) 
    {

        if (pConnectedController[i] == true)
        {
            UpdateControllerInput(i);
            UpdateJoySticksinput(i);
        }
    }

    UpdateKeyboardInput();
}

void GCInputManager::UpdateControllerInput(int controllerID)
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
                switch (pListofControllerKeys[controllerID][key.VirtualKey - j])
                {
                case NONE:
                    pListofControllerKeys[controllerID][key.VirtualKey - j] = DOWN;
                    break;
                case UP:
                    pListofControllerKeys[controllerID][key.VirtualKey - j] = DOWN;
                    break;
                case DOWN:
                    pListofControllerKeys[controllerID][key.VirtualKey - j] = PUSH;
                    break;
                }
            }

            else if (key.VirtualKey == j + i)
            {
                switch (pListofControllerKeys[controllerID][key.VirtualKey - j])
                {
                case PUSH:
                    pListofControllerKeys[controllerID][key.VirtualKey - j] = UP;
                    break;
                case UP:
                    pListofControllerKeys[controllerID][key.VirtualKey - j] = NONE;
                    break;
                case DOWN:
                    pListofControllerKeys[controllerID][key.VirtualKey - j] = UP;
                    break;

                }
            }
        }
    }
}

float GCInputManager::GetControllerLeftAxisX(int controllerID)
{
    return pControllersLeftAxis[controllerID][0];
}

float GCInputManager::GetControllerLeftAxisY(int controllerID) 
{
    return pControllersLeftAxis[controllerID][1];
}


float GCInputManager::GetControllerRightAxisX(int controllerID) 
{
    return pControllersRightAxis[controllerID][0];
}

float GCInputManager::GetControllerRightAxisY(int controllerID) 
{
    return pControllersRightAxis[controllerID][1];
}


void GCInputManager::UpdateJoySticksinput(int controllerID)
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

        rLX = pow(rLX, 2) * side[0];
        rLY = pow(rLY, 2) * side[1];


        if (sqrt(LX * LX + LY * LY) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) 
        {
            rLX = 0.0, rLY = 0.0;
        }


        pControllersLeftAxis[controllerID][0] = rLX; pControllersLeftAxis[controllerID][1] = rLY;


        side[0] = 1; side[1] = 1;

        float RX = state.Gamepad.sThumbRX;
        float RY = state.Gamepad.sThumbRY;


        if (RX <= -32767) RX = -32767;
        if (RY <= -32767) RY = -32767;

        float rRX = RX / 32767;  float rRY = RY / 32767;

        if (rRX < 0) side[0] = -1; if (rRY < 0) side[1] = -1;

        rRX = pow(rRX, 2) * side[0];
        rRY = pow(rRY, 2) * side[1];


        if (sqrt(RX * RX + RY * RY) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
        {
            rRX = 0.0, rRY = 0.0;
        }


        pControllersRightAxis[controllerID][0] = rRX; pControllersRightAxis[controllerID][1] = rRY;
    }
}