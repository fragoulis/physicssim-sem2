#include "Clock.h"
#include <cassert>
#include <ctime>
#include "CTimer.h"
using namespace tlib::time;

extern const double M_1_CLOCKS_PER_SEC = 1.0 / CLOCKS_PER_SEC;

const double Clock::MAX_DELTA_TIME = 0.016;

// ----------------------------------------------------------------------------
Clock::Clock():
m_dCurrentTime(0.0),
m_dTimeAccum(0.0),
m_dPreviousTime(0.0),
m_dTimeDelta(0.0),
m_lStart(0),
m_TimeSrc(0)
{
    // Zero all values
    for( int i=0; i<FRAMES_SAVED; ++i ) {
        m_vdFrameTimes[i] = 0.0;
    }
}

// ----------------------------------------------------------------------------
Clock::~Clock()
{
    // Detele time source
    delete m_TimeSrc;
    m_TimeSrc = 0;

    MGRTimeSrc::Destroy();
}

// ----------------------------------------------------------------------------
void Clock::FrameStep()
{
    assert(m_TimeSrc!=0);

    m_lNow = m_TimeSrc->get();

	if( !m_lNow ) return;

    // Update current time
    m_dPreviousTime = m_dCurrentTime;
    m_dCurrentTime = ( m_lNow - m_lStart ) * M_1_CLOCKS_PER_SEC;

    // Compute average time delta
    UpdateDeltaTime();

    // Update application timers
    TimerArray::const_iterator i = m_vTimers.begin();
    for( ; i!=m_vTimers.end(); ++i ) 
    {
        CTimer *timer = *i;
        if( timer->isRunning() )
            timer->update( m_dTimeDelta );
    }
}

// ----------------------------------------------------------------------------
void Clock::Start( MGRTimeSrc::TimeSourceType iType )
{
    // If clock is already running, delete the old time source and 
    // restart operation using the new one
    if( m_TimeSrc ) delete m_TimeSrc;
    m_TimeSrc = MGRTimeSrc::_Get().Get(iType);

    // Save current time
    m_lStart = m_TimeSrc->get();
}

// ----------------------------------------------------------------------------
void Clock::UpdateDeltaTime()
{
    // Push every value up one index
    for( int i=FRAMES_SAVED-1; i>0; --i )
        m_vdFrameTimes[i] = m_vdFrameTimes[i-1];

    // Save the last time delta
    m_vdFrameTimes[0] = m_dCurrentTime - m_dPreviousTime;

    // Calculate the average of the N previous time deltas
    double dSum = 0.0;
    //for( int i=0; i<FRAMES_SAVED; ++i ) {
    //    dSum += m_vdFrameTimes[i];
    //}
    for( int i=0; i<FRAMES_SAVED; dSum += m_vdFrameTimes[i++] );

    m_dTimeDelta = dSum / FRAMES_SAVED;
    m_dTimeAccum += m_dTimeDelta;

    if( m_dTimeDelta > MAX_DELTA_TIME )
        m_dTimeDelta = MAX_DELTA_TIME;
}