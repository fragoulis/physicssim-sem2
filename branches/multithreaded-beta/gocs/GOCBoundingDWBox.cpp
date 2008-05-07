#include "GOCBoundingDWBox.h"
#include "Templates/GOCTBoundingDWBox.h"
#include "GOCBoundingBox.h"
#include "GOCVisualVAPlane.h"
using namespace tlib::gocs;

#include "../Math/TVector3.h"
using tlib::math::Vec3f;


GOCBoundingDWBox::GOCBoundingDWBox( const GOCTBoundingDWBox * const tpl ):
GOCBoundingDeformable(tpl)
{
    GOCBoundingBox *box = new GOCBoundingBox;
    box->SetHalfSize(tpl->GetHalfSize());

    m_Primitive = (IGOCBoundingVolume*)box;
}

void GOCBoundingDWBox::WrapObject()
{
    GOCVisualVAPlane *visual = (GOCVisualVAPlane*)m_Visual;

    // Initialize minimum and maximun points
    Vec3f dummy;
    visual->GetVertex(0, dummy);

    float 
        minX = dummy.x(),
        minY = dummy.y(),
        minZ = dummy.z();

    float 
        maxX = minX, 
        maxY = minY, 
        maxZ = minZ;
    
    for( int i=1; i<visual->GetNumOfVertices(); ++i )
    {
        visual->GetVertex(i, dummy);
        
        if( dummy.x() < minX ) minX = dummy.x();
        else
        if( dummy.x() > maxX ) maxX = dummy.x();

        if( dummy.y() < minY ) minY = dummy.y();
        else
        if( dummy.y() > maxY ) maxY = dummy.y();

        if( dummy.z() < minZ ) minZ = dummy.z();
        else
        if( dummy.z() > maxZ ) maxZ = dummy.z();
    }

    Vec3f halfsize( maxX - minX, maxY - minY, maxZ - minZ );
    ((GOCBoundingBox*)m_Primitive)->SetHalfSize( halfsize*0.5f );
}