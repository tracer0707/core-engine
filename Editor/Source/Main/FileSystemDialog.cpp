#include "FileSystemDialog.h"

#include <filesystem>

#include <Core/Renderer/Renderer.h>
#include <Core/Shared/IO.h>
#include <Core/Shared/Path.h>
#include <Core/Shared/List.h>
#include <Core/Shared/String.h>
#include <Core/System/EventHandler.h>

#include "../Utils/FileSystemUtils.h"
#include "../Shared/IconsForkAwesome.h"
#include "../Shared/Tags.h"
#include "../Editor/Font.h"
#include "../Editor/Windows/FullscreenWindow.h"
#include "../Editor/Controls/LinearLayout.h"
#include "../Editor/Controls/Button.h"
#include "../Editor/Controls/InputText.h"
#include "../Editor/Controls/TreeView.h"
#include "../Editor/Controls/TreeNode.h"
#include "../Editor/Controls/Label.h"

namespace fs = std::filesystem;

namespace Editor
{
	FileSystemDialog::FileSystemDialog(Core::Application* app, Core::String title, FileSystemDialogType dialogType) : Core::Window(app, title, 800, 400)
	{
		_dialogType = dialogType;
		_mainFont = new Font((fs::current_path() / fs::path("Editor/Fonts/Roboto-Regular.ttf")).generic_string(), 15.0f);

		ImGuiIO& io = ImGui::GetIO();
		static const ImWchar icons_ranges[] = {ICON_MIN_FK, ICON_MAX_16_FK, 0};
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		icons_config.GlyphMinAdvanceX = 15.0f;
		io.Fonts->AddFontFromFileTTF(
			(fs::current_path() / fs::path("Editor/Fonts") / fs::path(FONT_ICON_FILE_NAME_FK)).generic_string().c_str(), 15.0f,
			&icons_config, icons_ranges);

		Font::rebuildFonts();

		_mainFont->setDefault();

		_layout = new LinearLayout(LayoutDirection::Vertical);
		_layout->setFitWidth(LayoutFitMode::FitAvailable);
		_layout->setWrapMode(LayoutWrapMode::NoWrap);
		_layout->getStyle().paddingX = 10;
		_layout->getStyle().paddingY = 10;

		_topLayout = new LinearLayout(LayoutDirection::Vertical);
		_topLayout->setFitWidth(LayoutFitMode::FitAvailable);
		_topLayout->setWrapMode(LayoutWrapMode::NoWrap);

		_treeView = new TreeView();

		rescanFs();

		_topLayout->addControl(_treeView);

		_bottomLayout = new LinearLayout(LayoutDirection::Horizontal);
		_bottomLayout->setHorizontalAlignment(LayoutHorizontalAlignment::Right);
		_bottomLayout->setVerticalAlignment(LayoutVerticalAlignment::Middle);
		_bottomLayout->setFitWidth(LayoutFitMode::FitAvailable);
		_bottomLayout->setFitHeight(LayoutFitMode::FitContent);
		_bottomLayout->setWrapMode(LayoutWrapMode::NoWrap);

		_selectedPath = new InputText();
		_selectedPath->setWidth(200.0f);
		_selectedCount = new Label();

		Button* okBtn = new Button(dialogType == FileSystemDialogType::Save ? "Save" : "Open");
		Button* cancelBtn = new Button("Cancel");

		okBtn->setSize(100, 24.0f);
		cancelBtn->setSize(100, 24.0f);

		okBtn->setEnabled(false);

		_bottomLayout->addControl(_selectedCount);
		_bottomLayout->addControl(_selectedPath);
		_bottomLayout->addControl(okBtn);
		_bottomLayout->addControl(cancelBtn);

		_layout->addControl(_topLayout);
		_layout->addControl(_bottomLayout);

		_wnd = new FullscreenWindow();
		_wnd->addControl(_layout);

		_treeView->setOnSelectionChanged([this, okBtn](Core::List<TreeNode*> lst) {
			if (!_multiple)
			{
				_eventHandler->addEvent([this, lst, okBtn]()
				{
					if (lst.count() > 0)
					{
						if (_dialogType == FileSystemDialogType::Open)
						{
							_selectedPath->setValue(lst.get(0)->getStringTag(TAG_FULL_PATH));
						}
						else
						{
							Core::String path = lst.get(0)->getStringTag(TAG_FULL_PATH);
							auto _path = fs::path(path.std_str());

							if (!fs::is_directory(_path))
							{
								Core::String dirPath = _path.parent_path().generic_string();
								if (!dirPath.endsWith("/")) dirPath += "/";
								_selectedCount->setText(dirPath);
								_selectedPath->setValue(_path.filename().generic_string());
							}
							else
							{
								if (!path.endsWith("/")) path += "/";
								_selectedCount->setText(path);
								okBtn->setEnabled(!_selectedPath->getValue().empty());
							}
						}
					}
					else
					{
						if (_dialogType == FileSystemDialogType::Open)
						{
							_selectedPath->setValue(Core::String::Empty);
						}
						else
						{
							_selectedCount->setText(Core::String::Empty);
							okBtn->setEnabled(false);
						}
					}
				});
			}
			else
			{
				bool valid = true;
				for (auto& p : lst)
				{
					auto path = fs::path(p->getStringTag(TAG_FULL_PATH).std_str());
					if (!fs::exists(path))
					{
						valid = false;
						break;
					}

					if (!(_showFiles ? !fs::is_directory(path) : fs::is_directory(path)))
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
						_selected.add(p->getStringTag(TAG_FULL_PATH).std_str());
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

		_selectedPath->setOnValueChanged([this, okBtn](Core::String value) {
			if (_multiple) return;

			if (_dialogType == FileSystemDialogType::Open)
			{
				if (_selectedPath->getValue() == Core::String::Empty)
				{
					okBtn->setEnabled(false);
					return;
				}

				auto path = fs::path(value.std_str());
				bool _exists = fs::exists(path);
				if (_showFiles)
				{
					okBtn->setEnabled(_exists && !fs::is_directory(path));
				}
				else
				{
					okBtn->setEnabled(_exists && fs::is_directory(path));
				}
			}
			else
			{
				if (_selectedCount->getText().empty()) return;
				if (_selectedPath->getValue().empty())
				{
					okBtn->setEnabled(false);
					return;
				}

				auto path = fs::path(_selectedCount->getText().std_str()) / fs::path(value.std_str());
				bool _exists = fs::exists(path);

				okBtn->setEnabled(!_exists);
			}
		});

		cancelBtn->setOnClick([this]() { close(); });

		okBtn->setOnClick([=]() {
			if (_onPathSelected != nullptr)
			{
				if (!_multiple)
				{
					if (_dialogType == FileSystemDialogType::Open)
					{
						if (_selectedPath->getValue() != Core::String::Empty)
						{
							_onPathSelected({_selectedPath->getValue()});
						}
					}
					else
					{
						Core::String _left = _selectedCount->getText();
						Core::String _right = _selectedPath->getValue();

						if (!_left.empty() && !_right.empty())
						{
							auto path = fs::path(_left.std_str()) / fs::path(_right.std_str());
							_onPathSelected({path.generic_string()});
						}
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
		if (_dialogType == FileSystemDialogType::Save)
		{
			_multiple = false;
			return;
		}
		_multiple = value;
		_selected.clear();
		_treeView->setSelectMultiple(value);
		_selectedPath->setVisible(!_multiple);
		_selectedPath->setValue(Core::String::Empty);
		_selectedCount->setText(Core::String::Empty);
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
		_layout->setHeight((float)_height);
		_topLayout->setHeight((float)_height - 50.0f);

		_renderer->setViewportSize(_width, _height);
		_renderer->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.1f, 0.1f, 0.1f, 1.0f));

		_renderer->beginUI();
		_wnd->update("File Dialog", _width, _height);
		_renderer->endUI();
	}
} // namespace Editor