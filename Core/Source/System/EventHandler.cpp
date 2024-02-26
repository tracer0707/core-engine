#include "EventHandler.h"

namespace Core
{
	EventHandler* EventHandler::_singleton = new EventHandler();

	EventHandler::EventHandler()
	{
	}

	EventHandler::~EventHandler()
	{
	}

	EventHandler* EventHandler::singleton()
	{
		return _singleton;
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