#include "CSGBrushInspector.h"

#include <stdexcept>

#include <Core/Content/ContentType.h>
#include <Core/Content/Material.h>

#include "../../../CSG/CSGBuilder.h"
#include "../../../CSG/CSGBrush.h"

#include "../../Controls/Table.h"
#include "../../Controls/LinearLayout.h"
#include "../../Controls/Label.h"
#include "../../Controls/Button.h"
#include "../../Controls/ContentSelect.h"
#include "../../Controls/Collapse.h"
#include "../../Controls/Dropdown.h"

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

		Table* mainTable = new Table();
		mainTable->setColumnsCount(2);
		
		Dropdown* operation = new Dropdown({"Add", "Subtract"});
		operation->setSelectedIndex(_brush->getBrushOperation() == CSGBrush::BrushOperation::Add ? 0 : 1);
		operation->setOnSelectItem([this](int index)
		{
			_brush->setBrushOperation(index == 0 ? CSGBrush::BrushOperation::Add : CSGBrush::BrushOperation::Subtract);
			CSGBuilder::singleton()->rebuild();
		});
		
		mainTable->addControl(new Label("Operation"));
		mainTable->addControl(operation);

		_mainLayout->addControl(mainTable);

		for (int i = 0; i < _brush->getFaces().count(); ++i)
		{
			Collapse* collapse = new Collapse("Face " + std::to_string(i));

			Table* table = new Table();
			table->setColumnsCount(2);

			ContentSelect* materialSelect = new ContentSelect();
			materialSelect->setContentType(Core::ContentType::Material);
			materialSelect->setContent((Core::Content*)_brush->getMaterial(i));
			materialSelect->setOnContentChanged([this, i](Core::Content* value)
			{
				_brush->setMaterial(i, (Core::Material*)value);
				CSGBuilder::singleton()->rebuild();
			});

			table->addControl(new Label("Material"));
			table->addControl(materialSelect);

			collapse->addControl(table);
			_mainLayout->addControl(collapse);
		}

		return _mainLayout;
	}
} // namespace Editor