#include "MainApp.h"
#include "Network/CPacket.h"

static MainApp g_MainApp;

CListeningThread MainApp::m_tListener;
CPhysicsThread MainApp::m_tPhysics;
CBitmapThread MainApp::m_tBitmap;

// ----------------------------------------------------------------------------
MainApp& MainApp::Get() { return g_MainApp; }

// ----------------------------------------------------------------------------
void MainApp::WritePacket( CPacket &packet )
{
    // push camera status
    packet.push<int>( m_tMain.GetState() );
}
