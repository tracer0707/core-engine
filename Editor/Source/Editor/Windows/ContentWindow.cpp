#include "ContentWindow.h"

#include <filesystem>
#include <imgui.h>

#include <Core/System/EventHandler.h>
#include <Core/Content/Texture2D.h>
#include <Core/Content/Material.h>
#include <Core/Content/Mesh.h>
#include <Core/Content/ContentManager.h>
#include <Core/Content/ContentDatabase.h>

#include "../../Utils/FileSystemUtils.h"
#include "../../Utils/TextureUtils.h"
#include "../../Main/EditorApp.h"
#include "../../Main/FileSystemDialog.h"
#include "../../Shared/IconsForkAwesome.h"
#include "../../Shared/Tags.h"
#include "../../Content/ContentSerializer.h"

#include "../Controls/LinearLayout.h"
#include "../Controls/SplitPanel.h"
#include "../Controls/Button.h"
#include "../Controls/ContentButton.h"
#include "../Controls/TreeView.h"
#include "../Controls/TreeNode.h"
#include "../Controls/Separator.h"
#include "../Controls/ContextMenu.h"
#include "../Controls/MenuItem.h"

#include "WindowList.h"
#include "WindowManager.h"
#include "ContentImportWindow.h"
#include "InspectorWindow.h"

#include "Inspector/MaterialInspector.h"

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
		_mainLayout->setFitWidth(LayoutFitMode::FitAvailable);
		_mainLayout->setFitHeight(LayoutFitMode::FitAvailable);

		LinearLayout* _toolbar = new LinearLayout(LayoutDirection::Horizontal);
		_toolbar->setFitWidth(LayoutFitMode::FitAvailable);
		_toolbar->setFitHeight(LayoutFitMode::FitContent);

		LinearLayout* _leftPane = new LinearLayout(LayoutDirection::Vertical);
		_leftPane->setFitHeight(LayoutFitMode::FitAvailable);

		_rightPane = new LinearLayout(LayoutDirection::Horizontal);
		_rightPane->setFitHeight(LayoutFitMode::FitAvailable);

		Separator* _separator = new Separator();

		SplitPanel* _splitPanel = new SplitPanel();
		_treeView = new TreeView();

		Core::Texture2D* _addTex = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/add.png"), _parent->getContentManager());

		Core::Texture2D* _importTex = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/down.png"), _parent->getContentManager());

		_createResourceBtn = new Button("Create", _addTex);
		_createResourceBtn->setHeight(24);
		_createResourceBtn->getStyle().paddingX = 8;
		_createResourceBtn->setUseContextMenu(true);
		_createResourceBtn->setEnabled(false);

		_importResourceBtn = new Button("Import", _importTex);
		_importResourceBtn->setHeight(24);
		_importResourceBtn->getStyle().paddingX = 8;
		_importResourceBtn->setEnabled(false);

		_importResourceBtn->setOnClick([this, parent]() {
			parent->getApplication()->getEventHandler()->addEvent([this, parent] {
				if (_fsDlg != nullptr) return;

				_fsDlg = new FileSystemDialog(parent->getApplication(), "Import Content", FileSystemDialogType::Open);
				_fsDlg->setIsMultiple(true);

				_fsDlg->setOnClose([this]() { _fsDlg = nullptr; });

				_fsDlg->setOnPathSelected([this, parent](Core::List<Core::String> fileNames) {
					ContentImportWindow* wnd = (ContentImportWindow*)parent->getWindow(CONTENT_IMPORT_WINDOW);
					wnd->setVisible(true);
					wnd->import(fileNames, _currentDir);
					wnd->setOnImportFinished([this](bool allFinished) {
						rescanStructure();
						rescanCurrentDir();
					});
				});
			});
		});

		ContextMenu* _createResourceBtnCm = _createResourceBtn->getContextMenu();
		
		MenuItem* _materialMenuItem = new MenuItem(ICON_FK_CIRCLE "Material");
		_createResourceBtnCm->addControl(_materialMenuItem);

		_materialMenuItem->setOnClick([this]() {
			createResource(".material", [this](const Core::String& path) {
				Core::Material* material = _parent->getContentManager()->createMaterial();
				ContentSerializer::serializeMaterial(material, path);
				_parent->getContentManager()->destroy(material);
			});
		});

		MenuItem* _sceneMenuItem = new MenuItem(ICON_FK_CUBES "Scene");
		_createResourceBtnCm->addControl(_sceneMenuItem);

		_sceneMenuItem->setOnClick([this]() {
			createResource(".scene", [this](const Core::String& path) {
				Core::Scene* scene = _parent->getContentManager()->createScene();
				ContentSerializer::serializeScene(scene, path);
				_parent->getContentManager()->destroy(scene);
			});
		});

		_toolbar->addControl(_createResourceBtn);
		_toolbar->addControl(_importResourceBtn);

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
			_importResourceBtn->setEnabled(true);
			setCurrentDir(nodes[0]->getStringTag(TAG_FULL_PATH));
		});

		_parent->getContentManager()->setOnResourceLoaded([](Core::Content*) { Core::ContentDatabase::singleton()->save(); });
	}

	ContentWindow::~ContentWindow() {}

	void ContentWindow::init()
	{
		rescanStructure();
	}

	void ContentWindow::rescanStructure()
	{
		_treeView->clear();
		FileSystemUtils::fsToTreeView(_contentDir, _treeView, nullptr, false, true, false);
	}

	void ContentWindow::rescanCurrentDir()
	{
		setCurrentDir(_currentDir);
	}

	void ContentWindow::setCurrentDir(Core::String path)
	{
		if (path.empty()) return;

		_rightPane->clear();
		_currentDir = path;

		Core::List<std::filesystem::path> entries = FileSystemUtils::getPathEntries(path);

		for (auto& it : entries)
		{
			if (fs::is_directory(it)) continue;

			Core::Texture2D* tex = nullptr;
			Core::Content* content = nullptr;
			Core::String ext = it.extension().generic_string();

			if (ext == ".texture")
			{
				tex = _parent->getContentManager()->loadTexture2DFromFile(it.generic_string());
				content = tex;
			}
			else if (ext == ".material")
			{
				tex = getIcon(ext);
				content = _parent->getContentManager()->loadMaterialFromFile(it.generic_string());
			}
			else if (ext == ".mesh")
			{
				tex = getIcon(ext);
				content = _parent->getContentManager()->loadMeshFromFile(it.generic_string());
			}
			else
			{
				tex = getIcon(ext);
			}

			if (content != nullptr)
			{
				ContentButton* thumbnail = new ContentButton();
				thumbnail->setImage(tex);
				thumbnail->setContent(content);
				thumbnail->setSize(THUMB_W, THUMB_H);
				thumbnail->setStringTag(TAG_FULL_PATH, it.generic_string());
				setInspector(thumbnail, ext);

				_rightPane->addControl(thumbnail);
			}

			/*if (fs::is_directory(it))
			{
				thumbnail->setOnDoubleClick([this, thumbnail]() {
					Core::String p = thumbnail->getStringTag(TAG_FULL_PATH);
					TreeNode* node = _treeView->findNodeByTag(0, p);
					if (node != nullptr)
					{
						node->openParents();
						_treeView->selectNode(node, false);
					}
					_parent->getEventHandler()->addEvent([this, p]() { setCurrentDir(p); });
				});
			}*/
		}
	}

	ContentButton* ContentWindow::createThumbnailForEdit(Core::String ext)
	{
		ContentButton* thumbnail = new ContentButton();
		Core::Texture2D* tex = getIcon(ext);
		thumbnail->setImage(tex);
		thumbnail->setSize(THUMB_W, THUMB_H);
		thumbnail->startEdit();
		return thumbnail;
	}

	void ContentWindow::setInspector(ContentButton* thumbnail, Core::String ext)
	{
		InspectorWindow* inspectorWnd = (InspectorWindow*)_parent->getWindow(INSPECTOR_WINDOW);

		if (ext == ".material")
		{
			thumbnail->setOnClick([this, thumbnail, inspectorWnd]() {
				Core::Material* mat = _parent->getContentManager()->loadMaterialFromFile(thumbnail->getStringTag(TAG_FULL_PATH));
				MaterialInspector* inspector = new MaterialInspector(mat, _parent->getRenderer(), _parent->getEventHandler());
				inspector->build();
				inspectorWnd->clear();
				inspectorWnd->addControl(inspector);
			});
		}
	}

	Core::Texture2D* ContentWindow::getIcon(Core::String ext)
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
		else if (ext == ".mesh")
		{
			iconName = "mesh.png";
		}
		else if (ext == ".material")
		{
			iconName = "material.png";
		}
		else
		{
			iconName = "fileEmpty.png";
		}

		if (iconName != Core::String::Empty)
		{
			return TextureUtils::loadCompressed(
				Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/content", iconName),
				_parent->getContentManager());
		}

		return nullptr;
	}

	void ContentWindow::createResource(const Core::String& extension, std::function<void(const Core::String&)> createAndSaveFunc)
	{
		_parent->getEventHandler()->addEvent([this, extension, createAndSaveFunc]() {
			ContentButton* thumbnail = createThumbnailForEdit(extension);

			thumbnail->setOnEditCancelled([this, thumbnail]() {
				_parent->getEventHandler()->addEvent([this, thumbnail]() {
					_rightPane->removeControl(thumbnail);
					delete thumbnail;
				});
			});

			thumbnail->setOnEditComplete([this, thumbnail, extension, createAndSaveFunc](Core::String newName) {
				if (!newName.endsWith(extension)) newName += extension;

				Core::String path = Core::Path::combine(_currentDir, newName);

				createAndSaveFunc(path);

				_parent->getEventHandler()->addEvent([this, thumbnail]() {
					_rightPane->removeControl(thumbnail);
					delete thumbnail;
					rescanCurrentDir();
				});
			});

			_rightPane->addControl(thumbnail);
		});
	}
} // namespace Editor