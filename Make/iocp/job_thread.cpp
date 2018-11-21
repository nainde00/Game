
// --------------------------------------------------------------------------
// cpp.
//
// --------------------------------------------------------------------------

#include "job_thread.h"
#include "item_base.h"
#include <tchar.h>

namespace YS_THREAD
{
	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	JOB_THREAD::JOB_THREAD(THREAD_IOCP* thread_iocp, tstring _name) : controller(thread_iocp), pause_event(false), THREAD_BASE(_name)
	{

	}

	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	JOB_THREAD::~JOB_THREAD()
	{

	}

	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	void JOB_THREAD::pause_thread()
	{
		pause_event = true;
	}

	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	void JOB_THREAD::resume_thread()
	{
		pause_event = false;
	}

	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	bool JOB_THREAD::do_work()
	{
		bool thread_terminated = false;
		ITEM_BASE* item_base = nullptr;
		LPOVERLAPPED overlap;

		while (!thread_terminated)
		{
			if (pause_event == true)
			{ 
				thread_terminated = true;
				continue;
			}

			if (controller != nullptr)
			{
				controller->get_job(item_base, overlap);
				if (item_base != nullptr)
				{
					if (item_base->termination_item)
					{
						thread_terminated = true;
					}
					else
					{
						//item에 등록된 do_work를 실행시킵니다.
						//만약 item_base에 timer code를 집어넣으면 job timer code가 되는것임...
						_tprintf(_T(" 여기 thread이름은.. : %s\n"), get_name().c_str());
						item_base->do_job_item();
						{
							//새로 셋팅하는부분이라 thread의 보호가 필요함~~.
							CMuetx::Owner Lock(item_base->m_iocp_lock);
							if (item_base->rerun_on_completion)
							{
								//다시 사용할떄~~~
								if (item_base->status == ITEM_BASE::STATUS_FINISHED)
								{
									//do_work()작업이 끝났으면..~~~
									item_base->reset();
									controller->run_item(item_base);
								}
							}
						}
					}
				}
				item_base = nullptr;
			}
		}
		return true;
	}
	


}