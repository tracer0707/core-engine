#include "BehaviorInstance.h"

extern "C" {
#include <lauxlib.h>
#include <lualib.h>
}

#include "Object.h"
#include "../Shared/String.h"
#include "../Content/Script.h"

namespace Core
{
	struct LuaObject
	{
		Object* object = nullptr;
	};

	BehaviorInstance::BehaviorInstance(lua_State* state, Script* script, Object* object)
	{
		_state = state;
		_script = script;
		_object = object;

		const String& source = _script->getSource();

		lua_State* L = _state;
		
		registerObject();
		createEnvironment();
		bindObject();

		int result = luaL_loadbuffer(L, source.c_str(), source.byteSize(), "");

		if (result != LUA_OK)
		{
			const char* error = lua_tostring(L, -1);

			// log error

			lua_pop(L, 1);
			return;
		}

		lua_rawgeti(L, LUA_REGISTRYINDEX, _environmentRef);
		lua_setupvalue(L, -2, 1);

		result = lua_pcall(L, 0, 0, 0);

		if (result != LUA_OK)
		{
			const char* error = lua_tostring(L, -1);

			// log error

			lua_pop(L, 1);
			return;
		}
	}

	BehaviorInstance::~BehaviorInstance()
	{
		if (_state != nullptr && _environmentRef != LUA_NOREF && _environmentRef != LUA_REFNIL)
		{
			luaL_unref(_state, LUA_REGISTRYINDEX, _environmentRef);

			_environmentRef = LUA_NOREF;
		}
	}

	void BehaviorInstance::init()
	{
		lua_State* L = _state;

		lua_rawgeti(L, LUA_REGISTRYINDEX, _environmentRef);

		lua_getfield(L, -1, "init");

		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 2);
			return;
		}

		if (lua_pcall(L, 0, 0, 0) != LUA_OK)
		{
			const char* error = lua_tostring(L, -1);

			// log error

			lua_pop(L, 1);
		}

		lua_pop(L, 1);
	}

	void BehaviorInstance::update(float& dt)
	{
		lua_State* L = _state;

		lua_rawgeti(L, LUA_REGISTRYINDEX, _environmentRef);

		lua_getfield(L, -1, "update");

		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 2);
			return;
		}

		lua_pushnumber(L, dt);

		if (lua_pcall(L, 1, 0, 0) != LUA_OK)
		{
			const char* error = lua_tostring(L, -1);

			// log error

			lua_pop(L, 1);
		}

		lua_pop(L, 1);
	}

	int BehaviorInstance::lua_Object_getName(lua_State* L)
	{
		auto* userData = static_cast<LuaObject*>(luaL_checkudata(L, 1, "Core.Object"));

		Object* object = userData->object;

		lua_pushstring(L, object->getName().c_str());

		return 1;
	}

	void BehaviorInstance::createEnvironment()
	{
		lua_State* L = _state;

		lua_newtable(L);
		lua_newtable(L);
		lua_pushglobaltable(L);

		lua_setfield(L, -2, "__index");
		lua_setmetatable(L, -2);

		_environmentRef = luaL_ref(L, LUA_REGISTRYINDEX);
	}

	void BehaviorInstance::registerObject()
	{
		lua_State* L = _state;

		if (luaL_newmetatable(L, "Core.Object"))
		{
			lua_pushcfunction(L, lua_Object_getName);
			lua_setfield(L, -2, "getName");

			lua_pushvalue(L, -1);
			lua_setfield(L, -2, "__index");
		}

		lua_pop(L, 1);
	}

	void BehaviorInstance::bindObject()
	{
		lua_State* L = _state;

		lua_rawgeti(L, LUA_REGISTRYINDEX, _environmentRef);

		pushObject();

		lua_setfield(L, -2, "self");

		lua_pop(L, 1);
	}

	void BehaviorInstance::pushObject()
	{
		lua_State* L = _state;

		auto* userData = static_cast<LuaObject*>(lua_newuserdatauv(L, sizeof(LuaObject), 0));

		userData->object = _object;

		luaL_getmetatable(L, "Core.Object");
		lua_setmetatable(L, -2);
	}
} // namespace Core