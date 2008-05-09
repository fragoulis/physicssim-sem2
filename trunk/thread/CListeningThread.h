#pragma once
#include "../Network/WSA.h"
#include "../Network/SocketAcceptor.h"
#include "IThread.h"
#include "../Util/TFreelist.h"
#include <string>
#include <vector>
using namespace tlib;
using namespace std;

class CListeningThread : public IThread
{
private:
    typedef TFreelist<SocketStream> Sockets;
    Sockets m_clients;
    string m_hostname;
    string m_port;

private:
    virtual void Run( void *lpArgs );
    virtual void OnStart();
};