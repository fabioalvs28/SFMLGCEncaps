#pragma once 

#include <thread>
#include <mutex>
#include <functional>
#include <Windows.h>


#pragma region STLThread
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
#pragma endregion

#pragma region WinThead
class WinThread //! GCWinThread
{
public:
	WinThread() : m_threadHandle(nullptr), m_threadID(0) {}
	~WinThread();

	bool Start(std::function<void()> task);
	void Join(); 
	DWORD GetThreadId() const { return m_threadID; }

private:

	static DWORD WINAPI ThreadFunc(LPVOID lpParam)
	{
		WinThread* thread = static_cast<WinThread*>(lpParam);
		thread->m_task();
		thread->m_isRunning = false;
		return 0;
	}

	HANDLE m_threadHandle; 
	DWORD m_threadID;
	std::function<void()> m_task;
	bool m_isRunning = false;
};
#pragma endregion
