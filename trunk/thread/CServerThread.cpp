#include "../MainApp.h"
#include "../Network/CPacket.h"
#include "CServerThread.h"
#include "ClientMutex.h"

#include <fstream>
using namespace std;

ofstream dout("serverdump.txt");
extern ofstream dout2;

// -----------------------------------------------------------------------------
void CServerThread::Run( void *lpArgs )
{
    CPacket packet;

    while(IsRunning())
    {
        if( !ClientMutex::clients.empty() )
        {
            MainApp::Get().WritePacket(packet);
            MainApp::Get().GetPhysics().WritePacket(packet);

            //if( ClientMutex::IsWritable() )
            {
                Streams::iterator iter = ClientMutex::clients.begin();
                for(; iter != ClientMutex::clients.end(); )
                {
                    SocketStream *stream = *iter;
	                if( !stream->isValid() )
                    {
                        ClientMutex::clients.erase(iter);
                        dout2 << "Removing invalid client" << std::endl;
                    }
                    else
                    {
                        ++iter;
                    
                        // send data
                        stream->send(packet.buffer(), packet.size());
                    }
                }

                // clear buffer
                packet.reset();

                //ClientMutex::ReleaseAll();

            } // if ( )

        } // if( !.. )

        // sleep for a while
        Sleep(10);
    }

} // Run()