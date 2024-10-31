#include "pch.h"
#include "Mouse.h"

void GCScriptMouse::CopyTo(GCComponent* pDestination)
{
	GCComponent::CopyTo(pDestination);
	GCScriptMouse* pNewComponent = static_cast<GCScriptMouse*>(pDestination);
}

void GCScriptMouse::Update()
{
	GCVEC2 mousePos = GCINPUTS::GetMousePos();
	m_pGameObject->m_transform.SetPosition(GCVEC3(mousePos.x, mousePos.y, 0));
}