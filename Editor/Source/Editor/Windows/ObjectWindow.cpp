#include "ObjectWindow.h"

#include <filesystem>

#include <Core/Shared/Path.h>
#include <Core/Shared/String.h>
#include <Core/System/EventHandler.h>

#include "WindowManager.h"

#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

#include "../../Utils/TextureUtils.h"
#include "../../Resources/Texture.h"
#include "../../Resources/TextureManager.h"

namespace fs = std::filesystem;

namespace Editor
{
	ObjectWindow::ObjectWindow(WindowManager* parent) : Window(parent, OBJECT_WINDOW)
	{
		
	}

	ObjectWindow::~ObjectWindow() {}

	void ObjectWindow::init()
	{
		Core::Renderer* renderer = _parent->getRenderer();

		/* Layout */

		_layoutMain = new LinearLayout(LayoutDirection::Horizontal);
		_layoutMain->setFitWidth(LayoutFitMode::FitAvailable);
		_layoutMain->setFitHeight(LayoutFitMode::FitContent);

		addControl(_layoutMain);

		/* Empty object */

		Button* emptyObject = new Button();
		Texture* emptyObjectImage = _textureManager->getIcon(EditorIcon::Object);
		emptyObject->setSize(32, 32);
		emptyObject->setImage(emptyObjectImage);
		emptyObject->setDragDropSource(true, "SCENE_OBJECT");
		emptyObject->setDragDropSourceLabel(Core::String("Empty Object"));
		emptyObject->setDragDropSourceData(DragDropData(Core::String("EmptyObject")));

		_layoutMain->addControl(emptyObject);

		/* Light object */

		Button* lightObject = new Button();
		Texture* lightObjectImage = _textureManager->getIcon(EditorIcon::PointLight);
		lightObject->setSize(32, 32);
		lightObject->setImage(lightObjectImage);

		_layoutMain->addControl(lightObject);

		/* Camera object */

		Button* cameraObject = new Button();
		Texture* cameraObjectImage = _textureManager->getIcon(EditorIcon::Camera);
		cameraObject->setSize(32, 32);
		cameraObject->setImage(cameraObjectImage);

		_layoutMain->addControl(cameraObject);
	}
} // namespace Editor