#pragma once
#include <cassert>

template<class Type> class TFreelist
{
public:
    TFreelist(): m_index(0), m_size(0), m_pool(0) {}
    ~TFreelist() { 
        delete[] m_pool;
    }

    void create( int size );
    void clear();
    inline Type * const get();
    int size() const { return m_index; }
    Type* object(int index) { return &m_pool[index]; }
    
private:
    int m_index;
    int m_size;
    Type *m_pool;
};

template<class Type>
void TFreelist<Type>::create( int size )
{
    if( m_pool ) delete[] m_pool;

    assert(size>0);
    m_size = size;
    m_pool = new Type[size];
    clear();
}

template<class Type>
void TFreelist<Type>::clear()
{
    //memset( m_pool, 0, sizeof(Type)*m_index );
    m_index = 0;
}

template<class Type>
Type* const TFreelist<Type>::get()
{
    assert(m_index<m_size);
    return &m_pool[m_index++];
}