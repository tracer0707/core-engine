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
    MeshRenderer::MeshRenderer(Object* owner, Renderer* renderer) : Component(owner)
    {
        _renderer = renderer;
    }

    MeshRenderer::~MeshRenderer()
    {
        mesh = nullptr;
    }

    unsigned int MeshRenderer::getComponentType()
    {
        return COMPONENT_MESHRENDERER;
    }

    AxisAlignedBox MeshRenderer::getWorldBoundingBox()
    {
        if (mesh == nullptr) return AxisAlignedBox::BOX_NULL;

        Transform* transform = owner->findComponent<Transform*>();

        AxisAlignedBox aab = mesh->getBoundingBox();
        aab.transform(transform->getTransformMatrix());

        return aab;
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

            _renderer->bindBuffer(subMesh->getVertexBuffer());

            glm::mat4& model = transform->getTransformMatrix();

            _renderer->drawBuffer(subMesh->getVertexBuffer(), GL_TRIANGLES,
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