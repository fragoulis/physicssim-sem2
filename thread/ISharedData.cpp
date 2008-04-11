#include "ISharedData.h"
#include <cassert>
using namespace tlib;

ISharedData::ISharedData() 
{
	m_mutex[READ] = CreateMutex( NULL, FALSE, "read_mutex" );
	assert(m_mutex[READ]);
    m_mutex[WRITE] = CreateMutex( NULL, FALSE, "write_mutex" );
    assert(m_mutex[WRITE]);
}

ISharedData::~ISharedData() 
{
	CloseHandle( m_mutex[READ] );
    CloseHandle( m_mutex[WRITE] );
}