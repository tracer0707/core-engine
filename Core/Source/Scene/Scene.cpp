#include "Scene.h"

#include <glm/glm.hpp>

#include "../Math/Mathf.h"
#include "../Components/Camera.h"
#include "../Components/Transform.h"
#include "../Assets/Mesh.h"
#include "../Assets/Material.h"
#include "../Scene/CSGModel.h"

#include "../Renderer/Renderer.h"

namespace Core
{
	Scene::Scene()
	{
		//mesh = Core::Mesh::loadFromFile("D:/Dev/C++/core-engine/x64/Release/Test Project/model.fbx");

        //Editor::Editor::setCamera(camera);
        //Editor::Editor::setSelectedMtx(&model);
	}

	Scene::~Scene()
	{
	}

	void Scene::render(Camera* camera)
	{
        int w = Renderer::singleton()->getWidth();
        int h = Renderer::singleton()->getHeight();

        if (camera == nullptr) return;

        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 proj = camera->getProjectionMatrix((float)w / (float)h);

        for (auto* it : csgModels)
        {
            Transform* transform = it->getTransform();
            Mesh* mesh = it->getMesh();

            if (mesh == nullptr) continue;

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
}