#pragma once
#pragma comment( lib, "glee" )
#pragma comment( lib, "Ws2_32.lib" )
#include "MainWindow.h"
#include "Thread/CBitmapThread.h"
#include "Thread/CSendThread.h"
#include "Thread/CRecvThread.h"
#include "InputMap.h"

#include "GOCS/CGameObject.h"    // The world object class
#include <vector>
#include <iostream>

using namespace tlib;
using namespace tlib::gocs;

class CPacket;

class MainApp : public gxbase::App 
{
private:
    //! The number of the cube walls :P
    static const int MAX_PLANES = 6;

    // Application threads
	MainWindow m_tMain;
    static CBitmapThread m_tBitmap;
    static CSendThread m_tClientSend;
    static CRecvThread m_tClientRecv;

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

    InputMap m_input;

public:
    // Accessors
    static MainApp& Get();
    static CBitmapThread& GetBitmap() { return m_tBitmap; }
    static CSendThread& GetClientSend() { return m_tClientSend; }
    static CRecvThread& GetClientRecv() { return m_tClientRecv; }
    bool GetInput( input_t &data ) const { return m_input.Get( data ); }
    bool ClearInput() { return m_input.Clear(); }
    bool SetMButton( bool down ) { return m_input.SetMButton( down ); }
    bool SetKey( int index, bool state ) { return m_input.SetKey( index, state ); }

    CGameObject* GetPlane(int index) { return m_Planes[index]; }
    CGameObject* GetLastSphere() { return m_LastSphere; }

    void GetNumOfSpheres( int &ss, int &bs ) {
        ss = m_iNumOfSmallSpheres;
        bs = m_iNumOfBigSpheres;
    }

    void OnCreate();
    void OnDestroy();
    void ClearSpheres();

    void InitTemplates();
    void InitPlanes();
    void InitCloth();
    void InitShelf();

    // Control functions
    void AddBigSphere();
    void AddSmallSphere();
    void ResetCube();
    void Reset();

    void ReadPacket( CPacket &packet );
    void WritePacket( CPacket &packet );
};