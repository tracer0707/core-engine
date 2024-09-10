#pragma once

#include "Window.h"

namespace Editor
{
	class LinearLayout;
	class CSGModifier;

	class CSGObjectWindow : public Window
	{
	private:
		CSGModifier* _modifier = nullptr;

		LinearLayout* layoutMain = nullptr;

		void disableAll();
		
	public:
		CSGObjectWindow();
		virtual ~CSGObjectWindow();

		void setModifier(CSGModifier* value) { _modifier = value; }
	};
}