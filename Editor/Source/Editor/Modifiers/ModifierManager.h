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

		void enableWindows(bool enable);

	public:
		static ModifierManager* singleton();

		void addModifier(Modifier* value) { _modifiers.add(value); }
		
		void setCurrentModifier(UString name);
		void unsetCurrentModifier();
		Modifier* getCurrentModifier() { return _currentModifier; }
		UString getCurrentModifierName();
	};
}