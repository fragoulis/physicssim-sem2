#pragma once
#include "IThread.h"
#include <list>
using namespace tlib;

class SocketStream;
class SocketAcceptor;

class CServerThread : public IThread
{
private:
    typedef std::list<SocketStream*> Streams;
    Streams m_clients;
    fd_set m_masks;
    SocketAcceptor *m_server;

    virtual void Run( void *lpArgs );

public:
    CServerThread():m_server(0) {}
    void addClient(SocketStream *stream) { 
        m_clients.push_back(stream); 
        FD_SET(stream->handle(), &m_masks);
    }
    void setServer(SocketAcceptor *server) { m_server = server; }
};