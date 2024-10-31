#pragma once
#include <assert.h>
#include <utility>

template <typename T>
class GCVector
{

public:
	GCVector();
	explicit GCVector( size_t initialSize );
	~GCVector();
	void Set( size_t index, const T& element );

	T& operator[]( size_t index ) { return m_pData[index]; }
	const T& operator[]( size_t index ) const { return m_pData[index]; }
	bool Insert( size_t index, const T& element );
	bool Remove( size_t index );
	T Get( size_t index );
	size_t GetIndex( const T& element );
	size_t GetSize() const { return m_size; }
	int GetCapacity() const { return m_capacity; }
	void PushBack( const T& element ) { Insert( m_size, element ); }
	void PushFront( const T& element ) { Insert( 0, element ); }

	void Clear();
	void DeepClear();
	bool IsEmpty() const;
	bool Find( const T& element ) const;

	T* begin() { return &m_pData[0]; }
	T* end() { return &m_pData[ m_size ]; }
	const T* begin() const { return &m_pData[0]; }
	const T* end() const { return &m_pData[ m_size ]; }

private:
	T* m_pData; // pointer to data
	size_t m_size; // size of the list
	size_t m_capacity; // capacity of the list

};






/////////////////////////////////
/// @brief Default construtor.
/////////////////////////////////
template <typename T>
GCVector<T>::GCVector() : GCVector(1) {}

////////////////////////////////////////////
/// @brief Constructor with initial size.
////////////////////////////////////////////
template <typename T>
GCVector<T>::GCVector( size_t capicity ) : m_pData( new T[ capicity ] ), m_size( 0 ), m_capacity( capicity ) {}

/////////////////////////
/// @brief Destructor.
/////////////////////////
template <typename T>
GCVector<T>::~GCVector()
{ delete[] m_pData; }

///////////////////////////////////////////////////////////////////////
/// @brief Set a value at index.
/// 
/// @param index Index of the list where you want to insert a value.
/// @param element Element you want to add in the list.
///////////////////////////////////////////////////////////////////////
template <typename T>
void GCVector<T>::Set( size_t index, const T& element )
{ m_pData[ index ] = element; }



///////////////////////////////////////////////////////////////////////
/// @brief Insert element at index.
/// 
/// @param index Index of the list where you want to insert a value.
/// @param element Element you want to add in the list.
/// 
/// @return True if function works.
///////////////////////////////////////////////////////////////////////
template <typename T>
bool GCVector<T>::Insert( size_t index, const T& element )
{
	if ( index > m_size )
		return false;
	
	if ( m_size == m_capacity )
	{
		// If the capacity is 0, we set it to 1 then we double it
		size_t newCapacity = ( m_capacity == 0 ) ? 1 : m_capacity * 2;
		T* newData = new T[ newCapacity ];


		// Copy elements before insertion point
		for ( size_t i = 0; i < index; i++ )
		{
			new ( &newData[i] ) T( std::move( m_pData[i] ) );
			m_pData[i].~T(); // Destruct old object
		}

		// Insert new element
		new (&newData[index]) T(element);

		// Copy remaining elements
		for ( size_t i = index; i < m_size; i++ )
		{
			new ( &newData[ i + 1 ] ) T( std::move( m_pData[i] ) );
			m_pData[i].~T(); // Destruct old object
		}

		delete[] m_pData;
		m_pData = newData;
		m_capacity = newCapacity;
	}

	else
	{
		// Shift elements to the right
		for ( size_t i = m_size; i > index; i-- )
		{
			new ( &m_pData[i] ) T( std::move( m_pData[ i - 1 ] ) );
			m_pData[ i - 1 ].~T();
		}
		// Insert new element
		new ( &m_pData[ index ] ) T( element );
	}

	m_size++;
	return true;
}

///////////////////////////////////////////////////////////////////////
/// @brief Remove element at index.
/// 
/// @param index Index of the list where you want to remove a value.
/// 
/// @return True if function works.
///////////////////////////////////////////////////////////////////////
template <typename T>
bool GCVector<T>::Remove( size_t index )
{
	if ( index >= m_size )
		return false;

	for ( size_t i = index; i < m_size - 1; i++ )
		m_pData[i] = m_pData[ i + 1 ];
	
	m_size--;
	return true;
}

////////////////////////////////////////////////////////////////////
/// @brief Get data at index.
/// 
/// @param index Index of the list where you want to get a value.
////////////////////////////////////////////////////////////////////
template <typename T>
T GCVector<T>::Get( size_t index )
{
	assert( index < m_size );
	return m_pData[index];
}

////////////////////////////////////////////////////////////////
/// @brief Get index with data.
/// 
/// @param element Element of the list you want to get index.
////////////////////////////////////////////////////////////////
template <typename T>
size_t GCVector<T>::GetIndex( const T& element )
{
	for ( size_t i = 0; i < m_size; i++ )
		if ( m_pData[i] == element )
			return i;
	return -1;
}



////////////////////////////////////////////////////////////////////
/// @brief Simple clear of the list.
/// 
/// @warning This function doesn't delete references in the list.
////////////////////////////////////////////////////////////////////
template <typename T>
void GCVector<T>::Clear()
{
	delete[] m_pData;
	m_pData = nullptr;
	m_size = 0;
	m_capacity = 0;
}

/////////////////////////////////////////////////
/// @brief Clear the list and delete the data.
/////////////////////////////////////////////////
template <typename T>
void GCVector<T>::DeepClear()
{
	for ( size_t i = 0; i < m_size; i++ )
		delete m_pData[i];
	Clear();
}

/////////////////////////////////////////////
/// @brief Check if the list is empty.
/// 
/// @return True if the list is empty.
/// @return False if the list isn't empty.
/////////////////////////////////////////////
template <typename T>
bool GCVector<T>::IsEmpty() const
{ return m_size == 0; }

/////////////////////////////////////////////////////////////
/// @brief Check if the list contains an element you give.
/// 
/// @return True if the element exist.
/// @return False if the element dont exist.
/////////////////////////////////////////////////////////////
template<typename T>
bool GCVector<T>::Find( const T& element ) const
{
	for ( size_t i = 0; i < m_size; i++ )
		if ( m_pData[i] == element )
			return true;
	return false;
}