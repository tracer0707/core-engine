#pragma once

namespace Core
{
	class Renderer;
	class Scene;
	class VertexBuffer;
} // namespace Core

namespace Editor
{
	class GizmoRenderer
	{
		private:
			Core::Renderer* _renderer = nullptr;
			Core::Scene* _scene = nullptr;
			Core::VertexBuffer* _wireframeBuffer = nullptr;

		public:
			GizmoRenderer(Core::Renderer* renderer, Core::Scene* scene);
			~GizmoRenderer();

			void renderGizmo();
	};
} // namespace Editor