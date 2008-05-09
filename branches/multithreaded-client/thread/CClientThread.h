#pragma once
#pragma comment( lib, "Ws2_32.lib" )
#include "IThread.h"
#include <string>
#include <vector>
using namespace tlib;

class CClientThread : public IThread
{
private:
    string m_hostname;
    string m_port;

private:
    virtual void Run( void *lpArgs );
    virtual void OnStart();
};