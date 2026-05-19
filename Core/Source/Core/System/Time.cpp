#include "Time.h"

#include <string.h>
#include <algorithm>

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

namespace Core
{
	void Time::beginTimer()
	{
		Uint64 now = SDL_GetPerformanceCounter();
		Uint64 freq = SDL_GetPerformanceFrequency();

		deltaTime = (float)(now - last) / (float)freq;
		deltaTime = std::min(deltaTime, 0.1f);
		last = now;

		smoothedDelta = smoothedDelta * 0.9f + deltaTime * 0.1f;

		frameRate = (int)(1.0f / smoothedDelta);
		frameTimeMs = deltaTime * 1000.0f;
	}
} // namespace Core