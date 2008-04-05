#ifndef __TLIB_CSCENEMANAGER_H__
#define __TLIB_CSCENEMANAGER_H__
#pragma once
#include <vector>
#include "Singleton.h"

namespace tlib
{

namespace gocs
{
    class IGOCVisual;
}

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
    typedef std::vector<gocs::IGOCVisual*> VisualArray_t;
    
    VisualArray_t m_vVisuals;
    VisualArray_t m_BigSpheres, m_SmallSpheres, m_Walls;
    gocs::IGOCVisual* m_Cloth;

private:
    friend Singleton<CSceneManager>;
    CSceneManager();
    ~CSceneManager();

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

}; // end CSceneManager

typedef CSceneManager MGRScene;

} // end namespace tlib

#endif // __TLIB_CSCENEMANAGER_H__