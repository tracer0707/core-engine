#pragma once

#include <functional>

namespace Editor
{
	class Control;
	class InspectorWindow;

	class Inspector
	{
			friend class InspectorWindow;

		private:
			std::function<void()> _onDestroy = nullptr;

		protected:
			InspectorWindow* _parent = nullptr;

		public:
			Inspector() = default;
			virtual ~Inspector();

			virtual Control* build() = 0;

			void setOnDestroy(std::function<void()> value) { _onDestroy = value; }

			InspectorWindow* getParent() const { return _parent; }
	};
} // namespace Editor