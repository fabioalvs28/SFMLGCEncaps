#pragma once

template <typename T>
class GCLinkedListNode
{
friend class GCLinkedList<T>;

private:
    GCLinkedListNode();
    ~GCLinkedListNode();
    
    void Destroy();
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
    
    T GetFirst() const { return m_pHead->m_data; };
    T GetLast() const { return m_pTail->m_data; };
    
    void PushBack( T& data );
    void PushFront( T& data );
    
    bool Query( T& data );
    
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
void GCLinkedListNode<T>::Destroy()
{
    if ( m_pNext != nullptr ) m_pNext->m_pPrev = m_pPrev;
    if ( m_pPrev != nullptr ) m_pPrev->m_pNext = m_pNext;
    delete this;
}

template <typename T>
T GCLinkedListNode<T>::Pop()
{
    T temp = m_data;
    Destroy();
    return temp;
}

template <typename T>
GCLinkedList<T>::GCLinkedList()
{
    m_pHead = nullptr;
    m_pTail = nullptr;
    m_size = 0;
}

template <typename T>
void GCLinkedList<T>::PushBack( T& data )
{
    GCLinkedListNode<T>* newNode = new GCLinkedListNode<T>();
    newNode->m_data = data;
    newNode->m_pNext = nullptr;
    newNode->m_pPrev = m_pTail;
    m_pTail = newNode;
    m_size++;
}

template <typename T>
void GCLinkedList<T>::PushFront( T& data )
{
    GCLinkedListNode<T>* newNode = new GCLinkedListNode<T>();
    newNode->m_data = data;
    newNode->m_pNext = m_pHead;
    newNode->m_pPrev = nullptr;
    m_pHead = newNode;
    m_size++;
}

template <typename T>
bool GCLinkedList<T>::Query( T& data )
{
    for ( GCLinkedListNode<T>* temp = m_pHead; temp != nullptr; temp->m_pNext )
        if ( temp->m_data == data ) return true;
    return false;
}