#include "GOCBoundingDWBox.h"
#include "Templates/GOCTBoundingDWBox.h"
#include "GOCBoundingBox.h"
#include "GOCPhysicsCloth.h"
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
    GOCPhysicsCloth *body = (GOCPhysicsCloth*)m_Body;

    float 
        minX = body->GetStaticPos(0).x(), 
        maxX = body->GetStaticPos(0).x(), 
        minY = body->GetStaticPos(0).y(), 
        maxY = body->GetStaticPos(0).y(),
        minZ = body->GetStaticPos(0).z(), 
        maxZ = body->GetStaticPos(0).z();
    
    for( int i=1; i<body->GetNumOfStatics(); ++i )
    {
        const Vec3f &pos = body->GetStaticPos(i);
        
        if( pos.x() < minX ) minX = pos.x();
        else
        if( pos.x() > maxX ) maxX = pos.x();

        if( pos.y() < minY ) minY = pos.y();
        else
        if( pos.y() > maxY ) maxY = pos.y();

        if( pos.z() < minZ ) minZ = pos.z();
        else
        if( pos.z() > maxZ ) maxZ = pos.z();
    }

    Vec3f halfsize( maxX - minX, maxY - minY, maxZ - minZ );
    ((GOCBoundingBox*)m_Primitive)->SetHalfSize( halfsize*0.5f );
}