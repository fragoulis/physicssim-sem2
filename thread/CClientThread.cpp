#include "../Network/WSA.h"
#include "../Network/CPacket.h"
#include "CClientThread.h"
#include "../MainApp.h"
#include "../Util/Config.h"

#include <fstream>
using namespace std;

ofstream dout("clientdump.txt");

// ----------------------------------------------------------------------------
void CClientThread::OnStart()
{
    CFG_SERVER_OPEN;
    CFG_LOAD("Server");

    CFG_str("hostname", m_hostname);
    CFG_str("port", m_port);
}

// ----------------------------------------------------------------------------
void CClientThread::Run( void *lpArgs )
{
	WSA wsa;
    CPacket packet;
    SocketStream client(INETAddr(m_hostname.c_str(), m_port.c_str()));  // random port
	//SocketStream client(INETAddr(hostName, portNumber), 2102 ); // be careful, this may given timeout errors

	while( client.isValid() )
    {
        packet.recv(client);

        if( 0 == packet.realSize() ) continue;

        MainApp::Get().ReadPacket(packet);
        packet.reset();
    }

    dout << "Client ended!" << std::endl;

} // Run()