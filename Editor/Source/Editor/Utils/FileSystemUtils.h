#pragma once

#include <Core/Shared/String.h>

namespace Editor
{
	class TreeView;
	class TreeNode;

	class FileSystemUtils
	{
	public:
		static void fsToTreeView(Core::String path, TreeView* treeView, TreeNode* rootNode, bool addFiles);
	};
}