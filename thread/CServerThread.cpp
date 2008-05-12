#include "../MainApp.h"
#include "../Network/CPacket.h"
#include "CServerThread.h"
#include "../Util/CLogger.h"

#include <fstream>
using namespace std;

extern ofstream dout;

// -----------------------------------------------------------------------------
void CServerThread::Run( void *lpArgs )
{
    CPacket packet;

    FD_ZERO (&m_masks);
    
    timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    while(IsRunning())
    {

		if( !m_clients.empty() )
        {
			fd_set readMasks = m_masks;

			int sel = select(0, &readMasks, NULL, NULL, &timeout);
			if( sel < 0 ) {
				dout << "Server port error: " << WSAGetLastError() << endl;
			}

			Streams::iterator iter = m_clients.begin();
			while(iter != m_clients.end())
			{
				SocketStream *stream = *iter;
				if( !stream->isValid() ) {
					dout << "SocketRecv deleted" << endl;
					delete stream; stream = 0;
					iter = m_clients.erase(iter);
				}
				else
				{
					if( FD_ISSET(stream->handle(), &readMasks) )
					{
						// recieve data
						packet.reset();
						int bytes = packet.recv(*stream);
						if( bytes <= 0 ) {
							dout << "SocketRecv(" << stream->handle() << ") closing due to error" << endl;
							stream->close();
							//FD_CLR(stream->handle(), &m_masks);
						}
						else {
							if( packet.realSize() == bytes ) {
								MainApp::Get().GetPhysics().ReadPacket(packet);
							}
							else
								_LOG("Error: packet from client was corrupted");
						}
					}

					++iter;
				}

			} // for( .. )

		} // if( client )...

		Sleep(1);

    } // while( .. )

} // Run()


// -----------------------------------------------------------------------------
void CServerThread::OnEnd()
{
	dout << __FUNCTION__ << endl;
	Streams::iterator iter = m_clients.begin();
    for(; iter != m_clients.end(); ++iter )
    {
		dout << "SocketRecv(" << (*iter)->handle() << ") deleted" << endl;
		delete *iter;
		*iter = 0;
	}
	m_clients.clear();
}