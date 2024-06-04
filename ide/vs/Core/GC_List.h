#pragma once
#include <assert.h>


template <typename T>
class GCList
{

public:
	GCList();
	explicit GCList(size_t initialSize);
	~GCList();
	void SetSize(size_t newSize) { m_size = newSize; }
	void SetCapacity(size_t newCapacity) { m_capacity = newCapacity; }
	void Set(size_t index, const T& element);

	void AllocateTab(size_t capicity);
	T& operator[](size_t index) { return m_data[index]; }
	bool Insert(size_t index, const T& element);
	bool Remove(size_t index);
	T Get(size_t index);
	int GetSize() const { return m_size; }
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
GCList<T>::GCList() : m_data(nullptr), m_size(0), m_capacity(0) {}

// Constructor with initial size
template <typename T>
GCList<T>::GCList(size_t initialSize) : m_data(new T[initialSize]), m_size(0), m_capacity(initialSize) {}

//Destructor
template <typename T>
GCList<T>::~GCList()
{
	delete[] m_data;
}

template <typename T>
void GCList<T>::AllocateTab(size_t capicity)
{
	m_data = new T[capicity];
	m_capacity = capicity;
	m_size = 0;
}
//Set element at index
template <typename T>
void GCList<T>::Set(size_t index, const T& element)
{
	m_data[index] = element;
}

//Assert element at index
template <typename T>
bool GCList<T>::Insert(size_t index, const T& element)
{
	if (m_data == nullptr)
		AllocateTab(index + 1); //check if data is null, if so, allocate memory for the new element

	if (index >= m_size)
	{
		if (m_size == m_capacity)
		{
			m_capacity *= 2;
			T* temp = new T[m_capacity];
			for (size_t i = 0; i < m_size; i++)
			{
				temp[i] = m_data[i];
			}
			delete[] m_data;
			m_data = temp;
		}
		for (size_t i = m_size; i > index; i--)
		{
			m_data[i] = m_data[i - 1];
		}
		m_data[index] = element;
		m_size++;
		return true;
	}
	return false;
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

