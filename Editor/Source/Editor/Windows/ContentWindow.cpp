#include "ContentWindow.h"

#include <imgui.h>

#include "WindowList.h"

#include "../Utils/FileSystemUtils.h"
#include "../Controls/LinearLayout.h"
#include "../Controls/SplitPanel.h"
#include "../Controls/Button.h"
#include "../Controls/TreeView.h"
#include "../Controls/TreeNode.h"

namespace Editor
{
    ContentWindow::ContentWindow(WindowManager* parent) : Window(parent, CONTENT_WINDOW)
    {
        LinearLayout* _mainLayout = new LinearLayout(LayoutDirection::Horizontal);
        LinearLayout* _leftPane = new LinearLayout(LayoutDirection::Vertical);
        LinearLayout* _rightPane = new LinearLayout(LayoutDirection::Horizontal);

        SplitPanel* _splitPanel = new SplitPanel(SplitPanelDirection::Horizontal);
        _splitPanel->setStartSize(200);
        _treeView = new TreeView();

        Button* testButton = new Button("test");

        _leftPane->setStretchY(true);
        _rightPane->setStretchY(true);
        _mainLayout->setStretchX(true);
        _mainLayout->setStretchY(true);

        _leftPane->addControl(_treeView);
        _rightPane->addControl(testButton);

        _splitPanel->addControl(_leftPane);
        _splitPanel->addControl(_rightPane);

        _mainLayout->addControl(_splitPanel);

        addControl(_mainLayout);
    }

    void ContentWindow::init()
    {
        rescanContent();
    }

    void ContentWindow::rescanContent()
    {
        _treeView->clear();
        FileSystemUtils::fsToTreeView(_contentDir, _treeView, nullptr, false, false);
    }
} // namespace Editor