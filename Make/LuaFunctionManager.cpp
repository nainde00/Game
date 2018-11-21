#include "LuaFunctionManager.h"

CLuaFunctionManager::CLuaFunctionManager(lua_State *lState)
{
	printf("Start CLuaFunctionManager class");
}
CLuaFunctionManager::~CLuaFunctionManager()

{
	printf("End CLuaFunctionManager class");
}

const char CLuaFunctionManager::className[] = "CLuaFunctionManager";

Lunar<CLuaFunctionManager>::RegType CLuaFunctionManager::methods[] = {
	LUNAR_DECLARE_METHOD(CLuaFunctionManager, LuaTRACE),
	LUNAR_DECLARE_METHOD(CLuaFunctionManager, LuaDebugPrint),
  {0,0}
};


int CLuaFunctionManager::LuaTRACE(lua_State *lState)
{
	const char *ret = lua_tostring(lState, 1);
	printf("%s\n",ret);
	return 0;
}

int CLuaFunctionManager::LuaDebugPrint(lua_State *lState)
{
	const char *ret2 = lua_tostring(lState, 1);
	printf("%s\n",ret2);
	return 0;
}