#include "CShaderManager.h"
#include "Util/CLogger.h"
#include "Util/assert.h"
#include <GL/GLee.h>
#include <cstdio>
using namespace tlib;

const int CShaderManager::INVALID_FILENAME  = -1;
const int CShaderManager::COMPILE_FAILED    = -2;

CShaderManager::CShaderManager():
m_uiActiveProg(0)
{}

CShaderManager::~CShaderManager()
{
    // Clean up shaders
    ShaderList::const_iterator iter = m_vList.begin();
    for(; iter != m_vList.end(); ++iter )
    {
        const _Shader& sh = *iter;
        glDetachShader( sh.uiProg, sh.uiVertex );
        glDetachShader( sh.uiProg, sh.uiPixel );
        glDeleteShader( sh.uiVertex );
        glDeleteShader( sh.uiPixel );
        glDeleteProgram( sh.uiProg );
    }
}

// ------------------------------------------------------------------------
void CShaderManager::begin( ShaderProgram iType )
{
    assert((iType>=0)&&(iType<NUM_OF_SHADERS));
    m_uiActiveProg = m_vProgIds[ iType ];
    glUseProgram( m_uiActiveProg );
    
}

// ------------------------------------------------------------------------
void CShaderManager::end() {
    glUseProgram(0);
    m_uiActiveProg = 0;
}

// ------------------------------------------------------------------------
void CShaderManager::Init()
{
    if( !addProgram( LIGHT_W_TEXTURE, 
                     "shaders/light_w_texture.vert", 
                     "shaders/light_w_texture.frag" ) ) {
        fassert("LIGHT_W_TEXTURE shader failed to compile");
    }
}

// ------------------------------------------------------------------------
bool CShaderManager::addProgram( ShaderProgram iType, 
                                 const char *vertex, const char *pixel )
{
    _ASSERT((iType>=0)&&(iType<NUM_OF_SHADERS));

    // Load vertex shader
    GLuint uiVertex = addShader( GL_VERTEX_SHADER, vertex );
    switch( uiVertex ) {
        case INVALID_FILENAME: 
            return false;
            break;
        case COMPILE_FAILED: 
            return false;
            break;
    }

    // Load pixel shader
    GLuint uiPixel = addShader( GL_FRAGMENT_SHADER, pixel );
    switch( uiPixel ) {
        case INVALID_FILENAME: 
            return false;
            break;
        case COMPILE_FAILED: 
            return false;
            break;
    }

    // Create program object
    GLuint uiProg = glCreateProgram();

    // Attach shaders to program
    glAttachShader( uiProg, uiVertex );
    glAttachShader( uiProg, uiPixel );

    // Link program
    glLinkProgram( uiProg );

    // Check if it linked
    int iDone = 0;
    glGetProgramiv( uiProg, GL_LINK_STATUS, &iDone );
    if( !iDone ) {
        // Clean up
        glDetachShader( uiProg, uiVertex );
        glDetachShader( uiProg, uiPixel );
        glDeleteShader( uiVertex );
        glDeleteShader( uiPixel );
        glDeleteProgram( uiProg );
	    return false;
    }
    
    // Push the program to the list
    m_vList.push_back( _Shader( uiProg, uiVertex, uiPixel ) );

    // Save program id to the program list
    m_vProgIds[ iType ] = uiProg;

    return true;
}

// ------------------------------------------------------------------------
int CShaderManager::addShader( GLenum shaderType, const char *file )
{
    // Load shader's source from
    const char *source = loadSource( file );
    if( !source ) 
    {
        delete [] source;
        return INVALID_FILENAME;
    }

    // Create the shader object
    GLuint uiHandle = glCreateShader( shaderType );

    // Load shader's source to object
    glShaderSource( uiHandle, 1, &source, NULL );

    // Compile shader
    glCompileShader( uiHandle );

    // Check if it compiled correctly
    int iDone = 0;
    glGetShaderiv( uiHandle, GL_COMPILE_STATUS, &iDone );
    if( !iDone ) {
        glDeleteShader( uiHandle );
	    delete [] source;
        return COMPILE_FAILED;
    }

    // Clean up
    delete [] source;

    // Return the shader's handle
    return uiHandle;

} // end addShader()

// ------------------------------------------------------------------------
char* CShaderManager::loadSource( const char *filename ) 
{
    if( !filename ) return NULL;

    FILE *fp;
    if( fopen_s( &fp, filename, "r" ) )
        return NULL;

    // seek to end of file
    fseek(fp, 0, SEEK_END);
    // how long is the file?
    long size = ftell(fp);
    // rewind
    fseek(fp, 0, SEEK_SET);
	
    // allocate
    char *buff = new char[size+1];
    if( !buff ) {
	    //printf("LoadSource: failed to allocate %ld bytes\n", size);
	    fclose(fp);
	    return NULL;
    }
    // read shader from file
    size_t count = fread(buff, 1, size, fp);
    // null terminate
    buff[count] = 0;
    // check for errors
    if( !count || ferror(fp) ) {
	    delete [] buff;
	    buff=NULL;
    }

    // close file
    fclose(fp);

    // return
    return buff;

} // end loadSource()

// ------------------------------------------------------------------------
int CShaderManager::getUniform( GLuint uiProg, const char *name ) 
{
    if( !uiProg || !name ) return 0;

    int i = glGetUniformLocation( uiProg, name );
    if( i < 0 ) {
        std::cerr << "GetUniform: uniform not found " << std::string(name) << std::endl;
    }

    return i;
}

// ------------------------------------------------------------------------
int CShaderManager::getUniform( const char *name ) 
{
    if( !m_uiActiveProg || !name ) return 0;

    int i = glGetUniformLocation( m_uiActiveProg, name );
    if( i < 0 )
    {
        std::cerr << "GetUniform: uniform not found " << std::string(name) << std::endl;
    }

    return i;
}

// ------------------------------------------------------------------------
void CShaderManager::printShaderInfoLog(ShaderProgram iType)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetShaderiv(m_vProgIds[ iType ], GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(m_vProgIds[ iType ], infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}

// ------------------------------------------------------------------------
void CShaderManager::printProgramInfoLog(ShaderProgram iType)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetProgramiv(m_vProgIds[ iType ], GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(m_vProgIds[ iType ], infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}