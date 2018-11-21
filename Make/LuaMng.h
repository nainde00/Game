#ifndef _LUAMNG
#define _LUAMNG
#include <stdlib.h>
#include <io.h>
#include "LuaFunctionManager.h"
#include "Singleton.h"

static const luaL_Reg lualibs[] = {
	{"base", luaopen_base },
	{"package", luaopen_package},
	{"table", luaopen_table },
	{"string", luaopen_string },
//	{"math", luaopen_math },
	{ NULL, NULL }	

};

typedef struct LuaHandle{
	int index;
	lua_State *lState;
}LuaHandle;

#define MAX_LUA_HANDLE 2000

class CLuaMng
{
public:
	
	std::list<LuaHandle *> m_lLuaHandle;
	
	int m_iIndex;

	CLuaMng();
	~CLuaMng();

public:	//Lua 파일과 연동하는 함수들
	LuaHandle * GetLuaHandle(void);	//멀티 쓰래드용. 루아 함수에서 사용시에 활성화 되지 않은 핸들을 가져다 쓴다.
	void RegistFunctions(LuaHandle *plState);	//루아 함수를 등록하는 함수.
	void RestoreLuaHandle(LuaHandle *lHandle);	//멀티 쓰래드용 루아 함수사용 종료후 사용한 핸들을 반환한다.
	void SetDoIntValue(LuaHandle *pLHandle,char *chName, int value); //int 값을 셋팅하는 함수.				--
	void SetDoStringValue(LuaHandle *pLHandle, char *chName, char *value);	//  String 값을 셋팅하는 함수.	-- 이 세함수는 루아 파일내의 변수에 해당 값을 지정할떄 사용된다.
	void SetDoFloatValue(LuaHandle *pLHandle, char *chName, float value);	// Float값을 셋팅하는 는함수.		--

public:
	int m_index;
	int GetStateSize(); //핸들 사이즈 가져옴
	bool MakeFileName(char *pchFileName, int iQuestNumber);	//읽어드릴 루아 파일 이름
	int Do(int UID, int iNPC, int iNPCSocketID, int iQuestNumber, int iChoice);	//루아 호출 함수. 각인자는 해당 루아 파일 에 전달한다.


};




//루아파일에서 쓸 함수들



int LuaTRACE2(lua_State *lState);



typedef Loki::SingletonHolder<CLuaMng> LuaMngSingleton;
#define g_CLuaMng LuaMngSingleton::Instance()

#endif //_LUAMNG