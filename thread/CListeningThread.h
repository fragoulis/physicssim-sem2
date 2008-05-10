#pragma once
#include "../Network/WSA.h"
#include "../Network/SocketAcceptor.h"
#include "CServerThread.h"
#include "CSendThread.h"
#include "../Util/TFreelist.h"
using namespace tlib;
using namespace std;

class CListeningThread : public IThread
{
private:
    typedef TFreelist<SocketStream> Sockets;
    Sockets m_clientPool;
    string m_hostname;
    string m_sendPort, m_recvPort;

    CServerThread m_recv;
    CSendThread m_send;

private:
    virtual void Run( void *lpArgs );
    virtual void OnStart();
    virtual void OnEnd();
};