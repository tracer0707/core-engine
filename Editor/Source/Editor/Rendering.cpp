#include "Rendering.h"

#include <Core/Interface/Transform.h>
#include <Core/Renderer/Color.h>
#include <Core/Renderer/Renderer.h>
#include <Core/Renderer/Primitives.h>
#include <Core/Scene/Object.h>

#include "EditorCamera.h"

namespace Editor
{
	void Rendering::renderGrid(Core::Renderer* renderer, Core::VertexBuffer* buffer, EditorCamera* camera)
	{
		Core::Color lineColor = Core::Color(0.8f, 0.8f, 0.8f, 0.8f);
		Core::Color xColor = Core::Color(0.8f, 0.0f, 0.0f, 0.8f);
		Core::Color zColor = Core::Color(0.0f, 0.0f, 0.8f, 0.8f);

		Core::Transform* t = camera->getTransform();
		glm::vec3 camPos = t->getPosition();

		Core::List<Core::Vertex> verts1;
		Core::List<Core::Vertex> verts2;

		float cellSize = 1.0f;
		float cellCount = 64.0f;
		int startX = camPos.x / cellSize;
		int startY = camPos.z / cellSize;

		int halfSize = (cellCount * cellSize) / 2.0f;

		int idx = 0;
		for (int j = startX; j < startX + cellCount; ++j, idx += 2)
		{
			glm::vec2 p1 = glm::vec2((j * cellSize), (startY * cellSize)) - glm::vec2(halfSize);
			glm::vec2 p2 = glm::vec2((j * cellSize), (startY * cellSize + cellCount * cellSize)) - glm::vec2(halfSize) - glm::vec2(0.0f, cellSize);

			Core::Vertex v1(glm::vec3(p1.x, 0, p1.y), lineColor);
			Core::Vertex v2(glm::vec3(p2.x, 0, p2.y), lineColor);

			verts1.add(v1);
			verts1.add(v2);
		}

		for (int i = startY; i < startY + cellCount; ++i, idx += 2)
		{
			glm::vec2 p1 = glm::vec2((startX * cellSize), (i * cellSize)) - glm::vec2(halfSize);
			glm::vec2 p2 = glm::vec2((startX * cellSize + cellCount * cellSize), (i * cellSize)) - glm::vec2(halfSize) - glm::vec2(cellSize, 0.0f);

			Core::Vertex v1(glm::vec3(p1.x, 0, p1.y), lineColor);
			Core::Vertex v2(glm::vec3(p2.x, 0, p2.y), lineColor);

			verts1.add(v1);
			verts1.add(v2);
		}

		startX = camPos.x / 64.0f * 64.0f;
		int startZ = camPos.z / 64.0f * 64.0f;

		glm::vec3 px1 = glm::vec3(startX - 32.0f, 0.0f, 0.0f);
		glm::vec3 px2 = glm::vec3(startX + 31.0f, 0.0f, 0.0f);

		glm::vec3 pz1 = glm::vec3(0.0f, 0.0f, startZ - 32.0f);
		glm::vec3 pz2 = glm::vec3(0.0f, 0.0f, startZ + 31.0f);

		if (startZ > -32.0f && startZ < 33.0f)
		{
			Core::Vertex v1(px1, xColor);
			Core::Vertex v2(px2, xColor);

			verts2.add(v1);
			verts2.add(v2);
		}

		if (startX > -32.0f && startX < 33.0f)
		{
			Core::Vertex v3(pz1, zColor);
			Core::Vertex v4(pz2, zColor);

			verts2.add(v3);
			verts2.add(v4);
		}

		unsigned int flags = C_CCW | C_CULL_BACK | C_ENABLE_DEPTH_TEST | C_ENABLE_DEPTH_WRITE | C_ENABLE_CULL_FACE | C_DEPTH_LEQUAL;

		glm::mat4 view = camera->getViewMatrix();
		glm::mat4 proj = camera->getProjectionMatrix();
		glm::mat4 mtx = glm::identity<glm::mat4>();

		Core::Primitives::lines(renderer, buffer, view, proj, mtx, verts1.ptr(), verts1.count(), flags);

		if (verts2.count() > 0) Core::Primitives::lines(renderer, buffer, view, proj, mtx, verts2.ptr(), verts2.count(), flags);
	}
} // namespace Editor