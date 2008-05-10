#include "../Network/WSA.h"
#include "../Network/CPacket.h"
#include "CSendThread.h"
#include "../MainApp.h"
#include "../Util/Config.h"

#include <fstream>
using namespace std;

ofstream dout("clientdump.txt");
ofstream dout3("senddump.txt");

// ----------------------------------------------------------------------------
void CSendThread::OnStart()
{
    CFG_SERVER_OPEN;
    CFG_LOAD("Server");

    CFG_str("hostname", m_hostname);
    CFG_str("sendPort", m_port);
}

// ----------------------------------------------------------------------------
void CSendThread::Run( void *lpArgs )
{
	WSA wsa;
    CPacket packet;
    
    bool isOpen = false;
    INETAddr connection = INETAddr(m_hostname.c_str(), m_port.c_str());
    SocketStream client;
    
    timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

	while(IsRunning())
    {
        if( !isOpen ) 
        {
            // connect to server
            dout3 << "Trying to connect... ";
            isOpen = client.open(connection);
            dout3 << (isOpen?"established":"failed: ");
            if( !isOpen ) dout3 << WSAGetLastError() << endl;
            else dout3 << endl;
        }
        
        if( isOpen )
        {
            // send data
            MainApp::Get().WritePacket(packet);
            int ret = client.send(packet.buffer(), packet.size());
            if( ret == 0 ) {
                dout3 << "sent ZERO to server" << std::endl;
            }
            else if( ret == SOCKET_ERROR ) {
                dout3 << "sent error " << WSAGetLastError() << " to server" << std::endl;
                //client.close();
                isOpen = false;
            }
            packet.reset();
        }

        Sleep(25);
    }

} // Run()