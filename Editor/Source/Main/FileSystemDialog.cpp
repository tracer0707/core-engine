#include "FileSystemDialog.h"

#include <filesystem>

#include <Core/Renderer/Renderer.h>
#include <Core/Shared/IO.h>
#include <Core/Shared/Path.h>
#include <Core/Shared/List.h>
#include <Core/Shared/String.h>

#include "../Utils/FileSystemUtils.h"
#include "../Shared/IconsForkAwesome.h"
#include "../Editor/Font.h"
#include "../Editor/Windows/FullscreenWindow.h"
#include "../Editor/Controls/LinearLayout.h"
#include "../Editor/Controls/Button.h"
#include "../Editor/Controls/TextInput.h"
#include "../Editor/Controls/TreeView.h"
#include "../Editor/Controls/TreeNode.h"
#include "../Editor/Controls/Label.h"

namespace Editor
{
	FileSystemDialog::FileSystemDialog(Core::Application* app) : Core::Window(app, "File Dialog", 800, 400)
	{
		_mainFont = new Font(Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Fonts/Roboto-Regular.ttf"), 15.0f);

		ImGuiIO& io = ImGui::GetIO();
		static const ImWchar icons_ranges[] = {ICON_MIN_FK, ICON_MAX_16_FK, 0};
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		icons_config.GlyphMinAdvanceX = 15.0f;
		io.Fonts->AddFontFromFileTTF(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Fonts", FONT_ICON_FILE_NAME_FK).std_str().c_str(), 15.0f,
			&icons_config, icons_ranges);

		Font::rebuildFonts();

		_mainFont->setDefault();

		_layout = new LinearLayout(LayoutDirection::Vertical);
		_layout->getStyle().paddingX = 10;
		_layout->getStyle().paddingY = 10;

		_topLayout = new LinearLayout(LayoutDirection::Vertical);

		_treeView = new TreeView();

		rescanFs();

		_topLayout->addControl(_treeView);

		_bottomLayout = new LinearLayout(LayoutDirection::Horizontal);
		_bottomLayout->setHorizontalAlignment(LayoutHorizontalAlignment::Center);

		_selectedPath = new TextInput();
		_selectedCount = new Label();
		_selectedCount->setVisible(false);

		Button* okBtn = new Button("OK");
		Button* cancelBtn = new Button("Cancel");

		okBtn->setEnabled(false);

		_bottomLayout->addControl(_selectedPath);
		_bottomLayout->addControl(_selectedCount);
		_bottomLayout->addControl(okBtn);
		_bottomLayout->addControl(cancelBtn);

		_bottomLayout->setHeight(32.0f);

		_layout->addControl(_topLayout);
		_layout->addControl(_bottomLayout);

		_wnd = new FullscreenWindow();
		_wnd->addControl(_layout);

		_treeView->setOnSelectionChanged([this, okBtn](Core::List<TreeNode*> lst) {
			if (!_multiple)
			{
				if (lst.count() > 0)
				{
					_selectedPath->setText(lst.get(0)->getStringTag(0));
					okBtn->setEnabled(true);
				}
				else
				{
					_selectedPath->setText(Core::String::Empty);
					okBtn->setEnabled(false);
				}
			}
			else
			{
				bool valid = true;
				for (auto& p : lst)
				{
					auto path = std::filesystem::path(p->getStringTag(0).std_str());
					if (!std::filesystem::exists(path))
					{
						valid = false;
						break;
					}

					if (!(_showFiles ? !std::filesystem::is_directory(path) : std::filesystem::is_directory(path)))
					{
						valid = false;
						break;
					}
				}

				if (valid)
				{
					_selected.clear();
					for (auto& p : lst)
					{
						_selected.add(p->getStringTag(0).std_str());
					}
				}

				okBtn->setEnabled(valid);

				if (valid)
				{
					_selectedCount->setText(std::to_string(lst.count()) + " files selected");
				}
				else
				{
					_selectedCount->setText(Core::String::Empty);
				}
			}
		});

		_selectedPath->setOnTextChanged([this, okBtn](Core::String value) {
			if (_multiple) return;

			auto path = std::filesystem::path(value.std_str());
			bool _exists = std::filesystem::exists(path);
			if (_showFiles)
			{
				okBtn->setEnabled(_exists && !std::filesystem::is_directory(path));
			}
			else
			{
				okBtn->setEnabled(_exists && std::filesystem::is_directory(path));
			}
		});

		cancelBtn->setOnClick([this]() { close(); });

		okBtn->setOnClick([=]() {
			if (_onPathSelected != nullptr)
			{
				if (!_multiple)
				{
					if (_selectedPath->getText() != Core::String::Empty)
					{
						_onPathSelected({_selectedPath->getText()});
					}
				}
				else if (!_selected.isEmpty())
				{
					_onPathSelected(_selected);
				}
			}

			close();
		});
	}

	FileSystemDialog::~FileSystemDialog()
	{
		delete _mainFont;
		delete _wnd;

		_mainFont = nullptr;
		_wnd = nullptr;
	}

	void FileSystemDialog::setShowFiles(bool value)
	{
		_showFiles = value;
		rescanFs();
	}

	void FileSystemDialog::setIsMultiple(bool value)
	{
		_multiple = value;
		_treeView->setSelectMultiple(value);
		_selectedPath->setVisible(!_multiple);
		_selectedCount->setVisible(_multiple);
	}

	void FileSystemDialog::rescanFs()
	{
		_treeView->clear();
		Core::List<Core::String> _diskDrives = FileSystemUtils::getDiskDrives();
		for (auto& d : _diskDrives)
		{
			FileSystemUtils::fsToTreeView(d, _treeView, nullptr, _showFiles, true);
		}
	}

	void FileSystemDialog::update()
	{
		_layout->setHeight(_height);
		_topLayout->setHeight(_height - 65);
	}

	void FileSystemDialog::render()
	{
		_renderer->setViewportSize(_width, _height);
		_renderer->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.1f, 0.1f, 0.1f, 1.0f));

		_renderer->beginUI();
		_wnd->update("File Dialog", _width, _height);
		_renderer->endUI();
	}
} // namespace Editor