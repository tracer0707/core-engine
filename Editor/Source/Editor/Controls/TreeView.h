#pragma once

#include <Core/Shared/String.h>
#include <Core/Shared/List.h>

#include "Control.h"

namespace Editor
{
	class TreeNode;

	class TreeView : public Control
	{
		private:
			Core::List<TreeNode*> _selectedNodes;

			std::function<void(Core::List<TreeNode*>&)> _onSelectionChanged = nullptr;

			bool _selectMultiple = false;

		public:
			TreeView();
			virtual ~TreeView();

			virtual int getControlType() const;
			virtual void update();

			TreeNode* createNode();
			void destroyNode(TreeNode* value);

			void setOnSelectionChanged(std::function<void(Core::List<TreeNode*>&)> callback) { _onSelectionChanged = callback; }

			const Core::List<TreeNode*>& getSelectedNodes() const { return _selectedNodes; }
			bool isNodeSelected(TreeNode* node) const;

			TreeNode* findNodeByTag(int key, Core::String value);
			TreeNode* findNodeByTag(int key, void* value);

			void selectNode(TreeNode* value, bool byUser = true);
			void clearSelection(bool byUser = true);

			bool getSelectMultiple() { return _selectMultiple; }
			void setSelectMultiple(bool value) { _selectMultiple = value; }

			virtual void clear();
	};
} // namespace Editor