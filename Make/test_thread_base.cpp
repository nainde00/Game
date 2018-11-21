// --------------------------------------------------------------------------
// cpp.
//
// --------------------------------------------------------------------------

#include "test_thread_base.h"
#include <tchar.h>

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
TEST_THREAD_BASE::TEST_THREAD_BASE(tstring name) : THREAD_BASE(name)
{

}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
TEST_THREAD_BASE::~TEST_THREAD_BASE()
{

}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
bool TEST_THREAD_BASE::do_work()
{
	
	for (int count = 0; count < 100; count++)
	{
		_tprintf(_T(" Thread_name : %s count = %d \n"), get_name().c_str(), count);
		Sleep(10);
	}



	return true;

}


