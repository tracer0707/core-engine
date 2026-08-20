#include "Raycast.h"

#include <iostream>

#include <Core/Math/Mathf.h>
#include <Core/Scene/Scene.h>
#include <Core/Scene/Object.h>
#include <Core/Content/Mesh.h>
#include <Core/Components/MeshRenderer.h>
#include <Core/Components/Transform.h>
#include <Core/Renderer/VertexBuffer.h>

namespace Editor
{
	bool Raycast::hitTest(Core::Scene* scene, Core::Ray& ray, RaycastHit* outHit)
	{
		bool hit = false;

		Core::List<std::pair<Core::Object*, float>> boundsIntersected;
		Core::List<Core::Object*>& objects = scene->getObjects();

		for (auto* obj : objects)
		{
			Core::MeshRenderer* meshRenderer = obj->findComponent<Core::MeshRenderer*>();
			if (meshRenderer == nullptr) continue;

			auto aab = meshRenderer->getWorldBoundingBox();
			auto boundsHit = aab.intersects(ray.origin, ray.direction);

			if (boundsHit.first)
			{
				boundsIntersected.add(std::make_pair(obj, boundsHit.second));
			}
		}

		boundsIntersected.sort([=](std::pair<Core::Object*, float>& a, std::pair<Core::Object*, float>& b) -> bool { return a.second < b.second; });

		for (auto& b : boundsIntersected)
		{
			Core::Object* obj = b.first;
			Core::MeshRenderer* meshRenderer = obj->findComponent<Core::MeshRenderer*>();
			Core::Transform* transform = obj->getTransform();
			Core::Mesh* mesh = meshRenderer->getMesh();

			if (transform == nullptr) continue;

			glm::mat4 mtx = transform->getTransformMatrix();

			if (meshTest(ray, mesh, mtx))
			{
				outHit->object = obj;
				outHit->mesh = mesh;
				return true;
			}
		}

		return false;
	}

	bool Raycast::meshTest(Core::Ray& ray, Core::Mesh* mesh, glm::mat4& mtx)
	{
		bool foundHit = false;
		float closestDistance = std::numeric_limits<float>::max();

		CSGBrush* closestBrush = nullptr;
		size_t closestFaceId = -1;

		Core::VertexBuffer* vb = mesh->getVertexBuffer();

		int sz = vb->indexArraySize > 0 ? vb->indexArraySize : vb->vertexArraySize;

		for (int j = 0; j < sz; j += 3)
		{
			Core::Vertex& v1 = vb->vertexArray[vb->indexArraySize > 0 ? vb->indexArray[j] : j];
			Core::Vertex& v2 = vb->vertexArray[vb->indexArraySize > 0 ? vb->indexArray[j + 1] : j + 1];
			Core::Vertex& v3 = vb->vertexArray[vb->indexArraySize > 0 ? vb->indexArray[j + 2] : j + 2];

			glm::vec3 p1 = mtx * glm::vec4(v1._position, 1.0f);
			glm::vec3 p2 = mtx * glm::vec4(v2._position, 1.0f);
			glm::vec3 p3 = mtx * glm::vec4(v3._position, 1.0f);

			auto hit = Core::Mathf::intersects(ray, p1, p2, p3, true, false);

			if (!hit.first) continue;

			if (hit.second < closestDistance)
			{
				closestDistance = hit.second;
				foundHit = true;

				for (int i = 0; i < mesh->getSubMeshCount(); ++i)
				{
					Core::SubMesh* subMesh = mesh->getSubMesh(i);
					if (j >= subMesh->getIndexOffset() && j < subMesh->getIndexOffset() + subMesh->getIndexCount())
					{
						//SubMesh found
						break;
					}
				}

				//TODO
			}
		}
		
		if (!foundHit) return false;

		return true;
	}
} // namespace Editor