#include "GOCPhysicsPoint.h"
#include "Templates/GOCTPhysicsPoint.h"
#include "CGameObject.h"
#include "../Math/Random.h"
using namespace tlib::gocs;

GOCPhysicsPoint::GOCPhysicsPoint( const GOCTPhysicsPoint * const tpl ):
m_Body(tpl->GetBody())
{
    m_vPrevPosition.Clear();
}

void GOCPhysicsPoint::Init()
{
    // The only thing we MUST do is copy the original's body location
    // from the object's transformation node

    m_Body.SetPosition( GetOwner()->GetPosition() );
    m_Body.SetAcceleration( m_vAcceleration );
}

void GOCPhysicsPoint::Update( float delta )
{
    // Integrate physics
    m_Body.Integrate(delta);
}

void GOCPhysicsPoint::UpdateOwner()
{
    // Save current position
    m_vPrevPosition = GetOwner()->GetPosition();

    // After updating copy the particle's position to the object's
    GetOwner()->SetPosition( m_Body.GetPosition() );
}