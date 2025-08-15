#include "ModifierManager.h"

#include "Modifier.h"
#include "CSGModifier.h"

#include "../Windows/WindowManager.h"
#include "../Windows/Window.h"

namespace Editor
{
    ModifierManager ModifierManager::_singleton;

    void ModifierManager::init(WindowManager* windowManager, Core::Scene* scene)
    {
        _scene = scene;
        _windowManager = windowManager;

        for (auto it : _modifiers)
        {
            it->init(_windowManager->getRenderer(), _scene, _windowManager->getAssetManager());
        }
    }

    Modifier* ModifierManager::addModifier(uint32_t type)
    {
        Modifier* modifier = nullptr;

        if (type == CSG_MODIFIER)
        {
            modifier = new CSGModifier();
        }

        assert(modifier != nullptr && "Unknown modifier");

        _modifiers.add(modifier);

        return modifier;
    }

    void ModifierManager::setCurrentModifier(uint32_t name)
    {
        Modifier* mod = nullptr;

        bool found = _modifiers.tryFind(mod, [=](Modifier* x) {
            return x->getName() == name;
        });

        assert(found && "Modifier is not registered");

        if (_currentModifier == mod) return;

        enableWindows(false);

        _currentModifier = mod;

        enableWindows(true);
    }

    void ModifierManager::unsetCurrentModifier()
    {
        enableWindows(false);
        _currentModifier = nullptr;
    }

    Modifier* ModifierManager::getModifier(uint32_t name)
    {
        Modifier* mod = nullptr;

        bool found = _modifiers.tryFind(mod, [=](Modifier* x) {
            return x->getName() == name;
        });

        if (found) return mod;
        return nullptr;
    }

    uint32_t ModifierManager::getCurrentModifierName()
    {
        if (_currentModifier == nullptr) return UINT32_MAX;
        return _currentModifier->getName();
    }

    void ModifierManager::enableWindows(bool enable)
    {
        if (_currentModifier == nullptr) return;
        _currentModifier->enableWindows(enable);
    }

    void ModifierManager::destroy()
    {
        _scene = nullptr;
        _currentModifier = nullptr;
        
        for (int i = 0; i < _modifiers.count(); ++i)
        {
            Modifier* modifier = _modifiers.get(i);
            delete modifier;
        }

        _modifiers.clear();
    }

    void ModifierManager::update()
    {
        if (_currentModifier == nullptr) return;
        _currentModifier->update();
    }

    void ModifierManager::render()
    {
        if (_currentModifier == nullptr) return;
        _currentModifier->render();
    }
}