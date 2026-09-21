#include "ContentMeta.h"

#include <Core/Content/ContentDatabase.h>
#include <Core/System/Application.h>

namespace Editor
{
	fs::path ContentMeta::getMetaPathFromUuid(Core::Application* app, Core::Uuid uuid)
	{
		fs::path root = app->getRootPath() / fs::path("Meta");

		if (!fs::exists(root))
		{
			fs::create_directories(root);
		}

		return root / fs::path(uuid.toString() + ".meta");
	}

	fs::path ContentMeta::getSourcePathFromUuid(Core::Application* app, Core::Uuid uuid)
	{
		fs::path root = app->getRootPath() / fs::path("Source");

		if (!fs::exists(root))
		{
			fs::create_directories(root);
		}

		return root / fs::path(uuid.toString());
	}

	fs::path ContentMeta::getThumbPathFromUuid(Core::Application* app, Core::Uuid uuid)
	{
		fs::path root = app->getRootPath() / fs::path("Thumbnails");

		if (!fs::exists(root))
		{
			fs::create_directories(root);
		}

		return root / fs::path(uuid.toString() + ".png");
	}
} // namespace Editor