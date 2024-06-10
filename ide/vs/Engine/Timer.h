#pragma once

#include <iostream>
#include <chrono>

class Timer
{
public:
	Timer();
	~Timer();

	void Init();

	void Reset();
	void Start();
	void Stop();
	void Tick();

	void SetTimeScale(float fTimeScale);
	void SetMaxDeltaTime(float maxDeltaTime);

	float DeltaTime() const;
	float FixedDeltaTime() const;
	float TotalTime() const;

	float GetFPS() const;

private:
	std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
	std::chrono::time_point<std::chrono::steady_clock> m_InitTime;

	float m_MaxDeltaTime;

	float m_deltaTime;
	float m_previousTime;
	float m_totalTime;

	float m_timeScale;
	float m_timeScaleBackup;

	int m_FPS;
};