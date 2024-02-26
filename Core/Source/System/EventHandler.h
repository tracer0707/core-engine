#pragma once

#include <vector>
#include <functional>

namespace Core
{
#define EVENT(FUNC, ...) Core::EventHandler::singleton()->addEvent([__VA_ARGS__]() FUNC)

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