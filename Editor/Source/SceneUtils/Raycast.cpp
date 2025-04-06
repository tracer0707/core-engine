#include "Raycast.h"

#include <iostream>

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

		Core::List<std::pair<Core::Object*, float>> boundsIntersected;
		Core::List<Core::Object*>& objects = scene->getObjects();

		for (auto* obj : objects)
		{
			if (ModifierManager::singleton()->getCurrentModifierName() == CSGModifier::NAME)
			{
				if (!obj->getFlags().getBit(LAYER_CSG)) continue;
			}

			Core::MeshRenderer* meshRenderer = obj->findComponent<Core::MeshRenderer*>();
			auto aab = meshRenderer->getWorldBoundingBox();
			auto boundsHit = aab.intersects(ray.origin, ray.direction);

			if (boundsHit.first)
			{
				boundsIntersected.add(std::make_pair(obj, boundsHit.second));
			}
		}

		boundsIntersected.sort([=](std::pair<Core::Object*, float>& a, std::pair<Core::Object*, float>& b) -> bool {
			return a.second < b.second;
		});

		for (auto& b : boundsIntersected)
		{
			Core::Object* obj = b.first;
			Core::MeshRenderer* meshRenderer = obj->findComponent<Core::MeshRenderer*>();
			Core::Transform* transform = obj->findComponent<Core::Transform*>();
			Core::Mesh* mesh = meshRenderer->getMesh();

			glm::mat4 mtx = transform->getTransformMatrix();
			unsigned long long brushId = -1;

			if (meshTest(ray, mesh, mtx, &brushId))
			{
				outHit->brushId = brushId;
				outHit->object = obj;
				return true;
			}
		}

		return false;
	}

	bool Raycast::meshTest(Core::Ray& ray, Core::Mesh* mesh, glm::mat4& mtx, unsigned long long* brushId)
	{
		CSGModel* csgModel = nullptr;

		if (ModifierManager::singleton()->getCurrentModifierName() == CSGModifier::NAME)
		{
			CSGModifier* mod = (CSGModifier*)ModifierManager::singleton()->getCurrentModifier();
			for (int i = 0; i < mod->getNumCsgModels(); ++i)
			{
				CSGModel* mdl = mod->getCsgModel(i);
				if (mdl->getMeshRenderer()->getMesh() == mesh)
				{
					csgModel = mdl;
				}
			}
		}

		for (int i = 0; i < mesh->getSubMeshesCount(); ++i)
		{
			Core::SubMesh* subMesh = mesh->getSubMesh(i);
			const Core::VertexBuffer* vb = subMesh->getVertexBuffer();

			int sz = vb->indexArraySize > 0 ? vb->indexArraySize : vb->vertexArraySize;

			for (int j = 0; j < sz; j += 3)
			{
				Core::Vertex& v1 = vb->vertexArray[vb->indexArraySize > 0 ? vb->indexArray[j] : j];
				Core::Vertex& v2 = vb->vertexArray[vb->indexArraySize > 0 ? vb->indexArray[j + 1] : j + 1];
				Core::Vertex& v3 = vb->vertexArray[vb->indexArraySize > 0 ? vb->indexArray[j + 2] : j + 2];

				glm::vec3 p1 = mtx * glm::vec4(v1.getPosition(), 1.0f);
				glm::vec3 p2 = mtx * glm::vec4(v2.getPosition(), 1.0f);
				glm::vec3 p3 = mtx * glm::vec4(v3.getPosition(), 1.0f);

				std::pair<bool, float> hit = Core::Mathf::intersects(ray, p1, p2, p3, true, true);

				if (hit.first)
				{
					if (csgModel != nullptr)
					{
						*brushId = csgModel->getBrushId(subMesh, j);
					}

					return true;
				}
			}
		}

		return false;
	}
}