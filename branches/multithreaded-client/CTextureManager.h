#pragma once
#include <map>
#include <string>
#include "Singleton.h"
#include "../../GX/Image.h"
using gxbase::Image;

namespace tlib
{

class CTextureManager : public Singleton<CTextureManager>
{
private:
    // Map a string to an opengl texture id
    typedef std::map< std::string, unsigned int >  TextureList;
    //! The associative array of textures
    TextureList m_mTextures;

    //! The texture's format
    // Always defaults to Image::RGB
    Image::Format m_iFormat;

public:
    //! Applies the texture to the scene
    void Apply( const std::string &filename ) const;
    
    //! This function is responsible for loading a texture by its
    //! filename. If a same filename already exists, it does not
    //! load anything.
    //! It returns the handle for the requested texture.
    GLuint GetTexture( const char *filename );

    //! Loads a texture from file
    GLuint LoadTexture( const std::string &filename );

    //! Format setter
    void SetFormat( Image::Format iFormat ) { m_iFormat = iFormat; }

private:
    friend Singleton<CTextureManager>;
    CTextureManager();

}; // end CTextureManager

typedef CTextureManager MGRTexture;

} // end namespace tlib