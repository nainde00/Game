
// --------------------------------------------------------------------------
// cpp.
//
// --------------------------------------------------------------------------

#include "item_base.h"
#include "job_thread.h"

namespace YS_THREAD
{
	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	void ITEM_BASE::do_job_item()
	{
		CMuetx::Owner Lock(m_iocp_lock);
		if (can_run())
		{
			on_change_status(STATUS_RUNNING);

			do_work();

			on_change_status(STATUS_FINISHED);
		}
	}

	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	ITEM_BASE::STATUS ITEM_BASE::get_status()
	{
		CMuetx::Owner Lock(m_iocp_lock);
		return status;
	}

	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	void ITEM_BASE::reset()
	{
		bool was_reset = false;
		rerun_on_completion = false;

		if ((status != STATUS_RUNNING) && (status != STATUS_QUEUED))
		{
			on_change_status(STATUS_IDLE);
			was_reset = true;
		}

		if (was_reset)
		{
			on_change_status(STATUS_IDLE);
		}
	}

	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	void ITEM_BASE::rerun(THREAD_IOCP& thread_iocp)
	{
		CMuetx::Owner Lock(m_iocp_lock);
		if (status == STATUS_RUNNING || status == STATUS_QUEUED)
		{
			//이미 돌고있으면 옵션값만 바꿈.
			request_rerun_on_completion();
		}
		else
		{
			reset();
			thread_iocp.run_item(this);
		}

	}
}
