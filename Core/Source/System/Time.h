#pragma once

#include <chrono>

namespace Core
{
	class Time
	{
	private:
		static float timeScale;
		static float deltaTime;
		static int fpsInterval;
		static int fpsTicks;
		static int frameRate;
		static std::chrono::steady_clock::time_point now;
		static std::chrono::steady_clock::time_point last;

	public:
		static float getTimeScale() { return timeScale; }
		static void setTimeScale(float value) { timeScale = value; }

		static float getDeltaTime() { return deltaTime; }
		static int getFramesPerSecond();

		static void beginTimer();
		static void endTimer();
	};
}