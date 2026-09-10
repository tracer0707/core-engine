#include "BehaviorInspector.h"

#include <Core/System/EventHandler.h>
#include <Core/Components/Behavior.h>
#include <Core/Content/Script.h>
#include <Core/Content/ContentManager.h>

#include "../../Controls/Table.h"
#include "../../Controls/Label.h"
#include "../../Controls/ContentSelect.h"

namespace Editor
{
	BehaviorInspector::BehaviorInspector(Core::Behavior* behavior, Core::EventHandler* eventHandler, Core::ContentManager* contentManager)
		: Inspector(eventHandler, contentManager)
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
		scriptSelect->setOnDragDrop([this](DragDropData* data, int x, int y) {
			Core::Script* script = _contentManager->loadScriptByUuid(data->valueUuid);
			_behavior->setScript(script);
			_eventHandler->addEvent([this]() {
				clear();
				build();
			});
		});

		table->addControl(scriptSelect);

		addControl(table);
	}
} // namespace Editor