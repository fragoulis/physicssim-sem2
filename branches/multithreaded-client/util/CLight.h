#ifndef __TLIB_UTIL_CLIGHT_H__
#define __TLIB_UTIL_CLIGHT_H__
#pragma once
#include "CColor.h"
#include "../Math/TVector3.h"
using tlib::math::Vec3f;

namespace tlib
{
namespace util
{

/** 
 * This class is an abstract base class that can describe a light.
 * It is used my the Light Manager to easily identify used/unused 
 * light sources and allocate them
 */
class CLight
{
public:
    // The types of lights
    enum LightType {
        POINT = 0,
        DIRECTIONAL,
        SPOT
    };

private:
    //! The light's position
    Vec3f m_vPosition;

    //! The light's Id.
    // This is a number between 0 and 7
    int m_iLightId;

    //! The light's type
    LightType m_iType;

    //! Flag that shows whether a light is used or not
    bool m_bIsOn;
    
    //! The w-component
    // Here we save the w-component of the light's position.
    // If the light is a directional light, we set this 0, 
    // otherwise is 1.
    float m_fW;

protected:
    //! Holds the constant attenuation for this light source
    float m_fConstantAttenuation;

    //! Holds the linear attenuation for this light source
    float m_fLinearAttenuation;

    //! Holds the quadratic attenuation for this light source
    float m_fQuadraticAttenuation;

    // The light's colors
    CColor m_Ambient;
    CColor m_Diffuse;
    CColor m_Specular;

public:
    CLight();
    virtual ~CLight() {}

    // Accessors
    int GetId() const { return m_iLightId; }
    LightType GetLightType() const { return m_iType; }
    float GetConstantAttenuation() const { return m_fConstantAttenuation; }
    float GetLinearAttenuation() const { return m_fLinearAttenuation; }
    float GetQuadraticAttenuation() const { return m_fQuadraticAttenuation; }
    const CColor& GetAmbient() const { return m_Ambient; }
    CColor& GetAmbient() { return m_Ambient; }
    const CColor& GetDiffuse() const { return m_Diffuse; }
    CColor& GetDiffuse() { return m_Diffuse; }
    const CColor& GetSpecular() const { return m_Specular; }
    CColor& GetSpecular() { return m_Specular; }

    // Mutators
    void SetId( int iLightId ) { m_iLightId = iLightId; }
    void SetLightType( LightType iType ) {
        m_iType = iType;
        if( DIRECTIONAL == iType )
            m_fW = 0.0f;
    }
    void SetAmbient( const CColor &ambient );
    void SetDiffuse( const CColor &diffuse );
    void SetSpecular( const CColor &specular );
    void SetConstantAttenuation( float fConstantAttenuation );
    void SetLinearAttenuation( float fLinearAttenuation );
    void SetQuadraticAttenuation( float fQuadraticAttenuation );

    //! Turns on the light
    void TurnOn();

    //! Turns off the light
    void TurnOff();

    //! Returns whether the light is on or off
    bool isOn() const { return m_bIsOn; }

    //! Positions the light source
    //! Alternatively, if the light is directional, this will set
    //! the light's direction
    void SetPosition( const Vec3f &vPos );

    //! Resets the light's attributes
    virtual void Reset();

    //! Applies the position of the light
    void Apply() const;

}; // end of CLight class

} // end namespace util
} // end namespace tlib

#endif // __TLIB_UTIL_CLIGHT_H__