#include "pch.h"
#include "Button.h"


void GCScriptButton::CopyTo( GCComponent* pDestination)
{
	GCComponent::CopyTo(pDestination);
	GCScriptButton* pNewComponent = static_cast<GCScriptButton*>(pDestination);
}


void GCScriptButton::OnClick()
{
	if (m_buttonType == Play)
		m_pPlayScene->SetActive();
	if (m_buttonType == Quit)
		GC::GetWindow()->MsgProc(NULL, WM_DESTROY, NULL, NULL);
		
}