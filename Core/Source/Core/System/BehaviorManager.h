#pragma once

#include <unordered_map>

struct lua_State;

namespace Core
{
	class Object;
	class Script;
	class BehaviorInstance;

	class BehaviorManager
	{
		private:
			lua_State* _state = nullptr;

			std::unordered_map<Object*, BehaviorInstance*> _behaviorInstances;

		public:
			BehaviorManager();
			~BehaviorManager();

			lua_State* getState() const { return _state; }

			BehaviorInstance* createInstance(Object* object, Script* script);
			void destroyInstance(BehaviorInstance* instance);
	};
}