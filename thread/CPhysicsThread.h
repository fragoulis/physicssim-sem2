#pragma once
#include "IThread.h"

namespace tlib
{

class CPhysicsThread : public IThread
{
protected:
    virtual void Run( void *lpArgs );
};

} // end namespace tlib