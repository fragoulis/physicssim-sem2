#include "CBitmapThread.h"
#include "../../GX/Image.h"

//-----------------------------------------------------------------------------
void CBitmapThread::Run( void *lpArgs )
{
    while(IsRunning())
    {

        Sleep(10);
    }

} // Run()

// ----------------------------------------------------------------------------
void CBitmapThread::OnStart()
{
}

// ----------------------------------------------------------------------------
void CBitmapThread::OnEnd()
{
}