#include "FileInput.h"

#include <System/Application.h>

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
			FileSystemDialog* dlg = new FileSystemDialog();
			_application->addWindow(dlg);
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
}