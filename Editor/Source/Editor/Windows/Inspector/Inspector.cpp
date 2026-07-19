#include "Inspector.h"

#include "../WindowManager.h"
#include "../InspectorWindow.h"

namespace Editor
{
	Core::EventHandler* Inspector::getEventHandler()
	{
		return ((InspectorWindow*)_parent)->getParent()->getEventHandler();
	}

	Inspector::Inspector() : LinearLayout(LayoutDirection::Vertical)
	{
		setWrapMode(LayoutWrapMode::NoWrap);
	}

	Inspector::~Inspector() { }
} // namespace Editor