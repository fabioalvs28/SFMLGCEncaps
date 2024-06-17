#pragma once
#include <assert.h>


template <typename T>
class GCVector
{

public:
	GCVector();
	explicit GCVector(size_t initialSize);
	~GCVector();
	void Set(size_t index, const T& element);

	T& operator[](size_t index) { return m_data[index]; }
	bool Insert(size_t index, const T& element);
	bool Remove(size_t index);
	T Get(size_t index);
	size_t GetIndex( const T& element); 
	size_t GetSize() const { return m_size; }
	int GetCapacity() const { return m_capacity; }
	void PushBack(const T& element) { Insert(m_size, element); }
	void PushFront(const T& element) { Insert(0, element); }

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
GCVector<T>::GCVector() : GCVector(1) {}

// Constructor with initial size
template <typename T>
GCVector<T>::GCVector(size_t capicity) : m_data(new T[capicity]), m_size(0), m_capacity(capicity) {}

//Destructor
template <typename T>
GCVector<T>::~GCVector()
{
	delete[] m_data;
}


/// <summary>
///  Set a value at index
/// </summary>
/// <param name="index">Index of the list where you want to insert a value</param>
/// <param name="element">Element you want to add in the list</param>

template <typename T>
void GCVector<T>::Set(size_t index, const T& element)
{
	m_data[index] = element;
}


/// <summary>
///  Insert element at index
///  WARNING : This function can't call the overload construtor of T 
/// </summary>
/// <param name="index">Index of the list where you want to insert a value</param>
/// <param name="element">Element you want to add in the list</param>
/// <returns>True if function works</returns>

template <typename T>
bool GCVector<T>::Insert(size_t index, const T& element)
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

/// <summary>
///  Remove element at index
/// </summary>
/// <param name="index">Index of the list where you want to remove a value</param>
/// <returns>True if function works</returns>

template <typename T>
bool GCVector<T>::Remove(size_t index)
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

/// <summary>
///  Get data at index
/// </summary>
/// <param name="index">Index of the list where you want to get a value</param>

template <typename T>
T GCVector<T>::Get(size_t index)
{
	assert(index < m_size);
	return m_data[index];
}


/// <summary>
///  Get index with data
/// </summary>
/// <param name="element">element of the list you want to get index </param>
template <typename T>
size_t GCVector<T>::GetIndex(const T& element)
{
	for (size_t i = 0; i < m_size; i++)
    {
        if (m_data[i] == element)
        {
            return i;
        }
    }
    return -1;
}


/// <summary>
///  Simple clear of the list
/// WARNING : This function doesn't delete references in the list
/// </summary>

template <typename T>
void GCVector<T>::Clear()
{
	delete[] m_data;
	m_data = nullptr;
	m_size = 0;
	m_capacity = 0;
}

/// <summary>
///  Clear the list and delete the data
/// </summary>

template <typename T>
void GCVector<T>::DeepClear()
{
	for (size_t i = 0; i < m_size; i++)
	{
		delete m_data[i];
	}
	Clear();
}

/// <summary>
///  Check if the list is empty
/// </summary>
/// /// <returns>True if the list is empty</returns>
/// <returns>False if the list isn't empty</returns>

template <typename T>
bool GCVector<T>::IsEmpty() const
{
	if (m_size == 0)
		return true;
	else
		return false;
}

