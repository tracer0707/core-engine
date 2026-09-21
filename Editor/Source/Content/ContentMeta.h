#pragma once

#include <filesystem>

#include <Core/Shared/Uuid.h>

namespace fs = std::filesystem;

namespace Core
{
	class Application;
}

namespace Editor
{
	class ContentMeta
	{
		public:
			static fs::path getMetaPathFromUuid(Core::Application* app, Core::Uuid uuid);
			static fs::path getSourcePathFromUuid(Core::Application* app, Core::Uuid uuid);
			static fs::path getThumbPathFromUuid(Core::Application* app, Core::Uuid uuid);
	};
}