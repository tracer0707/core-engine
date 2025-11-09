#pragma once

#include <string>
#include <filesystem>

#include <Core/Shared/String.h>
#include <Core/Shared/List.h>

namespace Editor
{
    class TreeView;
    class TreeNode;

    class FileSystemUtils
    {
      public:
        static Core::List<Core::String> getDiskDrives();
        static Core::List<std::filesystem::path> getPathEntries(Core::String path);
        static void fsToTreeView(Core::String path, TreeView* treeView, TreeNode* rootNode, bool addFiles, bool showRootNode);
    };
} // namespace Editor