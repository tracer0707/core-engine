#include "ThumbCacheManager.h"

#include <filesystem>
#include <vector>

#include <FreeImage.h>
#include <Core/Shared/Path.h>
#include <Core/Content/ContentManager.h>

#include "../Main/Editor.h"
#include "../Utils/TextureUtils.h"

namespace fs = std::filesystem;

namespace Editor
{
	ThumbCacheManager::ThumbCacheManager(Editor* app)
	{
		_app = app;
	}

	ThumbCacheManager::~ThumbCacheManager()
	{
		_app = nullptr;
	}

	Core::Texture* ThumbCacheManager::get(Core::String filePath)
	{
		auto relativePath = fs::path(filePath.std_str()).lexically_relative(_app->getContentPath().std_str());
		auto thumbPath = Core::Path::combine(_app->getCacheThumbPath(), relativePath.generic_string());

		if (fs::exists(thumbPath.std_str()))
		{
			return TextureUtils::loadCompressed(thumbPath, _app->getMainWindow()->getContentManager());
		}

		return nullptr;
	}

} // namespace Editor