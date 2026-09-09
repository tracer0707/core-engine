#include "Primitives.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

#include "Renderer.h"
#include "Program.h"
#include "VertexBuffer.h"
#include "../Shared/List.h"
#include "../Shared/Hash.h"

#include "../Content/Material.h"

namespace Core
{
	uint64_t u_color_Hash = Hash("u_color");

	void Primitives::lines(Renderer* renderer, VertexBuffer* buffer, glm::mat4& view, glm::mat4& proj, glm::mat4& model, Vertex* points,
						   unsigned int pointsCount, unsigned int flags)
	{
		renderer->updateBuffer(buffer, points, pointsCount, nullptr, 0);
		renderer->bindProgram(renderer->getUnlitColorProgram());
		for (auto& uniform : renderer->getUnlitColorProgram()->uniforms)
		{
			if (uniform.nameHash == u_color_Hash)
			{
				renderer->setUniform(uniform.location, glm::vec4(1.0f));
				break;
			}
		}
		renderer->bindBuffer(buffer, flags, view, proj, model);
		renderer->drawBufferArray(PrimitiveType::Line, 0, pointsCount);
	}

	void Primitives::wireCube(Renderer* renderer, VertexBuffer* buffer, glm::mat4& view, glm::mat4& proj, glm::mat4& model, glm::vec3 size,
							  glm::vec3 center, Color color, unsigned int flags)
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

		Core::Primitives::lines(renderer, buffer, view, proj, model, verts.ptr(), verts.count(), flags);
	}

	void Primitives::wireMesh(Renderer* renderer, VertexBuffer* buffer, glm::mat4& view, glm::mat4& proj, glm::mat4& model, List<glm::vec3>& vertices,
							  List<uint32_t>& indices, Color color, WireframeMode mode, unsigned int flags)
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

		Core::Primitives::lines(renderer, buffer, view, proj, model, verts.ptr(), verts.count(), flags);
	}

	void Primitives::sphere(Renderer* renderer, VertexBuffer* buffer, glm::mat4& view, glm::mat4& proj, glm::mat4& model, unsigned int sectors,
							unsigned int stacks, unsigned int flags)
	{
		if (renderer == nullptr || buffer == nullptr || buffer->getIndexArraySize() == 0)
		{
			return;
		}

		std::vector<Vertex> vertices;
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
				vertices.push_back(Vertex(normal, normal, uv, Color::WHITE));
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
		renderer->drawBufferIndexed(PrimitiveType::Triangle, 0, buffer->getIndexArraySize());
	}
} // namespace Core