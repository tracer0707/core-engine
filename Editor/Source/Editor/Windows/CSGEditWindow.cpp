#include "CSGEditWindow.h"

#include <filesystem>

#include <Core/Content/Texture2D.h>
#include <Core/Shared/Path.h>
#include <Core/Shared/String.h>
#include <Core/System/EventHandler.h>
#include <Core/Content/ContentManager.h>

#include "WindowManager.h"

#include "../Gizmo.h"
#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

#include "../../CSG/CSGBuilder.h"
#include "../../Utils/TextureUtils.h"

namespace Editor
{
	CSGEditWindow::CSGEditWindow(WindowManager* parent) : Window(parent, CSG_EDIT_WINDOW)
	{
		Core::ContentManager* contentMgr = parent->getContentManager();

		/* Layout */

		_layoutMain = new LinearLayout(LayoutDirection::Vertical);

		addControl(_layoutMain);

		/* CSG select */

		_csgSelectBtn = new Button();
		Core::Texture2D* csgAddBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/cube.png"), contentMgr);
		_csgSelectBtn->setSize(32, 32);
		_csgSelectBtn->setImage(csgAddBtnImage);
		_csgSelectBtn->setOnClick([this] {
			CSGBuilder::singleton()->setEditMode(CSGBuilder::EditMode::Select);
			invalidate();
		});

		_layoutMain->addControl(_csgSelectBtn);

		/* CSG edit vertices */

		_csgEditVerticesBtn = new Button();
		Core::Texture2D* csgEditVerticesBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/points.png"), contentMgr);
		_csgEditVerticesBtn->setSize(32, 32);
		_csgEditVerticesBtn->setImage(csgEditVerticesBtnImage);
		_csgEditVerticesBtn->setOnClick([this] {
			CSGBuilder::singleton()->setEditMode(CSGBuilder::EditMode::EditVertices);
			invalidate();
		});

		_layoutMain->addControl(_csgEditVerticesBtn);

		/* CSG edit edges */

		_csgEditEdgesBtn = new Button();
		Core::Texture2D* csgEditEdgesBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/edges.png"), contentMgr);
		_csgEditEdgesBtn->setSize(32, 32);
		_csgEditEdgesBtn->setImage(csgEditEdgesBtnImage);
		_csgEditEdgesBtn->setOnClick([this] {
			CSGBuilder::singleton()->setEditMode(CSGBuilder::EditMode::EditEdges);
			invalidate();
		});

		_layoutMain->addControl(_csgEditEdgesBtn);

		/* CSG edit faces */

		_csgEditFacesBtn = new Button();
		Core::Texture2D* csgEditFacesBtnImage = TextureUtils::loadCompressed(
			Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/csg/face.png"), contentMgr);
		_csgEditFacesBtn->setSize(32, 32);
		_csgEditFacesBtn->setImage(csgEditFacesBtnImage);
		_csgEditFacesBtn->setOnClick([this] {
			CSGBuilder::singleton()->setEditMode(CSGBuilder::EditMode::EditFaces);
			invalidate();
		});

		_layoutMain->addControl(_csgEditFacesBtn);

		invalidate();
	}

	CSGEditWindow::~CSGEditWindow() {}

	void CSGEditWindow::invalidate()
	{
		if (Gizmo::singleton()->getObjectType() == Gizmo::ObjectType::CSGBrush)
		{
			setVisible(true);
		}
		else
		{
			setVisible(false);
		}

		_csgSelectBtn->setActive(CSGBuilder::singleton()->getEditMode() == CSGBuilder::EditMode::Select);
		_csgEditVerticesBtn->setActive(CSGBuilder::singleton()->getEditMode() == CSGBuilder::EditMode::EditVertices);
		_csgEditEdgesBtn->setActive(CSGBuilder::singleton()->getEditMode() == CSGBuilder::EditMode::EditEdges);
		_csgEditFacesBtn->setActive(CSGBuilder::singleton()->getEditMode() == CSGBuilder::EditMode::EditFaces);
	}
} // namespace Editor