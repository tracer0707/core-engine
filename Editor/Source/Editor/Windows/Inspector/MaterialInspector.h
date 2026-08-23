#pragma once

#include "Inspector.h"

namespace Core
{
	class Material;
	class Renderer;
} // namespace Core

namespace Editor
{
	class MaterialInspector : public Inspector
	{
		private:
			Core::Material* _material = nullptr;
			Core::Renderer* _renderer = nullptr;

		public:
			MaterialInspector(Core::Material* material, Core::Renderer* renderer, Core::EventHandler* eventHandler);
			virtual ~MaterialInspector();

			virtual void build();
	};
} // namespace Editor