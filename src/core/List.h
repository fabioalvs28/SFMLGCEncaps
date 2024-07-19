#pragma once
#include "Define.h"

template <typename T> class GCList;



template <typename T>
class GCListNode
{
friend class GCList<T>;

public:
    GCListNode<T>* PushBefore( const T& data );
    GCListNode<T>* PushAfter( const T& data );
    
    void Remove();
    void Delete();
    
    T& GetData();
    GCListNode<T>* GetPrevious() const;
    GCListNode<T>* GetNext() const;

private:
    GCListNode( const T& data );
    ~GCListNode() {}

private:
    GCList<T>* m_pList;
    GCListNode<T>* m_pNext;
    GCListNode<T>* m_pPrev;
    T m_data;

};



template <typename T>
class GCList
{
friend class GCListNode<T>;

public:
    GCList();
    ~GCList() = default;
    
    void Init();
    
    GCListNode<T>* PushBack( const T& data );
    GCListNode<T>* PushFront( const T& data );
    
    void Clear();
    
    bool Find( const T& data ) const;
    
    GCListNode<T>* GetFirstNode() const;
    GCListNode<T>* GetLastNode() const;

private:
    GCListNode<T>* PushBefore( const T& data, GCListNode<T>* pNode );
    GCListNode<T>* PushAfter( const T& data, GCListNode<T>* pNode );
    
    void RemoveNode( const GCListNode<T>* pNode );
    void DeleteNode( const GCListNode<T>* pNode );

private:
    GCListNode<T>* m_pHead;
    GCListNode<T>* m_pTail;

};






///////////////////////////////////////////////////////////
/// @brief Default constructor for the GCListNode class.
/// 
/// @tparam T The type to be stored in the ListNode.
///////////////////////////////////////////////////////////
template <typename T>
GCListNode<T>::GCListNode( const T& data )
{
    m_pList = nullptr;
    m_pNext = nullptr;
    m_pPrev = nullptr;
    m_data = data;
}



//////////////////////////////////////////////////////////////////////////////
/// @brief Creates a new ListNode with the given data before this ListNode.
/// 
/// @tparam T The type that is stored in the List.
/// 
/// @param data The data to be added to the List.
/// 
/// @return A pointer to the newly created ListNode.
//////////////////////////////////////////////////////////////////////////////
template <typename T>
GCListNode<T>* GCListNode<T>::PushBefore( const T& data ) { return m_pList->PushBefore( data, this ); }

/////////////////////////////////////////////////////////////////////////////
/// @brief Creates a new ListNode with the given data after this ListNode.
/// 
/// @tparam T The type that is stored in the List.
/// 
/// @param data The data to be added to the List.
/// 
/// @return A pointer to the newly created ListNode.
/////////////////////////////////////////////////////////////////////////////
template <typename T>
GCListNode<T>* GCListNode<T>::PushAfter( const T& data ) { return m_pList->PushAfter( data, this ); }



//////////////////////////////////////////////////////
/// @brief Removes the ListNode from the List.
/// 
/// @tparam T The type that is stored in the List.
/// 
/// @note Does not delete the ListNode or its data.
//////////////////////////////////////////////////////
template <typename T>
void GCListNode<T>::Remove() { m_pList->RemoveNode( this ); }

//////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Deletes the ListNode.
/// 
/// @tparam T The type that is stored in the List.
/// 
/// @note If the stored data is a pointer, it does not delete the value behind the pointer.
//////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
void GCListNode<T>::Delete() { m_pList->DeleteNode( this ); }



/////////////////////////////////////////////////////
/// @tparam T The type that is stored in the List.
/// 
/// @return The data stored in the ListNode.
/////////////////////////////////////////////////////
template <typename T>
T& GCListNode<T>::GetData() { return m_data; };

//////////////////////////////////////////////////////////
/// @tparam T The type that is stored in the List.
/// 
/// @return A pointer to the previous Node in the List.
//////////////////////////////////////////////////////////
template <typename T>
GCListNode<T>* GCListNode<T>::GetPrevious() const { return m_pPrev; };

//////////////////////////////////////////////////////
/// @tparam T The type that is stored in the List.
/// 
/// @return A pointer to the next Node in the List.
//////////////////////////////////////////////////////
template <typename T>
GCListNode<T>* GCListNode<T>::GetNext() const { return m_pNext; };






///////////////////////////////////////////////////////
/// @brief Default constructor for the GCList class.
/// 
/// @tparam T the type to be stored in the List.
///////////////////////////////////////////////////////
template <typename T>
GCList<T>::GCList() { Init(); }



////////////////////////////////////////////////////////////
/// @brief Sets the head and tail of the List to nullptr.
/// 
/// @tparam T The type that is stored in the List.
////////////////////////////////////////////////////////////
template <typename T>
void GCList<T>::Init()
{
    m_pHead = nullptr;
    m_pTail = nullptr;
}



/////////////////////////////////////////////////////////////////////////////////
/// @brief Creates a new ListNode with the given data at the tail of the List.
/// 
/// @tparam T The type that is stored in the GCList.
/// 
/// @param data The data to be added to the List.
/////////////////////////////////////////////////////////////////////////////////
template <typename T>
GCListNode<T>* GCList<T>::PushBack( const T& data )
{
    GCListNode<T>* pNewNode = new GCListNode<T>( data );
    pNewNode->m_pList = this;
    pNewNode->m_pNext = nullptr;
    if ( m_pTail != nullptr )
    {
        pNewNode->m_pPrev = m_pTail;
        pNewNode->m_pPrev->m_pNext = pNewNode;
    }
    m_pTail = pNewNode;
    if ( m_pHead == nullptr )
        m_pHead = pNewNode;
    return pNewNode;
}

/////////////////////////////////////////////////////////////////////////////////
/// @brief Creates a new ListNode with the given data at the head of the List.
/// 
/// @tparam T The type that is stored in the GCList.
/// 
/// @param data The data to be added to the List.
/////////////////////////////////////////////////////////////////////////////////
template <typename T>
GCListNode<T>* GCList<T>::PushFront( const T& data )
{
    GCListNode<T>* pNewNode = new GCListNode<T>( data );
    pNewNode->m_pList = this;
    if ( m_pHead != nullptr )
    {
        pNewNode->m_pNext = m_pHead;
        pNewNode->m_pNext->m_pPrev = pNewNode;
    }
    pNewNode->m_pPrev = nullptr;
    m_pHead = pNewNode;
    if ( m_pTail == nullptr )
        m_pTail = pNewNode;
    return pNewNode;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Creates a new ListNode with the given data before the given ListNode.
/// 
/// @tparam T The type that is stored in the List.
/// 
/// @param data The data to be added to the List.
/// @param pNode a pointer to the ListNode before which the new ListNode will be created.
/// 
/// @return A pointer to the newly created ListNode.
////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
GCListNode<T>* GCList<T>::PushBefore( const T& data, GCListNode<T>* pNode )
{
    ASSERT( pNode != nullptr, LOG_FATAL, "Attempting to push before a nullptr Node" );
    ASSERT( pNode->m_pList == this, LOG_FATAL, "Attempting to push before a Node from a different List" );
    
    GCListNode<T>* pNewNode = new GCListNode<T>( data );
    pNewNode->m_pList = this;
    
    if ( pNode == m_pHead )
        m_pHead = pNewNode;
    else
    {
        pNode->m_pPrev->m_pNext = pNewNode;
        pNewNode->m_pPrev = pNode->m_pPrev;
    }
    
    pNewNode->m_pNext = pNode;
    pNode->m_pPrev = pNewNode;
    return pNewNode;
}

///////////////////////////////////////////////////////////////////////////////////////////
/// @brief Creates a new ListNode with the given data after the given ListNode.
/// 
/// @tparam T The type that is stored in the List.
/// 
/// @param data The data to be added to the List.
/// @param pNode A pointer to the ListNode after which the new ListNode will be created.
/// 
/// @return A pointer to the newly created ListNode.
///////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
GCListNode<T>* GCList<T>::PushAfter( const T& data, GCListNode<T>* pNode )
{
    ASSERT( pNode != nullptr, LOG_FATAL, "Attempting to push after a nullptr Node" );
    ASSERT( pNode->m_pList == this, LOG_FATAL, "Attempting to push after a Node from a different List" );
    
    GCListNode<T>* pNewNode = new GCListNode<T>( data );
    pNewNode->m_pList = this;
    
    if ( pNode == m_pTail )
        m_pTail = pNewNode;
    else
    {
        pNode->m_pNext->m_pPrev = pNewNode;
        pNewNode->m_pNext = pNode->m_pNext;
    }
    
    pNewNode->m_pPrev = pNode;
    pNode->m_pNext = pNewNode;
    return pNewNode;
}



////////////////////////////////////////////////////////////////
/// @brief Removes the ListNode from the List.
/// 
/// @tparam T The type that is stored in the List.
/// 
/// @param pNode The node to be removed from the List.
/// 
/// @warning You can't remove a node that is not in the List.
/// 
/// @note Does not delete the ListNode or its data.
////////////////////////////////////////////////////////////////
template <typename T>
void GCList<T>::RemoveNode( const GCListNode<T>* pNode )
{
    ASSERT( pNode != nullptr, LOG_FATAL, "Attempting to remove a nullptr Node from the List" );
    ASSERT( pNode->m_pList == this, LOG_FATAL, "Attempting to remove a Node from a different List" );
    
    if ( pNode == m_pHead )
        m_pHead = pNode->m_pNext;
    else
        pNode->m_pPrev->m_pNext = pNode->m_pNext;
    
    if ( pNode == m_pTail )
        m_pTail = pNode->m_pPrev;
    else
        pNode->m_pNext->m_pPrev = pNode->m_pPrev;
}

//////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Deletes the ListNode.
/// 
/// @tparam T The type that is stored in the List.
/// 
/// @param pNode The node to be deleted.
/// 
/// @warning You can't delete a node that is not in the List.
/// 
/// @note If the stored data is a pointer, it does not delete the value behind the pointer.
//////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
void GCList<T>::DeleteNode( const GCListNode<T>* pNode )
{
    ASSERT( pNode != nullptr, LOG_FATAL, "Attempting to delete a nullptr Node from the List" );
    ASSERT( pNode->m_pList == this, LOG_FATAL, "Attempting to delete a Node from a different List" );
    
    RemoveNode( pNode );
    delete pNode;
}

///////////////////////////////////////////////////////////////////////////////////////
/// @brief Deletes every node in the List.
/// 
/// @tparam T The type that is stored in the List.
/// 
/// @note If the stored data are pointers, it does not delete the value behind them.
///////////////////////////////////////////////////////////////////////////////////////
template <typename T>
void GCList<T>::Clear()
{
	if ( m_pHead == nullptr )
        return;
    
    for ( GCListNode<T>* pTemp = m_pHead->m_pNext; pTemp != nullptr; pTemp = pTemp->m_pNext )
    {
        delete m_pHead;
        m_pHead = pTemp;
    }
    Init();
}



//////////////////////////////////////////////////////////////
/// @brief Checks whether the list contains the given data.
/// 
/// @tparam T The type that is stored in the List.
/// 
/// @param data The data to be checked.
/// 
/// @return true if the given data is found is the List.
//////////////////////////////////////////////////////////////
template <typename T>
bool GCList<T>::Find( const T& data ) const
{
    for ( GCListNode<T>* pTemp = m_pHead; pTemp != nullptr; pTemp = pTemp->m_pNext )
        if ( pTemp->m_data == data )
            return true;
    return false;
}



//////////////////////////////////////////////////////////////
/// @tparam T The type that is stored in the List.
/// 
/// @return A pointer to the first Node of the List (head).
//////////////////////////////////////////////////////////////
template <typename T>
GCListNode<T>* GCList<T>::GetFirstNode() const { return m_pHead; };

/////////////////////////////////////////////////////////////
/// @tparam T The type that is stored in the List.
/// 
/// @return A pointer to the last Node of the List (tail).
/////////////////////////////////////////////////////////////
template <typename T>
GCListNode<T>* GCList<T>::GetLastNode() const { return m_pTail; };