#pragma once

template <typename T>
class GCLinkedList;

template <typename T>
class GCLinkedListNode
{
friend class GCLinkedList<T>;

public:
    GCLinkedListNode<T>* GetNext() const { return m_pNext; };
    GCLinkedListNode<T>* GetPrevious() const { return m_pPrev; };
    T& GetData() const { return m_data; };
    
    void Destroy();
    void DeepDestroy();
    void Remove();
    T Pop();

private:
    GCLinkedListNode();
    ~GCLinkedListNode();

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
    ~GCLinkedList() {}
    
    void Init();
    
    GCLinkedListNode<T>* GetFirstNode() const { return m_pHead; };
    GCLinkedListNode<T>* GetLastNode() const { return m_pTail; };
    
    void PushBack( const T& data );
    void PushFront( const T& data );
    
    void Clear();
    void DeepClear();
    
    bool Find( const T& data );
    
    int GetSize() const { return m_size; };

private:
    GCLinkedListNode<T>* m_pHead;
    GCLinkedListNode<T>* m_pTail;
    int m_size;

};



template <typename T>
void GCLinkedListNode<T>::Destroy()
{
    Remove();
    delete this;
}

template <typename T>
void GCLinkedListNode<T>::DeepDestroy()
{
    Remove();
    delete m_data;
    delete this;
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
GCLinkedListNode<T>::GCLinkedListNode()
{
    m_pNext = nullptr;
    m_pPrev = nullptr;
    m_data = NULL;
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
void GCLinkedList<T>::PushBack( const T& data )
{
    GCLinkedListNode<T>* pNewNode = new GCLinkedListNode<T>();
    pNewNode->m_data = data;
    pNewNode->m_pNext = nullptr;
    pNewNode->m_pPrev = m_pTail;
    m_pTail = pNewNode;
    m_size++;
}

template <typename T>
void GCLinkedList<T>::PushFront( const T& data )
{
    GCLinkedListNode<T>* pNewNode = new GCLinkedListNode<T>();
    pNewNode->m_data = data;
    pNewNode->m_pNext = m_pHead;
    pNewNode->m_pPrev = nullptr;
    m_pHead = pNewNode;
    m_size++;
}

template <typename T>
void GCLinkedList<T>::Clear()
{
    GCLinkedListNode<T>* pTemp;
    while ( pTemp != nullptr )
    {
        pTemp = m_pHead->m_pNext;
        delete m_pHead;
        m_pHead = pTemp;
    }
    Init();
}

template <typename T>
void GCLinkedList<T>::DeepClear()
{
    GCLinkedListNode<T>* pTemp;
    while ( pTemp != nullptr )
    {
        pTemp = m_pHead->m_pNext;
        delete m_pHead->m_data;
        delete m_pHead;
        m_pHead = pTemp;
    }
    Init();
}

template <typename T>
bool GCLinkedList<T>::Find( const T& data )
{
    for ( GCLinkedListNode<T>* pTemp = m_pHead; pTemp != nullptr; pTemp = pTemp->m_pNext )
        if ( pTemp->m_data == data ) return true;
    return false;
}