#include "Primitives.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "../Assets/Material.h"

namespace Core
{
	void Primitives::lines(glm::mat4& view, glm::mat4& proj, glm::mat4& model, Vertex* points, int pointsCount, UInt32 flags)
	{
		const Core::VertexBuffer* vb = Renderer::singleton()->createBuffer(points, pointsCount, nullptr, 0);

		Renderer::singleton()->bindBuffer(vb);
		Material::defaultMaterial->bind();
		Renderer::singleton()->drawBuffer(vb, GL_LINES, flags, view, proj, model);
		Renderer::singleton()->deleteBuffer(vb);
	}
}