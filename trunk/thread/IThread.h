#pragma once
#include <windows.h> // Windows API

namespace tlib
{

/**
 * @class IThread
 * Windows thread wrapper class.
 */
class IThread
{
private:
    typedef unsigned int (__stdcall *THREADFUNC)(void*);

    //! Flag for whether the thread is running
    bool m_bIsRunning;

    //! The thread's os handle
    HANDLE      m_Handle;
    
    //! The thread's parameters
    LPVOID      m_lpArgs;

    //! Pointer to the main thread function
    THREADFUNC  m_lpThreadFunc;

public:
    //! Constructor
    IThread();

    //! Virtual destructor
    virtual ~IThread() = 0 {};

    //! Override this to create and start the thread
    void Start( void *lpArgs = 0 );

    //! Override this to terminate the thread
    void Terminate();

    //! Returns whether the thread is running
    bool IsRunning() const { return m_bIsRunning; }

protected:
    static unsigned int __stdcall ThreadFunc( void *lpArgs )
    {
        IThread *pParent = reinterpret_cast<IThread*>(lpArgs);
        pParent->OnStart();
        pParent->Run( pParent->m_lpArgs );
        pParent->OnEnd();

        return STILL_ACTIVE;
    }

    //! Sets the running flag
    void Resume() { m_bIsRunning = true; }

    //! Unsets the running flag
    void Pause() { m_bIsRunning = false; }

    //! Override this with the main thread function
    virtual void Run( void *lpArgs ) = 0;

    //! Override this to add any initializations before the thread starts.
    virtual void OnStart() {};

    //! Override this to add any clean-up code after the thread exits.
    virtual void OnEnd() {};

}; // end IThread

} // end namespace tlib