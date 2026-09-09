#include "BehaviorInspector.h"

#include <Core/System/EventHandler.h>
#include <Core/Components/Behavior.h>
#include <Core/Content/Script.h>

#include "../../Controls/Table.h"
#include "../../Controls/Label.h"
#include "../../Controls/ContentSelect.h"

namespace Editor
{
	BehaviorInspector::BehaviorInspector(Core::Behavior* behavior, Core::EventHandler* eventHandler) : Inspector(eventHandler)
	{
		_behavior = behavior;
	}

	BehaviorInspector::~BehaviorInspector() {}

	void BehaviorInspector::build()
	{
		Table* table = new Table();
		table->setColumnsCount(2);

		table->addControl(new Label("Script"));
		ContentSelect* scriptSelect = new ContentSelect();
		scriptSelect->setContentType(Core::ContentType::Script);
		scriptSelect->setContent(_behavior->getScript());
		scriptSelect->setOnContentChanged([this](Core::Content* content) {
			_behavior->setScript((Core::Script*)content);
			_eventHandler->addEvent([this]() {
				clear();
				build();
			});
		});

		table->addControl(scriptSelect);

		addControl(table);
	}
} // namespace Editor