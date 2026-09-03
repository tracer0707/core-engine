#pragma once

#include <Core/Shared/String.h>

#include "Window.h"

namespace Editor
{
	class TreeView;
	class TreeNode;
	class LinearLayout;
	class Gizmo;

	class HierarchyWindow : public Window
	{
		private:
			TreeView* _objectTree = nullptr;
			LinearLayout* _linearLayout = nullptr;
			Gizmo* _gizmo = nullptr;

			void setInspector(TreeNode* node);

		public:
			HierarchyWindow(WindowManager* parent);
			virtual ~HierarchyWindow();

			virtual void onUpdate();

			TreeView* getTreeView() { return _objectTree; }
			void setGizmo(Gizmo* value) { _gizmo = value; }
	};
} // namespace Editor