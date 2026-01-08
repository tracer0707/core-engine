#pragma once

#include <functional>

namespace Editor
{
	class Control;

	class Inspector
	{
		private:
			std::function<void()> _onDestroy = nullptr;

		public:
			Inspector() = default;
			virtual ~Inspector();

			virtual Control* build() = 0;

			void setOnDestroy(std::function<void()> value) { _onDestroy = value; }
	};
} // namespace Editor