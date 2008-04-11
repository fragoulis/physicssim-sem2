#include "MainApp.h"

#include "CSceneManager.h"      // We need these to destroy the managers from the
#include "CPhysicsManager.h"    // app instead of the window
#include "GOCS/CGOCManager.h"   // The component manager

#include "goc_includes.h"

#include "Util/Config.h"

using namespace tlib::gocs;
using namespace tlib;

static MainApp g_MainApp;

CPhysicsThread MainApp::m_tPhysics;

// ----------------------------------------------------------------------------
MainApp& MainApp::Get() { return g_MainApp; }

// ----------------------------------------------------------------------------
void MainApp::OnCreate()
{
    MGRScene::_Get().Init();
    MGRPhysics::_Get().Init();
    CGOCManager::_Get();
}

// ----------------------------------------------------------------------------
void MainApp::OnDestroy()
{
    // Wait for threads to terminate
    m_tPhysics.Terminate();

    CGOCManager::Destroy();

    // Destroy managers after all objects are destroyed !!!
    MGRPhysics::Destroy();
    MGRScene::Destroy();
}
