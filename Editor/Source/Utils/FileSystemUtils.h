#pragma once

#include <string>
#include <filesystem>

#include <Core/Shared/String.h>
#include <Core/Shared/List.h>
#include <Core/Shared/Path.h>

namespace Editor
{
    class TreeView;
    class TreeNode;

    class FileSystemUtils
    {
      public:
        static Core::List<fs::path> getDiskDrives();
        static Core::List<fs::path> getPathEntries(const fs::path& path);
        static void enumerateFiles(const fs::path& root, Core::List<fs::path>& out);
        static void fsToTreeView(const fs::path& path, TreeView* treeView, TreeNode* rootNode, bool addFiles, bool showRootNode, bool lazyLoad = true);
    };
} // namespace Editor