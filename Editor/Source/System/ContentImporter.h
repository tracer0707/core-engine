#pragma once

#include <Core/Shared/String.h>
#include <Core/Renderer/TextureFormat.h>

namespace Core
{
	class Texture;
	class Mesh;
} // namespace Core

namespace Editor
{
	class EditorApp;

	class ContentImporter
	{
		private:
			EditorApp* _app = nullptr;

		public:
			ContentImporter(EditorApp* app);
			~ContentImporter();

			Core::Texture* importTexture(Core::String sourceFileName, Core::String targetFileName, Core::TextureFormat format);
			Core::Mesh* importMesh(Core::String sourceFileName, Core::String targetFileName);
	};
} // namespace Editor