#include "MeshRendererInspector.h"

#include <Core/System/EventHandler.h>
#include <Core/Components/MeshRenderer.h>
#include <Core/Content/Mesh.h>
#include <Core/Content/Material.h>

#include "../../Controls/Table.h"
#include "../../Controls/Label.h"
#include "../../Controls/ContentSelect.h"
#include "../../Controls/Collapse.h"

namespace Editor
{
	MeshRendererInspector::MeshRendererInspector(Core::MeshRenderer* meshRenderer, Core::EventHandler* eventHandler) : Inspector(eventHandler)
	{
		_meshRenderer = meshRenderer;
	}

	MeshRendererInspector::~MeshRendererInspector() {}

	void MeshRendererInspector::build()
	{
		Table* table = new Table();
		table->setColumnsCount(2);

		table->addControl(new Label("Mesh"));
		ContentSelect* meshSelect = new ContentSelect();
		meshSelect->setContentType(Core::ContentType::Mesh);
		meshSelect->setContent(_meshRenderer->getMesh());
		meshSelect->setOnContentChanged([this](Core::Content* content)
		{
			_meshRenderer->setMesh((Core::Mesh*)content);
			_eventHandler->addEvent([this]() {
				clear();
				build();
			});
		});
		table->addControl(meshSelect);

		addControl(table);

		if (_meshRenderer->getMaterialCount() > 0)
		{
			Collapse* materialsCollapse = new Collapse("Materials");
			Table* materialsTable = new Table();
			materialsTable->setColumnsCount(2);
			for (int i = 0; i < _meshRenderer->getMaterialCount(); ++i)
			{
				materialsTable->addControl(new Label("Material " + std::to_string(i)));
				ContentSelect* materialSelect = new ContentSelect();
				materialSelect->setContentType(Core::ContentType::Material);
				materialSelect->setContent(_meshRenderer->getMaterial(i));
				materialSelect->setOnContentChanged([this, i](Core::Content* content) { _meshRenderer->setMaterial(i, (Core::Material*)content); });
				materialsTable->addControl(materialSelect);
			}
			materialsCollapse->addControl(materialsTable);
			addControl(materialsCollapse);
		}
	}
} // namespace Editor