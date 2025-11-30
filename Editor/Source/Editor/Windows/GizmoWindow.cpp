#include "GizmoWindow.h"

#include <filesystem>

#include <Core/Content/Texture.h>
#include <Core/Shared/Path.h>
#include <Core/Shared/String.h>

#include "WindowList.h"

#include "WindowManager.h"
#include "../System/ContentLoader.h"
#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

namespace Editor
{
	GizmoWindow::GizmoWindow(WindowManager* parent) : Window(parent, GIZMO_WINDOW)
	{
		ContentLoader* contentLoader = parent->getContentLoader();

		/* Layout */
		
		LinearLayout* layoutMain = new LinearLayout(LayoutDirection::Horizontal);

		addControl(layoutMain);

		/* Undo */

		Button* undoBtn = new Button();
        Core::Texture* undoBtnImage = contentLoader->loadTextureFromFile(
            Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/undo.png"),
            Core::TextureFormat::RGBA8, LoadMethod::Direct);
		undoBtn->setSize(32, 32);
		undoBtn->setImage(undoBtnImage);

		layoutMain->addControl(undoBtn);

		/* Redo */

		Button* redoBtn = new Button();
        Core::Texture* redoBtnImage = contentLoader->loadTextureFromFile(
            Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/redo.png"),
            Core::TextureFormat::RGBA8, LoadMethod::Direct);
		redoBtn->setSize(32, 32);
		redoBtn->setImage(redoBtnImage);

		layoutMain->addControl(redoBtn);

		/* Select */

		Button* selectBtn = new Button();
        Core::Texture* selectBtnImage = contentLoader->loadTextureFromFile(
            Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/select.png"),
            Core::TextureFormat::RGBA8, LoadMethod::Direct);
		selectBtn->setSize(32, 32);
		selectBtn->setImage(selectBtnImage);

		layoutMain->addControl(selectBtn);

		/* Move */

		Button* moveBtn = new Button();
        Core::Texture* moveBtnImage = contentLoader->loadTextureFromFile(
            Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/move.png"),
            Core::TextureFormat::RGBA8, LoadMethod::Direct);
		moveBtn->setSize(32, 32);
		moveBtn->setImage(moveBtnImage);

		layoutMain->addControl(moveBtn);

		/* Rotate */

		Button* rotateBtn = new Button();
        Core::Texture* rotateBtnImage = contentLoader->loadTextureFromFile(
            Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/rotate.png"),
            Core::TextureFormat::RGBA8, LoadMethod::Direct);
		rotateBtn->setSize(32, 32);
		rotateBtn->setImage(rotateBtnImage);

		layoutMain->addControl(rotateBtn);

		/* Scale */

		Button* scaleBtn = new Button();
        Core::Texture* scaleBtnImage = contentLoader->loadTextureFromFile(
            Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/scale.png"),
            Core::TextureFormat::RGBA8, LoadMethod::Direct);
		scaleBtn->setSize(32, 32);
		scaleBtn->setImage(scaleBtnImage);

		layoutMain->addControl(scaleBtn);
	}

	GizmoWindow::~GizmoWindow()
	{
	}
}