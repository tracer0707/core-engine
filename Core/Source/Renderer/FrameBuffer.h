#pragma once

#include "../Config.h"

namespace Core
{
	class FrameBuffer
	{
	public:
		UInt32 frameBuffer = 0;
		UInt32 colorTexture = 0;
		UInt32 depthTexture = 0;
	};
}