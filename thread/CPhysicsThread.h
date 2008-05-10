#pragma once
#include "IThread.h"
#include "CubeControls.h"
#include "../InputMap.h"
#include "../GOCS/CGameObject.h"    // The world object class
#include "../GOCS/CGOCManager.h"    // The component manager
#include <vector>
#include <iostream>

using namespace tlib;
using namespace tlib::gocs;

class CPacket;

class CPhysicsThread : public IThread
{
protected:
    virtual void Run( void *lpArgs );

private:
    //! The number of the cube walls :P
    static const int MAX_PLANES = 6;

    typedef std::vector<CGameObject*> ObjectList;
    
    // Hold sphere stats
    int m_iNumOfBigSpheres;
    int m_iNumOfSmallSpheres;    

    // The game objects
    ObjectList   m_Spheres;
    CGameObject *m_LastSphere;
    CGameObject *m_Planes[MAX_PLANES];
    CGameObject *m_Cloth;
    CGameObject *m_Shelf;
    //CGameObject *m_Jelly;

    bool m_bPause;
    bool m_bReset;
    bool m_bRestartClockFromFile;
    bool m_bRestartClock;
    CubeControls m_cube;

public:
    CPhysicsThread();

    // Pause controls
    bool IsPaused() const { return m_bPause; }
    void TogglePause() { m_bPause = !m_bPause; }
    void SetReset() { m_bReset = true; }

    // Record controls
    void RestartClockFromFile() { m_bRestartClockFromFile = true; }
    void RestartClock() { m_bRestartClock = true; }

    void SetCubeAngles( float horiz, float verti ) {
        m_cube.angles.Set( horiz, verti );
    }

    //! Passes the number of spheres
    void GetNumOfSpheres( int &ss, int &bs ) {
        ss = m_iNumOfSmallSpheres;
        bs = m_iNumOfBigSpheres;
    }

    //! Returns the transformation of last sphere
    CGameObject* GetLastSphere() { return m_LastSphere; }
    CGameObject* GetPlane(int index) { return m_Planes[index]; }

    void WritePacket( CPacket &packet );
    void ReadPacket( CPacket &packet );

private:
    virtual void OnStart();
    virtual void OnEnd();

    // Input controller
    void HandleInput();

    // Initialization functions
    void InitTemplates();
    void InitPlanes();
    void InitCloth();
    void InitShelf();

    // Control functions
    void AddBigSphere();
    void AddSmallSphere();
    void RemoveLastSphere();
    void ToggleClothShelf();
    void ResetCube();
    void Reset();

    // Apply rotations
    void RotateCube( float delta );
    void RotateCube( const Quatf &qCirRot );
    void RotateCloth( const Quatf& );
    void RotateShelf( const Quatf& );
    void RotatePlanes( const Quatf& );
};