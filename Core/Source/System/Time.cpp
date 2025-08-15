#include "Time.h"

#include <string.h>

namespace Core
{
	int Time::getFramesPerSecond()
	{
		return frameRate;
	}

	void Time::beginTimer()
	{
		now = std::chrono::steady_clock::now();
	}

	void Time::endTimer()
	{
		last = std::chrono::steady_clock::now();

		auto elapsed = last - now;

		deltaTime = elapsed / std::chrono::milliseconds(1) / 1000.0f;

		if (fpsTicks < fpsInterval)
		{
			++fpsTicks;
		}
		else
		{
			fpsTicks = 0;
			frameRate = std::chrono::milliseconds(1000) / elapsed;
		}
	}
}