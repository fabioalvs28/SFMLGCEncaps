#pragma once

class GCState
{
public:

	virtual ~GCState() = default;

	virtual void Enter( GCState* previousState ) = 0;
	virtual void Update( float deltaTime ) = 0;	
	virtual void Exit( GCState* nextState ) = 0;

protected:
	GCState() = default;
};