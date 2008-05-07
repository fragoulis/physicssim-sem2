#include "CMutex.h"
#include <cassert>
using namespace tlib;

CMutex::CMutex() 
{
	m_mutex[READ] = CreateMutex( NULL, FALSE, "read_mutex" );
	assert(m_mutex[READ]);
    m_mutex[WRITE] = CreateMutex( NULL, FALSE, "write_mutex" );
    assert(m_mutex[WRITE]);
}

CMutex::~CMutex() 
{
	CloseHandle( m_mutex[READ] );
    CloseHandle( m_mutex[WRITE] );
}