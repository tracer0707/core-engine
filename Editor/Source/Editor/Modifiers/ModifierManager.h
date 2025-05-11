#pragma once

#include <Shared/List.h>
#include <Shared/String.h>

namespace Core
{
	class Scene;
}

namespace Editor
{
	class Modifier;

	class ModifierManager
	{
	private:
		static ModifierManager _singleton;
		Core::List<Modifier*> _modifiers;

		Modifier* _currentModifier = nullptr;
		Core::Scene* _scene = nullptr;

		void enableWindows(bool enable);

	public:
		static ModifierManager* singleton() { return &_singleton; }

		void addModifier(Modifier* value) { _modifiers.add(value); }
		
		void setCurrentModifier(uint32_t name);
		void unsetCurrentModifier();
		Modifier* getModifier(uint32_t name);
		Modifier* getCurrentModifier() { return _currentModifier; }
		uint32_t getCurrentModifierName();

		void init(Core::Scene* scene);
		void destroy();
		void update();
		void render();
	};
}