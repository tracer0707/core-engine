#include "ContentImportWindow.h"

#include <filesystem>

#include <Core/Shared/Uuid.h>
#include <Core/Shared/Path.h>
#include <Core/Content/ContentDatabase.h>

#include "WindowList.h"
#include "WindowManager.h"

#include "../../Content/ContentImporter.h"
#include "../../Content/ContentMeta.h"
#include "../../Shared/ContentExtensions.h"

#include "../Controls/LinearLayout.h"
#include "../Controls/Label.h"
#include "../Controls/Button.h"
#include "../Controls/Dropdown.h"
#include "../Controls/Separator.h"

#include "../../Serialization/Meta/Texture2DMeta.h"

static std::vector<Core::String> image_ext = Editor::ContentExtensions::getSupportedTextureExtensions();
static std::vector<Core::String> mesh_ext = Editor::ContentExtensions::getSupportedMeshExtensions();

namespace fs = std::filesystem;

namespace Editor
{
	ContentImportWindow::ContentImportWindow(WindowManager* parent) : Window(parent, CONTENT_IMPORT_WINDOW)
	{
		setCanAcceptDocking(false);
		setCanDock(false);

		LinearLayout* mainLayout = new LinearLayout(LayoutDirection::Vertical);
		mainLayout->setWrapMode(LayoutWrapMode::NoWrap);
		mainLayout->setFitWidth(LayoutFitMode::FitAvailable);
		mainLayout->getStyle().paddingX = 10.0f;
		mainLayout->getStyle().paddingY = 10.0f;

		_currentFileLbl = new Label();

		_importLayout = new LinearLayout(LayoutDirection::Vertical);
		_importLayout->setWrapMode(LayoutWrapMode::NoWrap);

		LinearLayout* buttonLayout = new LinearLayout();
		buttonLayout->setHorizontalAlignment(LayoutHorizontalAlignment::Right);
		buttonLayout->setFitWidth(LayoutFitMode::FitAvailable);
		buttonLayout->setFitHeight(LayoutFitMode::FitContent);
		buttonLayout->setWrapMode(LayoutWrapMode::NoWrap);

		_importBtn = new Button("Import");
		Button* cancelBtn = new Button("Cancel");

		_importBtn->setSize(100.0f, 24.0f);
		cancelBtn->setSize(100.0f, 24.0f);

		buttonLayout->addControl(_importBtn);
		buttonLayout->addControl(cancelBtn);

		cancelBtn->setOnClick([this]() { close(); });

		mainLayout->addControl(_currentFileLbl);
		mainLayout->addControl(new Separator());
		mainLayout->addControl(_importLayout);
		mainLayout->addControl(buttonLayout);

		addControl(mainLayout);
	}

	ContentImportWindow::~ContentImportWindow() {}

	void ContentImportWindow::init() {}

	void ContentImportWindow::import(Core::List<fs::path> value, const fs::path& targetPath)
	{
		_filesToImport = value;
		_targetPath = targetPath;

		importNext();
	}

	void ContentImportWindow::importNext()
	{
		if (_filesToImport.count() == 0)
		{
			close();
			return;
		}

		fs::path path = _filesToImport[0];
		_currentFileLbl->setText(Core::Path::toUtf8(path.filename()));

		_importLayout->clear();

		if (std::find(image_ext.begin(), image_ext.end(), Core::Path::toUtf8(path.extension()).toLower()) != image_ext.end())
		{
			prepareTextureLayout();
		}
		else if (std::find(mesh_ext.begin(), mesh_ext.end(), Core::Path::toUtf8(path.extension()).toLower()) != mesh_ext.end())
		{
			prepareMeshLayout();
		}
		else
		{
			_filesToImport.removeAt(0);
			importNext();
		}
	}

	void ContentImportWindow::prepareTextureLayout()
	{
		WindowManager* mgr = _parent;
		EditorApp* app = mgr->getApplication();

		Label* texFmtLbl = new Label("Texture format");
		Dropdown* texFormat = new Dropdown({"Uncompressed", "Color", "Normal"});
		texFormat->setSelectedIndex(1);

		_importLayout->addControl(texFmtLbl);
		_importLayout->addControl(texFormat);

		fs::path srcFileName = _filesToImport[0];
		fs::path dstFileName = _targetPath / Core::Path::fromUtf8(Core::Path::toUtf8(srcFileName.filename().stem()) + ".texture");

		_importBtn->setOnClick([this, app, srcFileName, dstFileName, texFormat]() {
			ContentImporter importer(app);
			Core::TextureFormat fmt;
			Serialization::Texture2DMeta meta;

			Core::Uuid uuid = Core::ContentDatabase::singleton()->getUuid(dstFileName);
			fs::path metaPath = ContentMeta::getMetaPathFromUuid((Core::Application*)app, uuid);

			int selectedIndex = texFormat->getSelectedIndex();
			if (selectedIndex == 0)
			{
				fmt = Core::TextureFormat::RGBA8;
				meta.usage = Serialization::Texture2DMeta::Usage::Uncompressed;
			}
			else if (selectedIndex == 1)
			{
				fmt = Core::TextureFormat::BC7;
				meta.usage = Serialization::Texture2DMeta::Usage::Color;
			}
			else
			{
				fmt = Core::TextureFormat::BC5;
				meta.usage = Serialization::Texture2DMeta::Usage::Normal;
			}

			importer.importTexture2D(srcFileName, dstFileName, fmt);

			meta.save(metaPath);

			_filesToImport.removeAt(0);

			if (_onImportFinished != nullptr)
			{
				_onImportFinished(_filesToImport.count() == 0);
			}

			importNext();
		});
	}

	void ContentImportWindow::prepareMeshLayout()
	{
		WindowManager* mgr = _parent;
		EditorApp* app = mgr->getApplication();

		fs::path srcFileName = _filesToImport[0];
		fs::path dstFileName = _targetPath / Core::Path::fromUtf8(Core::Path::toUtf8(srcFileName.filename().stem()) + ".mesh");

		_importBtn->setOnClick([this, app, srcFileName, dstFileName]() {
			ContentImporter importer(app);

			importer.importMesh(srcFileName, dstFileName);

			_filesToImport.removeAt(0);

			if (_onImportFinished != nullptr)
			{
				_onImportFinished(_filesToImport.count() == 0);
			}

			importNext();
		});
	}

	void ContentImportWindow::onUpdate()
	{
		_importLayout->setHeight(getHeight() - 105);
	}

	void ContentImportWindow::onClose()
	{
		_filesToImport.clear();
		_importLayout->clear();
	}

} // namespace Editor