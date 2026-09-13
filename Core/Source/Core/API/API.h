#pragma once

struct lua_State;

namespace Core
{
	struct LuaObject
	{
		void* object = nullptr;
	};

	class API
	{
		public:
			static void registerObjectAPI(lua_State* L);
	};
} // namespace Core::API