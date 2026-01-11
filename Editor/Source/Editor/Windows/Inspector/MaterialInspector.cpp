#include "MaterialInspector.h"

#include <stdexcept>

#include <Core/Content/ContentTypes.h>
#include <Core/Content/Material.h>

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
		textureSelect->setContentType(CONTENT_TYPE_TEXTURE);
		textureSelect->setContent((Core::Content*)_material->getTexture());
		textureSelect->setOnChange([this](Core::Content* value) {

		});

		table->addControl(new Label("Texture"));
		table->addControl(textureSelect);

		_mainLayout->addControl(table);
		return _mainLayout;
	}
} // namespace Editor