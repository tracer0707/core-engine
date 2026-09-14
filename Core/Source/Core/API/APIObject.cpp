#include "API.h"

#include <cstring>

extern "C" {
#include <lauxlib.h>
#include <lualib.h>
}

#include "../Scene/Object.h"

namespace Core
{
	void API::registerObjectAPI(lua_State* L)
	{
		if (luaL_newmetatable(L, "Core.Object"))
		{
			/*lua_pushcfunction(L, [](lua_State* L) -> int {
				LuaObject* userData = static_cast<LuaObject*>(luaL_checkudata(L, 1, "Core.Object"));
				Object* object = static_cast<Object*>(userData->object);
				lua_pushstring(L, object->getName().c_str());
				return 1;
			});
			lua_setfield(L, -2, "getName");*/

			/*lua_pushvalue(L, -1);
			lua_setfield(L, -2, "__index");*/

			lua_pushcfunction(L, [](lua_State* L) -> int {
				auto* userData = static_cast<LuaObject*>(luaL_checkudata(L, 1, "Core.Object"));

				Object* object = static_cast<Object*>(userData->object);

				const char* property = luaL_checkstring(L, 2);

				if (strcmp(property, "name") == 0)
				{
					lua_pushstring(L, object->getName().c_str());
					return 1;
				}

				if (strcmp(property, "transform") == 0)
				{
					//pushTransform(L, object->getTransform());
					return 1;
				}

				lua_pushnil(L);
				return 1;
			});

			lua_setfield(L, -2, "__index");
		}
		lua_pop(L, 1);
	}
} // namespace Core::API