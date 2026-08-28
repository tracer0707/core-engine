#include "TransformInspector.h"

#include <stdexcept>

#include <Core/Interface/Transform.h>
#include <Core/Math/Mathf.h>

#include "../../Controls/Table.h"
#include "../../Controls/Label.h"
#include "../../Controls/InputVec3.h"

namespace Editor
{
	TransformInspector::TransformInspector(Core::Transform* transform, Core::EventHandler* eventHandler) : Inspector(eventHandler)
	{
		_transform = transform;
		if (_transform == nullptr)
		{
			throw std::runtime_error("Object is null");
		}
	}

	TransformInspector::~TransformInspector() {}

	void TransformInspector::build()
	{
		Table* table = new Table();
		table->setColumnsCount(2);

		table->addControl(new Label("Position"));
		InputVec3* positionInput = new InputVec3();
		positionInput->setValue(_transform->getPosition());
		positionInput->setOnValueChanged([this](glm::vec3 value) { _transform->setPosition(value); });
		table->addControl(positionInput);

		table->addControl(new Label("Rotation"));
		InputVec3* rotationInput = new InputVec3();
		rotationInput->setValue(Core::Mathf::toEuler(_transform->getRotation()));
		rotationInput->setOnValueChanged([this](glm::vec3 value) { _transform->setRotation(Core::Mathf::toQuaternion(value)); });
		table->addControl(rotationInput);

		table->addControl(new Label("Scale"));
		InputVec3* scaleInput = new InputVec3();
		scaleInput->setValue(_transform->getScale());
		scaleInput->setOnValueChanged([this](glm::vec3 value) { _transform->setScale(value); });
		table->addControl(scaleInput);

		addControl(table);
	}
} // namespace Editor