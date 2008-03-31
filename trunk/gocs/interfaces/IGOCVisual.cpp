#include "IGOCVisual.h"
#include "../../CSceneManager.h"
using namespace tlib::gocs;

IGOCVisual::IGOCVisual()
{
    // Every visual component is automatically registered to the scene
    // manager in order for its owner to be rendered.

    MGRScene::_Get().Register( this );
}

IGOCVisual::~IGOCVisual()
{
    MGRScene::_Get().Unregister( this );
}