#include "CMaterialManager.h"
#include "Util/CMaterial.h"
using namespace tlib;
using tlib::util::CMaterial;

CMaterialManager::CMaterialManager()
{
    // *** NOTE **** MAKE USE OF FREELIST **** //

    // Create default materials
    CMaterial *mat = new CMaterial;
    mat->GetSpecular().Assign( 1.0f, 1.0f, 1.0f, 1.0f );
    mat->SetShininess(80.0f);
    m_Materials[SHINY] = mat;

    mat = new CMaterial;
    mat->GetSpecular().Assign( 1.0f, 1.0f, 1.0f, 1.0f );
    mat->SetShininess(5.0f);
    m_Materials[METAL] = mat;

    mat = new CMaterial;
    mat->GetSpecular().Assign( 0.2f, 0.2f, 0.2f, 1.0f );
    mat->SetShininess(5.0f);
    m_Materials[MATTE] = mat;
}

CMaterialManager::~CMaterialManager()
{
    MaterialList::iterator i = m_Materials.begin();
    for(; i != m_Materials.end(); ++i )
        delete i->second;
}

void CMaterialManager::Apply( EMaterial mat ) const 
{
    CMaterial *pMaterial = m_Materials[mat];
    assert(pMaterial);
    pMaterial->Apply();
}