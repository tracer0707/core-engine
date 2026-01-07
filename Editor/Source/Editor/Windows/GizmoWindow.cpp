#include "GizmoWindow.h"

#include <filesystem>

#include <Core/Content/ContentManager.h>
#include <Core/Content/Texture.h>
#include <Core/Shared/Path.h>
#include <Core/Shared/String.h>

#include "WindowList.h"

#include "WindowManager.h"
#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"
#include "../../Utils/TextureUtils.h"

namespace Editor
{
	GizmoWindow::GizmoWindow(WindowManager* parent) : Window(parent, GIZMO_WINDOW)
	{
		Core::ContentManager* contentMgr = parent->getContentManager();

		/* Layout */

		LinearLayout* layoutMain = new LinearLayout(LayoutDirection::Horizontal);

		addControl(layoutMain);

		/* Undo */

		Button* undoBtn = new Button();
		Core::Texture* undoBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/undo.png"), contentMgr);
		undoBtn->setSize(32, 32);
		undoBtn->setImage(undoBtnImage);

		layoutMain->addControl(undoBtn);

		/* Redo */

		Button* redoBtn = new Button();
		Core::Texture* redoBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/redo.png"), contentMgr);
		redoBtn->setSize(32, 32);
		redoBtn->setImage(redoBtnImage);

		layoutMain->addControl(redoBtn);

		/* Select */

		Button* selectBtn = new Button();
		Core::Texture* selectBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/select.png"), contentMgr);
		selectBtn->setSize(32, 32);
		selectBtn->setImage(selectBtnImage);

		layoutMain->addControl(selectBtn);

		/* Move */

		Button* moveBtn = new Button();
		Core::Texture* moveBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/move.png"), contentMgr);
		moveBtn->setSize(32, 32);
		moveBtn->setImage(moveBtnImage);

		layoutMain->addControl(moveBtn);

		/* Rotate */

		Button* rotateBtn = new Button();
		Core::Texture* rotateBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/rotate.png"), contentMgr);
		rotateBtn->setSize(32, 32);
		rotateBtn->setImage(rotateBtnImage);

		layoutMain->addControl(rotateBtn);

		/* Scale */

		Button* scaleBtn = new Button();
		Core::Texture* scaleBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/scale.png"), contentMgr);
		scaleBtn->setSize(32, 32);
		scaleBtn->setImage(scaleBtnImage);

		layoutMain->addControl(scaleBtn);
	}

	GizmoWindow::~GizmoWindow() {}
} // namespace Editor