#pragma once
#include <windows.h>


typedef struct Vector2
{
    int x;
    int y;
} Vector2;



struct Window
{
    Vector2 winPos;
    Vector2 winSize;
    Vector2 center;

};

class InputManager
{

public:

    InputManager();
    virtual ~InputManager() {};

    void UpdateKeyInput();
    void UpdateMousePos(const Window* winInfos);

    Vector2 GetMousePos();


    /// \brief Pour l'instant marche avec les lettres donc mettre 'A' par exemple. 
    bool GetKeyDown(char key);
    /// \brief Pour l'instant marche avec les lettres donc mettre 'A' par exemple. 
    bool GetKeyStay(char key);
    /// \brief Pour l'instant marche avec les lettres donc mettre 'A' par exemple. 
    bool GetKeyUp(char key);


    /// \brief int from 1 to 5 : left button, right button, middle button, X button 1, Xbutton 2
    bool GetMouseDown(int mouseButton);
    /// \brief int from 1 to 5 : left button, right button, middle button, X button 1, Xbutton 2
    bool GetMouseStay(int mouseButton);
    /// \brief int from 1 to 5 : left button, right button, middle button, X button 1, Xbutton 2
    bool getMouseUp(int mouseButton);

    bool OnMouseHover(Vector2 objectPos, Vector2 objSize);

    void SetLeavingWindows(bool canLeave); // Choose if the cursor can leave the windows or not. 

private:
    void InitKeysTable();
    bool canLeaveWin;
    BYTE listOfKeys[255];
    enum
    {
        NONE,
        PUSH,
        UP,
        DOWN,
    };
    Vector2 mousePos;

};
