#include "EditWindow.h"

#include <Assets/Texture.h>
#include <Shared/Path.h>
#include <Shared/String.h>

#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

namespace Editor
{
	EditWindow::EditWindow() : Window("Edit")
	{
		/* Layout */

		LinearLayout* layoutMain = new LinearLayout(LayoutDirection::Vertical);

		addControl(layoutMain);

		/* CSG add */

		Button* csgAddBtn = new Button();
		Core::Texture* csgAddBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/editor/add.png"), Core::TextureFormat::RGBA);
		csgAddBtn->setSize(32, 32);
		csgAddBtn->setImage(csgAddBtnImage);

		layoutMain->addControl(csgAddBtn);

		/* CSG edit points */

		Button* csgEditPointsBtn = new Button();
		Core::Texture* csgEditPointsBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/points.png"), Core::TextureFormat::RGBA);
		csgEditPointsBtn->setSize(32, 32);
		csgEditPointsBtn->setImage(csgEditPointsBtnImage);

		layoutMain->addControl(csgEditPointsBtn);

		/* CSG edit edges */

		Button* csgEditEdgesBtn = new Button();
		Core::Texture* csgEditEdgesBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/edges.png"), Core::TextureFormat::RGBA);
		csgEditEdgesBtn->setSize(32, 32);
		csgEditEdgesBtn->setImage(csgEditEdgesBtnImage);

		layoutMain->addControl(csgEditEdgesBtn);

		/* CSG edit faces */

		Button* csgEditFacesBtn = new Button();
		Core::Texture* csgEditFacesBtnImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/face.png"), Core::TextureFormat::RGBA);
		csgEditFacesBtn->setSize(32, 32);
		csgEditFacesBtn->setImage(csgEditFacesBtnImage);

		layoutMain->addControl(csgEditFacesBtn);
	}

	EditWindow::~EditWindow()
	{
	}
}