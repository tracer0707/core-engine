#include "MaterialInspector.h"

#include <stdexcept>

#include <Core/Shared/List.h>
#include <Core/Content/ContentTypes.h>
#include <Core/Content/Material.h>
#include <Core/Renderer/Renderer.h>
#include <Core/Renderer/Program.h>

#include "../../Controls/Table.h"
#include "../../Controls/LinearLayout.h"
#include "../../Controls/Label.h"
#include "../../Controls/Button.h"
#include "../../Controls/ContentSelect.h"
#include "../../Controls/Dropdown.h"

#include "../InspectorWindow.h"

#include "../../../Content/ContentSerializer.h"

namespace Editor
{
	MaterialInspector::MaterialInspector(Core::Material* material, Core::Renderer* renderer) : Inspector()
	{
		_material = material;
		_renderer = renderer;

		if (_material == nullptr)
		{
			throw std::runtime_error("Resource is null");
		}
	}

	MaterialInspector::~MaterialInspector() {}

	Control* MaterialInspector::build()
	{
		LinearLayout* _mainLayout = new LinearLayout(LayoutDirection::Vertical);
		Table* table = new Table();
		table->setColumnsCount(2);

		Label* programLabel = new Label("Shader");
		table->addControl(programLabel);

		const auto& shaderPrograms = _renderer->getShaderPrograms();
		Core::List<Core::String> programNames;
		for (const auto& pair : shaderPrograms)
		{
			programNames.add(pair.first);
		}

		Dropdown* programDropdown = new Dropdown(programNames);
		
		int currentProgramIndex = programNames.indexOf(_material->getProgram()->name);
		if (currentProgramIndex > -1)
		{
			programDropdown->setSelectedIndex(currentProgramIndex);
		}

		programDropdown->setOnSelectItem([this, programNames](int value) {
			Core::Program* newProgram = _renderer->getShaderProgram(programNames[value]);
			_material->setProgram(newProgram);
			_parent->rebuildInspector();
		});
		table->addControl(programDropdown);

		const Core::Program* program = _material->getProgram();

		if (program != nullptr)
		{
			for (const auto& uniform : program->uniforms)
			{
				table->addControl(new Label(uniform.name));
				table->addControl(new Label("test"));

				if (uniform.type == Core::UniformType::Int)
				{
				}
			}
		}

		// ContentSelect* textureSelect = new ContentSelect();
		// textureSelect->setContentType(CONTENT_TYPE_TEXTURE);
		// textureSelect->setContent((Core::Content*)_material->getTexture());
		//  textureSelect->setOnChange([this](Core::Content* value) {
		//  	_material->setTexture((Core::Texture*)value);
		//  	ContentSerializer::serializeMaterial(_material);
		//  });

		// table->addControl(new Label("Texture"));
		// table->addControl(textureSelect);

		_mainLayout->addControl(table);
		return _mainLayout;
	}
} // namespace Editor