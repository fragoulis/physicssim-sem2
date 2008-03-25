#include "MainWindow.h"
#include "MainApp.h"
#include "CSceneManager.h"
#include "CPhysicsManager.h"
#include "Time/Clock.h"
#include "Math/Random.h"

#include "GOCS/CGameObject.h"       // TODO: This dependecies exist because of the lack
#include "GOCS/GOCBoundingPlane.h"  //       of an inter-component event system.
#include "GOCS/GOCBoundingDWBox.h"
#include "GOCS/GOCBoundingBox.h"
#include "GOCS/GOCPhysicsCloth.h"

#include "Util/CLogger.h"

using namespace tlib;
using namespace tlib::math;
using namespace tlib::time;
using namespace tlib::gocs;

#include <iostream>
using namespace std;

const float DEG_TO_RAD = (float)M_PI / 180.0f;
const float ROTATION_STEP = 0.025f;
const float ROTATION_SPEED = 350.0f;

//-----------------------------------------------------------------------------
MainWindow::MainWindow():
m_fCubeHorizontalAngle(0.0f),
m_fCubeVerticalAngle(0.0f),
m_fAccumX(0.0f),
m_fAccumY(0.0f),
m_iMouseX(0),
m_iMouseY(0),
m_bPause(0),
m_bWireframe(0)
{
    // Initialize window
    SetSize( 768, 768 );

    //SetFullscreen(true);
    m_fFovY      = 80.0f;
    m_fDimRatio  = float(Width()) / float(Height());
    m_fPlanes[0] = 0.1f;
    m_fPlanes[1] = 1000.0f;

}

//-----------------------------------------------------------------------------
void MainWindow::OnCreate()
{
    GLWindowEx::OnCreate();
    _CLEAR_LOG

    glClearColor( 0.25f, 0.25f, 0.25f, 1.0f );
    glEnable        (GL_CULL_FACE);
    glEnable        (GL_DEPTH_TEST);
    glShadeModel    (GL_SMOOTH);

    MGRScene::_Instance().Init();
    MGRPhysics::_Instance().Init();
    Clock::_Instance().Start( MGRTimeSrc::SRC_CLOCK );

    // Initialize camera's position and direction
    //m_Camera.SetPosition( Vec3f( 10.0f, 10.0f, -10.0f ) );
    m_Camera.SetPosition( Vec3f( 0.0f, 0.25f, 1.25f ) );
    m_Camera.LookAt( Vec3f( 0.0f, 0.0f, 0.0f ) );

    // Setup lighting
#define LIGHT( index, vec ) \
    m_Lights[index].SetId(index); \
    m_Lights[index].SetPosition( vec ); \
    m_Lights[index++].TurnOn();

    int i=0;
    LIGHT( i, Vec3f( 0.0f, 0.0f, 0.4f ) );
    //LIGHT( i, Vec3f(-0.4f, 0.0f, 0.0f ) );
    //LIGHT( i, Vec3f( 0.0f, 0.4f, 0.0f ) );
    //LIGHT( i, Vec3f( 0.0f,-0.4f, 0.0f ) );
    //LIGHT( i, Vec3f( 0.0f, 0.0f, 0.4f ) );
    //LIGHT( i, Vec3f( 0.0f, 0.0f,-0.4f ) );
}

//-----------------------------------------------------------------------------
void MainWindow::OnDisplay()
{
    Clock::Instance().FrameStep();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    SetupView();
    m_Camera.Apply();
    m_Lights[0].Apply();

    //RenderHelpGrid( 40, 0.5f );
    MGRScene::Instance().Render();
    PrintStats();

    CGameObject *cloth = MainApp::Get().GetCloth();
    GOCBoundingDWBox *bnd = GET_OBJ_GOC( cloth, GOCBoundingDWBox, "BoundingVolume" );
    assert(bnd);

    glPushMatrix();
    {
        glTranslatef( cloth->GetTransform().GetPosition().x(), cloth->GetTransform().GetPosition().y(), cloth->GetTransform().GetPosition().z() );
        glBegin(GL_LINES);
        {
            const Vec3f& hs = ((GOCBoundingBox*)bnd->GetPrimitive())->GetHalfSize();

            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(hs.x(),0.0f, 0.0f);

            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f,hs.y(), 0.0f);

            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f,0.0f, hs.z());
        }
        glEnd();
    }
    glPopMatrix();

    SwapBuffers();
}

//-----------------------------------------------------------------------------
void MainWindow::OnIdle()
{
    float delta = (float)Clock::Instance().GetTimeDelta();
    
    if( m_bPause ) return;

    RotateCube( delta );
    CheckRotationAngles();
    
    MGRPhysics::Instance().Update( delta );
    Redraw();
}

//-----------------------------------------------------------------------------
void MainWindow::OnKeyboard( int key, bool down )
{
    if( down ) return;
    key = tolower(key);

    switch(key)
    {
    case '0': Reset(); break;
    case '1': 
        {
            if( RandBoolean() )
                MainApp::Get().AddBigSphere();
            else
                MainApp::Get().AddSmallSphere();
        }
        break;

    case '2':  MainApp::Get().RemoveLastSphere(); break;

    case 'q': Close(); break;
    case 'p': m_bPause = !m_bPause; break;
    case 'w': 
        {
            m_bWireframe = !m_bWireframe;
            if( m_bWireframe )
                glPolygonMode( GL_FRONT, GL_LINE );
            else
                glPolygonMode( GL_FRONT, GL_FILL );
        }
        break;
    
    case 'a': m_Lights[0].SetPosition( Vec3f( -0.04f, 0.0f, 0.0f ) ); break;
    case 's': m_Lights[0].SetPosition( Vec3f( 0.0f, 0.0f, 0.4f )   ); break;
    case 'd': m_Lights[0].SetPosition( Vec3f( 0.04f, 0.0f, 0.0f )  ); break;
    case 'f': m_Lights[0].SetPosition( Vec3f( 0.0f, 0.0f, -0.04f ) ); break;
    case 'z': m_Lights[0].TurnOff(); break;
    }

    Redraw();
}

//-----------------------------------------------------------------------------
void MainWindow::OnMouseButton( MouseButton button, bool down )
{
    if( button == MBLeft ) m_bIsMouseDown = down;

    Redraw();
}

//-----------------------------------------------------------------------------
void MainWindow::OnMouseMove( int x, int y )
{
    if( m_bIsMouseDown )
    {
        const int dX = x - m_iMouseX;
        const int dY = y - m_iMouseY;

        const float pcX = float(m_fAccumX) / Width();
        const float pcY = -float(m_fAccumY) / Height();

        m_fCubeHorizontalAngle = ROTATION_SPEED * pcX * DEG_TO_RAD;
        m_fCubeVerticalAngle   = ROTATION_SPEED * pcY * DEG_TO_RAD;

        m_fAccumX += dX;
        m_fAccumY += dY;
    }
    else {
        m_fAccumX = 0.0f;
        m_fAccumY = 0.0f;
    }

    m_iMouseX = x;
    m_iMouseY = y;

    Redraw();
}

//-----------------------------------------------------------------------------
void MainWindow::Reset()
{
}

//-----------------------------------------------------------------------------
void MainWindow::RotateCube( float delta )
{
    if( m_fCubeHorizontalAngle != 0.0f || m_fCubeVerticalAngle != 0.0f )
    {
        Quatf qCirRot, qHRot, qVRot;
        qHRot.FromVector( m_fCubeHorizontalAngle * delta, Vec3f( 0.0f, 0.0f, 1.0f ) );
        qVRot.FromVector( m_fCubeVerticalAngle * delta, Vec3f( 1.0f, 0.0f, 0.0f ) );
        qCirRot = qVRot * qHRot;
        
        for( int i=0; i<MainApp::MAX_PLANES; ++i )
        {
            // Must rotate the orientation 
            // Then we must also notify interested components about that ??? 
            // [Inter-component message systems?]
            // OnPositionChange() && OnOrientationChange() ???

            CGameObject *plane = MainApp::Get().GetPlane(i);
            
            // Rotate plane
            Vec3f &vPos = plane->GetTransform().GetPosition();
            qCirRot.Rotate( vPos );

            // Add the rotation
            Quatf &qRot = plane->GetTransform().GetOrientation();
            qRot = qCirRot * qRot;

            // Corrent normal
            GOCBoundingPlane *bnd = GET_OBJ_GOC( plane, GOCBoundingPlane, "BoundingVolume" );
            qCirRot.Rotate( bnd->GetNormal() );
            bnd->GetNormal().Normalize();
        }

        // Rotate cloth as well
        Vec3f &vPos = MainApp::Get().GetCloth()->GetTransform().GetPosition();
        qCirRot.Rotate( vPos );
        //Quatf &qRot = MainApp::Get().GetCloth()->GetTransform().GetOrientation();
        //qRot = qCirRot * qRot;

        CGameObject *cloth = MainApp::Get().GetCloth();
        GOCPhysicsCloth *physics = GET_OBJ_GOC( cloth, GOCPhysicsCloth, "Physics" );
        assert(physics);
        physics->Rotate(qCirRot);

        GOCBoundingDWBox *bnd = GET_OBJ_GOC( cloth, GOCBoundingDWBox, "BoundingVolume" );
        assert(bnd);
        bnd->WrapObject();
    }
} // RotateCube()

//-----------------------------------------------------------------------------
void MainWindow::CheckRotationAngles()
{
    // Check cube rotation angles
    if( m_fCubeHorizontalAngle > ROTATION_STEP ) 
        m_fCubeHorizontalAngle -= ROTATION_STEP;
    else if( m_fCubeHorizontalAngle <= -ROTATION_STEP ) 
        m_fCubeHorizontalAngle += ROTATION_STEP;
    else
        m_fCubeHorizontalAngle = 0.0f;

    if( m_fCubeVerticalAngle > ROTATION_STEP ) 
        m_fCubeVerticalAngle -= ROTATION_STEP;
    else if( m_fCubeVerticalAngle <= -ROTATION_STEP ) 
        m_fCubeVerticalAngle += ROTATION_STEP;
    else
        m_fCubeVerticalAngle = 0.0f;
}

//-----------------------------------------------------------------------------
void MainWindow::OnDestroy()
{
    Clock::Destroy();
}

//-----------------------------------------------------------------------------
void MainWindow::OnResize( int w, int h )
{
    m_fDimRatio = float(w) / float(h);
    SetupView();
}

//-----------------------------------------------------------------------------
void MainWindow::PrintStats()
{
    glRasterPos2f( .5f, .5f );
    Printf( "Number of Balls: %i", MainApp::Get().GetBallCount() );
}

//-----------------------------------------------------------------------------
void MainWindow::SetupView() const
{
    // Setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( m_fFovY, m_fDimRatio, m_fPlanes[0], m_fPlanes[1] );

    // Setup modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport( 0, 0, Width(), Height() );
}

//-----------------------------------------------------------------------------
void MainWindow::RenderHelpGrid( int lines, float size ) const
{
    // Render a helper grid
    glBegin(GL_LINES);
    {
        glColor3f( 1.0f, 1.0f, 0.0f );
        const float init_pos = -size*lines*0.5f;
        const float y = -0.6f;
        for( int kk=0; kk<lines; ++kk )
        {
            const float temp = init_pos + kk * size;

            glVertex3f( temp, y, init_pos );
            glVertex3f( temp, y, init_pos + size*lines );

            glVertex3f( init_pos, y, temp );
            glVertex3f( init_pos + size*lines, y, temp );
        }
    }
    glEnd();
}