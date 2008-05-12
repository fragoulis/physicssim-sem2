#include <GL/GLee.h>
#include "CSceneManager.h"
#include "MainApp.h"
#include "GOCS/Interfaces/IGOCVisual.h"
#include "CShaderManager.h"
#include "CTextureManager.h"
#include "CMaterialManager.h"
#include "CVertexArrayManager.h"
#include "ObjectMutex.h"
#include "Util/CLogger.h"
#include <algorithm>
using namespace tlib;
using tlib::gocs::IGOCVisual;

CSceneManager::CSceneManager(): 
m_Cloth(0),
m_Backplane(0),
m_Shelf(0),
m_uiBackPlaneTexture(0),
m_bTextured(true),
m_bLoaderActive(false)
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

	// Preload textures we are using :P
	MGRTexture::Get().GetTexture("source/images/fiberglass.jpg");
	MGRTexture::Get().GetTexture("source/images/bubbles-large.jpg");
	MGRTexture::Get().GetTexture("source/images/cloth.jpg");
	MGRTexture::Get().GetTexture("source/images/metal01-large.jpg");
	MGRTexture::Get().GetTexture("source/images/floor.jpg");
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
				_LOG("Registering big sphere");
            }
            else if( pVisComp->GetOwner()->Is("SmallSphere") ) {
                m_SmallSpheres.push_back( pVisComp );
				_LOG("Registering small sphere");
            }
            else if( pVisComp->GetOwner()->Is("Plane") ) {
                m_Walls.push_back( pVisComp );
				_LOG("Registering wall");
            }
            else if( pVisComp->GetOwner()->Is("Backplane") ) {
                m_Backplane = pVisComp;
				_LOG("Registering backwall");
            }
            else if( pVisComp->GetOwner()->Is("Cloth") ) {
                m_Cloth = pVisComp;
				_LOG("Registering cloth");
            }
            else if( pVisComp->GetOwner()->Is("Shelf") ) {
                m_Shelf = pVisComp;
				_LOG("Registering shelf");
            }
			else if( pVisComp->GetOwner()->Is("Floor") ) {
                m_Floor = pVisComp;
				_LOG("Registering floor");
            }
            else {
                fassert("Adding a visual component with invalid object id");
            }
        }
        __FINALLY { m_vizMutex.ReleaseAll(); }
    } // if( )

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
				_LOG("Unregistering big sphere");
            }
            else if( pVisComp->GetOwner()->Is("SmallSphere") ) {
                FIND_AND_ERASE(m_SmallSpheres, pVisComp);
				_LOG("Unregistering small sphere");
            }
            else if( pVisComp->GetOwner()->Is("Plane") ) {
                FIND_AND_ERASE(m_Walls, pVisComp);
				_LOG("Unregistering wall");
            }
            else if( pVisComp->GetOwner()->Is("Backplane") ) {
                m_Backplane = 0;
				_LOG("Unregistering backwall");
            }
            else if( pVisComp->GetOwner()->Is("Cloth") ) {
                m_Cloth = 0;
				_LOG("Unregistering cloth");
            }
            else if( pVisComp->GetOwner()->Is("Shelf") ) {
                m_Shelf = 0;
				_LOG("Unregistering shelf");
            }
			else if( pVisComp->GetOwner()->Is("Floor") ) {
                m_Floor = 0;
				_LOG("Unregistering floor");
            }
            else {
                fassert("Removing a visual component with invalid object id");
            }
        }
        __FINALLY { m_vizMutex.ReleaseAll(); }
    } // if( )

} // Unregister()

// ------------------------------------------------------------------------
void CSceneManager::GenerateNewBackplaneTexture() const
{
    if( !m_bLoaderActive ) return;

    // Delete previous texture if there is one
    if( m_uiBackPlaneTexture > 0 ) ClearBackplaneTexture();

    // Generate texture
    glGenTextures( 1, &m_uiBackPlaneTexture );
    assert(m_uiBackPlaneTexture);

    if( m_bpMutex.IsWritable() )
    {
        __TRY {
            glBindTexture( GL_TEXTURE_2D, m_uiBackPlaneTexture );
            m_image.gluBuild2DMipmaps();
        }
        __FINALLY { m_bpMutex.ReleaseAll(); }
    }

    m_image.Free();

    m_bLoaderActive = false;
}

// ------------------------------------------------------------------------
void CSceneManager::ClearBackplaneTexture() const
{ 
    glDeleteTextures( 1, &m_uiBackPlaneTexture );
    m_uiBackPlaneTexture = 0;
}

// ------------------------------------------------------------------------
void CSceneManager::LoadNewBackplaneTexture( const char *filename )
{
    if( m_bpMutex.IsWritable() )
    {
        __TRY {
            if( !m_image.Load( filename ) ) m_image.Free();
            else 
                m_bLoaderActive = true;
        }
        __FINALLY { m_bpMutex.ReleaseAll(); }
    }
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

	// Render floor
	{
		uiTextureId = MGRTexture::Get().GetTexture("source/images/floor.jpg");
		glBindTexture( GL_TEXTURE_2D, uiTextureId );
		MGRVertexArray::Get().Begin("Floor");
		float m[16];
		const Quatf &vOri = m_Floor->GetOwner()->GetOrientation();
		vOri.ToMatrix(m);
	    
		const Vec3f &vPos = m_Floor->GetOwner()->GetPosition();
		glPushMatrix();
			glTranslatef( vPos.x(), vPos.y(), vPos.z() );
			glMultMatrixf(m);
			MGRVertexArray::Get().Render();
		glPopMatrix();
		MGRVertexArray::Get().End();
	}

    if( m_vizMutex.IsReadable() )
    {
        __TRY
        {
            if( ObjectMutex::IsReadable() )
            {
                __TRY 
                { 
                    // ----------------------------------------------------------------
                    // Render small spheres
                    uiTextureId = MGRTexture::Get().GetTexture("source/images/fiberglass.jpg");
                    glBindTexture( GL_TEXTURE_2D, uiTextureId );
                    MGRVertexArray::Get().Begin("SmallSphere");
                    for( iter = m_SmallSpheres.begin(); iter != m_SmallSpheres.end(); ++iter )
                    {
                        const Vec3f &vPos = (*iter)->GetOwner()->GetPosition();
                        glPushMatrix();
                            glTranslatef( vPos.x(), vPos.y(), vPos.z() );
                            MGRVertexArray::Get().Render();
                        glPopMatrix();
                    }
                    MGRVertexArray::Get().End();

                    // ----------------------------------------------------------------
                    // Render big spheres
                    uiTextureId = MGRTexture::Get().GetTexture("source/images/bubbles-large.jpg");
                    glBindTexture( GL_TEXTURE_2D, uiTextureId );
                    MGRVertexArray::Get().Begin("BigSphere");
                    for( iter = m_BigSpheres.begin(); iter != m_BigSpheres.end(); ++iter )
                    {
                        const Vec3f &vPos = (*iter)->GetOwner()->GetPosition();
                        glPushMatrix();
                            glTranslatef( vPos.x(), vPos.y(), vPos.z() );
                            MGRVertexArray::Get().Render();
                        glPopMatrix();
                    }
                    MGRVertexArray::Get().End();

                    // ----------------------------------------------------------------
                    // Render cloth || shelf
                    uiTextureId = MGRTexture::Get().GetTexture("source/images/cloth.jpg");
                    glBindTexture( GL_TEXTURE_2D, uiTextureId );
                    if(m_Cloth) m_Cloth->Render();
                    if(m_Shelf) m_Shelf->Render();

                    // ----------------------------------------------------------------
                    // Render other walls
                    uiTextureId = MGRTexture::Get().GetTexture("source/images/metal01-large.jpg");
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
                    // Render backplane
                    if(m_Backplane)
                    {
                        uiTextureId = (m_uiBackPlaneTexture==0)?uiTextureId:m_uiBackPlaneTexture;
                        glBindTexture( GL_TEXTURE_2D, uiTextureId );
                        MGRVertexArray::Get().Begin("Wall");
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
                }
                __FINALLY { ObjectMutex::ReleaseWrite(); }

            } // if( .. )
        }
        __FINALLY { m_vizMutex.ReleaseWrite(); }

    } // if(  )

    MGRShader::Get().end();

} // Render()