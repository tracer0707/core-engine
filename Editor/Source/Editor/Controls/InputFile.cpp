#include "InputFile.h"

#include <Core/System/Application.h>
#include <Core/System/EventHandler.h>

#include "LinearLayout.h"
#include "InputText.h"
#include "Button.h"

#include "../../Main/FileSystemDialog.h"

namespace Editor
{
	InputFile::InputFile(Core::Application* application) : Control()
	{
		_application = application;

		_layout = new LinearLayout(LayoutDirection::Horizontal);

		_textInput = new InputText();
		Button* button = new Button("...");

		_layout->addControl(_textInput);
		_layout->addControl(button);

		button->setOnClick([this, application] {
			application->getEventHandler()->addEvent([this, application] {
				FileSystemDialog* dlg = new FileSystemDialog(_application);
				dlg->setOnPathSelected([this](Core::List<Core::String> fileNames) { _textInput->setValue(fileNames[0]); });
			});
		});

		addControl(_layout);
	}

	InputFile::~InputFile()
	{
		_application = nullptr;
	}

	float InputFile::getWidth() const
	{
		return _layout->getWidth();
	}

	void InputFile::setWidth(float value)
	{
		_layout->setWidth(value);
	}

	float InputFile::getHeight() const
	{
		return _layout->getHeight();
	}

	void InputFile::setHeight(float value)
	{
		_layout->setHeight(value);
	}

	Core::String InputFile::getFilePath() const
	{
		return _textInput->getValue();
	}

	void InputFile::setFilePath(Core::String value)
	{
		_textInput->setValue(value);
	}

	void InputFile::update()
	{
		if (!_visible) return;

		for (auto it : _controls)
		{
			it->update();
		}
	}
} // namespace Editor