
// --------------------------------------------------------------------------
// cpp.
//
// --------------------------------------------------------------------------
#include "job_manager.h"
#include "thread_iocp.h"
#include "item_base.h"

namespace YS_THREAD
{
	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	JOB_MANAGER::JOB_MANAGER()
	{

	}
	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	JOB_MANAGER::~JOB_MANAGER()
	{
		job_shutdown();

	}
	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	void JOB_MANAGER::job_shutdown()
	{
		for (JOB_THREAD_IOCP_MAP::iterator iter = job_iocp_map.begin(); iter != job_iocp_map.end();  ++iter)
		{
			iter->second->iocp_end();
			delete(iter->second);
		}
		job_iocp_map.clear();
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	void JOB_MANAGER::create_workers(int thread_id, tstring _thread_name)
	{
		JOB_THREAD_IOCP_MAP::iterator iter = job_iocp_map.find(thread_id);
		if (iter == job_iocp_map.end())
		{
			job_iocp_map[thread_id] = new THREAD_IOCP();
			job_iocp_map[thread_id]->create_workers(_thread_name);
		}
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	void JOB_MANAGER::run_job(ITEM_BASE& item, int thread_id)
	{

		JOB_THREAD_IOCP_MAP::iterator iter = job_iocp_map.find(thread_id);
		if (iter == job_iocp_map.end())
		{
			//셋팅되지 않는 thread_id.
			return;
		}
		else
		{
			iter->second->run_item(&item);
		}
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	void JOB_MANAGER::rerun_job(ITEM_BASE& item, int thread_id)
	{

		JOB_THREAD_IOCP_MAP::iterator iter = job_iocp_map.find(thread_id);
		if (iter == job_iocp_map.end())
		{
			//셋팅되지 않는 thread_id.
			return;
		}
		else
		{
			item.rerun(*(iter->second));
		}
	}
	
	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	void JOB_MANAGER::pause_workers(int thread_id)
	{
		JOB_THREAD_IOCP_MAP::iterator iter = job_iocp_map.find(thread_id);
		if (iter == job_iocp_map.end())
		{
			//셋팅되지 않는 thread_id.
			return;
		}
		else
		{
			iter->second->pause_thread();
		}

	}


	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	void JOB_MANAGER::resume_thread(int thread_id)
	{
		JOB_THREAD_IOCP_MAP::iterator iter = job_iocp_map.find(thread_id);
		if (iter == job_iocp_map.end())
		{
			//셋팅되지 않는 thread_id.
			return;
		}
		else
		{
			iter->second->resume_thread();
		}

	}



}