#pragma once

struct lua_State;

namespace Core
{
	class Script;
	class Object;

	class BehaviorInstance
	{
		private:
			lua_State* _state = nullptr;
			Script* _script = nullptr;
			Object* _object = nullptr;

			int _environmentRef = -2;

			static int lua_Object_getName(lua_State* L);

			void createEnvironment();
			void registerObject();
			void bindObject();
			void pushObject();

		public:
			BehaviorInstance(lua_State* state, Script* script, Object* object);
			~BehaviorInstance();

			void init();
			void update(float& dt);
	};
} // namespace Core