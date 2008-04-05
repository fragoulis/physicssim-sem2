#pragma once
#pragma comment( lib, "glee" )
#include "MainWindow.h"
#include "Thread/CShared.h"
#include <vector>

// The world object class
#include "GOCS/CGameObject.h"
#include "Thread/CPhysicsThread.h"

namespace tlib {
    namespace gocs {
        class CGameObject;
    }
}

using namespace tlib;
using tlib::gocs::CGameObject;


class MainApp : public gxbase::App 
{
public:
    static const int MAX_PLANES = 6;

private:
    typedef std::vector<CGameObject*> ObjectList;

    // Application threads
	MainWindow m_tMain;
    static CPhysicsThread m_tPhysics;
    //static CBitmapThread m_tBitmap;
    //static CServerThread m_tServer;
    
    int m_iNumOfBigSpheres;    // Hold sphere stats
    int m_iNumOfSmallSpheres;    

    ObjectList   m_Spheres;
    CGameObject *m_Planes[MAX_PLANES];
    CGameObject *m_Cloth;
    CGameObject *m_Self;
    //CGameObject *m_Jelly;

    //! Controls the simulation
    bool m_bPause;

public:
    void OnCreate();
    void OnDestroy();

    //! Passes the number of spheres
    void GetNumOfSpheres( int &ss, int &bs );

    // Accessors
    static MainApp& Get();
    static CPhysicsThread& GetPhysics() { return m_tPhysics; }
    CGameObject* GetPlane( int index ) { return m_Planes[index]; }
    CGameObject* GetCloth() { return m_Cloth; }

    void SetPause( bool p ) { m_bPause = p; }
    void TogglePause() { m_bPause = !m_bPause; }
    bool IsPaused() const { return m_bPause; }

    // Control functions
    void AddBigSphere();
    void AddSmallSphere();
    void RemoveLastSphere();
    void Reset();

private:
    void InitTemplates();
    void InitPlanes();
    void InitCloth();
};