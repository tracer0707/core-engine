#pragma once

#include "Window.h"
#include <Core/Shared/String.h>

namespace Editor
{
    class TreeView;
    class LinearLayout;

    class ContentWindow : public Window
    {
      private:
        Core::String _contentDir = Core::String::Empty;

        TreeView* _treeView = nullptr;
        LinearLayout* _rightPane = nullptr;

        void rescanContent();
        void setCurrentDir(Core::String path);

      public:
        ContentWindow(WindowManager* parent);
        virtual ~ContentWindow() = default;

        Core::String getContentDir() { return _contentDir; }
        void setContentDir(Core::String value) { _contentDir = value; }

        virtual void init();
    };
} // namespace Editor