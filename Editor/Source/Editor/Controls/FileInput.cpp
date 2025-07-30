#include "FileInput.h"

#include "LinearLayout.h"
#include "TextInput.h"
#include "Button.h"

namespace Editor
{
	FileInput::FileInput()
	{
		LinearLayout* layout = new LinearLayout(LayoutDirection::Horizontal);

		TextInput* textInput = new TextInput();
		Button* button = new Button("...");
		
		layout->addControl(textInput);
		layout->addControl(button);

		addControl(layout);
	}

	FileInput::~FileInput()
	{
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