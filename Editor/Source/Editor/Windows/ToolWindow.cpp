#include "ToolWindow.h"

#include <filesystem>

#include <Core/Shared/Path.h>
#include <Core/Shared/String.h>

#include "WindowList.h"
#include "WindowManager.h"

#include "../Gizmo.h"
#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"
#include "../Controls/Separator.h"
#include "../Controls/ContextMenu.h"
#include "../Controls/Checkbox.h"
#include "../Controls/Label.h"
#include "../Controls/InputFloat.h"
#include "../Controls/Table.h"

#include "../../Utils/TextureUtils.h"
#include "../../Resources/Texture.h"

namespace fs = std::filesystem;

namespace Editor
{
	ToolWindow::ToolWindow(WindowManager* parent) : Window(parent, TOOL_WINDOW)
	{
		Core::Renderer* renderer = _parent->getRenderer();

		/* Layout */

		LinearLayout* layoutMain = new LinearLayout(LayoutDirection::Horizontal);
		layoutMain->setFitWidth(LayoutFitMode::FitAvailable);
		layoutMain->setFitHeight(LayoutFitMode::FitContent);

		addControl(layoutMain);

		/* Undo */

		Button* undoBtn = new Button();
		Texture* undoBtnImage = Texture::loadFromFile(renderer, (fs::current_path() / fs::path("Editor/Icons/editor/undo.png")).generic_string());
		undoBtn->setSize(32, 32);
		undoBtn->setImage(undoBtnImage);

		layoutMain->addControl(undoBtn);

		/* Redo */

		Button* redoBtn = new Button();
		Texture* redoBtnImage = Texture::loadFromFile(renderer, (fs::current_path() / fs::path("Editor/Icons/editor/redo.png")).generic_string());
		redoBtn->setSize(32, 32);
		redoBtn->setImage(redoBtnImage);

		layoutMain->addControl(redoBtn);

		Separator* separator1 = new Separator(SeparatorDirection::Vertical);
		separator1->setHeight(30.0f);
		layoutMain->addControl(separator1);

		/* Select */

		_selectBtn = new Button();
		Texture* selectBtnImage = Texture::loadFromFile(renderer, (fs::current_path() / fs::path("Editor/Icons/editor/select.png")).generic_string());
		_selectBtn->setSize(32, 32);
		_selectBtn->setImage(selectBtnImage);
		_selectBtn->setOnClick([this]() {
			Gizmo::singleton()->setTransformMode(Gizmo::TransformMode::Select);
			invalidate();
		});

		layoutMain->addControl(_selectBtn);

		/* Move */

		_translateBtn = new Button();
		_translateBtn->setButtonType(ButtonType::Action);
		Texture* moveBtnImage = Texture::loadFromFile(renderer, (fs::current_path() / fs::path("Editor/Icons/editor/move.png")).generic_string());
		_translateBtn->setSize(45, 32);
		_translateBtn->setImage(moveBtnImage);
		_translateBtn->setOnClick([this]() {
			Gizmo::singleton()->setTransformMode(Gizmo::TransformMode::Translate);
			invalidate();
		});
		_translateBtn->setUseContextMenu(true);

		LinearLayout* translateContextMenuLayout = new LinearLayout(LayoutDirection::Vertical);
		translateContextMenuLayout->setFitWidth(LayoutFitMode::FitContent);
		translateContextMenuLayout->setHeight(50.0f);
		translateContextMenuLayout->setWrapMode(LayoutWrapMode::NoWrap);

		Table* translateContextMenuTable = new Table(2);
		translateContextMenuTable->setWidth(255.0f);

		Checkbox* enableMoveSnap = new Checkbox();
		enableMoveSnap->setValue(Gizmo::singleton()->getMoveSnap());
		enableMoveSnap->setOnValueChanged([](bool value) { Gizmo::singleton()->setMoveSnap(value); });
		Label* enableMoveSnapLabel = new Label("Enable Move Snap");
		translateContextMenuTable->addControl(enableMoveSnapLabel);
		translateContextMenuTable->addControl(enableMoveSnap);

		InputFloat* moveSnapStepInput = new InputFloat();
		moveSnapStepInput->setValue(Gizmo::singleton()->getMoveStepSize());
		moveSnapStepInput->setWidth(120.0f);
		moveSnapStepInput->setStep(2.0f);
		moveSnapStepInput->setLimitMin(0.125f);
		moveSnapStepInput->setLimitMax(16.0f);
		moveSnapStepInput->setIncrementType(InputFloat::IncrementType::Multiplicative);
		moveSnapStepInput->setOnValueChanged([](float value) { Gizmo::singleton()->setMoveStepSize(value); });
		Label* moveSnapStepLabel = new Label("Move Snap Step");
		translateContextMenuTable->addControl(moveSnapStepLabel);
		translateContextMenuTable->addControl(moveSnapStepInput);

		translateContextMenuLayout->addControl(translateContextMenuTable);

		ContextMenu* translateContextMenu = _translateBtn->getContextMenu();
		translateContextMenu->addControl(translateContextMenuLayout);

		layoutMain->addControl(_translateBtn);

		/* Rotate */

		_rotateBtn = new Button();
		_rotateBtn->setButtonType(ButtonType::Action);
		Texture* rotateBtnImage = Texture::loadFromFile(renderer, (fs::current_path() / fs::path("Editor/Icons/editor/rotate.png")).generic_string());
		_rotateBtn->setSize(45, 32);
		_rotateBtn->setImage(rotateBtnImage);
		_rotateBtn->setOnClick([this]() {
			Gizmo::singleton()->setTransformMode(Gizmo::TransformMode::Rotate);
			invalidate();
		});
		_rotateBtn->setUseContextMenu(true);

		LinearLayout* rotateContextMenuLayout = new LinearLayout(LayoutDirection::Vertical);
		rotateContextMenuLayout->setFitWidth(LayoutFitMode::FitContent);
		rotateContextMenuLayout->setHeight(50.0f);
		rotateContextMenuLayout->setWrapMode(LayoutWrapMode::NoWrap);

		Table* rotateContextMenuTable = new Table(2);
		rotateContextMenuTable->setWidth(255.0f);

		Checkbox* enableRotateSnap = new Checkbox();
		enableRotateSnap->setValue(Gizmo::singleton()->getRotateSnap());
		enableRotateSnap->setOnValueChanged([](bool value) { Gizmo::singleton()->setRotateSnap(value); });
		Label* enableRotateSnapLabel = new Label("Enable Rotate Snap");
		rotateContextMenuTable->addControl(enableRotateSnapLabel);
		rotateContextMenuTable->addControl(enableRotateSnap);

		InputFloat* rotateSnapStepInput = new InputFloat();
		rotateSnapStepInput->setValue(Gizmo::singleton()->getRotateStepSize());
		rotateSnapStepInput->setWidth(120.0f);
		rotateSnapStepInput->setStep(15.0f);
		rotateSnapStepInput->setLimitMin(15.0f);
		rotateSnapStepInput->setLimitMax(90.0f);
		rotateSnapStepInput->setIncrementType(InputFloat::IncrementType::Additive);
		rotateSnapStepInput->setOnValueChanged([](float value) { Gizmo::singleton()->setRotateStepSize(value); });
		Label* rotateSnapStepLabel = new Label("Rotate Snap Step");
		rotateContextMenuTable->addControl(rotateSnapStepLabel);
		rotateContextMenuTable->addControl(rotateSnapStepInput);

		rotateContextMenuLayout->addControl(rotateContextMenuTable);

		ContextMenu* rotateContextMenu = _rotateBtn->getContextMenu();
		rotateContextMenu->addControl(rotateContextMenuLayout);

		layoutMain->addControl(_rotateBtn);

		/* Scale */

		_scaleBtn = new Button();
		_scaleBtn->setButtonType(ButtonType::Action);
		Texture* scaleBtnImage = Texture::loadFromFile(renderer, (fs::current_path() / fs::path("Editor/Icons/editor/scale.png")).generic_string());
		_scaleBtn->setSize(45, 32);
		_scaleBtn->setImage(scaleBtnImage);
		_scaleBtn->setOnClick([this]() {
			Gizmo::singleton()->setTransformMode(Gizmo::TransformMode::Scale);
			invalidate();
		});
		_scaleBtn->setUseContextMenu(true);

		LinearLayout* scaleContextMenuLayout = new LinearLayout(LayoutDirection::Vertical);
		scaleContextMenuLayout->setFitWidth(LayoutFitMode::FitContent);
		scaleContextMenuLayout->setHeight(50.0f);
		scaleContextMenuLayout->setWrapMode(LayoutWrapMode::NoWrap);

		Table* scaleContextMenuTable = new Table(2);
		scaleContextMenuTable->setWidth(255.0f);

		Checkbox* enableScaleSnap = new Checkbox();
		enableScaleSnap->setValue(Gizmo::singleton()->getScaleSnap());
		enableScaleSnap->setOnValueChanged([](bool value) { Gizmo::singleton()->setScaleSnap(value); });
		Label* enableScaleSnapLabel = new Label("Enable Scale Snap");
		scaleContextMenuTable->addControl(enableScaleSnapLabel);
		scaleContextMenuTable->addControl(enableScaleSnap);

		InputFloat* scaleSnapStepInput = new InputFloat();
		scaleSnapStepInput->setValue(Gizmo::singleton()->getScaleStepSize());
		scaleSnapStepInput->setWidth(120.0f);
		scaleSnapStepInput->setStep(2.0f);
		scaleSnapStepInput->setLimitMin(0.125f);
		scaleSnapStepInput->setLimitMax(2.0f);
		scaleSnapStepInput->setIncrementType(InputFloat::IncrementType::Multiplicative);
		scaleSnapStepInput->setOnValueChanged([](float value) { Gizmo::singleton()->setScaleStepSize(value); });
		Label* scaleSnapStepLabel = new Label("Scale Snap Step");
		scaleContextMenuTable->addControl(scaleSnapStepLabel);
		scaleContextMenuTable->addControl(scaleSnapStepInput);

		scaleContextMenuLayout->addControl(scaleContextMenuTable);

		ContextMenu* scaleContextMenu = _scaleBtn->getContextMenu();
		scaleContextMenu->addControl(scaleContextMenuLayout);

		layoutMain->addControl(_scaleBtn);

		/* Bounds */

		_boundsBtn = new Button();
		_boundsBtn->setButtonType(ButtonType::Action);
		Texture* boundsBtnImage = Texture::loadFromFile(renderer, (fs::current_path() / fs::path("Editor/Icons/editor/button.png")).generic_string());
		_boundsBtn->setSize(45, 32);
		_boundsBtn->setImage(boundsBtnImage);
		_boundsBtn->setOnClick([this]() {
			Gizmo::singleton()->setTransformMode(Gizmo::TransformMode::Bounds);
			invalidate();
		});
		_boundsBtn->setUseContextMenu(true);

		LinearLayout* boundsContextMenuLayout = new LinearLayout(LayoutDirection::Vertical);
		boundsContextMenuLayout->setFitWidth(LayoutFitMode::FitContent);
		boundsContextMenuLayout->setHeight(50.0f);
		boundsContextMenuLayout->setWrapMode(LayoutWrapMode::NoWrap);

		Table* boundsContextMenuTable = new Table(2);
		boundsContextMenuTable->setWidth(255.0f);

		Checkbox* enableBoundsSnap = new Checkbox();
		enableBoundsSnap->setValue(Gizmo::singleton()->getBoundsSnap());
		enableBoundsSnap->setOnValueChanged([](bool value) { Gizmo::singleton()->setBoundsSnap(value); });
		Label* enableBoundsSnapLabel = new Label("Enable Bounds Snap");
		boundsContextMenuTable->addControl(enableBoundsSnapLabel);
		boundsContextMenuTable->addControl(enableBoundsSnap);

		InputFloat* boundsSnapStepInput = new InputFloat();
		boundsSnapStepInput->setValue(Gizmo::singleton()->getBoundsStepSize());
		boundsSnapStepInput->setWidth(120.0f);
		boundsSnapStepInput->setStep(2.0f);
		boundsSnapStepInput->setLimitMin(0.125f);
		boundsSnapStepInput->setLimitMax(16.0f);
		boundsSnapStepInput->setIncrementType(InputFloat::IncrementType::Multiplicative);
		boundsSnapStepInput->setOnValueChanged([](float value) { Gizmo::singleton()->setBoundsStepSize(value); });
		Label* boundsSnapStepLabel = new Label("Bounds Snap Step");
		boundsContextMenuTable->addControl(boundsSnapStepLabel);
		boundsContextMenuTable->addControl(boundsSnapStepInput);

		boundsContextMenuLayout->addControl(boundsContextMenuTable);

		ContextMenu* boundsContextMenu = _boundsBtn->getContextMenu();
		boundsContextMenu->addControl(boundsContextMenuLayout);

		layoutMain->addControl(_boundsBtn);

		Separator* separator2 = new Separator(SeparatorDirection::Vertical);
		separator2->setHeight(30.0f);
		layoutMain->addControl(separator2);

		/* Local Space */

		_localSpaceBtn = new Button();
		Texture* localSpaceBtnImage = Texture::loadFromFile(renderer, (fs::current_path() / fs::path("Editor/Icons/editor/local.png")).generic_string());
		_localSpaceBtn->setSize(32, 32);
		_localSpaceBtn->setImage(localSpaceBtnImage);
		_localSpaceBtn->setOnClick([this]() {
			Gizmo::singleton()->setTransformSpace(Gizmo::TransformSpace::Local);
			invalidate();
		});

		layoutMain->addControl(_localSpaceBtn);

		/* World Space */

		_worldSpaceBtn = new Button();
		Texture* worldSpaceBtnImage = Texture::loadFromFile(renderer, (fs::current_path() / fs::path("Editor/Icons/editor/world.png")).generic_string());
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
		_boundsBtn->setActive(Gizmo::singleton()->getTransformMode() == Gizmo::TransformMode::Bounds);
		_localSpaceBtn->setActive(Gizmo::singleton()->getTransformSpace() == Gizmo::TransformSpace::Local);
		_worldSpaceBtn->setActive(Gizmo::singleton()->getTransformSpace() == Gizmo::TransformSpace::World);
	}
} // namespace Editor