#include "IThread.h"
#include <process.h>

namespace tlib
{

// ----------------------------------------------------------------------------
IThread::IThread()
{
    m_lpThreadFunc = IThread::ThreadFunc;
}

// ----------------------------------------------------------------------------
void IThread::Start( void *lpArgs )
{
    m_lpArgs = lpArgs;

    unsigned threadId;
    m_Handle = (HANDLE)_beginthreadex(
		NULL,		    // no security attributes (child cannot inherited handle)
		0,	            // default stack size
		m_lpThreadFunc,	// code to run on new thread
		this,		    // pointer to host application class
		0,			    // run immediately (could create suspended)
		&threadId	    // OUT: returns thread ID
	);

    Resume();
}

// ----------------------------------------------------------------------------
void IThread::Terminate()
{
    Pause();

    // Wait for it to stop
	WaitForSingleObject( m_Handle, INFINITE );
    
	// Close thread handle
	CloseHandle( m_Handle );
	m_Handle = 0;
}

} // end namespace tlib