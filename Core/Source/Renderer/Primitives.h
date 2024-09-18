#pragma once

#include "../Config.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Color.h"

namespace Core
{
	class Vertex;

	class Primitives
	{
	public:
		static void lines(glm::mat4& view, glm::mat4& proj, glm::mat4& model, Vertex* points, int pointsCount, UInt32 flags);
		static void wireCube(glm::mat4& view, glm::mat4& proj, glm::mat4& model, glm::vec3 size, glm::vec3 center, Color color, UInt32 flags);
	};
}