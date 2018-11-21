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

public:	//Lua ���ϰ� �����ϴ� �Լ���
	LuaHandle * GetLuaHandle(void);	//��Ƽ �������. ��� �Լ����� ���ÿ� Ȱ��ȭ ���� ���� �ڵ��� ������ ����.
	void RegistFunctions(LuaHandle *plState);	//��� �Լ��� ����ϴ� �Լ�.
	void RestoreLuaHandle(LuaHandle *lHandle);	//��Ƽ ������� ��� �Լ���� ������ ����� �ڵ��� ��ȯ�Ѵ�.
	void SetDoIntValue(LuaHandle *pLHandle,char *chName, int value); //int ���� �����ϴ� �Լ�.				--
	void SetDoStringValue(LuaHandle *pLHandle, char *chName, char *value);	//  String ���� �����ϴ� �Լ�.	-- �� ���Լ��� ��� ���ϳ��� ������ �ش� ���� �����ҋ� ���ȴ�.
	void SetDoFloatValue(LuaHandle *pLHandle, char *chName, float value);	// Float���� �����ϴ� ���Լ�.		--

public:
	int m_index;
	int GetStateSize(); //�ڵ� ������ ������
	bool MakeFileName(char *pchFileName, int iQuestNumber);	//�о�帱 ��� ���� �̸�
	int Do(int UID, int iNPC, int iNPCSocketID, int iQuestNumber, int iChoice);	//��� ȣ�� �Լ�. �����ڴ� �ش� ��� ���� �� �����Ѵ�.


};




//������Ͽ��� �� �Լ���



int LuaTRACE2(lua_State *lState);



typedef Loki::SingletonHolder<CLuaMng> LuaMngSingleton;
#define g_CLuaMng LuaMngSingleton::Instance()

#endif //_LUAMNG