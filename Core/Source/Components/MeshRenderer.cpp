#include "MeshRenderer.h"

#include "ComponentList.h"

#include "Transform.h"
#include "../Assets/Material.h"
#include "../Assets/Mesh.h"

namespace Core
{
    MeshRenderer::MeshRenderer(Object* owner) : Component(owner) {}
    MeshRenderer::~MeshRenderer() {}

    UInt32 MeshRenderer::getComponentType()
    {
        return COMPONENT_MESHRENDERER;
    }

    void MeshRenderer::render()
    {
        Transform* transform = object->findComponent<Transform*>();
        Mesh* mesh = it->getMesh();

        if (mesh == nullptr) return;

        for (int i = 0; i < mesh->getSubMeshesCount(); ++i)
        {
            Core::SubMesh* subMesh = mesh->getSubMesh(i);
            Core::Material* material = subMesh->getMaterial();

            Core::Renderer::singleton()->bindBuffer(subMesh->getVertexBuffer());

            if (material != nullptr)
                material->bind();

            glm::mat4 model = transform->getTransformMatrix();

            Core::Renderer::singleton()->drawBuffer(subMesh->getVertexBuffer(), view, proj, model);
        }
    }
}