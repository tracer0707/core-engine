#include "Primitives.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "../Shared/List.h"

#include "../Assets/Material.h"

namespace Core
{
	void Primitives::lines(glm::mat4& view, glm::mat4& proj, glm::mat4& model, Vertex* points, int pointsCount, UInt32 flags)
	{
		const Core::VertexBuffer* vb = Renderer::singleton()->createBuffer(points, pointsCount, nullptr, 0);
		
		Material::defaultMaterial->bind();

		Renderer::singleton()->bindBuffer(vb);
		Renderer::singleton()->drawBuffer(vb, GL_LINES, flags, view, proj, model);
		Renderer::singleton()->deleteBuffer(vb);
	}

	void Primitives::wireCube(glm::mat4& view, glm::mat4& proj, glm::mat4& model, glm::vec3 size, glm::vec3 center, Color color, UInt32 flags)
	{
		Core::List<Core::Vertex> verts;

		/* BOTTOM */

		/* -X */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y, size.x * center.x), { 0, 0 }, color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y, size.z * center.z - size.z), { 0, 0 }, color));

		/* +X */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y, size.z * center.z), { 0, 0 }, color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y, size.z * center.z - size.z), { 0, 0 }, color));

		/* -Z */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y, size.z * center.z), { 0, 0 }, color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y, size.z * center.z), { 0, 0 }, color));

		/* +Z */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y, size.z * center.z - size.z), { 0, 0 }, color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y, size.z * center.z - size.z), { 0, 0 }, color));

		/* TOP */

		/* -X */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y + size.y, size.z * center.z), { 0, 0 }, color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y + size.y, size.z * center.z - size.z), { 0, 0 }, color));

		/* +X */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y + size.y, size.z * center.z), { 0, 0 }, color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y + size.y, size.z * center.z - size.z), { 0, 0 }, color));

		/* -Z */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y + size.y, size.z * center.z), { 0, 0 }, color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y + size.y, size.z * center.z), { 0, 0 }, color));

		/* +Z */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y + size.y, size.z * center.z - size.z), { 0, 0 }, color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y + size.y, size.z * center.z - size.z), { 0, 0 }, color));

		/* SIDE */

		/* -X */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y, size.z * center.z), { 0, 0 }, color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y + size.y, size.z * center.z), { 0, 0 }, color));

		/* -X */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y, size.z * center.z - size.z), { 0, 0 }, color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x, size.y * center.y + size.y, size.z * center.z - size.z), { 0, 0 }, color));

		/* +X */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y, size.z * center.z), { 0, 0 }, color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y + size.y, size.z * center.z), { 0, 0 }, color));

		/* +X */
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y, size.z * center.z - size.z), { 0, 0 }, color));
		verts.add(Core::Vertex(glm::vec3(size.x * center.x - size.x, size.y * center.y + size.y, size.z * center.z - size.z), { 0, 0 }, color));

		Core::Primitives::lines(view, proj, model, verts.ptr(), verts.count(), flags);
	}

	void Primitives::wireMesh(glm::mat4& view, glm::mat4& proj, glm::mat4& model, List<glm::vec3>& vertices, List<int>& indices, Color color, WireframeMode mode, UInt32 flags)
	{
		if (indices.count() == 0) return;

		Core::List<Core::Vertex> verts;

		if (mode == WireframeMode::Triangle)
		{
			if (indices.count() % 3 != 0)
			{
				throw std::exception("Indices count is not multiple of 3!");
			}

			for (int i = 0; i < indices.count(); i += 3)
			{
				int i0 = indices.get(i);
				int i1 = indices.get(i + 1);
				int i2 = indices.get(i + 2);

				verts.add(Core::Vertex(vertices.get(i0), { 0, 0 }, color));
				verts.add(Core::Vertex(vertices.get(i1), { 0, 0 }, color));
				verts.add(Core::Vertex(vertices.get(i1), { 0, 0 }, color));
				verts.add(Core::Vertex(vertices.get(i2), { 0, 0 }, color));
				verts.add(Core::Vertex(vertices.get(i2), { 0, 0 }, color));
				verts.add(Core::Vertex(vertices.get(i0), { 0, 0 }, color));
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

					verts.add(Core::Vertex(vertices.get(i0), { 0, 0 }, color));
					verts.add(Core::Vertex(vertices.get(i1), { 0, 0 }, color));
					verts.add(Core::Vertex(vertices.get(i1), { 0, 0 }, color));
					verts.add(Core::Vertex(vertices.get(i2), { 0, 0 }, color));
					verts.add(Core::Vertex(vertices.get(i2), { 0, 0 }, color));
					verts.add(Core::Vertex(vertices.get(i3), { 0, 0 }, color));
					verts.add(Core::Vertex(vertices.get(i3), { 0, 0 }, color));
					verts.add(Core::Vertex(vertices.get(i0), { 0, 0 }, color));
				}
			}
			else if (indices.count() % 3 == 0) // Polygon from two triangles
			{
				//TODO
			}
			else
			{
				throw std::exception("Indices count is not multiple of 3 or 4!");
			}
		}

		Core::Primitives::lines(view, proj, model, verts.ptr(), verts.count(), flags);
	}
}