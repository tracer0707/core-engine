#include "CSGBrushInspector.h"

#include <stdexcept>

#include <Core/Content/ContentType.h>
#include <Core/Content/Material.h>

#include "../../../CSG/CSGBrush.h"

#include "../../Controls/Table.h"
#include "../../Controls/LinearLayout.h"
#include "../../Controls/Label.h"
#include "../../Controls/Button.h"
#include "../../Controls/ContentSelect.h"
#include "../../Controls/Separator.h"

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

		for (int i = 0; i < _brush->getFaces().count(); ++i)
		{
			ContentSelect* materialSelect = new ContentSelect();
			materialSelect->setContentType(Core::ContentType::Material);
			materialSelect->setContent((Core::Content*)_brush->getMaterial(i));
			materialSelect->setOnContentChanged([this, i](Core::Content* value) { _brush->setMaterial(i, (Core::Material*)value); });

			table->addControl(new Label("Face " + std::to_string(i)));
			table->addControl(new Separator());

			table->addControl(new Label("Material"));
			table->addControl(materialSelect);
		}

		_mainLayout->addControl(table);
		return _mainLayout;
	}
} // namespace Editor