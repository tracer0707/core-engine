#include "CSGWindow.h"

#include "../Controls/TreeView.h"

namespace Editor
{
	CSGWindow::CSGWindow(): Window("CSG")
	{
		tvBrushes = new TreeView();

		addControl(tvBrushes);
	}

	CSGWindow::~CSGWindow()
	{
		tvBrushes = nullptr;
	}
}