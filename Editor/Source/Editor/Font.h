#pragma once

#include <Core/Shared/String.h>

class ImFont;

namespace Editor
{
	class Font
	{
	private:
		ImFont* _font = nullptr;

	public:
		Font(Core::String path, float size);
		~Font();

		static void rebuildFonts();

		void setDefault();
	};
}