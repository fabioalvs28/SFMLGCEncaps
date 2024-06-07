#include "Timer.h"

Timer::Timer() :
    m_startTime(0),
    m_previousTime(0),
    m_currentTime(0),
    m_secondsPerCount(0),
    m_deltaTime(0),
    m_totalTime(0),
    m_timeScale(1)
{}

Timer::~Timer() {
}

void Timer::Init()
{
    std::chrono::time_point<std::chrono::high_resolution_clock> spc = std::chrono::high_resolution_clock::now();
    m_secondsPerCount = spc.time_since_epoch().count();

    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    m_startTime = start.time_since_epoch().count();

    m_previousTime = m_startTime;
}

void Timer::Tick()
{
    std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
    m_currentTime = now.time_since_epoch().count();

    m_deltaTime = m_currentTime - m_previousTime;
    m_previousTime = m_currentTime;
    m_totalTime += m_deltaTime;
}

void Timer::Reset()
{
    m_startTime = 0;
    m_previousTime = 0;
    m_currentTime = 0;
    m_secondsPerCount = 0;
    m_deltaTime = 0;
    m_totalTime = 0;
    m_timeScale = 1;

    Init();
}

void Timer::SetTimeScale(float fTimeScale) {
    m_timeScale = fTimeScale;
}

float Timer::TotalTime() const
{
    return static_cast<float>(m_totalTime) / m_secondsPerCount;
}

float Timer::DeltaTime() const
{
    return static_cast<float>(m_deltaTime) / m_secondsPerCount * m_timeScale;
}