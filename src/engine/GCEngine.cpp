#include"pch.h"
#include "GCEngine.h"

int EVENT::CreateEventHandler( EventHandler* pHandler )
{ return GC::GetActiveEventManager()->AddHandler( pHandler ); }

void EVENT::CallEventHandler( int handlerID )
{ GC::GetActiveEventManager()->CallHandler( handlerID ); }

void EVENT::PushEvent( GCEvent* pEvent )
{ GC::GetActiveEventManager()->QueueEvent( pEvent ); }
