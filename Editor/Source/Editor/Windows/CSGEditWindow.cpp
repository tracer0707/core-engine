#include "CSGEditWindow.h"

#include <Assets/Texture.h>
#include <Shared/Path.h>
#include <Shared/String.h>
#include <System/EventHandler.h>

#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

#include "../Modifiers/ModifierManager.h"
#include "../Modifiers/CSGModifier.h"

namespace Editor
{
	UString CSGEditWindow::NAME = "Edit";

	CSGEditWindow::CSGEditWindow() : Window(NAME)
	{
		_modifier = (CSGModifier*)ModifierManager::singleton()->getModifier(CSGModifier::NAME);

		/* Layout */

		_layoutMain = new LinearLayout(LayoutDirection::Vertical);

		addControl(_layoutMain);

		/* CSG add */

		_csgAddBtn = new Button();
		Core::Texture* csgAddBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/editor/add.png"), Core::TextureFormat::RGBA);
		_csgAddBtn->setSize(32, 32);
		_csgAddBtn->setImage(csgAddBtnImage);
		_csgAddBtn->setOnClick([=] {
			activateAll(false);
			_modifier->setEditMode(CSGModifier::EditMode::AddBrush);
			_csgAddBtn->setActive(true);
		});

		_layoutMain->addControl(_csgAddBtn);

		/* CSG edit points */

		Button* csgEditPointsBtn = new Button();
		Core::Texture* csgEditPointsBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/points.png"), Core::TextureFormat::RGBA);
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
		Core::Texture* csgEditEdgesBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/edges.png"), Core::TextureFormat::RGBA);
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
		Core::Texture* csgEditFacesBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/face.png"), Core::TextureFormat::RGBA);
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
			checkControls();
			_csgAddBtn->setActive(true);
		}, =);
	}

	CSGEditWindow::~CSGEditWindow()
	{
	}

	void Editor::CSGEditWindow::checkControls()
	{
		enableAll(false);

		if (_modifier->getCurrentCsgModel() != nullptr)
		{
			_csgAddBtn->setEnabled(true);

			if (_modifier->getCurrentCsgBrush() != nullptr)
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