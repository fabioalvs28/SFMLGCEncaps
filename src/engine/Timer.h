#pragma once

#include <Windows.h>
#include <profileapi.h>
#include <timeapi.h>

class GCTime
{
public:
	GCTime();
	~GCTime();

	void Reset();
	void Start();
	void Stop();
	bool Update();

	void SetTimeScale(float timeScale);
	void SetMaxDeltaTime(float maxDeltaTime);

	float DeltaTime() const;
	float FixedDeltaTime() const;
	float TotalTime() const;

	float GetFPS() const;

private:
	// Constants
	const float m_fixedDeltaTime = 0.2f;
	const float m_defMaxDeltaTime = 0.4f;
	const float m_defMinDeltaTime = 0.1f;

	// System
	DWORD m_initTime;
	bool m_isPerformanceTimer;
	float m_frequency;
	I64 m_performTime;
	float m_previousTime;
	float m_sysDeltaTime;

	// Properties
	float m_maxDeltaTime;
	float m_timeScale;
	bool m_running;

	// Time
	float m_totalTime;
	float m_deltaTime;

	// FPS
	float m_fpsElapsed;
	int m_fpsCount;
	int m_fps;
};