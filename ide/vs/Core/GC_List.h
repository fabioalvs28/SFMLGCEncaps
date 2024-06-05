#pragma once
#include <assert.h>


template <typename T>
class GCList
{

public:
	GCList();
	explicit GCList(size_t initialSize);
	~GCList();
	void Set(size_t index, const T& element);

	T& operator[](size_t index) { return m_data[index]; }
	bool Insert(size_t index, const T& element);
	bool Remove(size_t index);
	T Get(size_t index);
	size_t GetSize() const { return m_size; }
	int GetCapacity() const { return m_capacity; }

	void Clear();
	void DeepClear();
	bool IsEmpty() const;

private:

	T* m_data; //pointer to data
	size_t m_size; //size of the list
	size_t m_capacity; //capacity of the list

};

//Default construtor
template <typename T>
GCList<T>::GCList() : GCList(1) {}

// Constructor with initial size
template <typename T>
GCList<T>::GCList(size_t capicity) : m_data(new T[capicity]), m_size(0), m_capacity(capicity) {}

//Destructor
template <typename T>
GCList<T>::~GCList()
{
	delete[] m_data;
}

//Set element at index
template <typename T>
void GCList<T>::Set(size_t index, const T& element)
{
	m_data[index] = element;
}

//Insert element at index
//WARNING : This function can't call the overload construtor of T 
template <typename T>
bool GCList<T>::Insert(size_t index, const T& element)
{
	if (index > m_size)
	{
		return false;
	}
	if (m_size == m_capacity)
	{
		m_capacity *= 2;
		T* temp = new T[m_capacity];
		memcpy(temp, m_data, m_size * sizeof(T));
		free(m_data);
		m_data = temp;
	}
	memcpy(m_data + 1, m_data, (m_size - index) * sizeof(T));
	m_data[index] = element;
	m_size++;
	return true;
}

//Remove element at index
template <typename T>
bool GCList<T>::Remove(size_t index)
{
	if (index >= m_size)
		return false;

	for (size_t i = index; i < m_size - 1; i++)
	{
		m_data[i] = m_data[i + 1];
	}
	m_size--;
	return true;
}

//Get data at index
template <typename T>
T GCList<T>::Get(size_t index)
{
	assert(index < m_size);
	return m_data[index];
}

//CLear the list
template <typename T>
void GCList<T>::Clear()
{
	delete[] m_data;
	m_data = nullptr;
	m_size = 0;
	m_capacity = 0;
}

//Clear the list and delete the data
template <typename T>
void GCList<T>::DeepClear()
{
	for (size_t i = 0; i < m_size; i++)
	{
		delete m_data[i];
	}
	Clear();
}

//Check if the list is empty
template <typename T>
bool GCList<T>::IsEmpty() const
{
	if (m_size == 0)
		return true;
	else
		return false;
}

