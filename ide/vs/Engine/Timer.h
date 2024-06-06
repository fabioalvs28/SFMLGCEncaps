#pragma once
#include "pch.h"
#include <iostream>
#include <chrono>

class Timer
{
public:
	Timer();
	~Timer();

	void Init();

	void Tick();
	void Reset();
	void SetTimeScale(float fTimeScale);

	float TotalTime() const;
	float DeltaTime() const;

private:
	float m_startTime;
	float m_previousTime;
	float m_currentTime;
	float m_secondsPerCount;
	float m_deltaTime;
	float m_totalTime;

	float m_timeScale;
};