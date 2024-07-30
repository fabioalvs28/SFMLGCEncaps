#include "pch.h"
#include "ScriptStart.h"

void ScriptStart::Start()
{
    m_pGameObject->m_transform.SetScale( GCVEC3( 5 , 5 , 0 ) );
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

ScriptStart* ScriptStart::Duplicate()
{
    ScriptStart* pNewScript = new ScriptStart();
    Copy( pNewScript );
    pNewScript->pKeyboard = pKeyboard;
    pNewScript->pMouse = pMouse;
    return pNewScript;
}