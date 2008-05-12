#include "../Network/WSA.h"
#include "../Network/CPacket.h"
#include "CRecvThread.h"
#include "../MainApp.h"
#include "../Util/Config.h"
#include "../Util/CLogger.h"

#include <fstream>
using namespace std;

// ----------------------------------------------------------------------------
void CRecvThread::OnStart()
{
    CFG_SERVER_OPEN;
    CFG_LOAD("Server");

    CFG_str("hostname", m_hostname);
    CFG_str("recvPort", m_port);
}

// ----------------------------------------------------------------------------
void CRecvThread::Run( void *lpArgs )
{
    WSA wsa;
    CPacket packet;
    
    bool isOpen = false;
    INETAddr connection = INETAddr(m_hostname.c_str(), m_port.c_str());
    SocketStream client;
    
    timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    fd_set readMasks;

	while(IsRunning())
    {
		if( isOpen )
		{
			FD_ZERO (&readMasks);
			FD_SET (client.handle(), &readMasks);

			int ret = select(0, &readMasks, NULL, NULL, &timeout);
			if( ret == SOCKET_ERROR )
			{
				int err;
				int errlen = sizeof(err);
				getsockopt(client.handle(), SOL_SOCKET, SO_ERROR, (char*)&err, &errlen);
				cout << "Socket error: " << err << endl;
				client.close();
				isOpen = false;
			}
		}

		if( !isOpen ) 
        {
            // connect to server
            isOpen = client.open(connection);
			if(isOpen) {
				_LOG("Trying to connect... connected");
			} else {
				_LOG("Trying to connect... failed");
			}
        }
        else if( FD_ISSET(client.handle(), &readMasks) )
        {
            // read what is in the socket
			packet.reset();
			int bytes = packet.recv(client);
			if( bytes <= 0 ) {
				_LOG("Error: recieving packet from server to client");
				client.close();
				isOpen = false;
			}
			else {
				if( packet.realSize() == bytes ) {
					MainApp::Get().ReadPacket(packet);
				}
				else
					_LOG("Error: packet from server was corrupted");
            }
        }

		Sleep(10);

    } // while( )

} // Run()