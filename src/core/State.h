#pragma once 
class GCStateMachine;

class GCState
{
public:
	
	GCState() : m_pManager(nullptr) {};
	virtual ~GCState() = default;

	virtual void Enter(GCStateMachine* manager, void* args = nullptr) = 0;
	virtual void Exit() = 0;
	virtual void Update(float deltaTime) = 0;

protected:
	GCStateMachine* m_pManager;
	void* m_pArgs;
};