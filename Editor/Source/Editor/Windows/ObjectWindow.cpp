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

		/* CSG cube */

		Button* csgCube = new Button();
		Core::Texture2D* csgCubeImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/cube.png"), contentMgr);
		csgCube->setSize(32, 32);
		csgCube->setImage(csgCubeImage);

		_layoutMain->addControl(csgCube);

		/* CSG sphere */

		Button* csgSphere = new Button();
		Core::Texture2D* csgSphereImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/sphere.png"), contentMgr);
		csgSphere->setSize(32, 32);
		csgSphere->setImage(csgSphereImage);

		_layoutMain->addControl(csgSphere);

		/* CSG cylinder */

		Button* csgCylinder = new Button();
		Core::Texture2D* csgCylinderImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/cylinder.png"), contentMgr);
		csgCylinder->setSize(32, 32);
		csgCylinder->setImage(csgCylinderImage);

		_layoutMain->addControl(csgCylinder);

		/* CSG cone */

		Button* csgCone = new Button();
		Core::Texture2D* csgConeImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/cone.png"), contentMgr);
		csgCone->setSize(32, 32);
		csgCone->setImage(csgConeImage);

		_layoutMain->addControl(csgCone);

		/* CSG stair */

		Button* csgStair = new Button();
		Core::Texture2D* csgStairImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/stairs.png"), contentMgr);
		csgStair->setSize(32, 32);
		csgStair->setImage(csgStairImage);

		_layoutMain->addControl(csgStair);
	}

	ObjectWindow::~ObjectWindow() {}
} // namespace Editor