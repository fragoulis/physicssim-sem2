#pragma once

namespace tlib 
{

/**
 * @class IThread
 * Portable thread class interface.
 * This interface is used for deriving platform specific thread classes.
 */
class IThread
{
private:
    //! Flag for whether the thread is running
    bool m_bIsRunning;

public:
    //! Constructor
    IThread():m_bIsRunning(false) {}

    //! Virtual destructor
    virtual ~IThread() {}

    //! Override this to create and start the thread
    virtual void Start( void *lpArgs = 0 ) = 0;

    //! Override this to terminate the thread
    virtual void Terminate() = 0;

    //! Returns whether the thread is running
    bool IsRunning() const { return m_bIsRunning; }

protected:
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