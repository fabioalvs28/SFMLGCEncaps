#pragma once

#include"pch.h"

#include "GCEngine.h"

int EVENT::CreateEventHandler(EventHandler* handler)
{
    return GC::GetActiveEventManager()->AddHandler(handler);
}

void EVENT::CallEventHandler(int handlerID)
{
    GC::GetActiveEventManager()->CallHandler(handlerID);
}

void EVENT::PushEvent(GCEvent* ev)
{
    GC::GetActiveEventManager()->QueueEvent(ev);
}
