#include "Behavior.h"

#include "../Scene/BehaviorInstance.h"
#include "../System/BehaviorManager.h"

namespace Core
{
	Behavior::Behavior(Object* owner, BehaviorManager* behaviorManager) : Component(owner)
	{
		_behaviorManager = behaviorManager;
	}

	Behavior::~Behavior() {}

	void Behavior::setScript(Script* script)
	{
		_script = script;

		if (_behaviorInstance != nullptr)
		{
			_behaviorManager->destroyInstance(_behaviorInstance);
			_behaviorInstance = nullptr;
		}

		if (_script != nullptr)
		{
			_behaviorInstance = _behaviorManager->createInstance(_owner, _script);
			_behaviorInstance->init();
		}
	}

	void Behavior::update(float& dt)
	{
	}
} // namespace Core