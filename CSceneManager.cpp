#include <GL/GLee.h>
#include "CSceneManager.h"
#include "GOCS/Interfaces/IGOCVisual.h"
#include "CShaderManager.h"
#include "CTextureManager.h"
#include "CMaterialManager.h"
#include "GOCS/CGameObject.h"
#include <algorithm>
using namespace tlib;
using tlib::gocs::IGOCVisual;

// ------------------------------------------------------------------------
CSceneManager::~CSceneManager()
{
    MGRShader::Destroy();
    MGRTexture::Destroy();
    MGRMaterial::Destroy();
}

// ------------------------------------------------------------------------
void CSceneManager::Init()
{
    MGRShader::_Instance().Init();
    MGRTexture::_Instance();
    MGRMaterial::_Instance();
}

// ------------------------------------------------------------------------
void CSceneManager::Unregister( gocs::IGOCVisual *pVisComp )
{
    m_vVisuals.erase( 
        std::find( m_vVisuals.begin(), m_vVisuals.end(), pVisComp ) 
        );
}

// ------------------------------------------------------------------------
void CSceneManager::Render() const
{
    // For simplicity, all scene elements are rendered with 
    // - per-pixel lighting
    // - single texture

    // Enable shader
    MGRShader::Instance().begin(MGRShader::LIGHT_W_TEXTURE);

    // Apply texture
    glEnable(GL_TEXTURE_2D);
    

    // Apply material
    //MGRMaterial::Instance().Apply(MGRMaterial::METAL);

    // Render cube walls
    // ------------------------------------------------------------------------
    
    // Render all balls
    // ------------------------------------------------------------------------
    VisualArray_t::const_iterator i = m_vVisuals.begin();
    for(; i != m_vVisuals.end(); ++i )
    {   
        IGOCVisual *v = *i;

        // Just for demostration .... :P
        GLuint uiTextureId;
        if( v->GetOwner()->Is("MyBigSphere") ) 
        {
            uiTextureId = MGRTexture::Instance().GetTexture("images/metal01-large.jpg");
            glBindTexture( GL_TEXTURE_2D, uiTextureId );
            glUniform1i( MGRShader::Instance().getUniform("colormap"), 0 );
            MGRMaterial::Instance().Apply(MGRMaterial::SHINY);
        }
        else if( v->GetOwner()->Is("MyCloth") ) {
            uiTextureId = MGRTexture::Instance().GetTexture("images/cloth.jpg");
            glUniform1i( MGRShader::Instance().getUniform("colormap"), 0 );
            glBindTexture( GL_TEXTURE_2D, uiTextureId );
            MGRMaterial::Instance().Apply(MGRMaterial::SHINY);   
        }
        else
        {
            uiTextureId = MGRTexture::Instance().GetTexture("images/fiberglass.jpg");
            glUniform1i( MGRShader::Instance().getUniform("colormap"), 0 );
            glBindTexture( GL_TEXTURE_2D, uiTextureId );
            MGRMaterial::Instance().Apply(MGRMaterial::METAL);
        }
        
        v->Render();
    }

    // Render all jelly objects
    // ------------------------------------------------------------------------

    // Render cloth
    // ------------------------------------------------------------------------

    // Disable texturing
    glDisable(GL_TEXTURE_2D);

    MGRShader::Instance().end();
}