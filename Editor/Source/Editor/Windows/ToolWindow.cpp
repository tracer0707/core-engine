#include "ToolWindow.h"

#include <filesystem>

#include <Core/Content/ContentManager.h>
#include <Core/Content/Texture2D.h>
#include <Core/Shared/Path.h>
#include <Core/Shared/String.h>

#include "WindowList.h"
#include "WindowManager.h"

#include "../Gizmo.h"
#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"
#include "../Controls/Separator.h"
#include "../../Utils/TextureUtils.h"

namespace Editor
{
	ToolWindow::ToolWindow(WindowManager* parent) : Window(parent, TOOL_WINDOW)
	{
		Core::ContentManager* contentMgr = parent->getContentManager();

		/* Layout */

		LinearLayout* layoutMain = new LinearLayout(LayoutDirection::Horizontal);
		layoutMain->setFitWidth(LayoutFitMode::FitAvailable);
		layoutMain->setFitHeight(LayoutFitMode::FitContent);

		addControl(layoutMain);

		/* Undo */

		Button* undoBtn = new Button();
		Core::Texture2D* undoBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/undo.png"), contentMgr);
		undoBtn->setSize(32, 32);
		undoBtn->setImage(undoBtnImage);

		layoutMain->addControl(undoBtn);

		/* Redo */

		Button* redoBtn = new Button();
		Core::Texture2D* redoBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/redo.png"), contentMgr);
		redoBtn->setSize(32, 32);
		redoBtn->setImage(redoBtnImage);

		layoutMain->addControl(redoBtn);

		layoutMain->addControl(new Separator(SeparatorDirection::Vertical));

		/* Select */

		_selectBtn = new Button();
		Core::Texture2D* selectBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/select.png"), contentMgr);
		_selectBtn->setSize(32, 32);
		_selectBtn->setImage(selectBtnImage);
		_selectBtn->setOnClick([this]() {
			Gizmo::singleton()->setTransformMode(Gizmo::TransformMode::Select);
			invalidate();
		});

		layoutMain->addControl(_selectBtn);

		/* Move */

		_translateBtn = new Button();
		Core::Texture2D* moveBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/move.png"), contentMgr);
		_translateBtn->setSize(32, 32);
		_translateBtn->setImage(moveBtnImage);
		_translateBtn->setOnClick([this]() {
			Gizmo::singleton()->setTransformMode(Gizmo::TransformMode::Translate);
			invalidate();
		});

		layoutMain->addControl(_translateBtn);

		/* Rotate */

		_rotateBtn = new Button();
		Core::Texture2D* rotateBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/rotate.png"), contentMgr);
		_rotateBtn->setSize(32, 32);
		_rotateBtn->setImage(rotateBtnImage);
		_rotateBtn->setOnClick([this]() {
			Gizmo::singleton()->setTransformMode(Gizmo::TransformMode::Rotate);
			invalidate();
		});

		layoutMain->addControl(_rotateBtn);

		/* Scale */

		_scaleBtn = new Button();
		Core::Texture2D* scaleBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/scale.png"), contentMgr);
		_scaleBtn->setSize(32, 32);
		_scaleBtn->setImage(scaleBtnImage);
		_scaleBtn->setOnClick([this]() {
			Gizmo::singleton()->setTransformMode(Gizmo::TransformMode::Scale);
			invalidate();
		});

		layoutMain->addControl(_scaleBtn);

		layoutMain->addControl(new Separator(SeparatorDirection::Vertical));

		/* Local Space */

		_localSpaceBtn = new Button();
		Core::Texture2D* localSpaceBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/local.png"), contentMgr);
		_localSpaceBtn->setSize(32, 32);
		_localSpaceBtn->setImage(localSpaceBtnImage);
		_localSpaceBtn->setOnClick([this]() {
			Gizmo::singleton()->setTransformSpace(Gizmo::TransformSpace::Local);
			invalidate();
		});

		layoutMain->addControl(_localSpaceBtn);

		/* World Space */

		_worldSpaceBtn = new Button();
		Core::Texture2D* worldSpaceBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/world.png"), contentMgr);
		_worldSpaceBtn->setSize(32, 32);
		_worldSpaceBtn->setImage(worldSpaceBtnImage);
		_worldSpaceBtn->setOnClick([this]() {
			Gizmo::singleton()->setTransformSpace(Gizmo::TransformSpace::World);
			invalidate();
		});

		layoutMain->addControl(_worldSpaceBtn);

		invalidate();
	}

	ToolWindow::~ToolWindow() {}

	void ToolWindow::invalidate()
	{
		_selectBtn->setActive(Gizmo::singleton()->getTransformMode() == Gizmo::TransformMode::Select);
		_translateBtn->setActive(Gizmo::singleton()->getTransformMode() == Gizmo::TransformMode::Translate);
		_rotateBtn->setActive(Gizmo::singleton()->getTransformMode() == Gizmo::TransformMode::Rotate);
		_scaleBtn->setActive(Gizmo::singleton()->getTransformMode() == Gizmo::TransformMode::Scale);
		_localSpaceBtn->setActive(Gizmo::singleton()->getTransformSpace() == Gizmo::TransformSpace::Local);
		_worldSpaceBtn->setActive(Gizmo::singleton()->getTransformSpace() == Gizmo::TransformSpace::World);
	}
} // namespace Editor