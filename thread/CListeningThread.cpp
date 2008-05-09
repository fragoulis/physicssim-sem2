#include "CListeningThread.h"
#include "ClientMutex.h"
#include "../Util/Config.h"

#include <fstream>
using namespace std;

ofstream dout2("listenerdump.txt");

// -----------------------------------------------------------------------------
void CListeningThread::OnStart()
{
    CFG_SERVER_OPEN;
    CFG_LOAD("Server");

    int clients;
    CFG_1i("clients", clients);
    m_clients.create(clients);

    CFG_str("hostname", m_hostname);
    CFG_str("port", m_port);
}

// -----------------------------------------------------------------------------
void CListeningThread::Run( void *lpArgs )
{
    WSA wsa;
    SocketAcceptor server( INETAddr(m_hostname.c_str(), m_port.c_str()) );
	server.listen();

    while(IsRunning())
    {
        SocketStream *newStream = m_clients.get();
        server.accept(*newStream);
        
        dout2 << "New client" << std::endl;

        //if( ClientMutex::IsWritable() || ClientMutex::clients.empty() )
        {
            ClientMutex::clients.push_back(newStream);
            //ClientMutex::ReleaseAll();
        }

        dout2 << "Clients=" << ClientMutex::clients.size() << std::endl;
    }

} // Run()