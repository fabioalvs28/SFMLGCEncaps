#include "pch.h"
#include "InputSystem.h"

InputSystem::InputSystem()
	:m_directions(nullptr),
	m_actions(nullptr)
{
}

InputSystem::~InputSystem()
{
	delete m_directions;
	delete m_actions;
}

void InputSystem::Initialize()
{
	m_directions = new Directions();
	m_actions = new Actions();

	m_directions->Up.inputs.push_back(GCKEYBOARD::Z);
	m_directions->Up.inputs.push_back(GCKEYBOARD::UP_ARROW);

	m_directions->Down.inputs.push_back(GCKEYBOARD::S);
	m_directions->Down.inputs.push_back(GCKEYBOARD::DOWN_ARROW);

	m_directions->Left.inputs.push_back(GCKEYBOARD::Q);
	m_directions->Left.inputs.push_back(GCKEYBOARD::LEFT_ARROW);

	m_directions->Right.inputs.push_back(GCKEYBOARD::D);
	m_directions->Right.inputs.push_back(GCKEYBOARD::RIGHT_ARROW);

	m_actions->Shoot.inputs.push_back(GCMOUSE::LEFT);
}