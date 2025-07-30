#pragma once

#include <Renderer/Color.h>

namespace Editor
{
	class Style
	{
	public:
		bool enabled = true;
		float borderSize = 1.0f;
		float opacity = 1.0f;
		float paddingX = 4.0f;
		float paddingY = 4.0f;
	};
}