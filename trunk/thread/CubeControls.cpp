#include "CubeControls.h"
using namespace tlib;

//#define WITHOUT_SHARED
#ifndef WITHOUT_SHARED

bool CubeAngles::Get( float &horizontal, float &vertical ) const
{
    if( IsReadable() )
    {
        __TRY 
        { 
           horizontal = m_fHorizontal;
           vertical = m_fVertical;
        }
        __FINALLY { ReleaseWrite(); }
        return true;
    }

    return false;
}

bool CubeAngles::Set( float horizontal, float vertical )
{
    if( IsWritable() )
    {
        __TRY 
        { 
           m_fHorizontal = horizontal;
           m_fVertical = vertical;
        }
        __FINALLY { ReleaseAll(); }
        return true;
    }

    return false;
}

bool CubeAngles::Add( float horizontal, float vertical )
{
    if( IsWritable() )
    {
        __TRY 
        { 
           m_fHorizontal += horizontal;
           m_fVertical += vertical;
        }
        __FINALLY { ReleaseAll(); }
        return true;
    }

    return false;
}

#else

bool CubeAngles::Get( float &horizontal, float &vertical ) const
{
    horizontal = m_fHorizontal;
    vertical = m_fVertical;
    return true;
}

bool CubeAngles::Set( float horizontal, float vertical )
{
    m_fHorizontal = horizontal;
    m_fVertical = vertical;
    return true;
}

#endif

CubeControls::CubeControls(): 
rotationStep(0.025f), 
horizontalAngle(0.0f), 
verticalAngle(0.0f)
{}

void CubeControls::CheckAngles() 
{
    // Check cube rotation angles
    if( horizontalAngle > rotationStep )
        horizontalAngle -= rotationStep;
    else if( horizontalAngle <= -rotationStep )
        horizontalAngle += rotationStep;
    else
        horizontalAngle = 0.0f;

    if( verticalAngle > rotationStep )
        verticalAngle -= rotationStep;
    else if( verticalAngle <= -rotationStep )
        verticalAngle += rotationStep;
    else
        verticalAngle = 0.0f;

    angles.Set( horizontalAngle, verticalAngle );
}