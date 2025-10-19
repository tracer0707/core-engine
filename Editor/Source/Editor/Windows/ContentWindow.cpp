#include "ContentWindow.h"

#include <imgui.h>

#include "WindowList.h"

#include "../Utils/FileSystemUtils.h"
#include "../Controls/LinearLayout.h"
#include "../Controls/Button.h"
#include "../Controls/TreeView.h"
#include "../Controls/TreeNode.h"

namespace Editor
{
	ContentWindow::ContentWindow(WindowManager* parent) : Window(parent, ASSETS_WINDOW)
	{
		LinearLayout* _mainLayout = new LinearLayout(LayoutDirection::Horizontal);
		_treeView = new TreeView();
	}

	void ContentWindow::init()
	{
		rescanContent();
	}

	void ContentWindow::rescanContent()
	{
		_treeView->clear();
		FileSystemUtils::fsToTreeView(_contentDir, _treeView, nullptr, false);
	}
}