#pragma once

#include <Core/Shared/String.h>
#include <Core/Content/Texture.h>

namespace Editor
{
	class Editor;

	class ThumbCacheManager
	{
		private:
			Editor* _app = nullptr;

		public:
			ThumbCacheManager(Editor* app);
			~ThumbCacheManager();

			Core::Texture* get(Core::String filePath);
	};
} // namespace Editor