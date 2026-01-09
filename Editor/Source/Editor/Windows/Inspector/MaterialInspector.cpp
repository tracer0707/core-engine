#include "MaterialInspector.h"

#include <stdexcept>

#include "../../Controls/Table.h"
#include "../../Controls/LinearLayout.h"
#include "../../Controls/Label.h"
#include "../../Controls/Button.h"
#include "../../Controls/ContentSelect.h"

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
		Table* table = new Table();
		table->setColumnsCount(2);

		ContentSelect* textureSelect = new ContentSelect();
		table->addControl(new Label("Texture"));
		table->addControl(textureSelect);

		_mainLayout->addControl(table);

		return _mainLayout;
	}
} // namespace Editor