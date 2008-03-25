#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__
#pragma once
#include "../GX/GXBase.h"

#include "Util/CCamera.h"
#include "Util/CLight.h"

using namespace gxbase;
using namespace tlib::util;

class MainWindow : public GLWindowEx 
{
private:
    // Window characteristics
    float m_fFovY, m_fDimRatio, m_fPlanes[2];

    // Window's camera
    CCamera m_Camera;

    // Windorw's light
    static const int MAX_LIGHTS = 1;
    CLight m_Lights[MAX_LIGHTS];

    // Rotating cube controls
    int   m_iMouseX, m_iMouseY;
    bool  m_bIsMouseDown;
    float m_fCubeHorizontalAngle, m_fCubeVerticalAngle,
          m_fAccumX, m_fAccumY;

    // General controls
    bool m_bPause, m_bWireframe;

    //typedef void (*KeyCallback)(MainWindow*);
    //typedef std::map< int, KeyCallback > KeyMap;
    //KeyMap keymap;

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

private:
    void Reset();
    void RotateCube( float delta );
    void CheckRotationAngles();
    void SetupView() const;
    void RenderHelpGrid( int lines, float size ) const;
    void PrintStats();
};

#endif // __MAIN_WINDOW_H__