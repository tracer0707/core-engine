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
      private:
        static Core::List<std::filesystem::path> getEntries(std::string path);

      public:
        static Core::List<Core::String> getDiskDrives();
        static void fsToTreeView(Core::String path, TreeView* treeView, TreeNode* rootNode, bool addFiles, bool showRootNode);
    };
} // namespace Editor