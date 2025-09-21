#pragma once

#include <chrono>

namespace Core
{
	class Time
	{
	private:
		float timeScale = 1.0f;
		float deltaTime = 0.0f;
		int fpsInterval = 100;
		int fpsTicks = 0;
		int frameRate = 0;
		std::chrono::steady_clock::time_point now;
		std::chrono::steady_clock::time_point last;

	public:
		float getTimeScale() { return timeScale; }
		void setTimeScale(float value) { timeScale = value; }

		float getDeltaTime() { return deltaTime; }
		int getFramesPerSecond();

		void beginTimer();
		void endTimer();
	};
}