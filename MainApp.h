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
using namespace tlib::gocs;

class MainApp : public gxbase::App 
{
public:
	MainWindow w;

    // Application's threads
    CPhysicsThread m_tPhysics;
    //CBitmapThread m_tBitmap;

    static const int MAX_PLANES = 6;

private:
    typedef std::vector<CGameObject*> ObjectList;

    ObjectList   m_Spheres;
    CGameObject *m_Planes[MAX_PLANES];
    CGameObject *m_Cloth;
    //CGameObject *m_Jelly;

public:
    void OnCreate();
    void OnDestroy();
    
    static MainApp& Get();

    int GetBallCount() const { return (int)m_Spheres.size(); }

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