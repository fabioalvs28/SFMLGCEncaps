#pragma once 

#include <thread>
#include <mutex>
#include <functional>

class GCThread
{
public:
	GCThread();
	~GCThread();

	operator std::thread& ();

	void Start(std::function<void()> task);
	void Join();
	void Detach();
	bool IsRunning() const;
	std::thread::id GetId() const;

private:
	std::thread m_thread;
	std::mutex m_mutex; 
	bool m_isRunning = false;
};