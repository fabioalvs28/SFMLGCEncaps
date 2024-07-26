#include "pch.h"

GCTime::GCTime()
{
	Reset();
}

GCTime::~GCTime()
{
}

void GCTime::Reset()
{
	// System
	m_initTime = timeGetTime();
	m_previousTime = m_initTime;
	m_isPerformanceTimer = false;
	m_frequency = 0.0f;
	LARGE_INTEGER frequency;
	memset(&frequency, 0, sizeof(LARGE_INTEGER));
	if (QueryPerformanceFrequency(&frequency) && frequency.QuadPart)
	{
		m_isPerformanceTimer = true;
		m_frequency = (float)frequency.QuadPart;
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		m_performTime = counter.QuadPart;
		m_previousTime = (float)(counter.QuadPart - m_performTime) / m_frequency;
	}
	m_sysDeltaTime = 0.0f;

	// Properties
	m_maxDeltaTime = m_defMaxDeltaTime;
	m_timeScale = 1.0f;
	m_running = true;

	// Time
	m_totalTime = 0.0f;
	m_deltaTime = 0.0f;

	// FPS
	m_fpsElapsed = 0.0f;
	m_fpsCount = 0;
	m_fps = 0;
}

void GCTime::Start()
{
	m_running = true;
}

void GCTime::Stop()
{
	m_running = false;
}

bool GCTime::Update()
{
	// Performance or Classic
	float cur = 0.0f;
	if (m_isPerformanceTimer)
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		cur = (float)(counter.QuadPart - m_performTime) / m_frequency;
	}
	else
		cur = (timeGetTime() - m_initTime) / 1000.0f;

	// System delta time
	float dt = cur - m_previousTime;
	m_previousTime = cur;

	// Pause
	if (m_running == false)
	{
		m_sysDeltaTime = 0.0f;
		m_deltaTime = 0.0f;
		return true;
	}

	// System delta time
	m_sysDeltaTime += dt;
	if (m_sysDeltaTime < m_defMinDeltaTime)
	{
		m_deltaTime = 0.0f;
		return false;
	}
	if (m_sysDeltaTime > m_maxDeltaTime)
		m_sysDeltaTime = m_maxDeltaTime;

	// FPS
	m_fpsCount++;
	m_fpsElapsed += m_sysDeltaTime;
	if (m_fpsElapsed >= 1.0f)
	{
		m_fps = m_fpsCount;
		m_fpsCount = 0;
		m_fpsElapsed = m_fpsElapsed - 1.0f;
	}

	// Delta time
	m_deltaTime = m_sysDeltaTime;
	m_deltaTime *= m_timeScale;
	m_sysDeltaTime = 0.0f;

	// Total time
	m_totalTime += m_deltaTime;

	// Ok
	return true;
}

void GCTime::SetMaxDeltaTime(float maxDeltaTime)
{
	m_maxDeltaTime = maxDeltaTime;
}

void GCTime::SetTimeScale(float timeScale)
{
	m_timeScale = timeScale;
}

float GCTime::DeltaTime() const
{
	return m_deltaTime;
}

float GCTime::FixedDeltaTime() const
{
	return m_fixedDeltaTime;
}

float GCTime::TotalTime() const
{
	return m_totalTime;
}

float GCTime::GetFPS() const
{
	return m_fps;
}