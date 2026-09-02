#pragma once

#include <Core/Shared/String.h>
#include <Core/Shared/Path.h>

struct ImFont;

namespace Editor
{
	class Font
	{
		private:
			ImFont* _font = nullptr;

		public:
			Font(const fs::path& path, float size);
			~Font();

			static void rebuildFonts();

			void setDefault();
	};
} // namespace Editor