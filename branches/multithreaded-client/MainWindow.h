#pragma once
#include "../../GX/GXBase.h"
#include "Util/CCamera.h"
#include "Util/CLight.h"
#include "Math/TQuaternion.h"
using tlib::math::Quatf;

#include <sstream>
#include <string>
using namespace std;

using namespace gxbase;
using namespace tlib::util;

class MainWindow : public GLWindowEx 
{
private:
    // Window characteristics
    float m_fFovY, m_fDimRatio, m_fPlanes[2];

    // Window's camera
    CCamera m_FrontCamera, m_BallCamera, *m_ActiveCam;
    bool m_bBallCamActive;

    // Windorw's light
    static const int MAX_LIGHTS = 1;
    CLight m_Lights[MAX_LIGHTS];

    // Rotating cube controls
    int m_iMouseX, m_iMouseY;
    float m_fAccumX, m_fAccumY;
    bool m_bIsMouseDown;
    float m_fCubeHorizontalAngle, m_fCubeVerticalAngle;

    // General controls
    bool m_bWireframe;
    bool m_bTextured;
    bool m_bShowControls;
    
    // Application states
    enum { AS_NORMAL = 0, AS_RECORD, AS_REPLAY };
    int m_AppState;

public:
	MainWindow();
	void OnCreate();
	void OnDisplay();
    void OnIdle();
	void OnResize( int w, int h );
    void OnDestroy();
	void OnKeyboard( int key, bool down );
    void OnMouseButton( MouseButton button, bool down );
    void OnMouseMove( int x, int y );

    void SetState( int s ) { m_AppState = s; }
    float GetAngleX() const { return m_fCubeHorizontalAngle; }
    float GetAngleY() const { return m_fCubeVerticalAngle; }
    
private:
    void SetupView() const;
    void RenderHelpGrid( int lines, float size ) const;
    void PrintStats();
    void UpdateActiveCamera();
};