#pragma once
#include <Windows.h>
#include <minwinbase.h>
#include <synchapi.h>

class GCCriticalSection
{
public:
	GCCriticalSection();
	GCCriticalSection(GCCriticalSection* pCriticalSection);
	~GCCriticalSection();

	void Enter();
	void Exit();

protected:
	void Create();
	void Destroy();

#if defined(__APPLE__)
	pthread_mutex_t m_cs;
#else
	CRITICAL_SECTION m_cs;
#endif

	GCCriticalSection* m_pParent;
	bool m_initialized;
};