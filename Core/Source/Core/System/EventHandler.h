#pragma once

#include <vector>
#include <functional>

namespace Core
{
	class Window;
	class Application;

	class EventHandler
	{
		friend class Window;
		friend class Application;

	private:
		std::vector<std::function<void()>> events;
		void processEvents();

	public:
		EventHandler();
		~EventHandler();

		void addEvent(std::function<void()> value);
	};
}