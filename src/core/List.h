#pragma once

template <typename T>
class GCList;

template <typename T>
class GCListNode
{
friend class GCList<T>;

public:
    GCListNode<T>* GetNext() const { return m_pNext; };
    GCListNode<T>* GetPrevious() const { return m_pPrev; };
    T& GetData() { return m_data; };

private:
    GCListNode();
    ~GCListNode();

private:
    GCListNode<T>* m_pNext;
    GCListNode<T>* m_pPrev;
    T m_data;

};

template <typename T>
class GCList
{

public:
    GCList();
    ~GCList() {}
    
    void Init();
    
    GCListNode<T>* GetFirstNode() const { return m_pHead; };
    GCListNode<T>* GetLastNode() const { return m_pTail; };
    
    void PushBack( const T& data );
    void PushFront( const T& data );
    
    void RemoveNode( const GCListNode<T>* pNode );
    void DeleteNode( const GCListNode<T>* pNode );
    void DeepDeleteNode( const GCListNode<T>* pNode );
    
    void Clear();
    void DeepClear();
    
    bool Find( const T& data );
    
    int GetSize() const { return m_size; };

private:
    GCListNode<T>* m_pHead;
    GCListNode<T>* m_pTail;
    int m_size;

};



template <typename T>
GCListNode<T>::GCListNode()
{
    m_pNext = nullptr;
    m_pPrev = nullptr;
    m_data = NULL;
}

template <typename T>
GCList<T>::GCList()
{
    Init();
}

template <typename T>
void GCList<T>::Init()
{
    m_pHead = nullptr;
    m_pTail = nullptr;
    m_size = 0;
}

template <typename T>
void GCList<T>::PushBack( const T& data )
{
    GCListNode<T>* pNewNode = new GCListNode<T>();
    pNewNode->m_data = data;
    pNewNode->m_pNext = nullptr;
    pNewNode->m_pPrev = m_pTail;
    m_pTail = pNewNode;
    m_size++;
}

template <typename T>
void GCList<T>::PushFront( const T& data )
{
    GCListNode<T>* pNewNode = new GCListNode<T>();
    pNewNode->m_data = data;
    pNewNode->m_pNext = m_pHead;
    pNewNode->m_pPrev = nullptr;
    m_pHead = pNewNode;
    m_size++;
}

template <typename T>
void GCList<T>::RemoveNode( const GCListNode<T>* pNode )
{
    if ( pNode == m_pHead ) m_pHead = pNode->m_pNext;
    else if ( pNode->m_pPrev != nullptr ) pNode->m_pPrev->m_pNext = pNode->m_pNext;
    if ( pNode->m_pNext != nullptr ) pNode->m_pNext->m_pPrev = pNode->m_pPrev;
    else if ( pNode == m_pTail ) m_pTail = pNode->m_pPrev;
    m_size--;
}

template <typename T>
void GCList<T>::DeleteNode( const GCListNode<T>* pNode )
{
    RemoveNode( pNode );
    delete pNode;
}

template <typename T>
void GCList<T>::DeepDeleteNode( const GCListNode<T>* pNode )
{
    RemoveNode( pNode );
    delete pNode->m_data;
    delete pNode;
}

template <typename T>
void GCList<T>::Clear()
{
    for ( GCListNode<T>* pTemp = m_pHead->m_pNext; pTemp != nullptr; pTemp = pTemp->m_pNext )
    {
        delete m_pHead;
        m_pHead = pTemp;
    }
    Init();
}

template <typename T>
void GCList<T>::DeepClear()
{
    for ( GCListNode<T>* pTemp = m_pHead->m_pNext; pTemp != nullptr; pTemp = pTemp->m_pNext )
    {
        delete m_pHead->m_data;
        delete m_pHead;
        m_pHead = pTemp;
    }
    Init();
}

template <typename T>
bool GCList<T>::Find( const T& data )
{
    for ( GCListNode<T>* pTemp = m_pHead; pTemp != nullptr; pTemp = pTemp->m_pNext )
        if ( pTemp->m_data == data ) return true;
    return false;
}