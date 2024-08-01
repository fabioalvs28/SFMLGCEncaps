#include "pch.h"
#include "ScriptStart.h"

void ScriptStart::Start()
{
    pKeyboard = GC::GetActiveInputSystem()->m_pKeyboard;
    pMouse = GC::GetActiveInputSystem()->m_pMouse;
}

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

void ScriptStart::CopyTo( Component* pDestination )
{
    Component::CopyTo( pDestination );
    ScriptStart* pScript = static_cast<ScriptStart*>( pDestination );
    pScript->pKeyboard = pKeyboard;
    pScript->pMouse = pMouse;
}