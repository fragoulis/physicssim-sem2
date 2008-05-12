#include "../Network/WSA.h"
#include "../Network/CPacket.h"
#include "CSendThread.h"
#include "../MainApp.h"
#include "../Util/Config.h"
#include "../Util/CLogger.h"

#include <fstream>
using namespace std;

ofstream dout("clientdump.txt");

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

	while(IsRunning())
    {
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
        
        if( isOpen )
        {
            // send data
			packet.reset();
            MainApp::Get().WritePacket(packet);
			if( packet.send(client, true) < 0 ) {
				_LOG("Error: sending packet from client to server");
				client.close();
				isOpen = false;
            }
        }

        //Sleep(10);
    }

} // Run()