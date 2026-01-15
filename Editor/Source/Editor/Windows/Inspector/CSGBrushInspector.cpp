#include "CSGBrushInspector.h"

#include <stdexcept>

#include <Core/Content/ContentTypes.h>
#include <Core/Content/Material.h>

#include "../../../CSG/CSGBrush.h"

#include "../../Controls/Table.h"
#include "../../Controls/LinearLayout.h"
#include "../../Controls/Label.h"
#include "../../Controls/Button.h"
#include "../../Controls/ContentSelect.h"

namespace Editor
{
	CSGBrushInspector::CSGBrushInspector(CSGBrush* brush) : Inspector()
	{
		_brush = brush;
		if (_brush == nullptr)
		{
			throw std::runtime_error("Object is null");
		}
	}

	CSGBrushInspector::~CSGBrushInspector() {}

	Control* CSGBrushInspector::build()
	{
		LinearLayout* _mainLayout = new LinearLayout();
		Table* table = new Table();
		table->setColumnsCount(2);

		ContentSelect* materialSelect = new ContentSelect();
		materialSelect->setContentType(CONTENT_TYPE_MATERIAL);
		materialSelect->setContent((Core::Content*)_brush->getMaterial(0));
		materialSelect->setOnChange([this](Core::Content* value) { _brush->setMaterial(0, (Core::Material*)value); });

		table->addControl(new Label("Material"));
		table->addControl(materialSelect);

		_mainLayout->addControl(table);
		return _mainLayout;
	}
} // namespace Editor