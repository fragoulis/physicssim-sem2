#pragma once
#include <windows.h>

//#define _USE_TRY_FINALLY
#ifdef _USE_TRY_FINALLY
#define __TRY __try
#define __FINALLY __finally
#else
#define __TRY
#define __FINALLY
#endif

namespace tlib
{

const DWORD WAITING_TIME = 500L;

/**
 * This class is used to control shared memory data reads and writes.
 * To use this, derive a class that contains the data you want and 
 * add the set/get functions as needed.
 */
class CMutex
{
private:
    enum { READ, WRITE, COUNT };

	HANDLE m_mutex[COUNT];

public:
	CMutex();
    virtual ~CMutex();

    //! Acquiring a read state means that the we must wait for the writing
    //! mutex only
	bool IsReadable() const {
		return ( WaitForSingleObject( m_mutex[WRITE], WAITING_TIME ) == WAIT_OBJECT_0 );
	}

    //! Acquiring a write state means that the we must wait for all mutexes
    bool IsWritable() const {
		return ( WaitForMultipleObjects( COUNT, m_mutex, TRUE, WAITING_TIME ) == WAIT_OBJECT_0 );
	}

    void ReleaseWrite() const { 
        ReleaseMutex( m_mutex[WRITE] ); 
    }

	void ReleaseAll() const { 
        ReleaseMutex( m_mutex[WRITE] ); 
        ReleaseMutex( m_mutex[READ] ); 
    }
};

} // end namespace tlib