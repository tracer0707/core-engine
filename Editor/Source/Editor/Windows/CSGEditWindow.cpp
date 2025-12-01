#include "CSGEditWindow.h"

#include <filesystem>

#include <Core/Content/Texture.h>
#include <Core/Shared/Path.h>
#include <Core/Shared/String.h>
#include <Core/System/EventHandler.h>

#include "WindowManager.h"

#include "../../System/ContentLoader.h"
#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

#include "../Modifiers/ModifierManager.h"
#include "../Modifiers/CSGModifier.h"

namespace Editor
{
	CSGEditWindow::CSGEditWindow(WindowManager* parent) : Window(parent, CSG_EDIT_WINDOW)
	{
		ModifierManager* modMgr = ModifierManager::singleton();
        ContentLoader* contentLoader = parent->getContentLoader();

		_modifier = (CSGModifier*)modMgr->getModifier(CSGModifier::NAME);

		/* Layout */

		_layoutMain = new LinearLayout(LayoutDirection::Vertical);

		addControl(_layoutMain);

		/* CSG select */

		_csgSelectBtn = new Button();
        Core::Texture* csgAddBtnImage = contentLoader->loadTextureFromFile(
            Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/cube.png"), Core::TextureFormat::RGBA8, LoadMethod::Direct);
		_csgSelectBtn->setSize(32, 32);
		_csgSelectBtn->setImage(csgAddBtnImage);
		_csgSelectBtn->setOnClick([=] {
			activateAll(false);
			_modifier->setEditMode(CSGModifier::EditMode::Select);
			_csgSelectBtn->setActive(true);
		});

		_layoutMain->addControl(_csgSelectBtn);

		/* CSG edit points */

		Button* csgEditPointsBtn = new Button();
        Core::Texture* csgEditPointsBtnImage = contentLoader->loadTextureFromFile(
            Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/points.png"), Core::TextureFormat::RGBA8,
            LoadMethod::Direct);
		csgEditPointsBtn->setSize(32, 32);
		csgEditPointsBtn->setImage(csgEditPointsBtnImage);
		csgEditPointsBtn->setOnClick([=] {
			activateAll(false);
			_modifier->setEditMode(CSGModifier::EditMode::EditVertices);
			csgEditPointsBtn->setActive(true);
		});

		_layoutMain->addControl(csgEditPointsBtn);

		/* CSG edit edges */

		Button* csgEditEdgesBtn = new Button();
        Core::Texture* csgEditEdgesBtnImage = contentLoader->loadTextureFromFile(
            Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/edges.png"), Core::TextureFormat::RGBA8,
            LoadMethod::Direct);
		csgEditEdgesBtn->setSize(32, 32);
		csgEditEdgesBtn->setImage(csgEditEdgesBtnImage);
		csgEditEdgesBtn->setOnClick([=] {
			activateAll(false);
			_modifier->setEditMode(CSGModifier::EditMode::EditEdges);
			csgEditEdgesBtn->setActive(true);
		});

		_layoutMain->addControl(csgEditEdgesBtn);

		/* CSG edit faces */

		Button* csgEditFacesBtn = new Button();
        Core::Texture* csgEditFacesBtnImage = contentLoader->loadTextureFromFile(
            Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/face.png"), Core::TextureFormat::RGBA8,
            LoadMethod::Direct);
		csgEditFacesBtn->setSize(32, 32);
		csgEditFacesBtn->setImage(csgEditFacesBtnImage);
		csgEditFacesBtn->setOnClick([=] {
			activateAll(false);
			_modifier->setEditMode(CSGModifier::EditMode::EditFaces);
			csgEditFacesBtn->setActive(true);
		});

		_layoutMain->addControl(csgEditFacesBtn);

		_parent->getEventHandler()->addEvent([this] {
			activateAll(false);
			invalidate();
			_csgSelectBtn->setActive(true);
		});
	}

	CSGEditWindow::~CSGEditWindow()
	{
	}

	void CSGEditWindow::invalidate()
	{
		enableAll(false);

		if (_modifier->getCurrentModel() != nullptr)
		{
			_csgSelectBtn->setEnabled(true);

			if (_modifier->getCurrentBrush() != nullptr)
			{
				enableAll(true);
			}
		}
	}

	void CSGEditWindow::activateAll(bool active)
	{
		for (int i = 0; i < _layoutMain->getControlsCount(); ++i)
		{
			Button* button = (Button*)_layoutMain->getControl(i);
			button->setActive(active);
		}
	}

	void CSGEditWindow::enableAll(bool enable)
	{
		for (int i = 0; i < _layoutMain->getControlsCount(); ++i)
		{
			Button* button = (Button*)_layoutMain->getControl(i);
			button->setEnabled(enable);
		}
	}
}