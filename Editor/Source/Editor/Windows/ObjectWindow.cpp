#include "ObjectWindow.h"

#include <Assets/Texture.h>
#include <Shared/Path.h>
#include <Shared/String.h>

#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

#include "../Modifiers/ModifierManager.h"
#include "../Modifiers/CSGModifier.h"

namespace Editor
{
	ObjectWindow::ObjectWindow() : Window("Tools")
	{
		/* Layout */

		LinearLayout* layout1 = new LinearLayout(LayoutDirection::Horizontal);
		LinearLayout* layout2 = new LinearLayout(LayoutDirection::Horizontal);
		LinearLayout* layoutMain = new LinearLayout(LayoutDirection::Vertical);

		layout1->setAlignmentHorizontal(LayoutAlignment::Center);
		layout2->setAlignmentHorizontal(LayoutAlignment::Center);

		layoutMain->addControl(layout1);
		layoutMain->addControl(layout2);

		addControl(layoutMain);

		/* CSG tool */

		Button* csgTool = new Button();
		Core::Texture* csgToolImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/csg.png"), Core::TextureFormat::RGBA);
		csgTool->setSize(32, 32);
		csgTool->setImage(csgToolImage);

		csgTool->setOnClick([=] {
			ModifierManager::singleton()->setCurrentModifier(CSGModifier::NAME);
		});

		layout1->addControl(csgTool);

		/* Light tool */

		Button* lightTool = new Button();
		Core::Texture* lightToolImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/gizmo/pointlight.png"), Core::TextureFormat::RGBA);
		lightTool->setSize(32, 32);
		lightTool->setImage(lightToolImage);

		csgTool->setOnClick([=] {
			
		});

		layout1->addControl(lightTool);

		/* Camera tool */

		Button* cameraTool = new Button();
		Core::Texture* cameraToolImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/gizmo/camera.png"), Core::TextureFormat::RGBA);
		cameraTool->setSize(32, 32);
		cameraTool->setImage(cameraToolImage);

		csgTool->setOnClick([=] {
			
		});

		layout1->addControl(cameraTool);

		/* CSG cube */

		Button* csgCube = new Button();
		Core::Texture* csgCubeImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/cube.png"), Core::TextureFormat::RGBA);
		csgCube->setSize(32, 32);
		csgCube->setImage(csgCubeImage);

		/* CSG sphere */

		Button* csgSphere = new Button();
		Core::Texture* csgSphereImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/sphere.png"), Core::TextureFormat::RGBA);
		csgSphere->setSize(32, 32);
		csgSphere->setImage(csgSphereImage);

		/* CSG cylinder */

		Button* csgCylinder = new Button();
		Core::Texture* csgCylinderImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/cylinder.png"), Core::TextureFormat::RGBA);
		csgCylinder->setSize(32, 32);
		csgCylinder->setImage(csgCylinderImage);

		/* CSG cone */

		Button* csgCone = new Button();
		Core::Texture* csgConeImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/cone.png"), Core::TextureFormat::RGBA);
		csgCone->setSize(32, 32);
		csgCone->setImage(csgConeImage);

		/* CSG stair */

		Button* csgStair = new Button();
		Core::Texture* csgStairImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/stairs.png"), Core::TextureFormat::RGBA);
		csgStair->setSize(32, 32);
		csgStair->setImage(csgStairImage);

		/* CSG polygon */

		Button* csgPolygon = new Button();
		Core::Texture* csgPolygonImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/polygon.png"), Core::TextureFormat::RGBA);
		csgPolygon->setSize(32, 32);
		csgPolygon->setImage(csgPolygonImage);

		layout2->addControl(csgCube);
		layout2->addControl(csgSphere);
		layout2->addControl(csgCylinder);
		layout2->addControl(csgCone);
		layout2->addControl(csgStair);
		layout2->addControl(csgPolygon);
	}

	ObjectWindow::~ObjectWindow()
	{
	}
}