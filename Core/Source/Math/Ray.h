#pragma once

#include <glm/vec3.hpp>

namespace Core
{
	struct Ray
	{
	public:
		glm::vec3 origin = glm::vec3(0, 0, 0);
		glm::vec3 direction = glm::vec3(0, 0, 0);

		Ray() = default;
		~Ray() = default;

		Ray(glm::vec3 start, glm::vec3 dir)
		{
			origin = start;
			direction = dir;
		}
	};
}