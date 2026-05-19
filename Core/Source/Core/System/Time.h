#pragma once

namespace Core
{
	class Time
	{
		private:
			float timeScale = 1.0f;
			float deltaTime = 0.0f;
			int frameRate = 0;
			unsigned long long last = 0;
			float smoothedDelta = 0.0f;
			float frameTimeMs = 0.0f;

		public:
			float getTimeScale() const { return timeScale; }
			void setTimeScale(float value) { timeScale = value; }

			float getDeltaTime() const { return deltaTime; }
			int getFramesPerSecond() const { return frameRate; }
			float getFrameTimeMs() const { return frameTimeMs; }

			void beginTimer();
	};
} // namespace Core