#include "Primitives.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

#include <Core/Shared/Hash.h>
#include <Core/Renderer/Renderer.h>
#include <Core/Renderer/Program.h>
#include <Core/Renderer/VertexBuffer.h>

#include <Core/Interface/Transform.h>
#include <Core/Content/Material.h>

#include "EditorCamera.h"

namespace Editor
{
	uint64_t u_color_Hash = Hash("u_color");

	void Primitives::lines(Core::Renderer* renderer, Core::VertexBuffer* buffer, glm::mat4& view, glm::mat4& proj, glm::mat4& model,
						   Core::Vertex* points, unsigned int pointsCount, unsigned int flags)
	{
		renderer->updateBuffer(buffer, points, pointsCount, nullptr, 0);
		renderer->bindProgram(renderer->getUnlitColorProgram());
		renderer->setUniform(renderer->getUnlitColorProgram()->getUniformLocation(u_color_Hash), glm::vec4(1.0f));
		renderer->bindBuffer(buffer, flags, view, proj, model);
		renderer->drawBufferArray(Core::PrimitiveType::Line, 0, pointsCount);
	}

	void Primitives::wireCube(Core::Renderer* renderer, Core::VertexBuffer* buffer, glm::mat4& view, glm::mat4& proj, glm::mat4& model,
							  glm::vec3 size, glm::vec3 center, Core::Color color, unsigned int flags)
	{
		Core::List<Core::Vertex> verts;

		/* BOTTOM */

		/* -X */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y, size.x * center.x), color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y, size.z * center.z - size.z), color));

		/* +X */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y, size.z * center.z), color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y, size.z * center.z - size.z), color));

		/* -Z */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y, size.z * center.z), color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y, size.z * center.z), color));

		/* +Z */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y, size.z * center.z - size.z), color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y, size.z * center.z - size.z), color));

		/* TOP */

		/* -X */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y + size.y, size.z * center.z), color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y + size.y, size.z * center.z - size.z), color));

		/* +X */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y + size.y, size.z * center.z), color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y + size.y, size.z * center.z - size.z), color));

		/* -Z */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y + size.y, size.z * center.z), color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y + size.y, size.z * center.z), color));

		/* +Z */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y + size.y, size.z * center.z - size.z), color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y + size.y, size.z * center.z - size.z), color));

		/* SIDE */

		/* -X */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y, size.z * center.z), color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y + size.y, size.z * center.z), color));

		/* -X */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y, size.z * center.z - size.z), color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y + size.y, size.z * center.z - size.z), color));

		/* +X */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y, size.z * center.z), color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y + size.y, size.z * center.z), color));

		/* +X */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y, size.z * center.z - size.z), color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y + size.y, size.z * center.z - size.z), color));

		Primitives::lines(renderer, buffer, view, proj, model, verts.ptr(), verts.count(), flags);
	}

	void Primitives::wireMesh(Core::Renderer* renderer, Core::VertexBuffer* buffer, glm::mat4& view, glm::mat4& proj, glm::mat4& model,
							  Core::List<glm::vec3>& vertices, Core::List<uint32_t>& indices, Core::Color color, WireframeMode mode,
							  unsigned int flags)
	{
		if (indices.count() == 0) return;

		Core::List<Core::Vertex> verts;

		if (mode == WireframeMode::Triangle)
		{
			if (indices.count() % 3 != 0)
			{
				throw std::runtime_error("Indices count is not multiple of 3!");
			}

			for (int i = 0; i < indices.count(); i += 3)
			{
				int i0 = indices.get(i);
				int i1 = indices.get(i + 1);
				int i2 = indices.get(i + 2);

				verts.add(Core::Vertex(vertices.get(i0), color));
				verts.add(Core::Vertex(vertices.get(i1), color));
				verts.add(Core::Vertex(vertices.get(i1), color));
				verts.add(Core::Vertex(vertices.get(i2), color));
				verts.add(Core::Vertex(vertices.get(i2), color));
				verts.add(Core::Vertex(vertices.get(i0), color));
			}
		}
		else if (mode == WireframeMode::Polygon)
		{
			if (indices.count() % 4 == 0)
			{
				for (int i = 0; i < indices.count(); i += 4)
				{
					int i0 = indices.get(i);
					int i1 = indices.get(i + 1);
					int i2 = indices.get(i + 2);
					int i3 = indices.get(i + 3);

					verts.add(Core::Vertex(vertices.get(i0), color));
					verts.add(Core::Vertex(vertices.get(i1), color));
					verts.add(Core::Vertex(vertices.get(i1), color));
					verts.add(Core::Vertex(vertices.get(i2), color));
					verts.add(Core::Vertex(vertices.get(i2), color));
					verts.add(Core::Vertex(vertices.get(i3), color));
					verts.add(Core::Vertex(vertices.get(i3), color));
					verts.add(Core::Vertex(vertices.get(i0), color));
				}
			}
			else if (indices.count() % 3 == 0) // Polygon from two triangles
			{
				// TODO
			}
			else
			{
				throw std::runtime_error("Indices count is not multiple of 3 or 4!");
			}
		}

		Primitives::lines(renderer, buffer, view, proj, model, verts.ptr(), verts.count(), flags);
	}

	void Primitives::sphere(Core::Renderer* renderer, Core::VertexBuffer* buffer, glm::mat4& view, glm::mat4& proj, glm::mat4& model,
							unsigned int sectors, unsigned int stacks, unsigned int flags)
	{
		if (renderer == nullptr || buffer == nullptr || buffer->getIndexArraySize() == 0)
		{
			return;
		}

		std::vector<Core::Vertex> vertices;
		std::vector<unsigned int> indices;
		
		constexpr float pi = 3.14159265358979323846f;
		constexpr float twoPi = pi * 2.0f;

		for (unsigned int stack = 0; stack <= stacks; ++stack)
		{
			float stackAngle = pi * 0.5f - static_cast<float>(stack) * pi / static_cast<float>(stacks);
			float xy = std::cos(stackAngle);
			float z = std::sin(stackAngle);

			for (unsigned int sector = 0; sector <= sectors; ++sector)
			{
				float sectorAngle = static_cast<float>(sector) * twoPi / static_cast<float>(sectors);
				glm::vec3 normal(xy * std::cos(sectorAngle), z, xy * std::sin(sectorAngle));
				glm::vec2 uv(static_cast<float>(sector) / static_cast<float>(sectors), static_cast<float>(stack) / static_cast<float>(stacks));
				vertices.push_back(Core::Vertex(normal, normal, uv, Core::Color::WHITE));
			}
		}

		for (unsigned int stack = 0; stack < stacks; ++stack)
		{
			unsigned int first = stack * (sectors + 1u);
			unsigned int second = first + sectors + 1u;

			for (unsigned int sector = 0; sector < sectors; ++sector)
			{
				indices.push_back(first + sector);
				indices.push_back(second + sector);
				indices.push_back(first + sector + 1u);
				indices.push_back(first + sector + 1u);
				indices.push_back(second + sector);
				indices.push_back(second + sector + 1u);
			}
		}

		renderer->updateBuffer(buffer, vertices.data(), static_cast<unsigned int>(vertices.size()), indices.data(), static_cast<unsigned int>(indices.size()));
		renderer->bindBuffer(buffer, flags, view, proj, model);
		renderer->drawBufferIndexed(Core::PrimitiveType::Triangle, 0, buffer->getIndexArraySize());
	}

	void Primitives::grid(Core::Renderer* renderer, Core::VertexBuffer* buffer, EditorCamera* camera)
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

		Primitives::lines(renderer, buffer, view, proj, mtx, verts1.ptr(), verts1.count(), flags);

		if (verts2.count() > 0) Primitives::lines(renderer, buffer, view, proj, mtx, verts2.ptr(), verts2.count(), flags);
	}
} // namespace Core