#include "ObjectWindow.h"

#include <Assets/Texture.h>
#include <Shared/Path.h>
#include <Shared/String.h>
#include <System/EventHandler.h>

#include "WindowManager.h"

#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

#include "../Modifiers/ModifierManager.h"
#include "../Modifiers/CSGModifier.h"

namespace Editor
{
	ObjectWindow::ObjectWindow(WindowManager* parent) : Window(parent, OBJECT_WINDOW)
	{
		ModifierManager* modMgr = ModifierManager::singleton();

		/* Layout */

		layoutMain = new LinearLayout(LayoutDirection::Horizontal);
		
		addControl(layoutMain);

		/* CSG tool */

		Button* csgTool = new Button();
		Core::Texture* csgToolImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/csg.png"), Core::TextureFormat::RGBA8);
		csgTool->setSize(32, 32);
		csgTool->setImage(csgToolImage);

		csgTool->setOnClick([=] {
			disableAll();
			if (setModifier(CSGModifier::NAME))
			{
				csgTool->setActive(true);
			}
		});

		layoutMain->addControl(csgTool);

		/* Light tool */

		Button* lightTool = new Button();
		Core::Texture* lightToolImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/gizmo/pointlight.png"), Core::TextureFormat::RGBA8);
		lightTool->setSize(32, 32);
		lightTool->setImage(lightToolImage);

		lightTool->setOnClick([=] {
			disableAll();
			modMgr->unsetCurrentModifier();
			lightTool->setActive(true);
			});

		layoutMain->addControl(lightTool);

		/* Camera tool */

		Button* cameraTool = new Button();
		Core::Texture* cameraToolImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/gizmo/camera.png"), Core::TextureFormat::RGBA8);
		cameraTool->setSize(32, 32);
		cameraTool->setImage(cameraToolImage);

		cameraTool->setOnClick([=] {
			disableAll();
			modMgr->unsetCurrentModifier();
			cameraTool->setActive(true);
			});

		layoutMain->addControl(cameraTool);

		EVENT({
			disableAll();
		}, =);
	}

	ObjectWindow::~ObjectWindow()
	{
	}

	void ObjectWindow::disableAll()
	{
		for (int i = 0; i < layoutMain->getControlsCount(); ++i)
		{
			Button* button = (Button*)layoutMain->getControl(i);
			button->setActive(false);
		}
	}

	bool ObjectWindow::setModifier(uint32_t name)
	{
		ModifierManager* modMgr = ModifierManager::singleton();

		if (modMgr->getCurrentModifierName() == name)
		{
			modMgr->unsetCurrentModifier();
			return false;
		}

		modMgr->setCurrentModifier(name);
		return true;
	}
}