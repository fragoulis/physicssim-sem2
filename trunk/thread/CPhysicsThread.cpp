#include "CPhysicsThread.h"
#include "../MainApp.h"
#include "../CPhysicsManager.h"
#include "../Time/Clock.h"
#include "../Util/CLogger.h"

using namespace tlib;
using namespace tlib::math;
using namespace tlib::time;

void CPhysicsThread::Run( void *lpArgs )
{
    int i = 0;
    while(IsRunning())
    {
        //Clock::Get().FrameStep();
        //float delta = (float)Clock::Get().GetTimeDelta();

        //if( !MainApp::Get().IsPaused() )
        //{
        //    //MGRPhysics::Get().Update( delta );
        //}
    }
}