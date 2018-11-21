// --------------------------------------------------------------------------
// 2014-06-12¿œ ¡¶¿€. youn-sung.
// 
// --------------------------------------------------------------------------
#pragma once

#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------
#include <map>
#include <tchar.h>
#include "thread_iocp.h"
#include "../Singleton/C11_Singleton.h"

using namespace std;
typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstring;
// --------------------------------------------------------------------------

namespace YS_THREAD 
{
	// --------------------------------------------------------------------------
	// Required Classes
	// --------------------------------------------------------------------------
	class THREAD_IOCP;
	class ITEM_BASE;
	class JOB_MANAGER : public YS_LIB::C11_SINGLETON < JOB_MANAGER >
	{

	public:
		enum THREAD_ID
		{
			FIRST_THRAED = 1,
			SECOND_THRAED = 2,
		};
		JOB_MANAGER();
		~JOB_MANAGER();

		void create_workers(int thread_id, tstring _thread_name);

		void run_job(ITEM_BASE& item, int thread_id);
		void rerun_job(ITEM_BASE& item, int thread_id);

		void pause_workers(int thread_id);
		void resume_thread(int thread_id);
		
		void job_shutdown();

	private:
		typedef map<int, THREAD_IOCP*> JOB_THREAD_IOCP_MAP;

		JOB_THREAD_IOCP_MAP job_iocp_map;
	};

#define g_JOB_MANAGER JOB_MANAGER::get_Instance()

}

#endif //JOB_HANDLER_H