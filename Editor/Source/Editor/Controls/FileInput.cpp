#include "FileInput.h"

#include <System/Application.h>
#include <System/EventHandler.h>

#include "ControlList.h"
#include "LinearLayout.h"
#include "TextInput.h"
#include "Button.h"

#include "../../Main/FileSystemDialog.h"

namespace Editor
{
	FileInput::FileInput(Core::Application* application)
	{
		_application = application;

		LinearLayout* layout = new LinearLayout(LayoutDirection::Horizontal);

		TextInput* textInput = new TextInput();
		Button* button = new Button("...");
		
		layout->addControl(textInput);
		layout->addControl(button);

		button->setOnClick([=] {
			application->getEventHandler()->addEvent([=]
			{
				FileSystemDialog* dlg = new FileSystemDialog(_application);
				dlg->setOnFileSelected([=](Core::String fileName)
				{
					textInput->setText(fileName);
				});
			});
		});

		addControl(layout);
	}

	FileInput::~FileInput()
	{
		_application = nullptr;
	}

	void FileInput::update()
	{
		if (!_visible) return;

		for (auto it : _controls)
		{
			it->update();
		}
	}

	int FileInput::getControlType()
	{
		return CONTROL_FILE_INPUT;
	}
}