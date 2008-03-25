#ifndef __TLIB_CMATERIALMANAGER_H__
#define __TLIB_CMATERIALMANAGER_H__
#pragma once
#include "Singleton.h"
#include <map>

namespace tlib
{
    
namespace util 
{
    class CMaterial;
}

class CMaterialManager : public Singleton<CMaterialManager>
{
public:
    // Material enumeration
    enum EMaterial { SHINY, METAL, MATTE };

private:
    typedef std::map<EMaterial,util::CMaterial*> MaterialList;
    //! List of available materials
    mutable MaterialList m_Materials;

public:
    //! Applies the material to the scene
    void Apply( EMaterial mat ) const;

private:
    friend Singleton<CMaterialManager>;
    CMaterialManager();
    ~CMaterialManager();
};

typedef CMaterialManager MGRMaterial;

} // end namespace tlib

#endif // __TLIB_CMATERIALMANAGER_H__