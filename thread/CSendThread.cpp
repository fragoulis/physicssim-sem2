#include "../MainApp.h"
#include "../Network/CPacket.h"
#include "CSendThread.h"
#include "../Util/CLogger.h"

#include <fstream>
using namespace std;

extern ofstream dout;

// -----------------------------------------------------------------------------
void CSendThread::Run( void *lpArgs )
{
    CPacket packet;

	while(IsRunning())
    {
        if( !m_clients.empty() )
        {
            packet.reset();
            MainApp::Get().WritePacket(packet);
            MainApp::Get().GetPhysics().WritePacket(packet);
			packet.finalize();

            Streams::iterator iter = m_clients.begin();
            while(iter != m_clients.end())
            {
                SocketStream *stream = *iter;
                if( !stream->isValid() ) {
					dout << "SocketSend deleted" << endl;
					delete stream; stream = 0;
                    iter = m_clients.erase(iter);
                }
                else
                {   
					if( packet.send(*stream) < 0 ) {
						dout << "SocketSend(" << stream->handle() << ") closing due to error" << endl;
						stream->close();
                    }

                    ++iter;
                }

            } // for( .. )

        } // if( !.. )

        // sleep for a while
        Sleep(10);

    } // while( .. )

} // Run()

// -----------------------------------------------------------------------------
void CSendThread::OnEnd()
{
	dout << __FUNCTION__ << endl;
	Streams::iterator iter = m_clients.begin();
    for(; iter != m_clients.end(); ++iter ) 
	{
		dout << "SocketSend(" << (*iter)->handle() << ") deleted" << endl;
		delete *iter;
		*iter = 0;
	}
	m_clients.clear();
}