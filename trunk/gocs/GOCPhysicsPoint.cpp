#include "GOCPhysicsPoint.h"
#include "Templates/GOCTPhysicsPoint.h"
#include "CGameObject.h"
#include "../Math/Random.h"
using namespace tlib::gocs;

GOCPhysicsPoint::GOCPhysicsPoint( const GOCTPhysicsPoint * const tpl ):
m_Body(tpl->GetBody())
{}

void GOCPhysicsPoint::Init()
{
    // The only thing we MUST do is copy the original's body location
    // from the object's transformation node
    
    m_Body.SetPosition( GetOwner()->GetTransform().GetPosition() );
}

void GOCPhysicsPoint::Update( float delta )
{
    m_Body.SetAcceleration(m_vAcceleration);
    m_Body.Integrate(delta);

    // After updating copy the particle's position to the object's
    //GetOwner()->GetTransform().SavePosition();
    GetOwner()->GetTransform().SetPosition( m_Body.GetPosition() );
}