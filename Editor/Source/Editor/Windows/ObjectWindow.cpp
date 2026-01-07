#include "ObjectWindow.h"

#include <filesystem>

#include <Core/Content/ContentManager.h>
#include <Core/Content/Texture.h>
#include <Core/Shared/Path.h>
#include <Core/Shared/String.h>
#include <Core/System/EventHandler.h>

#include "WindowManager.h"

#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

#include "../Modifiers/ModifierManager.h"
#include "../Modifiers/CSGModifier.h"

#include "../../Utils/TextureUtils.h"

namespace Editor
{
	ObjectWindow::ObjectWindow(WindowManager* parent) : Window(parent, OBJECT_WINDOW)
	{
		ModifierManager* modMgr = ModifierManager::singleton();
		Core::ContentManager* contentMgr = parent->getContentManager();

		/* Layout */

		layoutMain = new LinearLayout(LayoutDirection::Horizontal);

		addControl(layoutMain);

		/* CSG tool */

		Button* csgTool = new Button();
		Core::Texture* csgToolImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/csg.png"), contentMgr);
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
		Core::Texture* lightToolImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/gizmo/pointlight.png"), contentMgr);
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
		Core::Texture* cameraToolImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/gizmo/camera.png"), contentMgr);
		cameraTool->setSize(32, 32);
		cameraTool->setImage(cameraToolImage);

		cameraTool->setOnClick([=] {
			disableAll();
			modMgr->unsetCurrentModifier();
			cameraTool->setActive(true);
		});

		layoutMain->addControl(cameraTool);

		_parent->getEventHandler()->addEvent([this] { disableAll(); });
	}

	ObjectWindow::~ObjectWindow() {}

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
} // namespace Editor