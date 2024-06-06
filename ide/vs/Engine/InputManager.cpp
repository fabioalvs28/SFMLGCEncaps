#include "pch.h"
#include "InputManager.h"


InputManager::InputManager() {
    canLeaveWin = true;
    InitKeysTable();
};


void InputManager::InitKeysTable()
{
    for (int i = 0; i < 255; i++) {
        InputManager::listOfKeys[i] = NONE;

    }
}


void InputManager::UpdateKeyInput() {

    bool keyState;

    for (int i = 0; i < 255; i++) {
        keyState = GetAsyncKeyState(i);

        if (keyState == true) {

            switch (InputManager::listOfKeys[i]) {
            case InputManager::NONE:
                InputManager::listOfKeys[i] = DOWN;
                break;
            case InputManager::PUSH:
                InputManager::listOfKeys[i] = PUSH;
                break;
            case InputManager::UP:
                InputManager::listOfKeys[i] = DOWN;
                break;
            case InputManager::DOWN:
                InputManager::listOfKeys[i] = PUSH;
                break;

            }
        }
        else {

            switch (InputManager::listOfKeys[i]) {
            case InputManager::PUSH:
                InputManager::listOfKeys[i] = UP;
                break;
            case InputManager::UP:
                InputManager::listOfKeys[i] = NONE;
                break;
            case InputManager::DOWN:
                InputManager::listOfKeys[i] = UP;
                break;

            }
        }
    }

}



void InputManager::UpdateMousePos(const Window* winInfos) {

    POINT pointOnScreen;

    if (GetCursorPos(&pointOnScreen)) {

        if (pointOnScreen.x <= winInfos->winPos.x) {
            pointOnScreen.x = winInfos->winPos.x;
        }
        else if (pointOnScreen.x >= winInfos->winPos.x + winInfos->winSize.x) {
            pointOnScreen.x = winInfos->winPos.x + winInfos->winSize.x;
        }

        if (pointOnScreen.y <= winInfos->winPos.y) {
            pointOnScreen.y = winInfos->winPos.y;
        }
        else if (pointOnScreen.y >= winInfos->winPos.y + winInfos->winSize.y) {
            pointOnScreen.y = winInfos->winPos.y + winInfos->winSize.y;
        }

        if (canLeaveWin == false) {
            SetCursorPos(pointOnScreen.x, pointOnScreen.y);
        }


        mousePos.x = pointOnScreen.x - winInfos->center.x;
        mousePos.y = pointOnScreen.y - winInfos->center.y;

    }


}


Vector2 InputManager::GetMousePos() {
    return mousePos;
}


bool InputManager::GetKeyDown(char key) {
    if (listOfKeys[key] == DOWN) {
        return true;
    }
    return false;
}
bool InputManager::GetKeyStay(char key)
{
    if (listOfKeys[key] == PUSH) {
        return true;
    }
    return false;
}
bool InputManager::GetKeyUp(char key)
{
    if (listOfKeys[key] == UP) {
        return true;
    }
    return false;
}




bool InputManager::GetMouseDown(int mouseButton) {
    if (mouseButton > 2)
        mouseButton += 1;
    if (listOfKeys[mouseButton] == DOWN) {
        return true;
    }
    return false;
}

bool InputManager::GetMouseStay(int mouseButton) {
    if (mouseButton > 2)
        mouseButton += 1;
    if (listOfKeys[mouseButton] == PUSH) {
        return true;
    }
    return false;
}
bool InputManager::getMouseUp(int mouseButton) {
    if (mouseButton > 2)
        mouseButton += 1;
    if (listOfKeys[mouseButton] == UP) {
        return true;
    }
    return false;
}





bool InputManager::OnMouseHover(Vector2 objectPos, Vector2 objSize) {

    if (mousePos.x < objectPos.x)
        return false;
    if (mousePos.x > objectPos.x + objSize.x)
        return false;
    if (mousePos.y < objectPos.y)
        return false;
    if (mousePos.y > objectPos.y + objSize.y)
        return false;

    return true;
}



void InputManager::SetLeavingWindows(bool canLeave) {
    canLeaveWin = canLeave;
}