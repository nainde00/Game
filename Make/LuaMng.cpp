#include "LuaMng.h"
CLuaMng::CLuaMng()
{
	const luaL_Reg *lib;
	LuaHandle *temp;
	m_iIndex = 0;
	m_lLuaHandle.clear();

	for( int i = 0; i < MAX_LUA_HANDLE; i++)
	{
		temp = new LuaHandle;
		temp->index = m_index++;
		temp->lState = luaL_newstate();

		for(lib = lualibs; lib->func !=NULL; lib++)
		{
			lib->func(temp->lState);
			lua_settop(temp->lState, 0);

		}
		
		RegistFunctions(temp);

		m_lLuaHandle.push_back(temp);

	}

}

CLuaMng::~CLuaMng()
{
	LuaHandle *pLua;
	while(!m_lLuaHandle.empty())
	{
		pLua = m_lLuaHandle.front();
		lua_close(pLua->lState);
		delete pLua;

		m_lLuaHandle.pop_front();
	}
	m_lLuaHandle.clear();
}

LuaHandle * CLuaMng::GetLuaHandle(void)
{
	LuaHandle *retLuaHandle;

	//락이 들어가야되는 부분. 멀티 쓰래드 사용시 루아 파일에 순차적으로 접근하기 위하여.

	int iLuaHandleCount = m_lLuaHandle.empty();

	if(!iLuaHandleCount)
	{
		retLuaHandle = m_lLuaHandle.front();
		m_lLuaHandle.pop_front();
	}
	else
	{
		const luaL_Reg *lib;

		retLuaHandle = new LuaHandle;
		retLuaHandle->index = m_index++;
		retLuaHandle->lState = luaL_newstate();
		RegistFunctions(retLuaHandle);


		for(lib = lualibs; lib->func !=NULL; lib++)
		{
			lib->func(retLuaHandle->lState);
			lua_settop(retLuaHandle->lState, 0);

		}
		
	}

	return retLuaHandle;
}

void CLuaMng::RestoreLuaHandle(LuaHandle *lHandle)
{
	//락이 들어가야되는부분
	m_lLuaHandle.push_back(lHandle);
}

void CLuaMng::SetDoIntValue(LuaHandle *pLHandle,char *chName, int value)
{
	lua_pushnumber(pLHandle->lState, value);
	lua_setglobal(pLHandle->lState, chName);
}

void CLuaMng::SetDoStringValue(LuaHandle *pLHandle, char *chName, char *value)
{
	lua_pushstring(pLHandle->lState, value);
	lua_setglobal(pLHandle->lState, chName);
}
	

void CLuaMng::SetDoFloatValue(LuaHandle *pLHandle, char *chName, float value)
{
	lua_pushnumber(pLHandle->lState, value);
	lua_setglobal(pLHandle->lState, chName);
}





void CLuaMng::RegistFunctions(LuaHandle *plState)
{
	lua_register(plState->lState,"TRACE2", LuaTRACE2);
//	lua_register(plState->lState,"LuaDebugPrint", LuaDebugPrint);
	Lunar<CLuaFunctionManager>::Register(plState->lState);

}



int CLuaMng::GetStateSize()
{
	return m_lLuaHandle.size();
		
}

bool CLuaMng::MakeFileName(char *pchFileName, int iQuestNumber)
{
	//iQuestNumber 값에 따라서 읽어들이는 루아파일 이름들을 제어한다.
	if(iQuestNumber > 0 && 	iQuestNumber < 1000)	//case 문으로 제어해도 된다.
	{
		sprintf(pchFileName,"event\\Q%d.lua",iQuestNumber);
		return true;
	}

	return false;
}

int CLuaMng::Do(int UID, int iNPC, int iNPCSocketID, int iQuestNumber, int iChoice)
{
	int ret = -1;
	int iLuaDoFileError = 0;
	LuaHandle *temp;
	temp = GetLuaHandle(); //루아 핸들을 가져옴
	SetDoIntValue(temp, "UID", UID); // 루아파일의 UID 변수에 UID값을 등록 .. 이하 같음
	SetDoIntValue(temp, "choice",iChoice);
	SetDoIntValue(temp, "Npc",iNPC);
	SetDoIntValue(temp, "NpcSocketID", iNPCSocketID);
	
	char pchFileName[40];
	memset(pchFileName, 0 ,sizeof(pchFileName));

	MakeFileName(pchFileName, iQuestNumber);

	if(access(pchFileName, 00) == 0 )	
	{
		iLuaDoFileError = luaL_dofile(temp->lState, pchFileName);		//해당 루아파일에 접근.
		if( iLuaDoFileError != 0)
		{
			fprintf(stderr, lua_tostring(temp->lState, -1));
			if(access(".\\quest\\SyntaxError.lua",00)==0)
			{
				luaL_dofile(temp->lState,".\\quest\\SyntaxError.lua");
			}
			ret = 0;
		}
	}
	else
	{
		ret = 0;
	}

	lua_gc(temp->lState,LUA_GCCOLLECT, 0);
	RestoreLuaHandle(temp);
	return ret;


}




int LuaTRACE2(lua_State *lState)
{
	const char *ret = lua_tostring(lState, 1);
	printf("%s\n",ret);
	return 0;
}


