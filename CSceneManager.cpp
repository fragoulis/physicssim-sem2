#include <GL/GLee.h>
#include "CSceneManager.h"
#include "MainApp.h"
#include "GOCS/Interfaces/IGOCVisual.h"
#include "CShaderManager.h"
#include "CTextureManager.h"
#include "CMaterialManager.h"
#include "CVertexArrayManager.h"
#include "ObjectMutex.h"
#include <algorithm>
using namespace tlib;
using tlib::gocs::IGOCVisual;

CSceneManager::CSceneManager(): 
m_Cloth(0),
m_Backplane(0),
m_Shelf(0),
m_uiBackPlaneTexture(0),
m_bTextured(true),
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
    if( m_vizMutex.IsWritable() )
    {
        __TRY 
        { 
            if( pVisComp->GetOwner()->Is("BigSphere") ) {
                m_BigSpheres.push_back( pVisComp );
            }
            else if( pVisComp->GetOwner()->Is("SmallSphere") ) {
                m_SmallSpheres.push_back( pVisComp );
            }
            else if( pVisComp->GetOwner()->Is("Plane") ) {
                m_Walls.push_back( pVisComp );
            }
            else if( pVisComp->GetOwner()->Is("Backplane") ) {
                m_Backplane = pVisComp;
            }
            else if( pVisComp->GetOwner()->Is("Cloth") ) {
                m_Cloth = pVisComp;
            }
            else if( pVisComp->GetOwner()->Is("Shelf") ) {
                m_Shelf = pVisComp;
            }
            else {
                fassert("Adding a visual component with invalid object id");
            }
        }
        __FINALLY { m_vizMutex.ReleaseAll(); }
    } // if( )

    //if( m_vizMutex.IsWritable() )
    //{
    //    __TRY { m_vVisuals.push_back( pVisComp ); }
    //    __FINALLY { m_vizMutex.ReleaseAll(); }
    //}

} // Register()

#define FIND_AND_ERASE( list, obj ) \
    list.erase( \
            std::find( list.begin(), list.end(), obj ) \
        )

// ------------------------------------------------------------------------
void CSceneManager::Unregister( gocs::IGOCVisual *pVisComp )
{
    if( m_vizMutex.IsWritable() )
    {
        __TRY 
        { 
            if( pVisComp->GetOwner()->Is("BigSphere") ) {
                FIND_AND_ERASE(m_BigSpheres, pVisComp);
            }
            else if( pVisComp->GetOwner()->Is("SmallSphere") ) {
                FIND_AND_ERASE(m_SmallSpheres, pVisComp);
            }
            else if( pVisComp->GetOwner()->Is("Plane") ) {
                FIND_AND_ERASE(m_Walls, pVisComp);
            }
            else if( pVisComp->GetOwner()->Is("Backplane") ) {
                m_Backplane = 0;
            }
            else if( pVisComp->GetOwner()->Is("Cloth") ) {
                m_Cloth = 0;
            }
            else if( pVisComp->GetOwner()->Is("Shelf") ) {
                m_Shelf = 0;
            }
            else {
                fassert("Removing a visual component with invalid object id");
            }
        }
        __FINALLY { m_vizMutex.ReleaseAll(); }
    } // if( )

    //if( m_vizMutex.IsWritable() )
    //{
    //    __TRY { FIND_AND_ERASE(m_vVisuals, pVisComp); }
    //    __FINALLY { m_vizMutex.ReleaseAll(); }
    //}

} // Unregister()

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
    GenerateNewBackplaneTexture();

    VisualList::const_iterator iter;    // visual component iterator
    GLuint uiTextureId;                 // texture id

    // Enable shader
    MGRShader::Get().begin(MGRShader::LIGHT_W_TEXTURE);
    glUniform1i( MGRShader::Get().getUniform("colormap"), 0 );
    glUniform1i( MGRShader::Get().getUniform("isTextured"), m_bTextured );

    // Apply material
    MGRMaterial::Get().Apply(MGRMaterial::METAL);

    if( m_vizMutex.IsReadable() && ObjectMutex::IsReadable() )
    {
        __TRY 
        { 
            // ----------------------------------------------------------------
            // Render other walls
            uiTextureId = MGRTexture::Get().GetTexture("images/metal01-large.jpg");
            glBindTexture( GL_TEXTURE_2D, uiTextureId );
            MGRVertexArray::Get().Begin("Wall");
            for( iter = m_Walls.begin(); iter != m_Walls.end(); ++iter )
            {
                float m[16];
                const Quatf &vOri = (*iter)->GetOwner()->GetOrientation();
                vOri.ToMatrix(m);
                
                const Vec3f &vPos = (*iter)->GetOwner()->GetPosition();
                glPushMatrix();
                    glTranslatef( vPos.x(), vPos.y(), vPos.z() );
                    glMultMatrixf(m);
                    MGRVertexArray::Get().Render();
                glPopMatrix();
            }
            MGRVertexArray::Get().End();
            
            // ----------------------------------------------------------------
            // Render small spheres
            uiTextureId = MGRTexture::Get().GetTexture("images/fiberglass.jpg");
            glBindTexture( GL_TEXTURE_2D, uiTextureId );
            MGRVertexArray::Get().Begin("SmallSphere");
            for( iter = m_SmallSpheres.begin(); iter != m_SmallSpheres.end(); ++iter )
            {
                //(*iter)->Render();
                const Vec3f &vPos = (*iter)->GetOwner()->GetPosition();
                glPushMatrix();
                    glTranslatef( vPos.x(), vPos.y(), vPos.z() );
                    MGRVertexArray::Get().Render();
                glPopMatrix();
            }
            MGRVertexArray::Get().End();

            // ----------------------------------------------------------------
            // Render big spheres
            uiTextureId = MGRTexture::Get().GetTexture("images/bubbles-large.jpg");
            glBindTexture( GL_TEXTURE_2D, uiTextureId );
            MGRVertexArray::Get().Begin("BigSphere");
            for( iter = m_BigSpheres.begin(); iter != m_BigSpheres.end(); ++iter )
            {
                //(*iter)->Render();
                const Vec3f &vPos = (*iter)->GetOwner()->GetPosition();
                glPushMatrix();
                    glTranslatef( vPos.x(), vPos.y(), vPos.z() );
                    MGRVertexArray::Get().Render();
                glPopMatrix();
            }
            MGRVertexArray::Get().End();

            // ----------------------------------------------------------------
            // Render cloth || shelf
            uiTextureId = MGRTexture::Get().GetTexture("images/cloth.jpg");
            glBindTexture( GL_TEXTURE_2D, uiTextureId );
            m_Cloth->Render();
            m_Shelf->Render();

            // ----------------------------------------------------------------
            // Render backplane
            glBindTexture( GL_TEXTURE_2D, m_uiBackPlaneTexture );
            MGRVertexArray::Get().Begin("Wall");
            //m_Backplane->Render();
            float m[16];
            const Quatf &vOri = m_Backplane->GetOwner()->GetOrientation();
            vOri.ToMatrix(m);
            
            const Vec3f &vPos = m_Backplane->GetOwner()->GetPosition();
            glPushMatrix();
                glTranslatef( vPos.x(), vPos.y(), vPos.z() );
                glMultMatrixf(m);
                MGRVertexArray::Get().Render();
            glPopMatrix();
            MGRVertexArray::Get().End();
        }
        __FINALLY 
        { 
            m_vizMutex.ReleaseWrite(); 
            ObjectMutex::ReleaseWrite();
        }
    } // if(  )

    MGRShader::Get().end();
}