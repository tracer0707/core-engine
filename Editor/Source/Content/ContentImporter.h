#pragma once

#include <Core/Shared/String.h>
#include <Core/Shared/Path.h>
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

			void importTexture2D(const fs::path& sourceFileName, const fs::path& targetFileName, Core::TextureFormat format);
			void importMesh(const fs::path& sourceFileName, const fs::path& targetFileName);
	};
} // namespace Editor