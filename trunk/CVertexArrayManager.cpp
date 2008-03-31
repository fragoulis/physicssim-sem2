#include "CVertexArrayManager.h"
//#include "Util/VA/IVertexArray.h"
#include "Util/VA/CVASphere.h"
#include "Util/VA/CVAPlane.h"
#include "Util/std_help.h"

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
    // TODO: Draw this data out of a file

    float radius = 0.05f;
    int stacks = 5;
    int slices = 5;
    CVASphere *sphere = new CVASphere( "BigSphere", radius, stacks, slices );
    MAP_INSERT( m_Arrays, "BigSphere", sphere );

    radius = 0.02f;
    sphere = new CVASphere( "SmallSphere", radius, stacks, slices );
    MAP_INSERT( m_Arrays, "SmallSphere", sphere );

    Vec2f hsize( 0.5f, 0.5f );
    stacks = slices = 2;
    CVAPlane *plane = new CVAPlane( "Wall", hsize, stacks, slices );
    MAP_INSERT( m_Arrays, "Wall", plane );

    hsize.Set( 0.25f, 0.5f );
    stacks = 20;
    slices = 30;
    plane = new CVAPlane( "Cloth", hsize, stacks, slices );
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