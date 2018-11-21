// Make.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//
//���� �������.
// ������ ����(���κпϷ�) �ؾߵɲ� ����� ���� ó��..
// Lua ����.(�Ϸ�)
// �̵� ���� (������)
// ��� ���� ( ������)
// ���� ����°� ����.( �Ϸ� )
// �ǹ�����?

#include "stdafx.h"

//#include "LuaMng.h"
#include "Mutex.h"
#include "./dump/exception_handler.h"
#include "./dump/exception_handler_base.h"
#include "test_thread_base.h"
#include "./iocp/job_manager.h"
#include "test_item_base.h"
#include <tchar.h>
#include "test_c11singleton.h"
#include "Singleton\C11_Singleton.h"

int sum = 0;
unsigned int WINAPI ThreadFunc(void* arg);
CMuetx m_MutexLock;

#define _64bit
void crash_test(void)
{
	int *p = NULL;
	(*p) = 100;

}

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, ".949");
	//minidump�� �ʱ⼳��
	TCHAR filename[100] = L"c:\\dump\\dump.dmp";
	YS_DUMP::EXCEPTION_HANDLER* exception_handler = new YS_DUMP::EXCEPTION_HANDLER(filename, YS_DUMP::MINI_DUMP_WITH_FULL_MEMORY);
	//�̴ϴ��� ����.
	YS_DUMP::EXCEPTION_HANDLER_BASE::start_and_set_handler(exception_handler);
	//�����ҽ� �ѹ����ϸ�˴ϴ�.

//	crash_test();

//	char str[10][100];
	printf("Test\n");
//	g_CLuaMng.Do(100,100,100,100,2);
	
	/*
	HANDLE hThread[10];
	double th = 0;

	
	for( int i=0; i < 10; i++)
	{
		sprintf(str[i],"[thread Number %d]",i);
		hThread[i] = (HANDLE)_beginthreadex(NULL,0,ThreadFunc,(void*)str[i],0,(unsigned int*)&th);
	}


	for(int i=0; i< 10; i++)
	{
		WaitForSingleObject(hThread[i], INFINITE);
		CloseHandle(hThread[i]);
	}
	*/
	/*
	TEST_THREAD_BASE test_thread_base(L"thread �׽�Ʈ");
	TEST_THREAD_BASE test_thread_base2(L"thread �׽�Ʈ2");

	test_thread_base.run();
	test_thread_base2.run();

	test_thread_base.end();
	test_thread_base2.end();
	

	g_JOB_MANAGER.create_workers(JOB_MANAGER::FIRST_THRAED, L"1�� first_thread���");
	g_JOB_MANAGER.create_workers(JOB_MANAGER::SECOND_THRAED, L"2�� second_thread���");

	//1���� item ������ 1���� thread�Ҵ�.
	TEST_ITEM_BASE item_base1; //iocp�� ��ϵ� itemȣ��
	TEST_ITEM_BASE item_base2; //iocp�� ��ϵ� itemȣ��

	g_JOB_MANAGER.run_job(item_base1, JOB_MANAGER::FIRST_THRAED); //����� �����.
	//���� ������ item_base�� ���� �ٸ� thread�� ����� �ϰԵǸ� �ϳ��� thread���� ó���ϰԵ˴ϴ�.
	//item->status ��������.

	g_JOB_MANAGER.run_job(item_base2, JOB_MANAGER::SECOND_THRAED); //����� �����.

	printf("End Test");

	bool loop = true;
	int count = 0;
	while (loop)
	{
//		Sleep(10000);
		Sleep(10);
		count++;
		if (count == 1)
		{
			g_JOB_MANAGER.pause_workers(JOB_MANAGER::FIRST_THRAED);
			g_JOB_MANAGER.pause_workers(JOB_MANAGER::SECOND_THRAED);
//			printf("item_base ����");
		}

		if (count == 1000)
		{
			loop = false;
		}
	}

	g_JOB_MANAGER.job_shutdown();
	*/
	printf("\n ---> �������� <--- \n");
	TEST_C11SINGLETON::get_Instance().test_function();
	TEST_C11SINGLETON::destroyInstance();

	return 0;
}



unsigned int WINAPI ThreadFunc(void* arg)
{ 

 
 char *mes = (char*)arg;
 int i = 0;

 for(i=0; i<5; i++)
 {
  CMuetx::Owner Lock(m_MutexLock);
  Sleep(1000);
  {
	sum++;
  }
  printf("%s Thread Running Sum is %d\n",mes,sum);
 }

 return 0;
}

