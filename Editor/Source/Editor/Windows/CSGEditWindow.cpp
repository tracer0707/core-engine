#include "CSGEditWindow.h"

#include <Assets/Texture.h>
#include <Shared/Path.h>
#include <Shared/String.h>
#include <System/EventHandler.h>

#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

#include "../Modifiers/CSGModifier.h"

namespace Editor
{
	CSGEditWindow::CSGEditWindow(CSGModifier* modifier) : Window("Edit")
	{
		_modifier = modifier;

		/* Layout */

		layoutMain = new LinearLayout(LayoutDirection::Vertical);

		addControl(layoutMain);

		/* CSG add */

		Button* csgAddBtn = new Button();
		Core::Texture* csgAddBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/editor/add.png"), Core::TextureFormat::RGBA);
		csgAddBtn->setSize(32, 32);
		csgAddBtn->setImage(csgAddBtnImage);
		csgAddBtn->setOnClick([=] {
			disableAll();
			_modifier->setEditMode(CSGModifier::EditMode::AddBrush);
			csgAddBtn->setActive(true);
		});

		layoutMain->addControl(csgAddBtn);

		/* CSG edit points */

		Button* csgEditPointsBtn = new Button();
		Core::Texture* csgEditPointsBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/points.png"), Core::TextureFormat::RGBA);
		csgEditPointsBtn->setSize(32, 32);
		csgEditPointsBtn->setImage(csgEditPointsBtnImage);
		csgEditPointsBtn->setOnClick([=] {
			disableAll();
			_modifier->setEditMode(CSGModifier::EditMode::EditVertices);
			csgEditPointsBtn->setActive(true);
		});

		layoutMain->addControl(csgEditPointsBtn);

		/* CSG edit edges */

		Button* csgEditEdgesBtn = new Button();
		Core::Texture* csgEditEdgesBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/edges.png"), Core::TextureFormat::RGBA);
		csgEditEdgesBtn->setSize(32, 32);
		csgEditEdgesBtn->setImage(csgEditEdgesBtnImage);
		csgEditEdgesBtn->setOnClick([=] {
			disableAll();
			_modifier->setEditMode(CSGModifier::EditMode::EditEdges);
			csgEditEdgesBtn->setActive(true);
		});

		layoutMain->addControl(csgEditEdgesBtn);

		/* CSG edit faces */

		Button* csgEditFacesBtn = new Button();
		Core::Texture* csgEditFacesBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/face.png"), Core::TextureFormat::RGBA);
		csgEditFacesBtn->setSize(32, 32);
		csgEditFacesBtn->setImage(csgEditFacesBtnImage);
		csgEditFacesBtn->setOnClick([=] {
			disableAll();
			_modifier->setEditMode(CSGModifier::EditMode::EditFaces);
			csgEditFacesBtn->setActive(true);
		});

		layoutMain->addControl(csgEditFacesBtn);

		EVENT({
			disableAll();
			csgAddBtn->setActive(true);
		}, =);
	}

	CSGEditWindow::~CSGEditWindow()
	{
	}

	void CSGEditWindow::disableAll()
	{
		for (int i = 0; i < layoutMain->getControlsCount(); ++i)
		{
			Button* button = (Button*)layoutMain->getControl(i);
			button->setActive(false);
		}
	}
}