#include "Inspector.h"

#include "../WindowManager.h"
#include "../InspectorWindow.h"

namespace Editor
{
	Inspector::Inspector(Core::EventHandler* eventHandler) : LinearLayout(LayoutDirection::Vertical)
	{
		_eventHandler = eventHandler;

		setWrapMode(LayoutWrapMode::NoWrap);
		setFitWidth(LayoutFitMode::FitAvailable);
		setFitHeight(LayoutFitMode::FitContent);
	}

	Inspector::~Inspector() { }
} // namespace Editor