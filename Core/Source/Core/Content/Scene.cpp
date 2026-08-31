#include "Scene.h"

#include <glm/glm.hpp>

#include "../Scene/Object.h"

#include "../Math/Mathf.h"
#include "../System/Time.h"
#include "../Components/Camera.h"
#include "../Content/Mesh.h"
#include "../Content/Material.h"

#include "../Renderer/Renderer.h"

namespace Core
{
	Scene::Scene(Renderer* renderer, Time* time)
	{
        _renderer = renderer;
        _time = time;
	}

	Scene::~Scene()
	{
        _renderer = nullptr;
	}

    Object* Scene::createObject()
    {
        Object* object = new Object(_renderer);
        _objects.add(object);

        return object;
    }

    void Scene::removeObject(Object* object)
    {
        _objects.remove(object);
        delete object;
    }

    void Scene::render()
	{
        if (_mainCamera == nullptr) return;

		glm::mat4 view = _mainCamera->getViewMatrix();
		glm::mat4 proj = _mainCamera->getProjectionMatrix();

		render(view, proj);
	}

	void Scene::render(glm::mat4& view, glm::mat4& proj)
    {
		float dt = _time->getDeltaTime();

		for (int i = 0; i < _objects.count(); ++i)
		{
			Object* object = _objects.get(i);
			object->update(dt);
			object->render(view, proj);
		}
    }
}