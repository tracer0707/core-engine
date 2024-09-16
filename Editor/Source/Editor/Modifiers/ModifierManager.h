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
		static ModifierManager* _singleton;
		Core::List<Modifier*> _modifiers;

		Modifier* _currentModifier = nullptr;
		Core::Scene* _scene = nullptr;

		void enableWindows(bool enable);

	public:
		static ModifierManager* singleton();

		void addModifier(Modifier* value) { _modifiers.add(value); }
		
		void setCurrentModifier(UString name);
		void unsetCurrentModifier();
		Modifier* getCurrentModifier() { return _currentModifier; }
		UString getCurrentModifierName();

		void init(Core::Scene* scene);
		void update();
		void render();
	};
}