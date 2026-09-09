#pragma once

#include <filesystem>
#include <glm/matrix.hpp>

#include <Core/Shared/String.h>
#include <Core/Math/AxisAlignedBox.h>

namespace fs = std::filesystem;

namespace Core
{
	class Application;
	class ContentManager;
	class FrameBuffer;
	class Renderer;
	class Mesh;
	class Material;
}

namespace Editor
{
	class ThumbManager
	{
		private:
			Core::Application* _application = nullptr;
			Core::ContentManager* _contentManager = nullptr;

			void renderTextureThumbnail(const fs::path& sourcePath, const fs::path& thumbPath);
			void renderMeshThumbnail(const fs::path& sourcePath, const fs::path& thumbPath);
			void renderMaterialThumbnail(const fs::path& sourcePath, const fs::path& thumbPath);

			const Core::FrameBuffer* beginThumbnail(Core::Renderer* renderer);
			void finishThumbnail(Core::Renderer* renderer, const Core::FrameBuffer* frameBuffer, const fs::path& fileName);
			void getPreviewMatrices(const Core::AxisAlignedBox& box, glm::mat4& view, glm::mat4& projection, glm::mat4& model);
			void renderPreviewMesh(Core::Renderer* renderer, Core::Mesh* mesh, Core::Material* material, const fs::path& fileName);

		public:
			ThumbManager(Core::Application* application, Core::ContentManager* contentManager);
			~ThumbManager() = default;

			fs::path getThumbPath(const fs::path& sourcePath);
	};
} // namespace Editor