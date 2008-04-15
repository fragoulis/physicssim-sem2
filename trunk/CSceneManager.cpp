#include <GL/GLee.h>
#include "CSceneManager.h"
#include "MainApp.h"
#include "GOCS/Interfaces/IGOCVisual.h"
#include "CShaderManager.h"
#include "CTextureManager.h"
#include "CMaterialManager.h"
#include "CVertexArrayManager.h"
//#include "GOCS/CGameObject.h"
#include "ObjectMutex.h"
#include <algorithm>
using namespace tlib;
using tlib::gocs::IGOCVisual;

CSceneManager::CSceneManager(): 
m_Cloth(0), 
m_uiBackPlaneTexture(0),
m_bBackplaneChanged(false)
{}

// ------------------------------------------------------------------------
CSceneManager::~CSceneManager()
{
    MGRShader::Destroy();
    MGRTexture::Destroy();
    MGRMaterial::Destroy();
    MGRVertexArray::Destroy();
}

// ------------------------------------------------------------------------
void CSceneManager::Init()
{
    MGRShader::_Get().Init();
    MGRTexture::_Get();
    MGRMaterial::_Get();
    MGRVertexArray::_Get().Init();
}

// ----------------------------------------------------------------------------
void CSceneManager::Register( gocs::IGOCVisual *pVisComp ) 
{
    //if( pVisComp->GetOwner()->Is("BigSphere") )
    //{
    //    m_BigSpheres.push_back( pVisComp );
    //}
    //else if( pVisComp->GetOwner()->Is("SmallSphere") )
    //{
    //    m_SmallSpheres.push_back( pVisComp );
    //}
    //else if( pVisComp->GetOwner()->Is("Plane") )
    //{
    //    m_Walls.push_back( pVisComp );
    //}
    //else if( pVisComp->GetOwner()->Is("Cloth") )
    //{
    //    m_Cloth = pVisComp;
    //}
    
    if( m_vizMutex.IsWritable() )
    {
        __TRY { m_vVisuals.push_back( pVisComp ); }
        __FINALLY { m_vizMutex.ReleaseAll(); }
    }
}

#define FIND_AND_ERASE( list, obj ) \
    list.erase( \
            std::find( list.begin(), list.end(), obj ) \
        )

// ------------------------------------------------------------------------
void CSceneManager::Unregister( gocs::IGOCVisual *pVisComp )
{
    //if( pVisComp->GetOwner()->Is("BigSphere") )
    //{
    //    FIND_AND_ERASE(m_BigSpheres, pVisComp);
    //}
    //else if( pVisComp->GetOwner()->Is("SmallSphere") )
    //{
    //    FIND_AND_ERASE(m_SmallSpheres, pVisComp);
    //}
    //else if( pVisComp->GetOwner()->Is("Plane") )
    //{
    //    FIND_AND_ERASE(m_Walls, pVisComp);
    //}
    //else if( pVisComp->GetOwner()->Is("Cloth") )
    //{
    //    m_Cloth = 0;
    //}

    if( m_vizMutex.IsWritable() )
    {
        __TRY { FIND_AND_ERASE(m_vVisuals, pVisComp); }
        __FINALLY { m_vizMutex.ReleaseAll(); }
    }
}

// ------------------------------------------------------------------------
void CSceneManager::GenerateNewBackplaneTexture() const
{
    if( !m_bBackplaneChanged ) return;

    bool ret = m_image.glTexImage2D();
    if( !ret ) {
        m_image.Free();
        return;
    }

    // Delete previous texture if there is one
    if( m_uiBackPlaneTexture > 0 ) 
        glDeleteTextures( 1, &m_uiBackPlaneTexture );

    // Generate texture
    glGenTextures( 1, &m_uiBackPlaneTexture );
    assert(m_uiBackPlaneTexture);

    glBindTexture( GL_TEXTURE_2D, m_uiBackPlaneTexture );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    m_image.gluBuild2DMipmaps();

    m_image.Free();

    m_bBackplaneChanged = false;
}

// ------------------------------------------------------------------------
void CSceneManager::LoadNewBackplaneTexture( const char *filename )
{
    bool ret = m_image.Load( filename );
    if( !ret ) {
        m_image.Free();
        return;
    }

    m_bBackplaneChanged = true;
}

// ------------------------------------------------------------------------
void CSceneManager::Render() const
{
    // For simplicity, all scene elements are rendered with 
    // - per-pixel lighting
    // - single texture

    // Enable shader
    MGRShader::Get().begin(MGRShader::LIGHT_W_TEXTURE);

    GenerateNewBackplaneTexture();

    // Apply texture
    //GLuint uiTextureId = MGRTexture::Get().GetTexture("images/metal01-large.jpg");
    GLuint uiTextureId = m_uiBackPlaneTexture;
    glBindTexture( GL_TEXTURE_2D, uiTextureId );
    glUniform1i( MGRShader::Get().getUniform("colormap"), 0 );

    // Apply material
    MGRMaterial::Get().Apply(MGRMaterial::METAL);

    // Render cube walls
    // ------------------------------------------------------------------------
    //MGRVertexArray::Get().Begin("Walls");
    //for( all walls ) do
    ///* set position and orientation */
    //MGRVertexArray::Get().Render();
    //end for
    //MGRVertexArray::End();
    
    // Render all balls
    // ------------------------------------------------------------------------
    
    // To update the balls we must be sure that the physics thread is not 
    // currently editing the visual component list or the positions of the 
    // objects.
    if( m_vizMutex.IsReadable() && ObjectMutex::IsReadable() )
    {
        __TRY 
        { 
            VisualList::const_iterator i = m_vVisuals.begin();
            for(; i != m_vVisuals.end(); ++i )
            {   
                IGOCVisual *v = *i;
                v->Render();
            }
        }
        __FINALLY 
        { 
            m_vizMutex.ReleaseWrite(); 
            ObjectMutex::ReleaseWrite();
        }
    }

    // Render all jelly objects
    // ------------------------------------------------------------------------

    // Render cloth
    // ------------------------------------------------------------------------
    

    MGRShader::Get().end();
}