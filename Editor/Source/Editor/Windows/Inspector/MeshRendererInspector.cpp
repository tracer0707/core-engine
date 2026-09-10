#include "MeshRendererInspector.h"

#include <Core/System/EventHandler.h>
#include <Core/Components/MeshRenderer.h>
#include <Core/Content/Mesh.h>
#include <Core/Content/Material.h>
#include <Core/Content/ContentManager.h>

#include "../../Controls/Table.h"
#include "../../Controls/Label.h"
#include "../../Controls/ContentSelect.h"
#include "../../Controls/Collapse.h"

namespace Editor
{
	MeshRendererInspector::MeshRendererInspector(Core::MeshRenderer* meshRenderer, Core::EventHandler* eventHandler,
												 Core::ContentManager* contentManager)
		: Inspector(eventHandler, contentManager)
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
		meshSelect->setOnDragDrop([this](DragDropData* data, int x, int y)
		{
			Core::Mesh* mesh = _contentManager->loadMeshByUuid(data->valueUuid);
			_meshRenderer->setMesh(mesh);
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
			materialsCollapse->setCollapseType(CollapseType::Node);

			Table* materialsTable = new Table();
			materialsTable->setColumnsCount(2);
			for (int i = 0; i < _meshRenderer->getMaterialCount(); ++i)
			{
				materialsTable->addControl(new Label("Material " + std::to_string(i)));
				ContentSelect* materialSelect = new ContentSelect();
				materialSelect->setContentType(Core::ContentType::Material);
				materialSelect->setContent(_meshRenderer->getMaterial(i));
				materialSelect->setOnDragDrop([this, materialSelect, i](DragDropData* data, int x, int y)
				{
					Core::Material* material = _contentManager->loadMaterialByUuid(data->valueUuid);
					_meshRenderer->setMaterial(i, material);
					materialSelect->setContent(material);
				});
				materialsTable->addControl(materialSelect);
			}
			materialsCollapse->addControl(materialsTable);
			addControl(materialsCollapse);
		}
	}
} // namespace Editor