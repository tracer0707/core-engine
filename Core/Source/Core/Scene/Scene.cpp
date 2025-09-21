#include "Scene.h"

#include <glm/glm.hpp>

#include "Object.h"

#include "../Math/Mathf.h"
#include "../Components/Camera.h"
#include "../Components/Transform.h"
#include "../Assets/Mesh.h"
#include "../Assets/Material.h"

#include "../Renderer/Renderer.h"

namespace Core
{
	Scene::Scene(Renderer* renderer)
	{
        _renderer = renderer;
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

        float dt = 0.0f;

        for (int i = 0; i < _objects.count(); ++i)
        {
            Object* object = _objects.get(i);
            object->update(dt);
            object->render(_mainCamera);
        }
	}
}