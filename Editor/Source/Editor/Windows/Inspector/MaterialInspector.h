#pragma once

#include "Inspector.h"

namespace Core
{
	class Material;
}

namespace Editor
{
	class MaterialInspector : public Inspector
	{
		private:
			Core::Material* _material = nullptr;

		public:
			MaterialInspector(Core::Material* material);
			virtual ~MaterialInspector();

			virtual Control* build();
	};
} // namespace Editor