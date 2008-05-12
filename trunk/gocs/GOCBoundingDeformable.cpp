#include "GOCBoundingDeformable.h"
#include "Interfaces/IGOCTBounding.h"
#include "Interfaces/IGOCPhysicsDeformable.h"
#include "Interfaces/IGOCVisualVertexArray.h"
#include "CGameObject.h"
#include "../Physics/CParticle.h"
#include <cassert>
using namespace tlib::gocs;
using tlib::physics::CParticle;

GOCBoundingDeformable::GOCBoundingDeformable( const IGOCTBounding * const tpl ):
IGOCBoundingVolume(tpl),
m_iParticleIndex(0),
m_Primitive(0),
m_areas(0),
m_iNumOfAreas(0)
{
    SetVolumeType(VT_DEFORMABLE);
}

GOCBoundingDeformable::~GOCBoundingDeformable()
{
    delete m_Primitive;
	m_Primitive = 0;

	delete[] m_areas;
	m_areas = 0;
}

// ----------------------------------------------------------------------------
void GOCBoundingDeformable::Init()
{
    IGOCBoundingVolume::Init();

    m_Body = GET_GOC( IGOCPhysicsDeformable, "Physics" );
    assert(m_Body);
    
    m_Visual = GET_GOC( IGOCVisualVertexArray, "Visual" );
    assert(m_Visual);

    assert(m_Primitive);
    m_Primitive->SetOwner(GetOwner());

	//InitAreas();	

} // Init()

// ----------------------------------------------------------------------------
int GOCBoundingDeformable::GetArea(const Vec3f &pos)
{
	int closestIndex = -1;
	float closestDist;
	for( int i=0; i<m_iNumOfAreas; i++ )
	{
		b_area &cur = m_areas[i];
		CParticle *particle = m_Body->GetParticle(cur.cx, cur.cy);

		float dist = (particle->GetPosition() - pos).SqrLength();
		if( closestIndex == -1 || dist < closestDist ) {
			closestIndex = i;
			closestDist = dist;
		}
	}
	
	assert(closestIndex>-1);
	return closestIndex;
}

// ----------------------------------------------------------------------------
CParticle* GOCBoundingDeformable::GetParticle(int areaIndex)
{
    ++m_iParticleIndex;

	if( m_iParticleIndex == (b_area::areaSideMaxCount*b_area::areaSideMaxCount)+1 )
    {
        m_iParticleIndex = 0;
        return 0;
    }

	int index = m_areas[areaIndex].indices[m_iParticleIndex-1];
    return m_Body->GetParticle( index );
}

// ----------------------------------------------------------------------------
CParticle* GOCBoundingDeformable::GetParticle()
{
    ++m_iParticleIndex;

    if( m_iParticleIndex == m_Body->GetNumOfParticles()+1 )
    {
        m_iParticleIndex = 0;
        return 0;
    }

    return m_Body->GetParticle( m_iParticleIndex-1 );
}

// ----------------------------------------------------------------------------
void GOCBoundingDeformable::InitAreas()
{
	// extract the bounding areas
	int stacks = m_Body->GetStacks() / b_area::areaSideMaxCount;
	int slices = m_Body->GetSlices() / b_area::areaSideMaxCount;

	const int particles = m_Body->GetNumOfParticles();
	const int area_size = b_area::areaSideMaxCount*b_area::areaSideMaxCount;
	m_iNumOfAreas = particles / area_size;
	m_areas = new b_area[m_iNumOfAreas];

	// setup up areas
	const int _cx = b_area::areaSideMaxCount/2;
	const int _cy = b_area::areaSideMaxCount/2;

	for( int j=0; j<stacks; j++ )
	{
		for( int i=0; i<slices; i++ )
		{
			b_area &cur = m_areas[i+j*slices];
			cur.indices = new int[area_size];

			int incX = b_area::areaSideMaxCount*i;
			int incY = b_area::areaSideMaxCount*j;

			cur.cx = _cx + incX;
			cur.cy = _cy + incY;

			int limitX = incX+b_area::areaSideMaxCount;
			int limitY = incY+b_area::areaSideMaxCount;

			int current_index = 0;
			for( int pst=incY; pst<limitY; pst++ )
			{
				for( int psl=incX; psl<limitX; psl++ )
				{
					cur.indices[current_index++] = psl+pst*limitX;
					assert(current_index-1<area_size);
				} // for( )
			} // for( ) 
		} // for( )
	} // for( )
}