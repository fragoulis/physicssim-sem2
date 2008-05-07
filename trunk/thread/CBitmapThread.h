#pragma once
#include "IThread.h"
#include <string>
#include <vector>
using namespace tlib;

class CBitmapThread : public IThread
{
private:
    typedef std::vector<std::string> StringList;
    StringList m_images;
    int m_sleep, m_time, m_changeInterval;

private:
    virtual void Run( void *lpArgs );
    virtual void OnStart();
};