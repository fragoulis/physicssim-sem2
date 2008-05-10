#include "../MainApp.h"
#include "../Network/CPacket.h"
#include "CSendThread.h"

#include <fstream>
using namespace std;

extern ofstream dout;
extern ofstream dout2;
extern ofstream dout3;

// -----------------------------------------------------------------------------
void CSendThread::Run( void *lpArgs )
{
    CPacket packet;

    while(IsRunning())
    {
        if( !m_clients.empty() )
        {
            MainApp::Get().WritePacket(packet);
            MainApp::Get().GetPhysics().WritePacket(packet);

            Streams::iterator iter = m_clients.begin();
            for(int cnt=0; iter != m_clients.end(); cnt++)
            {
                SocketStream *stream = *iter;
                if( !stream->isValid() ) {
                    iter = m_clients.erase(iter);
                }
                else
                {
                    // send data
                    int ret = stream->send(packet.buffer(), packet.size());
                    if( ret == 0 ) {
                        dout3 << "sent ZERO to server" << std::endl;
                    }
                    else if( ret == SOCKET_ERROR ) {
                        int error = WSAGetLastError();
                        if( WSAECONNABORTED == error ) {
                            stream->close();
                            dout3 << "closing send client[" << cnt << "]" << std::endl;
                        }
                        else
                            dout3 << "sent error("<< error <<") on send client[" << cnt << "]" << std::endl;
                    }

                    ++iter;
                }

            } // for( .. )

            // clear buffer
            packet.reset();

        } // if( !.. )

        // sleep for a while
        Sleep(25);

    } // while( .. )

} // Run()