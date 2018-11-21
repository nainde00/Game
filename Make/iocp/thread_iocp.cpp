
// --------------------------------------------------------------------------
// cpp.
//
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------
#include "thread_iocp.h"
#include "item_base.h"
#include "job_thread.h"
#include "../thread/thread_base.h"

namespace YS_THREAD
{
	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	THREAD_IOCP::THREAD_IOCP()
	{
		create_completion_port();
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	THREAD_IOCP::~THREAD_IOCP()
	{
	
	}

	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	void THREAD_IOCP::iocp_end()
	{
		CloseHandle(static_cast<HANDLE>(completion_port));
		if (job_thread)
		{
			job_thread->pause_thread();
			job_thread->end();
			delete(job_thread);
		}
	}

	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	void THREAD_IOCP::create_completion_port()
	{
		completion_port = static_cast<IOCP_HANDLE>(CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0));
	}

	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	bool THREAD_IOCP::post_job(ITEM_BASE& item)
	{
		static const int MAX_TRIES = 128;

		bool result = FALSE;
		int try_count = 0;
		while (!result && (try_count++ < MAX_TRIES))
		{
			result = (PostQueuedCompletionStatus(completion_port, 0, reinterpret_cast<ULONG_PTR>(item.get_referenced_object()), nullptr) != 0);

			if (!result)
			{
				Sleep(10);
			}
		}
		return result;
	}

	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	bool THREAD_IOCP::get_job(ITEM_BASE*& item, LPOVERLAPPED& overlapped_ptr)
	{
		DWORD transferred;
		ITEM_BASE* item_out;

		bool result = (GetQueuedCompletionStatus(completion_port, &transferred, (PULONG_PTR)(&item_out), &overlapped_ptr, INFINITE) != 0);

		if (result)
		{
			item = item_out;
		}
		else
		{
			item = nullptr;
		}

		return result;
	}

	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	void THREAD_IOCP::run_item(ITEM_BASE* item)
	{
		if (item == nullptr)
		{
			return;
		}
		{
			CMuetx::Owner Lock(item->m_iocp_lock);


			if (item->status == ITEM_BASE::STATUS_IDLE)
			{
				//item base 가 초기화 되어있으면.
				item->on_change_status(ITEM_BASE::STATUS_QUEUED);
				post_job(*item);
			}
		}
	}

	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	void THREAD_IOCP::create_workers(tstring _name)
	{
		job_thread = new JOB_THREAD(this, _name);
		job_thread->run();
	}
	
	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	void THREAD_IOCP::pause_thread()
	{
		if (job_thread != nullptr)
		{
			job_thread->pause_thread();
		}
	}

	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	void THREAD_IOCP::resume_thread()
	{
		if (job_thread != nullptr)
		{
			job_thread->resume_thread();
		}


	}

}
