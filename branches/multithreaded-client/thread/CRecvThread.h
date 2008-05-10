#pragma once
#include "IThread.h"
#include <string>
#include <vector>
using namespace tlib;

class CRecvThread : public IThread
{
private:
    string m_hostname;
    string m_port;

private:
    virtual void Run( void *lpArgs );
    virtual void OnStart();
};