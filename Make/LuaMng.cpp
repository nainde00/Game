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

	//���� ���ߵǴ� �κ�. ��Ƽ ������ ���� ��� ���Ͽ� ���������� �����ϱ� ���Ͽ�.

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
	//���� ���ߵǴºκ�
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
	//iQuestNumber ���� ���� �о���̴� ������� �̸����� �����Ѵ�.
	if(iQuestNumber > 0 && 	iQuestNumber < 1000)	//case ������ �����ص� �ȴ�.
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
	temp = GetLuaHandle(); //��� �ڵ��� ������
	SetDoIntValue(temp, "UID", UID); // ��������� UID ������ UID���� ��� .. ���� ����
	SetDoIntValue(temp, "choice",iChoice);
	SetDoIntValue(temp, "Npc",iNPC);
	SetDoIntValue(temp, "NpcSocketID", iNPCSocketID);
	
	char pchFileName[40];
	memset(pchFileName, 0 ,sizeof(pchFileName));

	MakeFileName(pchFileName, iQuestNumber);

	if(access(pchFileName, 00) == 0 )	
	{
		iLuaDoFileError = luaL_dofile(temp->lState, pchFileName);		//�ش� ������Ͽ� ����.
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


