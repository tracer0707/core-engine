#pragma once

#include <Core/Renderer/Color.h>

namespace Editor
{
	struct Style
	{
		bool enabled = true;
		float borderSize = 1.0f;
		float opacity = 1.0f;
		float paddingX = 0.0f;
		float paddingY = 0.0f;
	};
}