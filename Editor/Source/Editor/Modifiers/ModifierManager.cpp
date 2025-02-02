#include "ModifierManager.h"

#include "Modifier.h"

#include "../Windows/Window.h"

namespace Editor
{
    ModifierManager ModifierManager::_singleton;

    void ModifierManager::setCurrentModifier(UString name)
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

    Modifier* ModifierManager::getModifier(UString name)
    {
        Modifier* mod = nullptr;

        bool found = _modifiers.tryFind(mod, [=](Modifier* x) {
            return x->getName() == name;
        });

        if (found) return mod;
        return nullptr;
    }

    UString ModifierManager::getCurrentModifierName()
    {
        if (_currentModifier == nullptr) return "";
        return _currentModifier->getName();
    }

    void ModifierManager::enableWindows(bool enable)
    {
        if (_currentModifier == nullptr) return;
        _currentModifier->enableWindows(enable);
    }

    void ModifierManager::init(Core::Scene* scene)
    {
        _scene = scene;

        for (auto it : _modifiers)
        {
            it->init(_scene);
        }
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