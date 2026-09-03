#pragma once

#include <Core/Shared/String.h>

#include "Window.h"

namespace Core
{
	class Scene;
}

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

			Core::Scene* _scene = nullptr;

			void setInspector(TreeNode* node);

		public:
			HierarchyWindow(WindowManager* parent);
			virtual ~HierarchyWindow();

			virtual void onUpdate();

			TreeView* getTreeView() { return _objectTree; }
			void setGizmo(Gizmo* value) { _gizmo = value; }

			void setScene(Core::Scene* value);

			void refreshHierarchy();
	};
} // namespace Editor