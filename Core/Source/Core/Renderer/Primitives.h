#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Color.h"
#include "../Shared/List.h"

namespace Core
{
    struct Vertex;
    class VertexBuffer;
    class Renderer;
    class Material;

    class Primitives
    {
      public:
        enum class WireframeMode
        {
            Triangle,
            Polygon
        };

        static void lines(Renderer* renderer, VertexBuffer* buffer, Material* material, glm::mat4& view, glm::mat4& proj, glm::mat4& model,
                          Vertex* points, int pointsCount, unsigned int flags);

        static void wireCube(Renderer* renderer, VertexBuffer* buffer, Material* material, glm::mat4& view, glm::mat4& proj, glm::mat4& model,
                             glm::vec3 size, glm::vec3 center, Color color, unsigned int flags);

        static void wireMesh(Renderer* renderer, VertexBuffer* buffer, Material* material, glm::mat4& view, glm::mat4& proj, glm::mat4& model,
                             List<glm::vec3>& vertices, List<int>& indices, Color color, WireframeMode mode, unsigned int flags);
    };
} // namespace Core