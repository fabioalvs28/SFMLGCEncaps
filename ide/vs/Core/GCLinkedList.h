#pragma once

template <typename T>
class GCLinkedListNode
{
friend class GCLinkedList<T>;

private:
    GCLinkedListNode();
    ~GCLinkedListNode();
    
    void Remove();
    T Pop();

private:
    GCLinkedListNode<T>* m_pNext;
    GCLinkedListNode<T>* m_pPrev;
    T m_data;

};

template <typename T>
class GCLinkedList
{

public:
    GCLinkedList();
    ~GCLinkedList();
    
    void Init();
    
    T GetFirst() const { return m_pHead->m_data; };
    T GetLast() const { return m_pTail->m_data; };
    
    void PushBack( T& data );
    void PushFront( T& data );
    void Clear();
    
    bool Find( T& data );
    
    int GetSize() const { return m_size; };

private:
    GCLinkedListNode<T>* m_pHead;
    GCLinkedListNode<T>* m_pTail;
    int m_size;

};



template <typename T>
GCLinkedListNode<T>::GCLinkedListNode()
{
    m_pNext = nullptr;
    m_pPrev = nullptr;
    m_data = NULL;
}

template <typename T>
void GCLinkedListNode<T>::Remove()
{
    if ( m_pNext != nullptr ) m_pNext->m_pPrev = m_pPrev;
    if ( m_pPrev != nullptr ) m_pPrev->m_pNext = m_pNext;
}

template <typename T>
T GCLinkedListNode<T>::Pop()
{
    T temp = m_data;
    Remove();
    return temp;
}

template <typename T>
GCLinkedList<T>::GCLinkedList()
{
    Init();
}

template <typename T>
void GCLinkedList<T>::Init()
{
    m_pHead = nullptr;
    m_pTail = nullptr;
    m_size = 0;
}

template <typename T>
void GCLinkedList<T>::PushBack( T& data )
{
    GCLinkedListNode<T>* pNewNode = new GCLinkedListNode<T>();
    pNewNode->m_data = data;
    pNewNode->m_pNext = nullptr;
    pNewNode->m_pPrev = m_pTail;
    m_pTail = pNewNode;
    m_size++;
}

template <typename T>
void GCLinkedList<T>::PushFront( T& data )
{
    GCLinkedListNode<T>* pNewNode = new GCLinkedListNode<T>();
    pNewNode->m_data = data;
    pNewNode->m_pNext = m_pHead;
    pNewNode->m_pPrev = nullptr;
    m_pHead = pNewNode;
    m_size++;
}

template <typename T>
bool GCLinkedList<T>::Query( T& data )
{
    for ( GCLinkedListNode<T>* pTemp = m_pHead; pTemp != nullptr; pTemp = pTemp->m_pNext )
        if ( pTemp->m_data == data ) return true;
    return false;
}