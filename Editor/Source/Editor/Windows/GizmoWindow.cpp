#include "GizmoWindow.h"

#include <filesystem>

#include <Core/Assets/AssetManager.h>
#include <Core/Assets/Texture.h>
#include <Core/Shared/Path.h>
#include <Core/Shared/String.h>

#include "WindowList.h"

#include "WindowManager.h"
#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

namespace Editor
{
	GizmoWindow::GizmoWindow(WindowManager* parent) : Window(parent, GIZMO_WINDOW)
	{
		Core::AssetManager* assetMgr = parent->getAssetManager();

		/* Layout */
		
		LinearLayout* layoutMain = new LinearLayout(LayoutDirection::Horizontal);

		addControl(layoutMain);

		/* Undo */

		Button* undoBtn = new Button();
		Core::Texture* undoBtnImage = assetMgr->loadTextureFromFile(Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/undo.png"), Core::TextureFormat::RGBA8);
		undoBtn->setSize(32, 32);
		undoBtn->setImage(undoBtnImage);

		layoutMain->addControl(undoBtn);

		/* Redo */

		Button* redoBtn = new Button();
		Core::Texture* redoBtnImage = assetMgr->loadTextureFromFile(Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/redo.png"), Core::TextureFormat::RGBA8);
		redoBtn->setSize(32, 32);
		redoBtn->setImage(redoBtnImage);

		layoutMain->addControl(redoBtn);

		/* Select */

		Button* selectBtn = new Button();
		Core::Texture* selectBtnImage = assetMgr->loadTextureFromFile(Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/select.png"), Core::TextureFormat::RGBA8);
		selectBtn->setSize(32, 32);
		selectBtn->setImage(selectBtnImage);

		layoutMain->addControl(selectBtn);

		/* Move */

		Button* moveBtn = new Button();
		Core::Texture* moveBtnImage = assetMgr->loadTextureFromFile(Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/move.png"), Core::TextureFormat::RGBA8);
		moveBtn->setSize(32, 32);
		moveBtn->setImage(moveBtnImage);

		layoutMain->addControl(moveBtn);

		/* Rotate */

		Button* rotateBtn = new Button();
		Core::Texture* rotateBtnImage = assetMgr->loadTextureFromFile(Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/rotate.png"), Core::TextureFormat::RGBA8);
		rotateBtn->setSize(32, 32);
		rotateBtn->setImage(rotateBtnImage);

		layoutMain->addControl(rotateBtn);

		/* Scale */

		Button* scaleBtn = new Button();
		Core::Texture* scaleBtnImage = assetMgr->loadTextureFromFile(Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/scale.png"), Core::TextureFormat::RGBA8);
		scaleBtn->setSize(32, 32);
		scaleBtn->setImage(scaleBtnImage);

		layoutMain->addControl(scaleBtn);
	}

	GizmoWindow::~GizmoWindow()
	{
	}
}