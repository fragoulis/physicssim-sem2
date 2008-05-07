#pragma once
#include <vector>
#include "../Singleton.h"
#include "ITimeSource.h"
#include "CTimeSourceManager.h"

namespace tlib
{
namespace time
{

class CTimer;

/**
 * @class Clock
 *
 */
class Clock : public Singleton<Clock>
{
private:
    //! The clock's time source
    ITimeSource *m_TimeSrc;
    
    //! When the timer started
    feed_t m_lStart;

    //! The curent feed
    feed_t m_lNow;

    //! The current time is the time passed since the start
    //! of the application (in seconds)
    double m_dCurrentTime;

    //! The current time as additions of the delta times
    double m_dTimeAccum;

    //! The previous time
    double m_dPreviousTime;

    //! Number of frames saved to compute the average time delta
    static const int FRAMES_SAVED = 5;

    //! This array holds the times of the last N feeds, giving 
    //! us the opportunity to calculate a more accurate delta time.
    //! The first item in the array is the last time step occured 
    //! [the last time delta].
    double m_vdFrameTimes[FRAMES_SAVED];

    //! The average time delta
    double m_dTimeDelta;

    //! The maximum delta time.
    //! Used for when we use the debugger adn then return and delta time 
    //! is up the roof.
    static const double MAX_DELTA_TIME;

    //! An array of child timers
    typedef std::vector<CTimer*> TimerArray;
    TimerArray m_vTimers;

public:
    // Accessors
    feed_t GetCurrentFeed() const   { return m_lNow; }
    double GetCurrentTime() const   { return m_dCurrentTime; }
    double GetTimeDelta() const     { return m_dTimeDelta; }
    double GetLastTimeDelta() const { return m_vdFrameTimes[0]; }
    double GetTimeAccum() const     { return m_dTimeAccum; }

    //! This function updates the current time, the time detla
    //! and all associated timers
    void FrameStep();

    //! Initiates the clock. We must pass a time source object
    //! to associate with the clock.
    void Start( MGRTimeSrc::TimeSourceType iType );

    //! Adds a timer to the clocks awareness list
    void AddTimer( CTimer *pTimer ) { m_vTimers.push_back(pTimer); }

private:
    friend Singleton<Clock>;
    //! Constructor
    Clock();

    //! Destructor
    ~Clock();

    //! Calculates the average time delta
    void UpdateDeltaTime();

}; // end Clock

} // end namespace time
} // end namespace tlib