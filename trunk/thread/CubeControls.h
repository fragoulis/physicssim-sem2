#pragma once
#include "CMutex.h"
#include "../Math/TQuaternion.h"
using tlib::math::Quatf;

namespace tlib
{

/**
 * Shared memory container class that holds the two cube angles
 */
class CubeAngles : public CMutex
{
public:
    bool Get( float &horizontal, float &vertical ) const;
    bool Set( float horizontal, float vertical );
    bool Add( float horizontal, float vertical );
    bool Clear() { return Set( 0.0f, 0.0f ); }

private:
    float m_fHorizontal;
    float m_fVertical;

}; // end CubeAngles

struct CubeControls
{
public:
    CubeControls();

    void CheckAngles();
    bool ReadAngles() { 
		return angles.Get( horizontalAngle, verticalAngle ); 
	}
    bool AddAngles( float horizontal, float vertical ) { 
        return angles.Add( horizontal, vertical ); 
    }

public:
    CubeAngles angles;
    float horizontalAngle, verticalAngle;
    Quatf rotationAccum;

private:
    float rotationStep;

}; // end CubeControls

} // end namespace tlib