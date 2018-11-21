// --------------------------------------------------------------------------
// 2014-06-16�� ����. youn-sung.
// JOB thread�Դϴ�. iocp�� �����͸� �����ð̴ϴ�.
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
