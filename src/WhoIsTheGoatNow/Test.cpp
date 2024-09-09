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
    count += 1;
    float fps = GC::GetActiveTimer()->DeltaTime();
    std::cout << fps << std::endl;
    if (count == 100) {
        std::string txtfps = std::to_string(fps);
        pText->SetText(txtfps, GCColor(255, 0, 0));
        count = 0;
    }
}