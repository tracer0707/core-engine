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
	const char* CSGObjectWindow::NAME = "CSGObject";

	CSGObjectWindow::CSGObjectWindow() : Window(NAME)
	{
		_modifier = (CSGModifier*)ModifierManager::singleton()->getModifier(CSGModifier::NAME);

		/* Layout */

		_layoutMain = new LinearLayout(LayoutDirection::Horizontal);
		
		addControl(_layoutMain);

		/* CSG add model */

		_csgAddModelBtn = new Button();
		Core::Texture* csgAddModelBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/editor/add.png"), Core::TextureFormat::RGBA8);
		_csgAddModelBtn->setSize(32, 32);
		_csgAddModelBtn->setImage(csgAddModelBtnImage);
		_csgAddModelBtn->setOnClick([=] {
			_modifier->addModel();
		});

		_layoutMain->addControl(_csgAddModelBtn);

		/* CSG cube */

		Button* csgCube = new Button();
		Core::Texture* csgCubeImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/cube.png"), Core::TextureFormat::RGBA8);
		csgCube->setSize(32, 32);
		csgCube->setImage(csgCubeImage);
		csgCube->setOnClick([=] {
			_modifier->addBrush(CSGModifier::BrushType::Cube);
		});

		_layoutMain->addControl(csgCube);

		/* CSG sphere */

		Button* csgSphere = new Button();
		Core::Texture* csgSphereImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/sphere.png"), Core::TextureFormat::RGBA8);
		csgSphere->setSize(32, 32);
		csgSphere->setImage(csgSphereImage);
		csgSphere->setOnClick([=] {
			_modifier->addBrush(CSGModifier::BrushType::Sphere);
		});

		_layoutMain->addControl(csgSphere);

		/* CSG cylinder */

		Button* csgCylinder = new Button();
		Core::Texture* csgCylinderImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/cylinder.png"), Core::TextureFormat::RGBA8);
		csgCylinder->setSize(32, 32);
		csgCylinder->setImage(csgCylinderImage);
		csgCylinder->setOnClick([=] {
			_modifier->addBrush(CSGModifier::BrushType::Cylinder);
		});

		_layoutMain->addControl(csgCylinder);

		/* CSG cone */

		Button* csgCone = new Button();
		Core::Texture* csgConeImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/cone.png"), Core::TextureFormat::RGBA8);
		csgCone->setSize(32, 32);
		csgCone->setImage(csgConeImage);
		csgCone->setOnClick([=] {
			_modifier->addBrush(CSGModifier::BrushType::Cone);
		});

		_layoutMain->addControl(csgCone);

		/* CSG stair */

		Button* csgStair = new Button();
		Core::Texture* csgStairImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/stairs.png"), Core::TextureFormat::RGBA8);
		csgStair->setSize(32, 32);
		csgStair->setImage(csgStairImage);
		csgStair->setOnClick([=] {
			_modifier->addBrush(CSGModifier::BrushType::Stair);
		});

		_layoutMain->addControl(csgStair);

		/* CSG polygon */

		Button* csgPolygon = new Button();
		Core::Texture* csgPolygonImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/polygon.png"), Core::TextureFormat::RGBA8);
		csgPolygon->setSize(32, 32);
		csgPolygon->setImage(csgPolygonImage);
		csgPolygon->setOnClick([=] {
			_modifier->addBrush(CSGModifier::BrushType::Polygon);
		});

		_layoutMain->addControl(csgPolygon);

		EVENT({
			invalidate();
		}, =);
	}

	CSGObjectWindow::~CSGObjectWindow()
	{
	}

	void Editor::CSGObjectWindow::invalidate()
	{
		enableAll(false);

		if (_modifier->getCurrentModel() != nullptr)
		{
			enableAll(true);
		}
	}

	void CSGObjectWindow::activateAll(bool active)
	{
		for (int i = 0; i < _layoutMain->getControlsCount(); ++i)
		{
			Button* button = (Button*)_layoutMain->getControl(i);
			button->setActive(active);
		}

		_csgAddModelBtn->setActive(true);
	}

	void Editor::CSGObjectWindow::enableAll(bool enable)
	{
		for (int i = 0; i < _layoutMain->getControlsCount(); ++i)
		{
			Button* button = (Button*)_layoutMain->getControl(i);
			button->setEnabled(enable);
		}

		_csgAddModelBtn->setEnabled(true);
	}
}