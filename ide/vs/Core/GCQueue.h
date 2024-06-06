#pragma once
#include <queue>


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

template<typename T>
void GCQueue<T>::Push(const T& value)
{
	m_Queue.push(value);
}

template<typename T>
void GCQueue<T>::Pop()
{
	if (!IsEmpty())
		m_Queue.pop();
}

template<typename T>
T& GCQueue<T>::Front()
{
	if (!IsEmpty())
		return m_Queue.front();
}

template<typename T>
T& GCQueue<T>::Back()
{
	return m_Queue.back();
}