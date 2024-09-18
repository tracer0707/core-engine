#include "CSGObjectWindow.h"

#include <Assets/Texture.h>
#include <Shared/Path.h>
#include <Shared/String.h>
#include <System/EventHandler.h>

#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

#include "../Modifiers/ModifierManager.h"
#include "../Modifiers/CSGModifier.h"

#include "../Modifiers/CSGModifier.h"

namespace Editor
{
	CSGObjectWindow::CSGObjectWindow(CSGModifier* modifier) : Window("CSGObject")
	{
		_modifier = modifier;

		/* Layout */

		layoutMain = new LinearLayout(LayoutDirection::Horizontal);
		
		addControl(layoutMain);

		/* CSG cube */

		Button* csgCube = new Button();
		Core::Texture* csgCubeImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/cube.png"), Core::TextureFormat::RGBA);
		csgCube->setSize(32, 32);
		csgCube->setImage(csgCubeImage);
		csgCube->setOnClick([=] {
			disableAll();
			_modifier->setBrushType(CSGModifier::BrushType::Cube);
			csgCube->setActive(true);
		});

		layoutMain->addControl(csgCube);

		/* CSG sphere */

		Button* csgSphere = new Button();
		Core::Texture* csgSphereImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/sphere.png"), Core::TextureFormat::RGBA);
		csgSphere->setSize(32, 32);
		csgSphere->setImage(csgSphereImage);
		csgSphere->setOnClick([=] {
			disableAll();
			_modifier->setBrushType(CSGModifier::BrushType::Sphere);
			csgSphere->setActive(true);
		});

		layoutMain->addControl(csgSphere);

		/* CSG cylinder */

		Button* csgCylinder = new Button();
		Core::Texture* csgCylinderImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/cylinder.png"), Core::TextureFormat::RGBA);
		csgCylinder->setSize(32, 32);
		csgCylinder->setImage(csgCylinderImage);
		csgCylinder->setOnClick([=] {
			disableAll();
			_modifier->setBrushType(CSGModifier::BrushType::Cylinder);
			csgCylinder->setActive(true);
		});

		layoutMain->addControl(csgCylinder);

		/* CSG cone */

		Button* csgCone = new Button();
		Core::Texture* csgConeImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/cone.png"), Core::TextureFormat::RGBA);
		csgCone->setSize(32, 32);
		csgCone->setImage(csgConeImage);
		csgCone->setOnClick([=] {
			disableAll();
			_modifier->setBrushType(CSGModifier::BrushType::Cone);
			csgCone->setActive(true);
		});

		layoutMain->addControl(csgCone);

		/* CSG stair */

		Button* csgStair = new Button();
		Core::Texture* csgStairImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/stairs.png"), Core::TextureFormat::RGBA);
		csgStair->setSize(32, 32);
		csgStair->setImage(csgStairImage);
		csgStair->setOnClick([=] {
			disableAll();
			_modifier->setBrushType(CSGModifier::BrushType::Stair);
			csgStair->setActive(true);
		});

		layoutMain->addControl(csgStair);

		/* CSG polygon */

		Button* csgPolygon = new Button();
		Core::Texture* csgPolygonImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/polygon.png"), Core::TextureFormat::RGBA);
		csgPolygon->setSize(32, 32);
		csgPolygon->setImage(csgPolygonImage);
		csgPolygon->setOnClick([=] {
			disableAll();
			_modifier->setBrushType(CSGModifier::BrushType::Polygon);
			csgPolygon->setActive(true);
		});

		layoutMain->addControl(csgPolygon);

		EVENT({
			disableAll();
			csgCube->setActive(true);
		}, =);
	}

	CSGObjectWindow::~CSGObjectWindow()
	{
	}

	void CSGObjectWindow::disableAll()
	{
		for (int i = 0; i < layoutMain->getControlsCount(); ++i)
		{
			Button* button = (Button*)layoutMain->getControl(i);
			button->setActive(false);
		}
	}
}