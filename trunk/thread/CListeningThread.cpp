#include "CListeningThread.h"
#include "../Util/Config.h"

#include <fstream>
using namespace std;

ofstream dout("serverdump.txt");

// -----------------------------------------------------------------------------
void CListeningThread::OnStart()
{
    CFG_SERVER_OPEN;
    CFG_LOAD("Server");

    //int clients;
    //CFG_1i("clients", clients);
    //m_clientPool.create(clients);

    CFG_str("hostname", m_hostname);
    CFG_str("sendPort", m_sendPort);
    CFG_str("recvPort", m_recvPort);
}

// -----------------------------------------------------------------------------
void CListeningThread::Run( void *lpArgs )
{
    WSA wsa;

	SocketAcceptor sendServer( INETAddr(m_hostname.c_str(), m_sendPort.c_str()) );
	sendServer.listen();
	SocketAcceptor recvServer( INETAddr(m_hostname.c_str(), m_recvPort.c_str()) );
	recvServer.listen();

    m_recv.setServer(&recvServer);
    m_recv.Start();
    m_send.Start();

    //linger ling;
	//ling.l_onoff = 1;
	//ling.l_linger = 0;
    //setsockopt( sendServer.handle(), SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling) );
    //setsockopt( recvServer.handle(), SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling) );

    fd_set allMasks;
	FD_ZERO (&allMasks);
	FD_SET (sendServer.handle(), &allMasks);
	FD_SET (recvServer.handle(), &allMasks);

    timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    while(IsRunning())
    {
        fd_set readMasks = allMasks;

        // Final parameter of Null causes statement to block
		if( select(0, &readMasks, NULL, NULL, &timeout) < 0 ) {
			dout << "Listener port error: " << WSAGetLastError() << endl;
			break;
		}

		if( FD_ISSET (sendServer.handle(), &readMasks) ) 
        {
			SocketStream *newStream = new SocketStream;
			dout << "SocketSend created" << endl;
            if( sendServer.accept(*newStream) )
			{
				dout << "SocketSend(" << newStream->handle() << ") accepted" << endl;
				// push the stream to the sending list
				m_send.addClient(newStream);
			}
			else
			{
				delete newStream;
				newStream = 0;
			}
		} 
        else if( FD_ISSET (recvServer.handle(), &readMasks) ) 
        {
			SocketStream *newStream = new SocketStream;
			dout << "SocketRecv created" << endl;
            if( recvServer.accept(*newStream) )
			{
				dout << "SocketRecv(" << newStream->handle() << ") accepted" << endl;
				// push the stream to the recieving list
				m_recv.addClient(newStream);
			}
			else
			{
				delete newStream;
				newStream = 0;
			}
		} 
	} // while( )

} // Run()

// -----------------------------------------------------------------------------
void CListeningThread::OnEnd()
{
	dout << __FUNCTION__ << endl;
    m_send.Terminate();
    m_recv.Terminate();
	//Sleep(20000);
}