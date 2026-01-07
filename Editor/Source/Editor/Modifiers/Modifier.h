#pragma once

#include <Core/Shared/String.h>
#include <Core/Shared/List.h>

namespace Core
{
	class Scene;
	class Renderer;
	class ContentManager;
} // namespace Core

namespace Editor
{
	class Window;
	class ContentLoader;

	class Modifier
	{
			friend class ModifierManager;

		private:
			virtual void enableWindows(bool enable) = 0;

		protected:
			uint32_t _name = 0;

			Core::Renderer* _renderer = nullptr;
			Core::Scene* _scene = nullptr;
			Core::ContentManager* _contentManager = nullptr;

		public:
			Modifier(uint32_t name);
			virtual ~Modifier();

			uint32_t getName() { return _name; }

			virtual void init(Core::Renderer* renderer, Core::Scene* scene, Core::ContentManager* contentManager);
			virtual void update() = 0;
			virtual void render() = 0;
	};
} // namespace Editor