#include "BehaviorManager.h"

extern "C" {
#include <lauxlib.h>
#include <lualib.h>
}

#include "../Scene/Object.h"
#include "../Scene/BehaviorInstance.h"

namespace Core
{
	BehaviorManager::BehaviorManager()
	{
		_state = luaL_newstate();
		luaL_openlibs(_state);
	}

	BehaviorManager::~BehaviorManager()
	{
		lua_close(_state);
		_state = nullptr;
	}

	BehaviorInstance* BehaviorManager::createInstance(Object* object, Script* script)
	{
		BehaviorInstance* instance = new BehaviorInstance(_state, script, object);
		_behaviorInstances[object] = instance;

		return instance;
	}

	void BehaviorManager::destroyInstance(BehaviorInstance* instance)
	{
		auto it = std::find_if(_behaviorInstances.begin(), _behaviorInstances.end(), [instance](const auto& pair) { return pair.second == instance; });
		
		delete it->second;
		
		if (it != _behaviorInstances.end())
		{
			_behaviorInstances.erase(it);
		}
	}
} // namespace Core