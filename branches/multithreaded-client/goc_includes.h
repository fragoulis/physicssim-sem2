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

#include "GOCS/CGameObject.h"       // TODO: This dependecies exist because of the lack
                                    //       of an inter-component event system.