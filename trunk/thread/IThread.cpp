#include "IThread.h"
#include <process.h>
#include "../Util/CLogger.h"

namespace tlib
{

// ----------------------------------------------------------------------------
IThread::IThread(): m_bIsRunning(false)
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

    if( NULL == m_Handle )
    {
        _LOG("CreateThread error");
        return;
    }

    m_bIsRunning = true;
}

// ----------------------------------------------------------------------------
void IThread::Terminate()
{
    Stop();

    // Wait for it to stop
	WaitForSingleObject( m_Handle, INFINITE );

    _LOG("Thread terminated!");
	
    // Close thread handle
	CloseHandle( m_Handle );
	m_Handle = 0;
}

void IThread::Resume()
{ 
    m_bIsRunning = true;
    ResumeThread(m_Handle);
}

void IThread::Suspend() 
{ 
    SuspendThread(m_Handle);
    m_bIsRunning = false;
}

void IThread::Toggle()
{ 
    if( m_bIsRunning ) Suspend();
    else Resume();
}

void IThread::OnStart() {}

void IThread::OnEnd() {}

} // end namespace tlib