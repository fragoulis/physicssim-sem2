#pragma once
#pragma comment( lib, "glee" )
#include "MainWindow.h"
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
    //! Returns the static instance of the main app
    static MainApp& Get();

    // Application threads
	MainWindow m_tMain;
    CPhysicsThread m_tPhysics;
    //CBitmapThread m_tBitmap;
    //CServerThread m_tServer;

    static const int MAX_PLANES = 6;

private:
    typedef std::vector<CGameObject*> ObjectList;
    
    int m_iNumOfBigSpheres;    // Hold sphere stats
    int m_iNumOfSmallSpheres;    

    ObjectList   m_Spheres;
    CGameObject *m_Planes[MAX_PLANES];
    CGameObject *m_Cloth;
    CGameObject *m_Self;
    //CGameObject *m_Jelly;

public:
    void OnCreate();
    void OnDestroy();

    void GetNumOfSpheres( int &ss, int &bs ) { 
        ss = m_iNumOfSmallSpheres;
        bs = m_iNumOfBigSpheres;
    }

    CGameObject* GetPlane( int index ) { return m_Planes[index]; }
    CGameObject* GetCloth() { return m_Cloth; }

    void AddBigSphere();
    void AddSmallSphere();
    void RemoveLastSphere();
    void Reset();

private:
    void InitTemplates();
    void InitPlanes();
    void InitCloth();
};
