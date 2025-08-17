#include "EventHandler.h"

namespace Core
{
	EventHandler::EventHandler() {}

	EventHandler::~EventHandler()
	{
		events.clear();
	}

	void EventHandler::processEvents()
	{
		while (events.size() > 0)
		{
			events[0]();
			events.erase(events.begin());
		}
	}

	void EventHandler::addEvent(std::function<void()> value)
	{
		events.push_back(value);
	}
}