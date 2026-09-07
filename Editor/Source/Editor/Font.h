#pragma once

#include <Core/Shared/String.h>
#include <Core/Shared/Path.h>

struct ImFont;
template <typename T>
struct ImVector;

namespace Editor
{
	class Font
	{
		private:
			ImFont* _font = nullptr;

#ifdef IMGUI_USE_WCHAR32
			ImVector<unsigned int>* _glyphRanges = nullptr;
#else
			ImVector<unsigned short>* _glyphRanges = nullptr;
#endif

		public:
			Font(const fs::path& path, float size);
			~Font();

			static void rebuildFonts();

			void setDefault();
	};
} // namespace Editor