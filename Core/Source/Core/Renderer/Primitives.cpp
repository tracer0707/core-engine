#include "Primitives.h"

#include <stdexcept>

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
						   int pointsCount, unsigned int flags)
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
} // namespace Core