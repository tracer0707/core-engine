#include "MeshRenderer.h"

#include "ComponentList.h"

#include "Transform.h"

#include "../Scene/Object.h"
#include "../Assets/Material.h"
#include "../Assets/Mesh.h"
#include "../Components/Camera.h"
#include "../Renderer/Renderer.h"

namespace Core
{
    MeshRenderer::MeshRenderer(Object* owner) : Component(owner) {}
    MeshRenderer::~MeshRenderer()
    {
        mesh = nullptr;
    }

    UInt32 MeshRenderer::getComponentType()
    {
        return COMPONENT_MESHRENDERER;
    }

    void MeshRenderer::render(Camera* camera)
    {
        if (mesh == nullptr) return;

        Transform* transform = owner->findComponent<Transform*>();

        int w = Renderer::singleton()->getWidth();
        int h = Renderer::singleton()->getHeight();

        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 proj = camera->getProjectionMatrix((float)w / (float)h);

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