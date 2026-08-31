#pragma once

namespace Core
{
	class Renderer;
	class VertexBuffer;
} // namespace Core

namespace Editor
{
	class EditorCamera;

	class Rendering
	{
		public:
			static void renderGrid(Core::Renderer* renderer, Core::VertexBuffer* buffer, EditorCamera* camera);
	};
} // namespace Editor