#include "ModifierManager.h"

#include "Modifier.h"

namespace Editor
{
    ModifierManager* ModifierManager::_singleton = new ModifierManager();

    ModifierManager* ModifierManager::singleton()
    {
        return _singleton;
    }

    void ModifierManager::setCurrentModifier(UString name)
    {
        Modifier* mod = nullptr;

        bool found = _modifiers.tryFind(mod, [=](Modifier* x) {
            return x->getName() == name;
        });

        assert(found && "Modifier is not registered");

        _currentModifier = mod;
    }
}