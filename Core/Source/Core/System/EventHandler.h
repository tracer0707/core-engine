#pragma once

#include <vector>
#include <functional>

namespace Core
{
	class EventHandler
	{
	private:
		std::vector<std::function<void()>> events;

	public:
		EventHandler();
		~EventHandler();

		void addEvent(std::function<void()> value);
		void processEvents();
	};
}