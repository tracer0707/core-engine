#pragma once

namespace Core
{
    class Camera;
    class Renderer;
    class Material;
    class VertexBuffer;
} // namespace Core

namespace Editor
{
    class Rendering
    {
      public:
        static void renderGrid(Core::Renderer* renderer, Core::VertexBuffer* buffer, Core::Material* material, Core::Camera* camera);
    };
} // namespace Editor