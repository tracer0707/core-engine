#pragma once

#include "../Config.h"

#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Color.h"
#include "../Shared/List.h"

namespace Core
{
	class Vertex;

	class Primitives
	{
	public:
		enum class WireframeMode
		{
			Triangle,
			Polygon
		};

		static void lines(glm::mat4& view, glm::mat4& proj, glm::mat4& model, Vertex* points, int pointsCount, UInt32 flags);
		static void wireCube(glm::mat4& view, glm::mat4& proj, glm::mat4& model, glm::vec3 size, glm::vec3 center, Color color, UInt32 flags);
		static void wireMesh(glm::mat4& view, glm::mat4& proj, glm::mat4& model, List<glm::vec3>& vertices, List<int>& indices, Color color, WireframeMode mode, UInt32 flags);
	};
}