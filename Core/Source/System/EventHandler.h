#pragma once

#include <vector>
#include <functional>

#define EVENT(FUNC, ...) Core::EventHandler::singleton()->addEvent([__VA_ARGS__]() FUNC)

namespace Core
{
	class EventHandler
	{
	private:
		std::vector<std::function<void()>> events;
		static EventHandler* _singleton;

	public:
		EventHandler();
		~EventHandler();

		static EventHandler* singleton();

		void addEvent(std::function<void()> value);
		void processEvents();
	};
}