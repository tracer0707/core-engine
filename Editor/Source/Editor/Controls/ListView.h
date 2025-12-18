#pragma once

#include <functional>

#include <Core/Shared/List.h>

#include "Control.h"

namespace Editor
{
	class ListView : public Control
	{
	private:
		Core::List<Control*> _selectedItems;
		std::function<void(Core::List<Control*>&)> _onSelectionChanged = nullptr;
		std::function<void(Control*)> _onItemClick = nullptr;

		float _actualWidth = 0.0f;
        float _actualHeight = 0.0f;

	public:
		ListView();
		virtual ~ListView();

		virtual float getWidth() const;
        virtual float getHeight() const;

		virtual int getControlType() const;
		virtual void update();

		void setOnSelectionChanged(std::function<void(Core::List<Control*>&)> callback) { _onSelectionChanged = callback; }
		void setOnItemClick(std::function<void(Control*)> callback) { _onItemClick = callback; }

		const Core::List<Control*>& getSelectedItems() const { return _selectedItems; }
		bool isItemSelected(Control* node);

		void selectItem(Control* value, bool byUser = true);
		void clearSelection(bool byUser = true);
	};
}