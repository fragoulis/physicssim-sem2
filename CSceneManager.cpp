#include <GL/GLee.h>
#include "CSceneManager.h"
#include "GOCS/Interfaces/IGOCVisual.h"
#include "CShaderManager.h"
#include "CTextureManager.h"
#include "CMaterialManager.h"
#include "CVertexArrayManager.h"
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
    MGRShader::Get().begin(MGRShader::LIGHT_W_TEXTURE);

    // Apply texture
    glEnable(GL_TEXTURE_2D);
    GLuint uiTextureId = MGRTexture::Get().GetTexture("images/metal01-large.jpg");
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
    VisualArray_t::const_iterator i = m_vVisuals.begin();
    for(; i != m_vVisuals.end(); ++i )
    {   
        IGOCVisual *v = *i;
        v->Render();
    }

    // Render all jelly objects
    // ------------------------------------------------------------------------

    // Render cloth
    // ------------------------------------------------------------------------

    // Disable texturing
    glDisable(GL_TEXTURE_2D);

    MGRShader::Get().end();
}