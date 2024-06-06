#include "InputManager.h"



GCInputManager::GCInputManager()
{
    m_canLeaveWin = true;
    InitKeysTable();
};


void GCInputManager::InitKeysTable()
{
    for (int i = 0; i < 255; i++)
    {
        pListOfKeys[i] = NONE;

    }
}


void GCInputManager::UpdateKeyInput()
{

    bool keyState;

    for (int i = 0; i < 255; i++)
    {
        keyState = GetAsyncKeyState(i);

        if (keyState == true)
        {

            switch (pListOfKeys[i])
            {
            case NONE:
                pListOfKeys[i] = DOWN;
                break;
            case PUSH:
                pListOfKeys[i] = PUSH;
                break;
            case UP:
                pListOfKeys[i] = DOWN;
                break;
            case DOWN:
                pListOfKeys[i] = PUSH;
                break;

            }
        }
        else
        {

            switch (pListOfKeys[i])
            {
            case PUSH:
                pListOfKeys[i] = UP;
                break;
            case UP:
                pListOfKeys[i] = NONE;
                break;
            case DOWN:
                pListOfKeys[i] = UP;
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
    if (pListOfKeys[vKey] == DOWN)
    {
        return true;
    }
    return false;
}
bool GCInputManager::GetKeyStay(int vKey)
{
    if (pListOfKeys[vKey] == PUSH)
    {
        return true;
    }
    return false;
}
bool GCInputManager::GetKeyUp(int vKey)
{
    if (pListOfKeys[vKey] == UP)
    {
        return true;
    }
    return false;
}




bool GCInputManager::GetMouseDown(int mouseButton)
{
    if (mouseButton > 2)
        mouseButton += 1;
    if (pListOfKeys[mouseButton] == DOWN)
    {
        return true;
    }
    return false;
}

bool GCInputManager::GetMouseStay(int mouseButton)
{
    if (mouseButton > 2)
        mouseButton += 1;
    if (pListOfKeys[mouseButton] == PUSH)
    {
        return true;
    }
    return false;
}
bool GCInputManager::getMouseUp(int mouseButton)
{
    if (mouseButton > 2)
        mouseButton += 1;
    if (pListOfKeys[mouseButton] == UP)
    {
        return true;
    }
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