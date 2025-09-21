#pragma once

#include <Core/Shared/List.h>
#include <Core/Shared/String.h>

#include "ModifierList.h"

namespace Core
{
	class Scene;
}

namespace Editor
{
	class Modifier;
	class WindowManager;
	class CSGModifier;

	class ModifierManager
	{
	private:
		static ModifierManager _singleton;

		WindowManager* _windowManager = nullptr;
		
		Core::List<Modifier*> _modifiers;

		Modifier* _currentModifier = nullptr;
		Core::Scene* _scene = nullptr;

		void enableWindows(bool enable);

		Modifier* addModifier(uint32_t type);

	public:
		static ModifierManager* singleton() { return &_singleton; }

		WindowManager* getWindowManager() { return _windowManager; }

		template <typename T>
		T addModifier() {}

		void setCurrentModifier(uint32_t name);
		void unsetCurrentModifier();
		Modifier* getModifier(uint32_t name);
		Modifier* getCurrentModifier() { return _currentModifier; }
		uint32_t getCurrentModifierName();

		void init(WindowManager* windowManager, Core::Scene* scene);
		void destroy();
		void update();
		void render();
	};

	template <>
	inline CSGModifier* ModifierManager::addModifier<CSGModifier*>()
	{
		return (CSGModifier*)addModifier(CSG_MODIFIER);
	}
}