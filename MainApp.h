#pragma once
#pragma comment( lib, "glee" )
#pragma comment( lib, "Ws2_32.lib" )

#include "Thread/CListeningThread.h"
#include "MainWindow.h"
#include "Thread/CPhysicsThread.h"
#include "Thread/CBitmapThread.h"

class CPacket;

class MainApp : public gxbase::App 
{
private:
    // Application threads
	MainWindow m_tMain;
    static CListeningThread m_tListener;
    static CPhysicsThread m_tPhysics;
    static CBitmapThread m_tBitmap;

    InputMap m_input;

public:
    // Accessors
    static MainApp& Get();
    static CPhysicsThread& GetPhysics() { return m_tPhysics; }
    static CBitmapThread& GetBitmap() { return m_tBitmap; }
    static CListeningThread& GetListener() { return m_tListener; }
    bool GetInput( input_t &data ) const { return m_input.Get( data ); }
    bool ClearInput() { return m_input.Clear(); }
    bool AccumInput(const input_t &data) { return m_input.AccumInput(data); }
    bool SetMButton( bool down ) { return m_input.SetMButton( down ); }
    bool SetKey( int index, bool state ) { return m_input.SetKey( index, state ); }

    void WritePacket( CPacket &packet );
};