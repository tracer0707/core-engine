#include "ObjectInspector.h"

#include <Core/Components/Component.h>
#include <Core/Scene/Object.h>

#include "TransformInspector.h"
#include "../../Controls/Collapse.h"

namespace Editor
{
	ObjectInspector::ObjectInspector(Core::Object* object)
	{
		_object = object;
	}

	ObjectInspector::~ObjectInspector() {}

	void ObjectInspector::build()
	{
		for (auto& it : _object->getComponents())
		{
			if (it->getComponentType() == Core::ComponentType::Transform)
			{
				Collapse* collapse = new Collapse("Transform");
				Inspector* inspector = new TransformInspector((Core::Transformable*)it);
				inspector->build();
				collapse->addControl(inspector);
				addControl(collapse);
			}
		}
	}
} // namespace Editor