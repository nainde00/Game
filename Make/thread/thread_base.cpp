
// --------------------------------------------------------------------------
// cpp.
//
// --------------------------------------------------------------------------

#include "thread_base.h"
#include <windows.h>
#include <process.h>
#include "atomic_counter.h"

// --------------------------------------------------------------------------
// Static Initialisation
// --------------------------------------------------------------------------
YS_THREAD::ATOMIC_COUNTER YS_THREAD::THREAD_BASE::thread_counter;

namespace YS_THREAD
{
	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	THREAD_BASE::THREAD_BASE(const tstring& name_in) : name(name_in), status(NOT_STARTED)
	{

	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	THREAD_BASE::~THREAD_BASE()
	{
		//메모리가 해제할때 thread를 종료시킨다.
		//안전안전~~
		dispose_os_thread();
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	long THREAD_BASE::get_thread_count(void)
	{
		return thread_counter;
	}


	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	bool THREAD_BASE::thread_run_internal(void)
	{
		bool result = false;
		// increase the thread count
		++thread_counter;

		set_status(RUNNING);
		result = do_work();
		set_status(FINISHED);
		
//		dispose_os_thread();
		// decrease the thread count again
		--thread_counter;
		return result;
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	bool THREAD_BASE::create_os_thread(void)
	{
		set_status(CREATED);
		hThread = (HANDLE)_beginthreadex(nullptr, 0, os_thread_callback, this, 0, (unsigned int*)&id);
			
		bool result = hThread != NULL;
		if (result == false)
		{
			set_status(FAILED);
		}

		return result;

	}
	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	bool THREAD_BASE::dispose_os_thread(void)
	{
		if (status == RUNNING && id != 0)
		{
			if (hThread)
			{
				TerminateThread(hThread, static_cast<DWORD>(-1));
				CloseHandle(hThread);
				set_status(FINISHED);
				hThread = nullptr;
				id = 0;
			}
		}
		return true;
	}


	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	unsigned int WINAPI THREAD_BASE::os_thread_callback(void* parameter)
	{
		THREAD_BASE* thread = static_cast< THREAD_BASE* >(parameter);
		return thread->thread_run_internal();
	}

	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	void THREAD_BASE::run(void)
	{
		create_os_thread();
	}


	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	bool THREAD_BASE::end(void)
	{
		
		if (status != FINISHED && id != 0)
		{
			// 가급적이면 join으로 종료 해주세용~ http://support.microsoft.com/kb/136885/ko/
			DWORD result;
			do
			{
				result = MsgWaitForMultipleObjects(1, &hThread, false, 100, QS_ALLINPUT);
				//QS_ALLINPUT : 모든 메세지를 que에~~
				if (result == WAIT_OBJECT_0 + 1)
				{
					// TODO:  must handle WM_QUIT; see Raymond's blog for details
					//result 값
					//WAIT_OBJECT_0 : thread 완료
					//WAIT_IO_COMPLETION : User mode APC queued
					//WAIT_FAILED : wait 실패
					//메세지 펌핑
					MSG message;
					BOOL got_message = PeekMessage(&message, nullptr, 0, 0, PM_REMOVE);
					if (got_message)
					{
						TranslateMessage(&message);
						DispatchMessage(&message);
					}
				}
			} while (result == WAIT_OBJECT_0 + 1);
			return (result == WAIT_OBJECT_0);
		}
		return true;
	}

}