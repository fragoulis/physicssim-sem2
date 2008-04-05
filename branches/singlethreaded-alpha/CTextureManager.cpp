#include "CTextureManager.h"
#include "Util/CLogger.h"
using namespace tlib;

CTextureManager::CTextureManager():
m_iFormat(Image::RGB)
{}

void CTextureManager::Apply( const std::string &filename ) const
{
}

// ----------------------------------------------------------------------------
GLuint CTextureManager::GetTexture( const char *filename )
{
    std::string sTexture( filename );

    // Check is the requested texture is already loaded
    // and if it is return its handle.
    GLuint uiTexId = m_mTextures[ sTexture ];
    if( uiTexId ) 
    {
        //_LOG("Texture already loaded "+ string(filename) +"["+ toStr<GLuint>(uiTexId) +"]");
        // Make sude the format is set to default;
        m_iFormat = Image::RGB;
        return uiTexId;
    }

    return LoadTexture( sTexture );
}

// ----------------------------------------------------------------------------
GLuint CTextureManager::LoadTexture( const std::string &sTexture )
{
    // Now try, to load the texture using the gxbase image class
    Image image;

    // Load image file
    if( !image.Load( sTexture.c_str() ) )
    {
        // Return nothing if this fails
        _LOG("Failed to load texture "+ sTexture);
        image.Free();
        // Make sude the format is set to default;
        m_iFormat = Image::RGB;
        return 0;
    }

    // Change the format if necessary
    if( m_iFormat != Image::RGB ) 
    {
        image.SetFormat( m_iFormat );
        // Make sude the format is set to default;
        m_iFormat = Image::RGB;
    }

    // Generate texture
    GLuint uiTexId;
    glGenTextures( 1, &uiTexId );

    _LOG("Loaded texture "+ string(sTexture.c_str()) +"["+ toStr<GLuint>(uiTexId) +"]");

    // Generate mipmaps and free image from memory
    glBindTexture( GL_TEXTURE_2D, uiTexId );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    image.gluBuild2DMipmaps();
    image.Free();

    // Push it to the texture list
    m_mTextures[ sTexture ] = uiTexId;

    return uiTexId;
}