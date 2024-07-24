#pragma once
#include "pch.h"
#include "CriticalSection.h"

GCCriticalSection::GCCriticalSection()
{
	m_pParent = nullptr;
	m_initialized = false;

	Create();
}

GCCriticalSection::GCCriticalSection(GCCriticalSection* pCriticalSection)
{
	m_pParent = pCriticalSection;
	m_initialized = false;

	if (pCriticalSection)
		Enter();
}

GCCriticalSection::~GCCriticalSection()
{	
	Exit();
	Destroy();
}

void GCCriticalSection::Create()
{
	if (m_pParent)
		return;

	m_initialized = true;

#if defined(__APPLE__)
	pthread_mutexattr_t Attr;
	pthread_mutexattr_init(&Attr);
	pthread_mutexattr_settype(&Attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&m_cs, &Attr);
#else
	InitializeCriticalSection(&m_cs);
#endif
}

void GCCriticalSection::Destroy()
{
	if (m_pParent)
		return;

	if (m_initialized)
	{
		m_initialized = false;
#if defined(__APPLE__)
		pthread_mutex_destroy(&m_cs);
#else
		DeleteCriticalSection(&m_cs);
#endif
	}
}

void GCCriticalSection::Enter()
{
	if (m_pParent)
	{
		m_pParent->Enter();
		return;
	}

	if (m_initialized)
	{
#if defined(__APPLE__)
		pthread_mutex_lock(&m_cs);
#else
		EnterCriticalSection(&m_cs);
#endif
	}
}

void GCCriticalSection::Exit()
{
	if (m_pParent)
	{
		m_pParent->Exit();
		return;
	}

	if (m_initialized)
	{
#if defined(__APPLE__)
		pthread_mutex_unlock(&m_cs);
#else
		LeaveCriticalSection(&m_cs);
#endif
	}
}