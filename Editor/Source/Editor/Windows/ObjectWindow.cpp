#include "ObjectWindow.h"

#include <filesystem>

#include <Core/Content/ContentManager.h>
#include <Core/Content/Texture2D.h>
#include <Core/Shared/Path.h>
#include <Core/Shared/String.h>
#include <Core/System/EventHandler.h>

#include "WindowManager.h"

#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

#include "../../Utils/TextureUtils.h"

namespace Editor
{
	ObjectWindow::ObjectWindow(WindowManager* parent) : Window(parent, OBJECT_WINDOW)
	{
		Core::ContentManager* contentMgr = parent->getContentManager();

		/* Layout */

		_layoutMain = new LinearLayout(LayoutDirection::Horizontal);
		_layoutMain->setFitWidth(LayoutFitMode::FitAvailable);
		_layoutMain->setFitHeight(LayoutFitMode::FitContent);

		addControl(_layoutMain);

		/* Empty object */

		Button* emptyObject = new Button();
		Core::Texture2D* emptyObjectImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/empty.png"), contentMgr);
		emptyObject->setSize(32, 32);
		emptyObject->setImage(emptyObjectImage);
		emptyObject->setDragDropSource(true, "SCENE_OBJECT");
		emptyObject->setDragDropSourceLabel(Core::String("Empty Object"));
		emptyObject->setDragDropSourceData(DragDropData({Core::String("EmptyObject"), nullptr}));

		_layoutMain->addControl(emptyObject);

		/* Light object */

		Button* lightObject = new Button();
		Core::Texture2D* lightObjectImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/gizmo/pointlight.png"), contentMgr);
		lightObject->setSize(32, 32);
		lightObject->setImage(lightObjectImage);

		_layoutMain->addControl(lightObject);

		/* Camera object */

		Button* cameraObject = new Button();
		Core::Texture2D* cameraObjectImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/gizmo/camera.png"), contentMgr);
		cameraObject->setSize(32, 32);
		cameraObject->setImage(cameraObjectImage);

		_layoutMain->addControl(cameraObject);
	}

	ObjectWindow::~ObjectWindow() {}
} // namespace Editor