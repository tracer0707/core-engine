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
		UString _name = "";

		Core::Scene* _scene = nullptr;

	public:
		Modifier(UString name);
		virtual ~Modifier();

		UString getName() { return _name; }

		virtual void init(Core::Scene* scene) = 0 { _scene = scene; }
		virtual void update() = 0;
		virtual void render() = 0;
	};
}