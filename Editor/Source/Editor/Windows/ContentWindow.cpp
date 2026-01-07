#include "ContentWindow.h"

#include <filesystem>
#include <imgui.h>

#include <Core/System/EventHandler.h>
#include <Core/Content/Texture.h>
#include <Core/Content/Material.h>
#include <Core/Content/ContentManager.h>

#include "WindowList.h"

#include "../../Utils/FileSystemUtils.h"
#include "../../Utils/TextureUtils.h"
#include "../../System/ThumbCacheManager.h"
#include "../../Main/Editor.h"
#include "../../Shared/IconsForkAwesome.h"

#include "../Controls/LinearLayout.h"
#include "../Controls/SplitPanel.h"
#include "../Controls/Button.h"
#include "../Controls/TreeView.h"
#include "../Controls/TreeNode.h"
#include "../Controls/Separator.h"
#include "../Controls/ContextMenu.h"
#include "../Controls/MenuItem.h"
#include "../Windows/WindowManager.h"

#include <iostream>

namespace fs = std::filesystem;

namespace Editor
{
	static int THUMB_W = 64;
	static int THUMB_H = 80;

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

		Core::Texture* _addTex = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/add.png"), _parent->getContentManager());
		_createResourceBtn = new Button(_addTex);
		_createResourceBtn->setSize(30, 30);
		_createResourceBtn->setUseContextMenu(true);
		_createResourceBtn->setEnabled(false);

		ContextMenu* _createResourceBtnCm = _createResourceBtn->getContextMenu();
		MenuItem* _materialMenuItem = new MenuItem(ICON_FK_CIRCLE "Material");
		_createResourceBtnCm->addControl(_materialMenuItem);

		_materialMenuItem->setOnClick([this]() {
			_parent->getEventHandler()->addEvent([this]() {
				Button* thumbnail = createThumbnailForEdit(".mat");
				thumbnail->setOnEditCancelled([this, thumbnail]() {
					_parent->getEventHandler()->addEvent([this, thumbnail]() {
						_rightPane->removeControl(thumbnail);
						delete thumbnail;
					});
				});
				thumbnail->setOnEditComplete([this, thumbnail]() {
					if (!thumbnail->getText().endsWith(".mat")) thumbnail->setText(thumbnail->getText().std_str() + ".mat");
					Core::Material* mat = _parent->getContentManager()->createMaterial();
					Core::String path = Core::Path::combine(_currentDir, thumbnail->getText());
					// nlohmann::serialize(mat, path);
				});
				_rightPane->addControl(thumbnail);
			});
		});

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
			_createResourceBtn->setEnabled(true);
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
		FileSystemUtils::fsToTreeView(_contentDir, _treeView, nullptr, false, false, false);
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
			Core::Texture* tex = _parent->getThumbCacheManager()->get(it.generic_string());

			if (tex == nullptr)
			{
				tex = getIcon(it.extension().generic_string());
			}

			thumbnail->setImage(tex);
			thumbnail->setSize(THUMB_W, THUMB_H);
			thumbnail->setStringTag(0, it.generic_string());

			if (fs::is_directory(it))
			{
				thumbnail->setOnDoubleClick([this, thumbnail]() {
					Core::String p = thumbnail->getStringTag(0);
					TreeNode* node = _treeView->findNodeByTag(0, p);
					if (node != nullptr)
					{
						node->openParents();
						_treeView->selectNode(node, false);
					}
					_parent->getEventHandler()->addEvent([this, p]() { setCurrentDir(p); });
				});
			}

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
		else if (ext == ".mat")
		{
			iconName = "material.png";
		}

		if (iconName != Core::String::Empty)
		{
			return TextureUtils::loadCompressed(
				Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/content", iconName),
				_parent->getContentManager());
		}

		return nullptr;
	}

	Button* ContentWindow::createThumbnailForEdit(Core::String ext)
	{
		Button* thumbnail = new Button();
		Core::Texture* tex = getIcon(ext);
		thumbnail->setImage(tex);
		thumbnail->setSize(THUMB_W, THUMB_H);
		thumbnail->startEdit();
		return thumbnail;
	}

	void ContentWindow::clearLoadedResources()
	{
		for (auto it : _loadedThumbs)
		{
			_parent->getContentManager()->destroy(it);
		}

		_loadedThumbs.clear();
	}
} // namespace Editor