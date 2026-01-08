#include "MaterialInspector.h"

#include <stdexcept>

#include "../../Controls/LinearLayout.h"
#include "../../Controls/Button.h"

namespace Editor
{
	MaterialInspector::MaterialInspector(Core::Material* material) : Inspector()
	{
		_material = material;
		if (_material == nullptr)
		{
			throw std::runtime_error("Resource is null");
		}
	}

	MaterialInspector::~MaterialInspector() {}

	Control* MaterialInspector::build()
	{
		LinearLayout* _mainLayout = new LinearLayout();
		Button* btn = new Button("Test inspector");
		_mainLayout->addControl(btn);

		return _mainLayout;
	}
} // namespace Editor