
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
						//item�� ��ϵ� do_work�� �����ŵ�ϴ�.
						//���� item_base�� timer code�� ��������� job timer code�� �Ǵ°���...
						_tprintf(_T(" ���� thread�̸���.. : %s\n"), get_name().c_str());
						item_base->do_job_item();
						{
							//���� �����ϴºκ��̶� thread�� ��ȣ�� �ʿ���~~.
							CMuetx::Owner Lock(item_base->m_iocp_lock);
							if (item_base->rerun_on_completion)
							{
								//�ٽ� ����ҋ�~~~
								if (item_base->status == ITEM_BASE::STATUS_FINISHED)
								{
									//do_work()�۾��� ��������..~~~
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