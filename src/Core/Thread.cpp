#include "pch.h"
#include "Thread.h"
#include "Log.h"

#pragma region STLTHREAD

GCThread::GCThread() : m_isRunning(false) {}

GCThread::~GCThread()
{
	if (m_thread.joinable())
	{
		m_thread.join();
	}
}

GCThread::operator std::thread& ()
{
	return m_thread;
}


void GCThread::Start(std::function<void()> task)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_isRunning = true; 
	m_thread = std::thread([this, task]()
		{
			task();
			std::lock_guard<std::mutex> lock(m_mutex);
			m_isRunning = false;
		});
}

void GCThread::Join()
{
	if (m_thread.joinable())
	{
		m_thread.join();
	}
}

void GCThread::Detach()
{
	if (m_thread.joinable())
	{
		m_thread.detach();
	}
}

bool GCThread::IsRunning() const
{
	return m_isRunning;
}

std::thread::id GCThread::GetId() const
{
	return m_thread.get_id();
}

#pragma endregion

#pragma region WinThread

WinThread::~WinThread()
{
	Join();
}

bool WinThread::Start(std::function<void()> task)
{
	m_task = task; 

	if (m_isRunning)
		return false; 

	m_isRunning = true;

	m_threadHandle = CreateThread(
		nullptr,
		0,
		ThreadFunc,
		this,
		0,
		&m_threadID
	);

	if (m_threadHandle == nullptr)
	{
		LogCoreError("Failed to create thread", GetLastError());
		m_isRunning = false;
		return false;
	}
	return true;
}

void WinThread::Join()
{
	if (m_threadHandle != nullptr)
	{
		WaitForSingleObject(m_threadHandle, INFINITE);
		CloseHandle(m_threadHandle);
		m_threadHandle = nullptr;
	}
}
#pragma endregion
