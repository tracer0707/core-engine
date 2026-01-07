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
	class Editor;

	class ContentImporter
	{
		private:
			Editor* _app = nullptr;

			void loadTexture(Core::String fileName, Core::TextureFormat format, unsigned char** data, int& size, int& w, int& h);

		public:
			ContentImporter(Editor* app);
			~ContentImporter();

			Core::Texture* importTexture(Core::String sourceFileName, Core::String targetFileName, Core::TextureFormat format);
			Core::Mesh* importMesh(Core::String sourceFileName, Core::String targetFileName);
	};
} // namespace Editor