#pragma once

#include <Shared/List.h>
#include <Shared/String.h>

namespace Editor
{
	class Modifier;

	class ModifierManager
	{
	private:
		static ModifierManager* _singleton;
		Core::List<Modifier*> _modifiers;

		Modifier* _currentModifier = nullptr;

	public:
		static ModifierManager* singleton();

		void addModifier(Modifier* value) { _modifiers.add(value); }
		
		void setCurrentModifier(UString name);
		Modifier* getCurrentModifier() { return _currentModifier; }
	};
}