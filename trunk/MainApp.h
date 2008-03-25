#pragma once
#pragma comment( lib, "glee" )
#include "MainWindow.h"
#include <vector>

// The world object class
#include "GOCS/CGameObject.h"

namespace tlib {
    namespace gocs {
        class CGameObject;
    }
}
using tlib::gocs::CGameObject;

class MainApp : public gxbase::App 
{
public:
	MainWindow w;
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

private:
    void InitTemplates();
    void InitPlanes();
    void InitCloth();
};