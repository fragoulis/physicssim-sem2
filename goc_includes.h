#pragma once

/**
 * I am aware the engine is missing an object manager to get rid
 * of these dependencies as well but there is not time.
 * I created this separate file only to improve code's visual 
 * quality.
 */

// Visual component templates
#include "GOCS/Templates/GOCTVisualVASphere.h"  
#include "GOCS/Templates/GOCTVisualVAPlane.h"
#include "GOCS/Templates/GOCTVisualIMQuad.h"
// Physical component templates
#include "GOCS/Templates/GOCTPhysicsPoint.h"
#include "GOCS/Templates/GOCTPhysicsCloth.h"
// Collision componenet templates
#include "GOCS/Templates/GOCTBoundingSphere.h"
#include "GOCS/Templates/GOCTBoundingPlane.h"
#include "GOCS/Templates/GOCTBoundingFinitePlane.h"
#include "GOCS/Templates/GOCTBoundingDWBox.h"

#include "GOCS/CGameObject.h"       // TODO: This dependecies exist because of the lack
#include "GOCS/GOCBoundingPlane.h"  //       of an inter-component event system.
#include "GOCS/GOCBoundingFinitePlane.h"
#include "GOCS/GOCBoundingDWBox.h"
#include "GOCS/GOCBoundingBox.h"
#include "GOCS/GOCPhysicsCloth.h"