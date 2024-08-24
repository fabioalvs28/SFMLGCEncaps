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
    std::cout << GCINPUTS::GetMousePos().x << "  " << GCINPUTS::GetMousePos().y << std::endl;
}