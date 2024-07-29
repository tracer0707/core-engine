#include "MeshRenderer.h"

#include "ComponentList.h"

namespace Core
{
    MeshRenderer::MeshRenderer(Object* owner) : Component(owner) {}
    MeshRenderer::~MeshRenderer() {}

    UInt32 MeshRenderer::getComponentType()
    {
        return COMPONENT_MESHRENDERER;
    }
}