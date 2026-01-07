#include "CSGObjectWindow.h"

#include <filesystem>

#include <Core/Content/ContentManager.h>
#include <Core/Content/Texture.h>
#include <Core/Shared/Path.h>
#include <Core/Shared/String.h>
#include <Core/System/EventHandler.h>

#include "WindowManager.h"

#include "../../Utils/TextureUtils.h"
#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

#include "../Modifiers/ModifierManager.h"
#include "../Modifiers/CSGModifier.h"

namespace Editor
{
	CSGObjectWindow::CSGObjectWindow(WindowManager* parent) : Window(parent, CSG_OBJECT_WINDOW)
	{
		ModifierManager* modMgr = ModifierManager::singleton();
		Core::ContentManager* contentMgr = parent->getContentManager();

		_modifier = (CSGModifier*)modMgr->getModifier(CSGModifier::NAME);

		/* Layout */

		_layoutMain = new LinearLayout(LayoutDirection::Horizontal);

		addControl(_layoutMain);

		/* CSG add model */

		_csgAddModelBtn = new Button();
		Core::Texture* csgAddModelBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/editor/add.png"), contentMgr);
		_csgAddModelBtn->setSize(32, 32);
		_csgAddModelBtn->setImage(csgAddModelBtnImage);
		_csgAddModelBtn->setOnClick([=] { _modifier->addModel(); });

		_layoutMain->addControl(_csgAddModelBtn);

		/* CSG cube */

		Button* csgCube = new Button();
		Core::Texture* csgCubeImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/cube.png"), contentMgr);
		csgCube->setSize(32, 32);
		csgCube->setImage(csgCubeImage);
		csgCube->setOnClick([=] { _modifier->addBrush(CSGModifier::BrushType::Cube); });

		_layoutMain->addControl(csgCube);

		/* CSG sphere */

		Button* csgSphere = new Button();
		Core::Texture* csgSphereImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/sphere.png"), contentMgr);
		csgSphere->setSize(32, 32);
		csgSphere->setImage(csgSphereImage);
		csgSphere->setOnClick([=] { _modifier->addBrush(CSGModifier::BrushType::Sphere); });

		_layoutMain->addControl(csgSphere);

		/* CSG cylinder */

		Button* csgCylinder = new Button();
		Core::Texture* csgCylinderImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/cylinder.png"), contentMgr);
		csgCylinder->setSize(32, 32);
		csgCylinder->setImage(csgCylinderImage);
		csgCylinder->setOnClick([=] { _modifier->addBrush(CSGModifier::BrushType::Cylinder); });

		_layoutMain->addControl(csgCylinder);

		/* CSG cone */

		Button* csgCone = new Button();
		Core::Texture* csgConeImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/cone.png"), contentMgr);
		csgCone->setSize(32, 32);
		csgCone->setImage(csgConeImage);
		csgCone->setOnClick([=] { _modifier->addBrush(CSGModifier::BrushType::Cone); });

		_layoutMain->addControl(csgCone);

		/* CSG stair */

		Button* csgStair = new Button();
		Core::Texture* csgStairImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/stairs.png"), contentMgr);
		csgStair->setSize(32, 32);
		csgStair->setImage(csgStairImage);
		csgStair->setOnClick([=] { _modifier->addBrush(CSGModifier::BrushType::Stair); });

		_layoutMain->addControl(csgStair);

		/* CSG polygon */

		Button* csgPolygon = new Button();
		Core::Texture* csgPolygonImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/polygon.png"), contentMgr);
		csgPolygon->setSize(32, 32);
		csgPolygon->setImage(csgPolygonImage);
		csgPolygon->setOnClick([=] { _modifier->addBrush(CSGModifier::BrushType::Polygon); });

		_layoutMain->addControl(csgPolygon);

		_parent->getEventHandler()->addEvent([this] { invalidate(); });
	}

	CSGObjectWindow::~CSGObjectWindow() {}

	void CSGObjectWindow::invalidate()
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

	void CSGObjectWindow::enableAll(bool enable)
	{
		for (int i = 0; i < _layoutMain->getControlsCount(); ++i)
		{
			Button* button = (Button*)_layoutMain->getControl(i);
			button->setEnabled(enable);
		}

		_csgAddModelBtn->setEnabled(true);
	}
} // namespace Editor