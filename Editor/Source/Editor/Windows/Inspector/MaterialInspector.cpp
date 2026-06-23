#include "MaterialInspector.h"

#include <stdexcept>

#include <Core/Shared/List.h>
#include <Core/Content/Material.h>
#include <Core/Renderer/Renderer.h>
#include <Core/Renderer/Program.h>

#include "../../Controls/Table.h"
#include "../../Controls/LinearLayout.h"
#include "../../Controls/Label.h"
#include "../../Controls/Button.h"
#include "../../Controls/ContentSelect.h"
#include "../../Controls/Dropdown.h"
#include "../../Controls/InputVec4.h"

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
				
				if (uniform.type == Core::UniformType::Sampler2D)
				{
					ContentSelect* textureSelect = new ContentSelect();
					textureSelect->setContentType(Core::ContentType::Texture2D);
					textureSelect->setContent((Core::Content*)_material->getTexture2D(uniform.nameHash));
					textureSelect->setOnContentChanged([this, uniform](Core::Content* value) {
						_material->setTexture2D(uniform.nameHash, (Core::Texture2D*)value);
						ContentSerializer::serializeMaterial(_material);
					});
					table->addControl(textureSelect);
				}
				else if (uniform.type == Core::UniformType::Vec4)
				{
					InputVec4* vec4Input = new InputVec4();
					vec4Input->setValue(_material->getVec4(uniform.nameHash));
					vec4Input->setOnValueChanged([this, uniform](glm::vec4 value) {
						_material->setVec4(uniform.nameHash, value);
						ContentSerializer::serializeMaterial(_material);
					});
					table->addControl(vec4Input);
				}
				else
				{
					table->addControl(new Label("Element is not editable"));
				}
			}
		}

		_mainLayout->addControl(table);
		return _mainLayout;
	}
} // namespace Editor