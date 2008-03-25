#ifndef __TLIB_UTIL_CMATERIAL_H__
#define __TLIB_UTIL_CMATERIAL_H__
#pragma once
#include "CColor.h"

namespace tlib
{
namespace util
{
    
/**
 * @class CMaterial
 *
 */
class CMaterial
{
private:
    //! Ambient color
    CColor m_Ambient;

    //! Diffuse color
    CColor m_Diffuse;

    //! Emission color
    CColor m_Emission;

    //! Specular color
    CColor m_Specular;

    //! Specular shininess
    float m_fShininess;

public:
    CMaterial();

    //! Applies the material
    void Apply() const;

    // Accessors
    const CColor& GetAmbient() const { return m_Ambient; }
    CColor& GetAmbient() { return m_Ambient; }

    const CColor& GetDiffuse() const { return m_Diffuse; }
    CColor& GetDiffuse() { return m_Diffuse; }

    const CColor& GetEmission() const { return m_Emission; }
    CColor& GetEmission() { return m_Emission; }

    const CColor& GetSpecular() const { return m_Specular; }
    CColor& GetSpecular() { return m_Specular; }

    float GetShininess() const { return m_fShininess; }

    // Mutators
    void SetAmbient( const CColor &ambient ) { m_Ambient = ambient; }
    void SetDiffuse( const CColor &diffuse ) { m_Diffuse = diffuse; }
    void SetEmission( const CColor &emission ) { m_Emission = emission; }
    void SetSpecular( const CColor &specular ) { m_Specular = specular; }
    void SetShininess( float fShininess ) { m_fShininess = fShininess; }

}; // end CMaterial

} // end namespace util
} // end namespace tlib

#endif // __TLIB_UTIL_CMATERIAL_H__