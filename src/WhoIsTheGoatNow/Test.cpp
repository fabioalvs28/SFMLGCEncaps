#include "pch.h"
#include "GCEngine.h"
#include "Test.h"

void GCScriptTest::CopyTo(GCComponent* pDestination)
{
    GCComponent::CopyTo(pDestination);
    GCScriptTest* pNewComponent = static_cast<GCScriptTest*>(pDestination);
}

void GCScriptTest::Update()
{
    if (GCINPUTS::GetKeyDown(GCKEYBOARD::A))
        m_pGameObject->GetComponent<GCAnimator>()->PlayAnimation("DarkGoatForward",true);
}

void GCScriptTest::OnClick()
{
    //counter += 25;
    //pText->SetColor( GCColor( 0 , counter , 0 ) );
}