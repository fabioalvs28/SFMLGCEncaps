#include "pch.h"
#include "ScriptStart.h"

void ScriptStart::Start() {}

void ScriptStart::Update()
{
    for (int i = 0; i < 256 ; i++ )
    {
        if (pKeyboard->GetKeyDown(i))
        {
            std::cout << "Key Down: " << i << std::endl;
        }
    }
}

ScriptStart* ScriptStart::Duplicate() { return nullptr; }