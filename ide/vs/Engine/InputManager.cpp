#include "pch.h"
#include "InputManager.h"



InputManager::InputManager()
{
    m_canLeaveWin = true;
    InitKeysTable();
};


void InputManager::InitKeysTable()
{
    for (int i = 0; i < 255; i++)
    {
        InputManager::pListOfKeys[i] = NONE;

    }
}


void InputManager::UpdateKeyInput()
{

    bool keyState;

    for (int i = 0; i < 255; i++)
    {
        keyState = GetAsyncKeyState(i);

        if (keyState == true)
        {

            switch (InputManager::pListOfKeys[i])
            {
            case InputManager::NONE:
                InputManager::pListOfKeys[i] = DOWN;
                break;
            case InputManager::PUSH:
                InputManager::pListOfKeys[i] = PUSH;
                break;
            case InputManager::UP:
                InputManager::pListOfKeys[i] = DOWN;
                break;
            case InputManager::DOWN:
                InputManager::pListOfKeys[i] = PUSH;
                break;

            }
        }
        else
        {

            switch (InputManager::pListOfKeys[i])
            {
            case InputManager::PUSH:
                InputManager::pListOfKeys[i] = UP;
                break;
            case InputManager::UP:
                InputManager::pListOfKeys[i] = NONE;
                break;
            case InputManager::DOWN:
                InputManager::pListOfKeys[i] = UP;
                break;

            }
        }
    }

}



void InputManager::UpdateMousePos(const GCWINDOW* pWinInfos)
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


Vector2 InputManager::GetMousePos()
{
    return m_mousePos;
}


bool InputManager::GetKeyDown(char key)
{
    if (pListOfKeys[key] == DOWN)
    {
        return true;
    }
    return false;
}
bool InputManager::GetKeyStay(char key)
{
    if (pListOfKeys[key] == PUSH)
    {
        return true;
    }
    return false;
}
bool InputManager::GetKeyUp(char key)
{
    if (pListOfKeys[key] == UP)
    {
        return true;
    }
    return false;
}




bool InputManager::GetMouseDown(int mouseButton)
{
    if (mouseButton > 2)
        mouseButton += 1;
    if (pListOfKeys[mouseButton] == DOWN)
    {
        return true;
    }
    return false;
}

bool InputManager::GetMouseStay(int mouseButton)
{
    if (mouseButton > 2)
        mouseButton += 1;
    if (pListOfKeys[mouseButton] == PUSH)
    {
        return true;
    }
    return false;
}
bool InputManager::getMouseUp(int mouseButton)
{
    if (mouseButton > 2)
        mouseButton += 1;
    if (pListOfKeys[mouseButton] == UP)
    {
        return true;
    }
    return false;
}





bool InputManager::OnMouseHover(Vector2 objectPos, Vector2 objSize) {

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



void InputManager::SetLeavingWindows(bool canLeave) {
    m_canLeaveWin = canLeave;
}