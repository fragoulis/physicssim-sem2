#include "../MainApp.h"
#include "../Network/CPacket.h"
#include "CServerThread.h"

#include <fstream>
using namespace std;

extern ofstream dout;
extern ofstream dout2;

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
        if( m_masks.fd_count==0 ) continue;

        fd_set readMasks = m_masks;

        if( select(0, &readMasks, NULL, NULL, &timeout) < 0 ) {
            dout2 << "Server port error: " << WSAGetLastError() << endl;
			break;
		}

        Streams::iterator iter = m_clients.begin();
        for(int cnt=0; iter != m_clients.end(); cnt++)
        {
            SocketStream *stream = *iter;
            if( !stream->isValid() ) {
                iter = m_clients.erase(iter);
                dout2 << "Removing invalid client[" << cnt << "]" << std::endl;
            }
            else
            {
                if( FD_ISSET(stream->handle(), &readMasks) )
                {
                    // recieve data
                    int ret = packet.recv(*stream);
                    if( ret == 0 ) {
                        dout2 << "Closing recv client[" << cnt << "]" << std::endl;
                        stream->close();
                    }
                    else if( ret == SOCKET_ERROR ) {
                        dout2 << "recieving error " << WSAGetLastError() << " from client[" << cnt << "]" << std::endl;
                    } else {
                        MainApp::Get().GetPhysics().ReadPacket(packet);
                        packet.reset();
                    }
                }
                else
                    dout2 << "Unknown FD client[" << cnt << "]" << std::endl;

                ++iter;
            }

        } // for( .. )

    } // while( .. )

} // Run()