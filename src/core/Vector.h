#pragma once
#include <assert.h>
#include <utility>


template <typename T>
class GCVector
{

public:
	GCVector();
	explicit GCVector(size_t initialSize);
	~GCVector();
	void Set(size_t index, const T& element);

	T& operator[](size_t index) { return m_data[index]; }
	const T& operator[](size_t index) const { return m_data[index]; }
	bool Insert(size_t index, const T& element);
	bool Remove(size_t index);
	T Get(size_t index);
	size_t GetIndex(const T& element);
	size_t GetSize() const { return m_size; }
	int GetCapacity() const { return m_capacity; }
	void PushBack(const T& element) { Insert(m_size, element); }
	void PushFront(const T& element) { Insert(0, element); }

	void Clear();
	void DeepClear();
	bool IsEmpty() const;
	bool Find(const T& element) const;

	T* begin() { return &m_data[0]; }
	T* end() { return &m_data[m_size]; }
	const T* begin() const { return &m_data[0]; }
	const T* end() const { return &m_data[m_size]; }

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
		//if the capacity is 0, we set it to 1 then we double it
		size_t newCapacity = (m_capacity == 0) ? 1 : m_capacity * 2;
		T* newData = new T[newCapacity];


		//Copy elements before insertion point
		for (size_t i = 0; i < index; i++)
		{
			new (&newData[i]) T(std::move(m_data[i]));
			m_data[i].~T(); //Destruct old object
		}

		//Insert new element
		new (&newData[index]) T(element);

		//Copy remaining elements
		for (size_t i = index; i < m_size; i++)
		{
			new (&newData[i + 1]) T(std::move(m_data[i]));
			m_data[i].~T(); //Destruct old object
		}

		delete[] m_data;
		m_data = newData;
		m_capacity = newCapacity;
	}

	else
	{
		//Shift elements to the right
		for (size_t i = m_size; i > index; i--)
		{
			new (&m_data[i]) T(std::move(m_data[i - 1]));
			m_data[i - 1].~T();
		}
		//Insert new element
		new (&m_data[index]) T(element);
	}

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


/// <summary>
///  Check if the list contains an element you give
/// </summary>
/// /// <returns>True if the element exist</returns>
/// <returns>False if the element dont exist</returns>

template<typename T>
bool GCVector<T>::Find(const T& element) const
{
	for (size_t i = 0; i < m_size; i++)
	{
		if (m_data[i] == element)
		{
			return true;
		}
	}
	return false;
}