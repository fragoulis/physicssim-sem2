#pragma once
#include "IThread.h"
using namespace tlib;

class CBitmapThread : public IThread
{
protected:
    virtual void Run( void *lpArgs );

private:
    virtual void OnStart();
    virtual void OnEnd();
};