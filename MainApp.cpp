#include "MainApp.h"

static MainApp g_MainApp;

CPhysicsThread MainApp::m_tPhysics;

// ----------------------------------------------------------------------------
MainApp& MainApp::Get() { return g_MainApp; }

// ----------------------------------------------------------------------------
void MainApp::OnCreate()
{   
}

// ----------------------------------------------------------------------------
void MainApp::OnDestroy()
{
}
