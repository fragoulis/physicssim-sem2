#include "MainApp.h"

static MainApp g_MainApp;

CPhysicsThread MainApp::m_tPhysics;
CBitmapThread MainApp::m_tBitmap;

// ----------------------------------------------------------------------------
MainApp& MainApp::Get() { return g_MainApp; }
