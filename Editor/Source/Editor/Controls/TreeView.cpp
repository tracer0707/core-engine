#include "TreeView.h"

#include <System/EventHandler.h>

namespace Editor
{
	TreeView::TreeView() {}

	TreeView::~TreeView() {}

	void TreeView::update()
	{
		for (auto it : _controls)
		{
			it->update();
		}
	}
}