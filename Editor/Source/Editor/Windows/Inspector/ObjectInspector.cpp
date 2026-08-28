#include "ObjectInspector.h"

#include <Core/Components/Component.h>
#include <Core/Interface/Transform.h>
#include <Core/Scene/Object.h>
#include <Core/System/EventHandler.h>

#include "TransformInspector.h"
#include "MeshRendererInspector.h"

#include "../../Controls/Collapse.h"
#include "../../Controls/Button.h"
#include "../../Controls/ContextMenu.h"
#include "../../Controls/MenuItem.h"

namespace Editor
{
	ObjectInspector::ObjectInspector(Core::Object* object, Core::EventHandler* eventHandler) : Inspector(eventHandler)
	{
		_object = object;
	}

	ObjectInspector::~ObjectInspector() {}

	void ObjectInspector::build()
	{
		Collapse* collapse = new Collapse("Transform");
		Inspector* inspector = new TransformInspector(_object->getTransform(), _eventHandler);
		inspector->build();
		collapse->addControl(inspector);
		addControl(collapse);

		for (auto& it : _object->getComponents())
		{
			if (it->getComponentType() == Core::ComponentType::MeshRenderer)
			{
				Collapse* collapse = new Collapse("Mesh Renderer");
				Inspector* inspector = new MeshRendererInspector((Core::MeshRenderer*)it, _eventHandler);
				inspector->build();
				collapse->addControl(inspector);
				addControl(collapse);
			}
		}

		LinearLayout* bottomLayout = new LinearLayout(LayoutDirection::Horizontal);
		bottomLayout->setWrapMode(LayoutWrapMode::NoWrap);
		bottomLayout->setHorizontalAlignment(LayoutHorizontalAlignment::Center);
		bottomLayout->setFitWidth(LayoutFitMode::FitAvailable);

		Button* addComponentButton = new Button("Add Component");
		addComponentButton->setUseContextMenu(true);
		
		MenuItem* _materialMenuItem = new MenuItem("Mesh Renderer");
		_materialMenuItem->setOnClick([this]() {
			_object->addComponent<Core::MeshRenderer*>();
			_eventHandler->addEvent([this]() {
				clear();
				build();
			});
		});

		addComponentButton->getContextMenu()->addControl(_materialMenuItem);
		bottomLayout->addControl(addComponentButton);
		addControl(bottomLayout);
	}
} // namespace Editor