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

#include "../../Main/EditorApp.h"
#include "../../Utils/TextureUtils.h"
#include "../../Resources/Texture.h"
#include "../../Content/ContentSerializer.h"

namespace fs = std::filesystem;

namespace Editor
{
	ToolWindow::ToolWindow(WindowManager* parent) : Window(parent, TOOL_WINDOW) {}

	ToolWindow::~ToolWindow() {}

	void ToolWindow::init()
	{
		Core::Renderer* renderer = _parent->getRenderer();

		/* Layout */

		LinearLayout* layoutMain = new LinearLayout(LayoutDirection::Horizontal);
		layoutMain->setFitWidth(LayoutFitMode::FitAvailable);
		layoutMain->setFitHeight(LayoutFitMode::FitContent);

		addControl(layoutMain);

		/* Save */

		_saveBtn = new Button();
		Texture* saveBtnImage = Texture::loadFromFile(renderer, fs::current_path() / fs::path("Editor/Icons/editor/save.png"));
		_saveBtn->setSize(32, 32);
		_saveBtn->setImage(saveBtnImage);
		_saveBtn->setOnClick([this]() {
			Core::Scene* currentScene = ((EditorApp::MainWindow*)_parent->getApplication()->getMainWindow())->getScene();
			if (currentScene != nullptr)
			{
				ContentSerializer::serializeScene(currentScene);
			}
		});

		layoutMain->addControl(_saveBtn);

		Separator* separator0 = new Separator(SeparatorDirection::Vertical);
		separator0->setHeight(30.0f);
		layoutMain->addControl(separator0);

		/* Undo */

		Button* undoBtn = new Button();
		Texture* undoBtnImage = Texture::loadFromFile(renderer, fs::current_path() / fs::path("Editor/Icons/editor/undo.png"));
		undoBtn->setSize(32, 32);
		undoBtn->setImage(undoBtnImage);

		layoutMain->addControl(undoBtn);

		/* Redo */

		Button* redoBtn = new Button();
		Texture* redoBtnImage = Texture::loadFromFile(renderer, fs::current_path() / fs::path("Editor/Icons/editor/redo.png"));
		redoBtn->setSize(32, 32);
		redoBtn->setImage(redoBtnImage);

		layoutMain->addControl(redoBtn);

		Separator* separator1 = new Separator(SeparatorDirection::Vertical);
		separator1->setHeight(30.0f);
		layoutMain->addControl(separator1);

		/* Select */

		_selectBtn = new Button();
		Texture* selectBtnImage = Texture::loadFromFile(renderer, fs::current_path() / fs::path("Editor/Icons/editor/select.png"));
		_selectBtn->setSize(32, 32);
		_selectBtn->setImage(selectBtnImage);
		_selectBtn->setOnClick([this]() {
			_gizmo->setTransformMode(Gizmo::TransformMode::Select);
			invalidate();
		});

		layoutMain->addControl(_selectBtn);

		/* Move */

		_translateBtn = new Button();
		_translateBtn->setButtonType(ButtonType::Action);
		Texture* moveBtnImage = Texture::loadFromFile(renderer, fs::current_path() / fs::path("Editor/Icons/editor/move.png"));
		_translateBtn->setSize(45, 32);
		_translateBtn->setImage(moveBtnImage);
		_translateBtn->setOnClick([this]() {
			_gizmo->setTransformMode(Gizmo::TransformMode::Translate);
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
		enableMoveSnap->setValue(_gizmo->getMoveSnap());
		enableMoveSnap->setOnValueChanged([this](bool value) { _gizmo->setMoveSnap(value); });
		Label* enableMoveSnapLabel = new Label("Enable Move Snap");
		translateContextMenuTable->addControl(enableMoveSnapLabel);
		translateContextMenuTable->addControl(enableMoveSnap);

		InputFloat* moveSnapStepInput = new InputFloat();
		moveSnapStepInput->setValue(_gizmo->getMoveStepSize());
		moveSnapStepInput->setWidth(120.0f);
		moveSnapStepInput->setStep(2.0f);
		moveSnapStepInput->setLimitMin(0.125f);
		moveSnapStepInput->setLimitMax(16.0f);
		moveSnapStepInput->setIncrementType(InputFloat::IncrementType::Multiplicative);
		moveSnapStepInput->setOnValueChanged([this](float value) { _gizmo->setMoveStepSize(value); });
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
		Texture* rotateBtnImage = Texture::loadFromFile(renderer, fs::current_path() / fs::path("Editor/Icons/editor/rotate.png"));
		_rotateBtn->setSize(45, 32);
		_rotateBtn->setImage(rotateBtnImage);
		_rotateBtn->setOnClick([this]() {
			_gizmo->setTransformMode(Gizmo::TransformMode::Rotate);
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
		enableRotateSnap->setValue(_gizmo->getRotateSnap());
		enableRotateSnap->setOnValueChanged([this](bool value) { _gizmo->setRotateSnap(value); });
		Label* enableRotateSnapLabel = new Label("Enable Rotate Snap");
		rotateContextMenuTable->addControl(enableRotateSnapLabel);
		rotateContextMenuTable->addControl(enableRotateSnap);

		InputFloat* rotateSnapStepInput = new InputFloat();
		rotateSnapStepInput->setValue(_gizmo->getRotateStepSize());
		rotateSnapStepInput->setWidth(120.0f);
		rotateSnapStepInput->setStep(15.0f);
		rotateSnapStepInput->setLimitMin(15.0f);
		rotateSnapStepInput->setLimitMax(90.0f);
		rotateSnapStepInput->setIncrementType(InputFloat::IncrementType::Additive);
		rotateSnapStepInput->setOnValueChanged([this](float value) { _gizmo->setRotateStepSize(value); });
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
		Texture* scaleBtnImage = Texture::loadFromFile(renderer, fs::current_path() / fs::path("Editor/Icons/editor/scale.png"));
		_scaleBtn->setSize(45, 32);
		_scaleBtn->setImage(scaleBtnImage);
		_scaleBtn->setOnClick([this]() {
			_gizmo->setTransformMode(Gizmo::TransformMode::Scale);
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
		enableScaleSnap->setValue(_gizmo->getScaleSnap());
		enableScaleSnap->setOnValueChanged([this](bool value) { _gizmo->setScaleSnap(value); });
		Label* enableScaleSnapLabel = new Label("Enable Scale Snap");
		scaleContextMenuTable->addControl(enableScaleSnapLabel);
		scaleContextMenuTable->addControl(enableScaleSnap);

		InputFloat* scaleSnapStepInput = new InputFloat();
		scaleSnapStepInput->setValue(_gizmo->getScaleStepSize());
		scaleSnapStepInput->setWidth(120.0f);
		scaleSnapStepInput->setStep(2.0f);
		scaleSnapStepInput->setLimitMin(0.125f);
		scaleSnapStepInput->setLimitMax(2.0f);
		scaleSnapStepInput->setIncrementType(InputFloat::IncrementType::Multiplicative);
		scaleSnapStepInput->setOnValueChanged([this](float value) { _gizmo->setScaleStepSize(value); });
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
		Texture* boundsBtnImage = Texture::loadFromFile(renderer, fs::current_path() / fs::path("Editor/Icons/editor/button.png"));
		_boundsBtn->setSize(45, 32);
		_boundsBtn->setImage(boundsBtnImage);
		_boundsBtn->setOnClick([this]() {
			_gizmo->setTransformMode(Gizmo::TransformMode::Bounds);
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
		enableBoundsSnap->setValue(_gizmo->getBoundsSnap());
		enableBoundsSnap->setOnValueChanged([this](bool value) { _gizmo->setBoundsSnap(value); });
		Label* enableBoundsSnapLabel = new Label("Enable Bounds Snap");
		boundsContextMenuTable->addControl(enableBoundsSnapLabel);
		boundsContextMenuTable->addControl(enableBoundsSnap);

		InputFloat* boundsSnapStepInput = new InputFloat();
		boundsSnapStepInput->setValue(_gizmo->getBoundsStepSize());
		boundsSnapStepInput->setWidth(120.0f);
		boundsSnapStepInput->setStep(2.0f);
		boundsSnapStepInput->setLimitMin(0.125f);
		boundsSnapStepInput->setLimitMax(16.0f);
		boundsSnapStepInput->setIncrementType(InputFloat::IncrementType::Multiplicative);
		boundsSnapStepInput->setOnValueChanged([this](float value) { _gizmo->setBoundsStepSize(value); });
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
		Texture* localSpaceBtnImage = Texture::loadFromFile(renderer, fs::current_path() / fs::path("Editor/Icons/editor/local.png"));
		_localSpaceBtn->setSize(32, 32);
		_localSpaceBtn->setImage(localSpaceBtnImage);
		_localSpaceBtn->setOnClick([this]() {
			_gizmo->setTransformSpace(Gizmo::TransformSpace::Local);
			invalidate();
		});

		layoutMain->addControl(_localSpaceBtn);

		/* World Space */

		_worldSpaceBtn = new Button();
		Texture* worldSpaceBtnImage = Texture::loadFromFile(renderer, fs::current_path() / fs::path("Editor/Icons/editor/world.png"));
		_worldSpaceBtn->setSize(32, 32);
		_worldSpaceBtn->setImage(worldSpaceBtnImage);
		_worldSpaceBtn->setOnClick([this]() {
			_gizmo->setTransformSpace(Gizmo::TransformSpace::World);
			invalidate();
		});

		layoutMain->addControl(_worldSpaceBtn);

		invalidate();
	}

	void ToolWindow::invalidate()
	{
		_saveBtn->setActive(((EditorApp::MainWindow*)_parent->getApplication()->getMainWindow())->getScene() != nullptr);
		_selectBtn->setActive(_gizmo->getTransformMode() == Gizmo::TransformMode::Select);
		_translateBtn->setActive(_gizmo->getTransformMode() == Gizmo::TransformMode::Translate);
		_rotateBtn->setActive(_gizmo->getTransformMode() == Gizmo::TransformMode::Rotate);
		_scaleBtn->setActive(_gizmo->getTransformMode() == Gizmo::TransformMode::Scale);
		_boundsBtn->setActive(_gizmo->getTransformMode() == Gizmo::TransformMode::Bounds);
		_localSpaceBtn->setActive(_gizmo->getTransformSpace() == Gizmo::TransformSpace::Local);
		_worldSpaceBtn->setActive(_gizmo->getTransformSpace() == Gizmo::TransformSpace::World);
	}
} // namespace Editor