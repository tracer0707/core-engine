#include "EditorIcons.h"

#include <filesystem>

#include <Core/Shared/Path.h>
#include <Core/Content/Texture.h>

#include "../System/ContentLoader.h"

namespace Editor
{
	Core::Texture* loadEditorIcon(ContentLoader* loader, Core::String path)
	{
		return loader->loadTextureFromFile(Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons", path),
										   Core::TextureFormat::RGBA8, LoadMethod::Direct);
	}
} // namespace Editor