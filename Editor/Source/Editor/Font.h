#pragma once

#include <Shared/String.h>

class ImFont;

namespace Editor
{
	class Font
	{
	private:
		ImFont* _font = nullptr;

	public:
		Font(UString path, float size);
		~Font();

		void setDefault();
	};
}