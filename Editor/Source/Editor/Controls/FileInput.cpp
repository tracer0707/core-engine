#include "FileInput.h"

#include <Core/System/Application.h>
#include <Core/System/EventHandler.h>

#include "ControlList.h"
#include "LinearLayout.h"
#include "TextInput.h"
#include "Button.h"

#include "../../Main/FileSystemDialog.h"

namespace Editor
{
	FileInput::FileInput(Core::Application* application) : Control()
	{
		_application = application;

		_layout = new LinearLayout(LayoutDirection::Horizontal);

		_textInput = new TextInput();
		Button* button = new Button("...");
		
		_layout->addControl(_textInput);
        _layout->addControl(button);

		button->setOnClick([=] {
			application->getEventHandler()->addEvent([=]
			{
				FileSystemDialog* dlg = new FileSystemDialog(_application);
				dlg->setOnPathSelected([=](Core::String fileName)
				{
					_textInput->setText(fileName);
				});
			});
		});

		addControl(_layout);
	}

	FileInput::~FileInput()
	{
		_application = nullptr;
    }

    float FileInput::getWidth() const
    {
        return _layout->getWidth();
    }

    void FileInput::setWidth(float value)
	{
        _layout->setWidth(value);
	}

    float FileInput::getHeight() const
    {
        return _layout->getHeight();
    }

    void FileInput::setHeight(float value)
	{
        _layout->setHeight(value);
	}

	Core::String FileInput::getFilePath() const
	{
		return _textInput->getText();
	}

	void FileInput::setFilePath(Core::String value)
	{
		_textInput->setText(value);
	}

	int FileInput::getControlType() const
	{
		return CONTROL_FILE_INPUT;
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