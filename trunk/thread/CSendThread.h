#pragma once
#include "IThread.h"
#include <list>
using namespace tlib;

class SocketStream;

class CSendThread : public IThread
{
private:
    typedef std::list<SocketStream*> Streams;
    Streams m_clients;

    virtual void Run( void *lpArgs );
	virtual void OnEnd();

public:
    void addClient(SocketStream *stream) { m_clients.push_back(stream); }
};