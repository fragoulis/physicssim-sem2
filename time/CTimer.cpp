#include "CTimer.h"
#include "Clock.h"
using namespace tlib::time;

CTimer::CTimer():
m_dDuration(0.0),
m_dElapsedTime(0.0),
m_bIsRunning(false),
m_fScale(1.0f)
{
    Clock::Instance().AddTimer(this);
}

CTimer::CTimer(int i){}