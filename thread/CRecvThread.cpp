#include "../Network/WSA.h"
#include "../Network/CPacket.h"
#include "CRecvThread.h"
#include "../MainApp.h"
#include "../Util/Config.h"
#include "../Util/assert.h"

#include <fstream>
using namespace std;

ofstream dout2("recvdump.txt");

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
        FD_ZERO (&readMasks);
	    FD_SET (client.handle(), &readMasks);

        int ret = select(0, &readMasks, NULL, NULL, &timeout);
        if( ret == SOCKET_ERROR )
        {
            dout2 << "Socket error: " << WSAGetLastError() << endl;
            isOpen = false;
        }

        if( !isOpen ) 
        {
            // connect to server
            dout2 << "Trying to connect... ";
            isOpen = client.open(connection);
            dout2 << (isOpen?"established":"failed");
            if( !isOpen ) dout2 << WSAGetLastError() << endl;
            else dout2 << endl;
        }
        else if( FD_ISSET(client.handle(), &readMasks) )
        {
            // read what is in the socket
            int ret = packet.recv(client);
            if( ret == 0 ) {
                dout2 << "recieving ZERO from server" << std::endl;
            }
            else if( ret == SOCKET_ERROR ) {
                dout2 << "recieving error " << WSAGetLastError() << " from server" << std::endl;
                //client.close();
                isOpen = false;
            }
            else {
                MainApp::Get().ReadPacket(packet);
                packet.reset();
            }
        }
        else
            dout2 << "Unknown FD. isOpen=" << isOpen << endl;
    }

} // Run()