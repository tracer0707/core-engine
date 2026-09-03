#pragma once

namespace Core
{
	class Renderer;
	class Scene;
	class VertexBuffer;
} // namespace Core

namespace Editor
{
	class Gizmo;

	class GizmoRenderer
	{
		private:
			Core::Renderer* _renderer = nullptr;
			Core::Scene* _scene = nullptr;
			Core::VertexBuffer* _wireframeBuffer = nullptr;
			Gizmo* _gizmo = nullptr;

		public:
			GizmoRenderer(Core::Renderer* renderer);
			~GizmoRenderer();

			void setGizmo(Gizmo* value) { _gizmo = value; }
			void setScene(Core::Scene* value) { _scene = value; }
			void renderGizmo();
	};
} // namespace Editor