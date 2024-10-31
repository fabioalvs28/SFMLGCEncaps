#pragma once 
#include "pch.h"

struct InputGroup 
{
	std::vector<int> inputs;
};

struct Directions 
{
	InputGroup Up;
	InputGroup Down;
	InputGroup Left;
	InputGroup Right;
};

struct Actions 
{
	InputGroup Shoot;
};


class InputSystem 
{
public:
	InputSystem();
	~InputSystem();
	void Initialize();

	Directions* GetDirections() { return m_directions; };
	Actions* GetActions() { return m_actions; };

	void SetDirections(Directions* directions) { m_directions = directions; };
	void SetActions(Actions* actions) { m_actions = actions; };

private:
	Directions* m_directions;
	Actions* m_actions;

};