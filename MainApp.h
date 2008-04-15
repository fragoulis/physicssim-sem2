#pragma once
#pragma comment( lib, "glee" )
#include "MainWindow.h"
#include "Thread/CPhysicsThread.h"
#include "Thread/CBitmapThread.h"
#include <vector>

class MainApp : public gxbase::App 
{
private:
    // Application threads
	MainWindow m_tMain;
    static CPhysicsThread m_tPhysics;
    static CBitmapThread m_tBitmap;
    //static CServerThread m_tServer;

    InputMap m_input;

public:
    // Accessors
    static MainApp& Get();
    static CPhysicsThread& GetPhysics() { return m_tPhysics; }
    static CBitmapThread& GetBitmap() { return m_tBitmap; }
    bool GetInput( input_t &data ) const { return m_input.Get( data ); }
    bool ClearInput() { return m_input.Clear(); }
    bool SetMButton( bool down ) { return m_input.SetMButton( down ); }
    bool SetKey( int index, bool state ) { return m_input.SetKey( index, state ); }
};