#pragma once
#pragma comment( lib, "glee" )
#include "MainWindow.h"
#include <vector>

// The world object class
#include "Thread/CPhysicsThread.h"

using namespace tlib;

class MainApp : public gxbase::App 
{
private:
    // Application threads
	MainWindow m_tMain;
    static CPhysicsThread m_tPhysics;
    //static CBitmapThread m_tBitmap;
    //static CServerThread m_tServer;

    InputMap m_input;

public:
    void OnCreate();
    void OnDestroy();

    // Accessors
    static MainApp& Get();
    static CPhysicsThread& GetPhysics() { return m_tPhysics; }
    bool GetInput( input_t &data ) { return m_input.Get( data ); }
    bool SetMButton( bool down ) { return m_input.SetMButton( down ); }
    bool SetKey( int index, bool state ) { return m_input.SetKey( index, state ); }
};