#include "pch.h"
#include "Thread.h"

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