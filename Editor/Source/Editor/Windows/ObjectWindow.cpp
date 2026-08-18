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

		/* Light tool */

		Button* lightTool = new Button();
		Core::Texture2D* lightToolImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/gizmo/pointlight.png"), contentMgr);
		lightTool->setSize(32, 32);
		lightTool->setImage(lightToolImage);

		_layoutMain->addControl(lightTool);

		/* Camera tool */

		Button* cameraTool = new Button();
		Core::Texture2D* cameraToolImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/gizmo/camera.png"), contentMgr);
		cameraTool->setSize(32, 32);
		cameraTool->setImage(cameraToolImage);

		_layoutMain->addControl(cameraTool);
	}

	ObjectWindow::~ObjectWindow() {}
} // namespace Editor