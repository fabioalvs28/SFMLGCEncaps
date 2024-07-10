#pragma once
#include <queue>

/*
	   /!\ WARNING : This class encapsulate the STL this is not our OWN methods. /!\
*/


template <typename T>
class GCQueue
{
public:
	GCQueue() = default;

	void Push(const T& value);
	void Pop();
	T& Front();
	T& Back();
	size_t GetSize() const { return m_Queue.size(); };
	bool IsEmpty() const { return m_Queue.empty(); };

private:
	std::queue<T> m_Queue;
};

/// <summary>
///  Push a value at the end of the list
/// </summary>
/// <param name="value">Value that you want to push</param>

template<typename T>
void GCQueue<T>::Push(const T& value)
{
	m_Queue.push(value);
}

/// <summary>
///  Erase the first element of the list
/// </summary>

template<typename T>
void GCQueue<T>::Pop()
{
	if (!IsEmpty())
		m_Queue.pop();
}

/// <summary>
/// Return the first element of the list
/// </summary>
/// <typeparam name="T"></typeparam>

template<typename T>
T& GCQueue<T>::Front()
{
	if (!IsEmpty())
		return m_Queue.front();
}

/// <summary>
/// Return the last element of the list
/// </summary>
/// <typeparam name="T"></typeparam>

template<typename T>
T& GCQueue<T>::Back()
{
	return m_Queue.back();
}