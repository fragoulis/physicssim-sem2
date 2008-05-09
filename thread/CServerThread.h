#pragma once
#include "IThread.h"
#include <string>
#include <vector>
using namespace tlib;

class CServerThread : public IThread
{
private:
    virtual void Run( void *lpArgs );
};