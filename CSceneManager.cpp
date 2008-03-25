#include <GL/GLee.h>
#include "CSceneManager.h"
#include "GOCS/Interfaces/IGOCVisual.h"
#include "CShaderManager.h"
#include "CTextureManager.h"
#include "CMaterialManager.h"
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
    GLuint uiTextureId = MGRTexture::Instance().GetTexture("images/metal01-large.jpg");
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, uiTextureId );
    glUniform1i( MGRShader::Instance().getUniform("colormap"), 0 );

    // Apply material
    MGRMaterial::Instance().Apply(MGRMaterial::METAL);

    // Render cube walls
    // ------------------------------------------------------------------------
    
    // Render all balls
    // ------------------------------------------------------------------------
    VisualArray_t::const_iterator i = m_vVisuals.begin();
    for(; i != m_vVisuals.end(); ++i )
        (*i)->Render();

    // Render all jelly objects
    // ------------------------------------------------------------------------

    // Render cloth
    // ------------------------------------------------------------------------

    // Disable texturing
    glDisable(GL_TEXTURE_2D);

    MGRShader::Instance().end();
}