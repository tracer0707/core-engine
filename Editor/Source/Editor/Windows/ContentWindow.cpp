#include "ContentWindow.h"

#include <filesystem>
#include <imgui.h>

#include <Core/System/EventHandler.h>
#include <Core/Content/Texture2D.h>
#include <Core/Content/Material.h>
#include <Core/Content/Mesh.h>
#include <Core/Content/Scene.h>
#include <Core/Content/Script.h>
#include <Core/Content/ContentManager.h>
#include <Core/Content/ContentDatabase.h>
#include <Core/Shared/Path.h>

#include "../../Utils/FileSystemUtils.h"
#include "../../Utils/TextureUtils.h"
#include "../../Main/EditorApp.h"
#include "../../Main/FileSystemDialog.h"
#include "../../Main/ThumbManager.h"
#include "../../Shared/IconsForkAwesome.h"
#include "../../Shared/Tags.h"
#include "../../Content/ContentSerializer.h"
#include "../../Resources/Texture.h"

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

		Texture* _addTex = Texture::loadFromFile(_parent->getRenderer(), fs::current_path() / fs::path("Editor/Icons/editor/add.png"));
		Texture* _importTex = Texture::loadFromFile(_parent->getRenderer(), fs::current_path() / fs::path("Editor/Icons/editor/down.png"));

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

				_fsDlg->setOnPathSelected([this, parent](Core::List<fs::path> fileNames) {
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
		
		MenuItem* _materialMenuItem = new MenuItem(ICON_FK_CIRCLE " Material");
		_createResourceBtnCm->addControl(_materialMenuItem);

		_materialMenuItem->setOnClick([this]() {
			createResource(Core::Path::fromUtf8("new.material"), [this](const fs::path& path) {
				Core::Material* material = _parent->getContentManager()->createMaterial();
				ContentSerializer::serializeMaterial(material, path);
				_parent->getContentManager()->destroy(material);
			});
		});

		MenuItem* _sceneMenuItem = new MenuItem(ICON_FK_CUBES " Scene");
		_createResourceBtnCm->addControl(_sceneMenuItem);

		_sceneMenuItem->setOnClick([this]() {
			createResource(Core::Path::fromUtf8("new.scene"), [this](const fs::path& path) {
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
			setCurrentDir(Core::Path::fromUtf8(nodes[0]->getStringTag(TAG_FULL_PATH)));
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

	void ContentWindow::setCurrentDir(const fs::path& path)
	{
		if (path.empty()) return;

		_rightPane->clear();
		_currentDir = path;

		Core::List<fs::path> entries = FileSystemUtils::getPathEntries(path);

		for (auto& it : entries)
		{
			if (fs::is_directory(it)) continue;

			Core::Uuid contentUuid = Core::ContentDatabase::singleton()->getUuid(it);
			Core::String contentName = Core::Path::toUtf8(it.filename().stem());
			Core::ContentType contentType = getContentTypeFromPath(it);
			int contentTypeInt = static_cast<int>(contentType);

			Texture* tex = getThumb(it);

			ContentButton* thumbnail = new ContentButton();
			thumbnail->setImage(tex);
			thumbnail->setContentUuid(contentUuid);
			thumbnail->setContentName(contentName);
			thumbnail->setContentType(contentType);
			thumbnail->setSize(THUMB_W, THUMB_H);
			thumbnail->setStringTag(TAG_FULL_PATH, Core::Path::toUtf8(it));
			thumbnail->setDragDropSource(true, Core::String("CONTENT_") + std::to_string(contentTypeInt));
			thumbnail->setDragDropSourceLabel(contentName);
			thumbnail->setDragDropSourceData(DragDropData(contentUuid));
			if (contentType == Core::ContentType::Scene)
			{
				thumbnail->setOnDoubleClick([this, it]() {
					_parent->getEventHandler()->addEvent([this, it]() {
						((EditorApp::MainWindow*)_parent->getApplication()->getMainWindow())->loadScene(it);
						_parent->invalidateAll();
					});
				});
			}
			setInspector(thumbnail, contentType);

			_rightPane->addControl(thumbnail);
		}
	}

	void ContentWindow::setInspector(ContentButton* thumbnail, const Core::ContentType& contentType)
	{
		InspectorWindow* inspectorWnd = (InspectorWindow*)_parent->getWindow(INSPECTOR_WINDOW);

		if (contentType == Core::ContentType::Material)
		{
			thumbnail->setOnClick([this, thumbnail, inspectorWnd]() {
				Core::Material* mat = _parent->getContentManager()->loadMaterialFromFile(Core::Path::fromUtf8(thumbnail->getStringTag(TAG_FULL_PATH)));
				MaterialInspector* inspector = new MaterialInspector(mat, _parent->getRenderer(), _parent->getEventHandler(), _parent->getContentManager());
				inspector->build();
				inspectorWnd->clear();
				inspectorWnd->addControl(inspector);
			});
		}
	}

	void ContentWindow::createResource(const fs::path& thumbPath, std::function<void(const fs::path&)> createAndSaveFunc)
	{
		_parent->getEventHandler()->addEvent([this, thumbPath, createAndSaveFunc]() {
			ContentButton* thumbnail = createContentButtonForEdit(thumbPath);

			thumbnail->setOnEditCancelled([this, thumbnail]() {
				_parent->getEventHandler()->addEvent([this, thumbnail]() {
					_rightPane->removeControl(thumbnail);
					delete thumbnail;
				});
			});

			thumbnail->setOnEditComplete([this, thumbnail, thumbPath, createAndSaveFunc](Core::String newName) {
				Core::String ext = Core::Path::toUtf8(thumbPath.extension());
				if (!newName.endsWith(ext)) newName += ext;

				fs::path path = _currentDir / Core::Path::fromUtf8(newName);

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

	ContentButton* ContentWindow::createContentButtonForEdit(const fs::path& thumbPath)
	{
		ContentButton* thumbnail = new ContentButton();
		Texture* tex = getThumb(thumbPath);
		thumbnail->setImage(tex);
		thumbnail->setSize(THUMB_W, THUMB_H);
		thumbnail->startEdit();
		return thumbnail;
	}

	Texture* ContentWindow::getThumb(const fs::path& path)
	{
		ThumbManager thumbManager(_parent->getApplication(), _parent->getContentManager());
		
		fs::path thumbPath = thumbManager.getThumbPath(path);
		
		if (!fs::exists(thumbPath))
		{
			Core::String iconName = Core::String::Empty;
			Core::String ext = Core::Path::toUtf8(path.extension());

			if (ext == Core::String::Empty)
			{
				iconName = "folder.png";
			}
			else if (ext == ".ttf")
			{
				iconName = "font.png";
			}
			else if (ext == ".texture")
			{
				iconName = "texture.png";
			}
			else if (ext == ".mesh")
			{
				iconName = "mesh.png";
			}
			else if (ext == ".material")
			{
				iconName = "material.png";
			}
			else if (ext == ".scene")
			{
				iconName = "scene.png";
			}
			else if (ext == ".lua")
			{
				iconName = "script.png";
			}
			else
			{
				iconName = "fileEmpty.png";
			}

			thumbPath = fs::current_path() / fs::path("Editor/Icons/content") / Core::Path::fromUtf8(iconName);
		}

		auto it = _iconCache.find(thumbPath);
		if (it != _iconCache.end())
		{
			return it->second;
		}

		Texture* tex = Texture::loadFromFile(_parent->getRenderer(), thumbPath);
		if (tex != nullptr)
		{
			_iconCache[thumbPath] = tex;
		}

		return tex;
	}

	Core::ContentType ContentWindow::getContentTypeFromPath(const fs::path& path)
	{
		Core::String ext = Core::Path::toUtf8(path.extension());
		Core::ContentType contentType = Core::ContentType::None;

		if (ext == ".texture")
		{
			contentType = Core::ContentType::Texture2D;
		}
		else if (ext == ".material")
		{
			contentType = Core::ContentType::Material;
		}
		else if (ext == ".mesh")
		{
			contentType = Core::ContentType::Mesh;
		}
		else if (ext == ".scene")
		{
			contentType = Core::ContentType::Scene;
		}
		else if (ext == ".lua")
		{
			contentType = Core::ContentType::Script;
		}

		return contentType;
	}
} // namespace Editor