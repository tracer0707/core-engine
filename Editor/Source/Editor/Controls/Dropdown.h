#pragma once

#include "Control.h"

#include <functional>

#include <Core/Shared/String.h>
#include <Core/Shared/List.h>

namespace Editor
{
	class Dropdown : public Control
	{
		private:
			Core::List<Core::String> _items;

			float _actualWidth = 0.0f;
			float _actualHeight = 0.0f;

			int current_item_index = 0;

			std::function<void(int)> _onSelectItem = nullptr;

		public:
			Dropdown() = default;
			Dropdown(Core::List<Core::String> items);
			virtual ~Dropdown();

			void setOnSelectItem(std::function<void(int)> value) { _onSelectItem = value; }
			void setItems(Core::List<Core::String> value) { _items = value; }
			int getSelectedIndex() { return current_item_index; }

			virtual float getWidth() const;
			virtual float getHeight() const;

			virtual int getControlType() const;
			virtual void update();
	};
} // namespace Editor