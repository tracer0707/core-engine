#include "ContentWindow.h"

#include <filesystem>
#include <imgui.h>

#include <Core/Content/Texture.h>

#include "WindowList.h"

#include "../../Utils/FileSystemUtils.h"
#include "../../Utils/EditorIcons.h"
#include "../../System/ThumbCacheManager.h"
#include "../../System/ContentLoader.h"
#include "../../Main/Editor.h"

#include "../Controls/LinearLayout.h"
#include "../Controls/SplitPanel.h"
#include "../Controls/Button.h"
#include "../Controls/TreeView.h"
#include "../Controls/TreeNode.h"
#include "../Controls/Separator.h"
#include "../Windows/WindowManager.h"

namespace fs = std::filesystem;

namespace Editor
{
	ContentWindow::ContentWindow(WindowManager* parent) : Window(parent, CONTENT_WINDOW)
	{
		LinearLayout* _mainLayout = new LinearLayout(LayoutDirection::Vertical);
		_mainLayout->setWrapMode(LayoutWrapMode::NoWrap);

		LinearLayout* _toolbar = new LinearLayout();
		LinearLayout* _leftPane = new LinearLayout(LayoutDirection::Vertical);
		_rightPane = new LinearLayout();

		Separator* _separator = new Separator();

		SplitPanel* _splitPanel = new SplitPanel();
		_treeView = new TreeView();

		Core::Texture* _addTex = loadEditorIcon(_parent->getContentLoader(), "editor/add.png");
		Button* _createResourceBtn = new Button(_addTex);
		_createResourceBtn->setSize(30, 30);

		_toolbar->setHeight(30);
		_toolbar->addControl(_createResourceBtn);

		_leftPane->addControl(_treeView);
		_leftPane->setWidth(200);

		_splitPanel->addControl(_leftPane);
		_splitPanel->addControl(_rightPane);

		_mainLayout->addControl(_toolbar);
		_mainLayout->addControl(_separator);
		_mainLayout->addControl(_splitPanel);

		addControl(_mainLayout);

		_treeView->setOnSelectionChanged([this](Core::List<TreeNode*>& nodes) {
			if (nodes.count() == 0) return;
			setCurrentDir(nodes[0]->getStringTag(0));
		});
	}

	ContentWindow::~ContentWindow()
	{
		clearLoadedResources();
	}

	void ContentWindow::init()
	{
		rescanContent();
	}

	void ContentWindow::rescanContent()
	{
		_treeView->clear();
		FileSystemUtils::fsToTreeView(_contentDir, _treeView, nullptr, false, false);
	}

	void ContentWindow::setCurrentDir(Core::String path)
	{
		_rightPane->clear();

		clearLoadedResources();

		_currentDir = path;

		Core::List<std::filesystem::path> entries = FileSystemUtils::getPathEntries(path);

		for (auto& it : entries)
		{
			Button* thumbnail = new Button(it.filename().generic_string());
			Core::Texture* tex = _parent->getThumbCacheManager()->getOrCreate(it.generic_string());

			if (tex == nullptr)
			{
				tex = getIcon(it.extension().generic_string());
			}

			thumbnail->setImage(tex);
			thumbnail->setSize(64, 80);

			_rightPane->addControl(thumbnail);

			if (tex != nullptr)
			{
				_loadedThumbs.add(tex);
			}
		}
	}

	Core::Texture* ContentWindow::getIcon(Core::String ext)
	{
		Core::String iconName = Core::String::Empty;

		if (ext == Core::String::Empty)
		{
			iconName = "folder.png";
		}
		else if (ext == ".ttf")
		{
			iconName = "font.png";
		}
		else if (ext == ".fbx")
		{
			iconName = "mesh.png";
		}

		if (iconName != Core::String::Empty)
		{
			return loadEditorIcon(_parent->getContentLoader(), Core::Path::combine("content", iconName));
		}

		return nullptr;
	}

	void ContentWindow::clearLoadedResources()
	{
		for (auto it : _loadedThumbs)
		{
			_parent->getContentLoader()->unload(it);
		}

		_loadedThumbs.clear();
	}
} // namespace Editor