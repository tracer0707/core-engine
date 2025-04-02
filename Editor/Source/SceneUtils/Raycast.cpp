#include "Raycast.h"

#include <Math/Mathf.h>
#include <Scene/Scene.h>
#include <Scene/Object.h>
#include <Assets/Mesh.h>
#include <Components/MeshRenderer.h>
#include <Components/Transform.h>
#include <Renderer/VertexBuffer.h>

#include "../Shared/Layers.h"
#include "../Editor/Modifiers/ModifierManager.h"
#include "../Editor/Modifiers/CSGModifier.h"
#include "../CSG/CSGModel.h"
#include "../CSG/CSGBrush.h"

namespace Editor
{
	bool Raycast::hitTest(Core::Scene* scene, Core::Ray& ray, RaycastHit* outHit)
	{
		bool hit = false;

		if (ModifierManager::singleton()->getCurrentModifierName() == CSGModifier::NAME)
		{
			return csgTest(scene, ray, outHit);
		}

		return false;
	}

	bool Raycast::csgTest(Core::Scene* scene, Core::Ray& ray, RaycastHit* outHit)
	{
		outHit = nullptr;
		Core::List<Core::Object*> boundsIntersected;
		Core::List<Core::Object*>& objects = scene->getObjects();

		for (auto obj : objects)
		{
			if (!obj->getFlags().getBit(LAYER_CSG)) continue;

			Core::MeshRenderer* meshRenderer = obj->findComponent<Core::MeshRenderer*>();
			auto boundsHit = meshRenderer->getWorldBoundingBox().intersects(ray.origin, ray.direction);

			if (boundsHit.first)
			{
				boundsIntersected.add(obj);
			}
		}

		for (auto obj : boundsIntersected)
		{
			Core::MeshRenderer* meshRenderer = obj->findComponent<Core::MeshRenderer*>();
			Core::Transform* transform = obj->findComponent<Core::Transform*>();
			Core::Mesh* mesh = meshRenderer->getMesh();

			glm::mat4 mtx = transform->getTransformMatrix();

			for (int i = 0; i < mesh->getSubMeshesCount(); ++i)
			{
				Core::SubMesh* subMesh = mesh->getSubMesh(i);
				const Core::VertexBuffer* vb = subMesh->getVertexBuffer();

				if (vb->indexArraySize > 0)
				{
					for (int j = 0; j < vb->indexArraySize; j += 3)
					{
						Core::Vertex& v1 = vb->vertexArray[vb->indexArray[j]];
						Core::Vertex& v2 = vb->vertexArray[vb->indexArray[j + 1]];
						Core::Vertex& v3 = vb->vertexArray[vb->indexArray[j + 2]];

						glm::vec3 p1 = mtx * glm::vec4(v1.getPosition(), 1.0f);
						glm::vec3 p2 = mtx * glm::vec4(v2.getPosition(), 1.0f);
						glm::vec3 p3 = mtx * glm::vec4(v3.getPosition(), 1.0f);

						std::pair<bool, float> hit = Core::Mathf::intersects(ray, p1, p2, p3, true, true);
					}
				}
			}
		}

		return false;
	}
}