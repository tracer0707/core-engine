#pragma once

#include <cstdint>

namespace Core
{
    class FrameBuffer
    {
      public:
        uint32_t frameBuffer = 0u;
		uint32_t colorBuffer = 0u;
		uint32_t depthBuffer = 0u;
        uint32_t width = 0u;
		uint32_t height = 0u;
    };
} // namespace Core