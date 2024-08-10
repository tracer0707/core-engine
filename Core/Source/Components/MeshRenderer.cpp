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

        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 proj = camera->getProjectionMatrix();

        for (int i = 0; i < mesh->getSubMeshesCount(); ++i)
        {
            SubMesh* subMesh = mesh->getSubMesh(i);
            Material* material = subMesh->getMaterial();

            if (material != nullptr)
                material->bind();

            Renderer::singleton()->bindBuffer(subMesh->getVertexBuffer());

            glm::mat4 model = transform->getTransformMatrix();

            Renderer::singleton()->drawBuffer(subMesh->getVertexBuffer(), GL_TRIANGLES,
                C_CCW
                | C_CULL_BACK
                | C_ENABLE_DEPTH_TEST
                | C_ENABLE_DEPTH_WRITE
                | C_ENABLE_CULL_FACE
                | C_DEPTH_LEQUAL,
                view, proj, model);
        }
    }
}