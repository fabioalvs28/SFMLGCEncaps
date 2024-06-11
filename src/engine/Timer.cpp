#include "pch.h"

#include "Timer.h"

#define FIXED_UPDATE 0.2

Timer::Timer()
{
    Init();
}

Timer::~Timer() {
}

void Timer::Init()
{
    m_previousTime = 0;
    m_deltaTime = -1.0;
    m_totalTime = 0;
    m_timeScale = 1;
    m_timeScaleBackup = m_timeScale;
    m_MaxDeltaTime = 0;

    m_StartTime = std::chrono::steady_clock::now();
    m_InitTime = std::chrono::steady_clock::now();
}

void Timer::Reset()
{
    Init();
}

void Timer::Start()
{
    m_timeScale = m_timeScaleBackup;
}

void Timer::Stop()
{
    if (m_timeScale == 0)
        return;

    m_timeScaleBackup = m_timeScale;
    m_timeScale = 0;
}

void Timer::Tick()
{
    std::chrono::duration<float> totalTimeDuration = std::chrono::steady_clock::now() - m_InitTime;
    
    m_totalTime = totalTimeDuration.count();

    m_previousTime = m_deltaTime;

    std::chrono::duration<float> duration = std::chrono::steady_clock::now() - m_StartTime;
    
    m_deltaTime = duration.count() * m_timeScale;
    if (m_MaxDeltaTime > 0 && m_deltaTime > m_MaxDeltaTime)
        m_deltaTime = m_MaxDeltaTime;
    
    m_FPS = m_deltaTime != 0  ? 1 / m_deltaTime : -1;

    m_StartTime = std::chrono::steady_clock::now();
}

void Timer::SetMaxDeltaTime(float maxDeltaTime)
{
    m_MaxDeltaTime = maxDeltaTime;
}

void Timer::SetTimeScale(float timeScale) 
{
    m_timeScale = timeScale;
}

float Timer::DeltaTime() const
{
    return m_deltaTime;
}

float Timer::FixedDeltaTime() const
{
    return FIXED_UPDATE;
}

float Timer::TotalTime() const
{
    return m_totalTime;
}

float Timer::GetFPS() const
{   
    return m_FPS;
}