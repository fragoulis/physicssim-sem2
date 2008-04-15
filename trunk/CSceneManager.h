#pragma once
#include <vector>
#include "Singleton.h"
#include "Thread/CMutex.h"
#include "../GX/Image.h"
#include "Util/util_def.h"
#include "../GX/Image.h"

namespace tlib
{

PREDEF_1( gocs, IGOCVisual )

/**
 * @class CSceneManager
 * 
 * Manages the object's rendering.
 * Is responsible for sorting the objects and rendering them. It does not
 * actually hold pointers to objects, but rather pointers to objects' 
 * visual components.
 */
class CSceneManager : public Singleton<CSceneManager>
{
private:
    //! The array of visual components
    typedef std::vector<gocs::IGOCVisual*> VisualList;
   
    //! Mutex used to protect the list of visual components
    //! from access errors
    CMutex m_vizMutex;
    
    //! List of visual components
    VisualList m_vVisuals;

    //VisualList m_BigSpheres, m_SmallSpheres, m_Walls;
    gocs::IGOCVisual* m_Cloth;

    // The next variables are used to repeatedly load a new texture with
    // the bitmap thread. They are mutable because the fuction that 
    // generates the new thread is called from the Render() which is 
    // constant.

    //! Texture id for the back plane
    mutable unsigned m_uiBackPlaneTexture;

    //! The texture id
    mutable gxbase::Image m_image;

    //! Flag that will tell us if a new texture has been loaded
    mutable bool m_bBackplaneChanged;

private:
    friend Singleton<CSceneManager>;
    CSceneManager();
    ~CSceneManager();

    void GenerateNewBackplaneTexture() const;

public:
    //! Initialize scene manager
    void Init();

    //! Iterates through the array of visual components and 
    //! renders the scene objects
    void Render() const;

    //! Adds a component to the array
    void Register( gocs::IGOCVisual *pVisComp );

    //! Removes a component from the array
    void Unregister( gocs::IGOCVisual *pVisComp );

    //! Loads a new texture for the back plane
    void LoadNewBackplaneTexture( const char *filename );

}; // end CSceneManager

typedef CSceneManager MGRScene;

} // end namespace tlib