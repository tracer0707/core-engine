#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Core/Shared/List.h>
#include <Core/Renderer/Color.h>

namespace Core
{
	struct Vertex;
	class VertexBuffer;
	class Renderer;
}

namespace Editor
{
	class EditorCamera;

	class Primitives
	{
		public:
			enum class WireframeMode
			{
				Triangle,
				Polygon
			};

			static void lines(Core::Renderer* renderer, Core::VertexBuffer* buffer, glm::mat4& view, glm::mat4& proj, glm::mat4& model,
							  Core::Vertex* points, unsigned int pointsCount, unsigned int flags);

			static void wireCube(Core::Renderer* renderer, Core::VertexBuffer* buffer, glm::mat4& view, glm::mat4& proj, glm::mat4& model, glm::vec3 size,
								 glm::vec3 center, Core::Color color, unsigned int flags);

			static void wireMesh(Core::Renderer* renderer, Core::VertexBuffer* buffer, glm::mat4& view, glm::mat4& proj, glm::mat4& model,
								 Core::List<glm::vec3>& vertices, Core::List<uint32_t>& indices, Core::Color color, WireframeMode mode,
								 unsigned int flags);

			static void sphere(Core::Renderer* renderer, Core::VertexBuffer* buffer, glm::mat4& view, glm::mat4& proj, glm::mat4& model,
							   unsigned int sectors, unsigned int stacks, unsigned int flags);

			static void grid(Core::Renderer* renderer, Core::VertexBuffer* buffer, EditorCamera* camera);
	};
} // namespace Core