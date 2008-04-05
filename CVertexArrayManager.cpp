#include "CVertexArrayManager.h"
#include "Util/VA/CVASphere.h"
#include "Util/VA/CVAPlane.h"
#include "Util/std_help.h"
#include "Util/Config.h"

using namespace tlib;
using namespace tlib::util;

CVertexArrayManager::CVertexArrayManager():
m_Active(0)
{}

CVertexArrayManager::~CVertexArrayManager()
{
    DELETE_MAP( VAList, m_Arrays );
}

void CVertexArrayManager::Init()
{
    float radius, size[2], normal[3];
    int stacks, slices;

    // TODO: Draw this data out of a file
    CFG_CLIENT_OPEN;

    // ------------------------------------------------------------------------
    CFG_LOAD("Sphere");

    CFG_1f("BigRadius", radius);
    CFG_1i("BigStacks", stacks);
    CFG_1i("BigSlices", slices);
    CVASphere *sphere = new CVASphere( "BigSphere", radius, stacks, slices );
    MAP_INSERT( m_Arrays, "BigSphere", sphere );

    CFG_1f("SmallRadius", radius);
    CFG_1i("SmallStacks", stacks);
    CFG_1i("SmallSlices", slices);
    sphere = new CVASphere( "SmallSphere", radius, stacks, slices );
    MAP_INSERT( m_Arrays, "SmallSphere", sphere );

    // ------------------------------------------------------------------------
    CFG_LOAD("Plane");

    CFG_2fv("Halfsize", size);
    CFG_3fv("Normal", normal);
    CFG_1i("Stacks", stacks);
    CFG_1i("Slices", slices);
    
    CVAPlane *plane = new CVAPlane( "Wall", size, normal, stacks, slices );
    MAP_INSERT( m_Arrays, "Wall", plane );

    // ------------------------------------------------------------------------
    CFG_LOAD("Cloth");

    CFG_2fv("Halfsize", size);
    CFG_3fv("Normal", normal);
    CFG_1i("Stacks", stacks);
    CFG_1i("Slices", slices);

    plane = new CVAPlane( "Cloth", size, normal, stacks, slices );
    MAP_INSERT( m_Arrays, "Cloth", plane );
}

void CVertexArrayManager::Begin( const VArrayId &key )
{
    // If there is another array currently active end it first
    if( m_Active ) End();

    m_Arrays[ key ]->Begin();
    m_Active = const_cast<VArrayId*>(&key);
}

void CVertexArrayManager::Render()
{
    assert(m_Active);
    m_Arrays[ *m_Active ]->Render();
}

void CVertexArrayManager::End()
{
    assert(m_Active);
    m_Arrays[ *m_Active ]->End();
    m_Active = 0;
}