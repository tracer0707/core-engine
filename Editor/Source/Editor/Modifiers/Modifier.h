#pragma once

#include <Shared/String.h>
#include <Shared/List.h>

namespace Core
{
	class Scene;
}

namespace Editor
{
	class Window;

	class Modifier
	{
		friend class ModifierManager;

	private:
		virtual void enableWindows(bool enable) = 0;

	protected:
		uint32_t _name = 0;

		Core::Scene* _scene = nullptr;

	public:
		Modifier(uint32_t name);
		virtual ~Modifier();

		uint32_t getName() { return _name; }

		virtual void init(Core::Scene* scene) { _scene = scene; }
		virtual void update() = 0;
		virtual void render() = 0;
	};
}