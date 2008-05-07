#ifndef __TLIB_CSHADERMANAGER_H__
#define __TLIB_CSHADERMANAGER_H__
#pragma once
#include "Singleton.h"
#include <vector>

namespace tlib
{

class CShaderManager : public Singleton<CShaderManager>
{
    friend Singleton<CShaderManager>;

public:
    //! An enumeration of the shader programs we will use
    enum ShaderProgram {
        LIGHT_W_TEXTURE,
        LIGHT_W_TEXTURE_REFRACT,
        NUM_OF_SHADERS
    };

private:
    //! This struct holds a program handle and two shader handles
    //! This will help us keep track of our shaders
    struct _Shader {
        _Shader( unsigned _uiProg, 
                 unsigned _uiVertex, 
                 unsigned _uiPixel ):
        uiProg(_uiProg),
        uiVertex(_uiVertex),
        uiPixel(_uiPixel)
        {}
            
        unsigned uiProg;
        unsigned uiVertex;
        unsigned uiPixel;
    };

    typedef std::vector<_Shader> ShaderList;

    //! The list of programmes
    unsigned int m_vProgIds[NUM_OF_SHADERS];

    //! The program id in use
    unsigned int m_uiActiveProg;

    //! A list of shaders and programs
    ShaderList m_vList;

    // Return codes
    static const int INVALID_FILENAME;
    static const int COMPILE_FAILED;

public:
    //! Initializes the manager by creating all programs
    void Init();

    //! Returns the requested program id
    unsigned int getProgram( ShaderProgram iType ) const {
        assert((iType>=0)&&(iType<NUM_OF_SHADERS));
        return m_vProgIds[ iType ];
    }

    //! Get index of a uniform variable
    int getUniform( unsigned uiProg, const char *name );
    int getUniform( const char *name );

    //! Starts the use of a program
    void begin( ShaderProgram iType );

    //! Stops the use of a program
    void end();

    void printShaderInfoLog(ShaderProgram iType);
    void printProgramInfoLog(ShaderProgram iType);

private:
    CShaderManager();
    ~CShaderManager();

    //! Creates a vertex shader, compiles it and attaches it
    //! to a program
    int addShader( unsigned shaderType, const char *file );

    //! Loads the source of a shader from file
    char* loadSource( const char *file );

    //! Adds a new shader program by type
    bool addProgram( ShaderProgram iType, 
                     const char *vertex, const char *pixel );

}; // end CShaderManager

typedef CShaderManager MGRShader;

} // end namspace tlib

#endif // __TLIB_CSHADERMANAGER_H__