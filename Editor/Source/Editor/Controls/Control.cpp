#include "Control.h"

#include <Core/Shared/Uuid.h>

namespace Editor
{
	Control::Control() : Container()
	{
		_id = Core::Uuid::create().toString();
	}

	Control::~Control()
	{
		_parent = nullptr;
	}

	void Control::setSize(float width, float height)
	{
		setWidth(width);
		setHeight(height);
	}

	void Control::setObjectTag(int key, void* value)
	{
		_objectTags[key] = value;
	}

	void* Control::getObjectTag(int key) const
	{
		if (_objectTags.find(key) == _objectTags.end())
		{
			return nullptr;
		}

		return _objectTags.at(key);
	}

	void Control::setStringTag(int key, Core::String value)
	{
		_stringTags[key] = value;
	}

	Core::String Control::getStringTag(int key) const
	{
		if (_stringTags.find(key) == _stringTags.end())
		{
			return Core::String::Empty;
		}

		return _stringTags.at(key);
	}
} // namespace Editor