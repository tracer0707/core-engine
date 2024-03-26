#include "Scene.h"

#include <glm/glm.hpp>

#include "../Math/Mathf.h"
#include "../Components/Camera.h"
#include "../Assets/Mesh.h"
#include "../Assets/Material.h"

#include "../Renderer/Renderer.h"

//Temp. Remove
#include "../Editor/Editor.h"

namespace Core
{
	glm::quat rotation = glm::identity<glm::quat>();
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1.0f);

    static glm::mat4 model = glm::identity<glm::mat4x4>();

	float yRot = 0.0f;

    Core::Mesh* mesh = nullptr;
    Core::Camera* camera = nullptr;

	Scene::Scene()
	{
		camera = new Core::Camera();
		camera->setPosition(glm::vec3(0, 0.15f, -1.5f));

		mesh = Core::Mesh::loadFromFile("D:/Dev/C++/core-engine/x64/Release/Test Project/model.fbx");

        Editor::Editor::setCamera(camera);
        Editor::Editor::setSelectedMtx(&model);
	}

	Scene::~Scene()
	{
	}

	void Scene::render()
	{
        /*glm::mat4 model = glm::identity<glm::mat4x4>();
        glm::mat4 rotMat = glm::mat4_cast(rotation);

        glm::vec3 pos = glm::inverse(rotMat) * (glm::vec4(position, 1.0f));

        model = glm::translate(model, pos);
        model = rotMat * model;
        model = glm::scale(model, scale);

        yRot += 1.0f;
        rotation = Core::Mathf::toQuaternion(glm::vec3(90.0f, 0, 0)) * Core::Mathf::toQuaternion(glm::vec3(0, 0, yRot));*/

        int w = Renderer::singleton()->getWidth();
        int h = Renderer::singleton()->getHeight();

        for (int i = 0; i < mesh->getSubMeshesCount(); ++i)
        {
            Core::SubMesh* subMesh = mesh->getSubMesh(i);
            Core::Material* material = subMesh->getMaterial();

            glm::mat4 view = camera->getViewMatrix();
            glm::mat4 proj = camera->getProjectionMatrix((float)w / (float)h);

            Core::Renderer::singleton()->bindBuffer(subMesh->getVertexBuffer());

            if (material != nullptr)
                material->bind();

            Core::Renderer::singleton()->drawBuffer(subMesh->getVertexBuffer(), view, proj, model);
        }
	}
}