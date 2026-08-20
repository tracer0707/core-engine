#include "ContentImportWindow.h"

#include <filesystem>

#include <Core/Shared/Path.h>

#include "WindowList.h"

#include "../../Content/ContentImporter.h"
#include "WindowManager.h"

#include "../Controls/LinearLayout.h"
#include "../Controls/Label.h"
#include "../Controls/Button.h"
#include "../Controls/Dropdown.h"
#include "../Controls/Separator.h"

static std::vector<std::string> image_extensions = {".jpg",	 ".jpeg", ".jpe",  ".jif",	".jfif", ".jfi", ".png", ".gif", ".bmp", ".dib",  ".tiff",
													".tif",	 ".webp", ".heic", ".heif", ".avif", ".cr2", ".cr3", ".crw", ".nef", ".nrw",  ".arw",
													".srf",	 ".sr2",  ".raf",  ".orf",	".rw2",	 ".raw", ".dng", ".psd", ".psb", ".ai",	  ".eps",
													".epsf", ".epsi", ".svg",  ".svgz", ".tga",	 ".dds", ".exr", ".hdr", ".ico", ".icns", ".jp2",
													".j2k",	 ".jxr",  ".jxl",  ".apng", ".pcx",	 ".xbm", ".xpm", ".pnm", ".ppm", ".pgm",  ".pbm"};

static std::vector<std::string> mesh_extensions = {".fbx", ".3ds", ".obj"};

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

		LinearLayout* buttonLayout = new LinearLayout();
		buttonLayout->setHorizontalAlignment(LayoutHorizontalAlignment::Right);
		buttonLayout->setFitWidth(LayoutFitMode::FitAvailable);
		buttonLayout->setFitHeight(LayoutFitMode::FitContent);

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

	void ContentImportWindow::import(Core::List<Core::String> value, Core::String targetPath)
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

		fs::path path = _filesToImport[0].std_str();
		_currentFileLbl->setText(path.filename().generic_string());

		_importLayout->clear();

		if (std::find(image_extensions.begin(), image_extensions.end(), path.extension().generic_string()) != image_extensions.end())
		{
			prepareTextureLayout();
		}
		else if (std::find(mesh_extensions.begin(), mesh_extensions.end(), path.extension().generic_string()) != mesh_extensions.end())
		{
			prepareMeshLayout();
		}
	}

	void ContentImportWindow::prepareTextureLayout()
	{
		WindowManager* mgr = _parent;
		EditorApp* app = mgr->getApplication();

		Label* texFmtLbl = new Label("Texture format");
		Dropdown* texFormat = new Dropdown({"RGBA", "BC7"});

		_importLayout->addControl(texFmtLbl);
		_importLayout->addControl(texFormat);

		Core::String srcFileName = _filesToImport[0];
		fs::path path = srcFileName.std_str();
		Core::String dstFileName = Core::Path::combine(_targetPath, path.filename().stem().generic_string() + ".texture");

		_importBtn->setOnClick([this, app, srcFileName, dstFileName, texFormat]() {
			ContentImporter importer(app);
			Core::TextureFormat fmt;

			if (texFormat->getSelectedIndex() == 0)
				fmt = Core::TextureFormat::RGBA8;
			else
				fmt = Core::TextureFormat::BC7;

			importer.importTexture2D(srcFileName, dstFileName, fmt);

			_filesToImport.removeAt(0);
			importNext();
		});
	}

	void ContentImportWindow::prepareMeshLayout()
	{
		WindowManager* mgr = _parent;
		EditorApp* app = mgr->getApplication();

		Core::String srcFileName = _filesToImport[0];
		fs::path path = srcFileName.std_str();
		Core::String dstFileName = Core::Path::combine(_targetPath, path.filename().stem().generic_string() + ".mesh");

		_importBtn->setOnClick([this, app, srcFileName, dstFileName]() {
			ContentImporter importer(app);
			
			importer.importMesh(srcFileName, dstFileName);

			_filesToImport.removeAt(0);
			importNext();
		});
	}

	void ContentImportWindow::onUpdate()
	{
		_importLayout->setHeight(getHeight() - 100);
	}

	void ContentImportWindow::onClose()
	{
		_filesToImport.clear();
		_importLayout->clear();
	}

} // namespace Editor