#pragma once

#include <Core/Shared/String.h>
#include <Core/Content/Texture.h>

namespace Editor
{
	class EditorApp;

	class ThumbCacheManager
	{
		private:
			EditorApp* _app = nullptr;

		public:
			ThumbCacheManager(EditorApp* app);
			~ThumbCacheManager();

			Core::Texture* get(Core::String filePath);
	};
} // namespace Editor