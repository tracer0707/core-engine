#include "CSGEditWindow.h"

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
	CSGEditWindow::CSGEditWindow(WindowManager* parent) : Window(parent, CSG_EDIT_WINDOW)
	{
		ModifierManager* modMgr = ModifierManager::singleton();

		_modifier = (CSGModifier*)modMgr->getModifier(CSGModifier::NAME);

		/* Layout */

		_layoutMain = new LinearLayout(LayoutDirection::Vertical);

		addControl(_layoutMain);

		/* CSG select */

		_csgSelectBtn = new Button();
		Core::Texture* csgAddBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/cube.png"), Core::TextureFormat::RGBA8);
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
		Core::Texture* csgEditPointsBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/points.png"), Core::TextureFormat::RGBA8);
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
		Core::Texture* csgEditEdgesBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/edges.png"), Core::TextureFormat::RGBA8);
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
		Core::Texture* csgEditFacesBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/face.png"), Core::TextureFormat::RGBA8);
		csgEditFacesBtn->setSize(32, 32);
		csgEditFacesBtn->setImage(csgEditFacesBtnImage);
		csgEditFacesBtn->setOnClick([=] {
			activateAll(false);
			_modifier->setEditMode(CSGModifier::EditMode::EditFaces);
			csgEditFacesBtn->setActive(true);
		});

		_layoutMain->addControl(csgEditFacesBtn);

		EVENT({
			activateAll(false);
			invalidate();
			_csgSelectBtn->setActive(true);
		}, =);
	}

	CSGEditWindow::~CSGEditWindow()
	{
	}

	void Editor::CSGEditWindow::invalidate()
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

	void Editor::CSGEditWindow::enableAll(bool enable)
	{
		for (int i = 0; i < _layoutMain->getControlsCount(); ++i)
		{
			Button* button = (Button*)_layoutMain->getControl(i);
			button->setEnabled(enable);
		}
	}
}