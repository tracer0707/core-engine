#pragma once

#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

namespace Core
{
	class Renderer;
}

namespace Editor
{
	class Texture;

	enum class EditorIcon
	{
		Save,
		Undo,
		Redo,
		Select,
		Translate,
		Rotate,
		Scale,
		Bounds,
		LocalSpace,
		WorldSpace,
		Object,
		PointLight,
		Camera,
		Add,
		ChevronDown,
		Folder,
		Font,
		Texture,
		Mesh,
		Material,
		Scene,
		Script,
		File,
	};

	class TextureManager
	{
		private:
			Core::Renderer* _renderer = nullptr;

			std::unordered_map<fs::path, Texture*> _textureCache;
			std::unordered_map<EditorIcon, Texture*> _iconCache;

		public:
			TextureManager(Core::Renderer* renderer);
			~TextureManager();

			Texture* loadFromFile(const fs::path& fileName);

			Texture* getIcon(EditorIcon icon);
			void loadIcons(const fs::path& rootPath);
	};
} // namespace Editor