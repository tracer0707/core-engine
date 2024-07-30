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
	Scene::Scene()
	{
		
	}

	Scene::~Scene()
	{

	}

    Object* Scene::createObject()
    {
        Object* object = new Object();
        objects.add(object);

        return object;
    }

    void Scene::addObject(Object* object)
    {
        if (object != nullptr)
            objects.add(object);
    }

    void Scene::removeObject(Object* object)
    {
        objects.remove(object);
    }

    void Scene::render()
	{
        if (mainCamera == nullptr) return;

        float dt = 0.0f;

        for (int i = 0; i < objects.count(); ++i)
        {
            Object* object = objects.get(i);
            object->update(dt);
            object->render(mainCamera);
        }
	}
}