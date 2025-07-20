#include "GizmoWindow.h"

#include <Assets/Texture.h>
#include <Shared/Path.h>
#include <Shared/String.h>

#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

namespace Editor
{
	const char* GizmoWindow::NAME = "Gizmo";

	GizmoWindow::GizmoWindow(WindowManager* parent) : Window(parent, NAME)
	{
		/* Layout */
		
		LinearLayout* layoutMain = new LinearLayout(LayoutDirection::Horizontal);

		addControl(layoutMain);

		/* Undo */

		Button* undoBtn = new Button();
		Core::Texture* undoBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/editor/undo.png"), Core::TextureFormat::RGBA8);
		undoBtn->setSize(32, 32);
		undoBtn->setImage(undoBtnImage);

		layoutMain->addControl(undoBtn);

		/* Redo */

		Button* redoBtn = new Button();
		Core::Texture* redoBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/editor/redo.png"), Core::TextureFormat::RGBA8);
		redoBtn->setSize(32, 32);
		redoBtn->setImage(redoBtnImage);

		layoutMain->addControl(redoBtn);

		/* Select */

		Button* selectBtn = new Button();
		Core::Texture* selectBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/editor/select.png"), Core::TextureFormat::RGBA8);
		selectBtn->setSize(32, 32);
		selectBtn->setImage(selectBtnImage);

		layoutMain->addControl(selectBtn);

		/* Move */

		Button* moveBtn = new Button();
		Core::Texture* moveBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/editor/move.png"), Core::TextureFormat::RGBA8);
		moveBtn->setSize(32, 32);
		moveBtn->setImage(moveBtnImage);

		layoutMain->addControl(moveBtn);

		/* Rotate */

		Button* rotateBtn = new Button();
		Core::Texture* rotateBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/editor/rotate.png"), Core::TextureFormat::RGBA8);
		rotateBtn->setSize(32, 32);
		rotateBtn->setImage(rotateBtnImage);

		layoutMain->addControl(rotateBtn);

		/* Scale */

		Button* scaleBtn = new Button();
		Core::Texture* scaleBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/editor/scale.png"), Core::TextureFormat::RGBA8);
		scaleBtn->setSize(32, 32);
		scaleBtn->setImage(scaleBtnImage);

		layoutMain->addControl(scaleBtn);
	}

	GizmoWindow::~GizmoWindow()
	{
	}
}