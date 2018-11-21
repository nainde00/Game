#ifndef _LuaFunctionManager
#define _LuaFunctionManager

#include <list>
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"




class CLuaFunctionManager
{
public:
	CLuaFunctionManager(lua_State *lState );
	~CLuaFunctionManager();
	static const char className[];
	static Lunar<CLuaFunctionManager>::RegType methods[];
	
public:	//클래스 사용 함수들
	int LuaTRACE(lua_State *lState);
	int LuaDebugPrint(lua_State *lState);


};







#endif //_LuaFunctionManager