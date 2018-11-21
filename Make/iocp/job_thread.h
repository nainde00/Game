// --------------------------------------------------------------------------
// 2014-06-16일 제작. youn-sung.
// JOB thread입니다. iocp의 데이터를 가져올겁니당.
// --------------------------------------------------------------------------
#ifndef _JOB_THREAD
#define _JOB_THREAD
// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------
#include "thread_iocp.h"
#include "../thread/thread_base.h"
namespace YS_THREAD
{
	class THREAD_IOCP;
	class JOB_THREAD : public THREAD_BASE
	{
	public:
		JOB_THREAD(THREAD_IOCP* thread_iocp,tstring _name);
		virtual ~JOB_THREAD();
		void pause_thread();
		void resume_thread();
		
	private:
		virtual bool do_work();


		THREAD_IOCP* controller;
		bool pause_event;
		
	};
}


#endif //_JOB_THREAD
