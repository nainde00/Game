// --------------------------------------------------------------------------
// 2014-06-16일 제작. youn-sung.
// 기본 class입니다. iocp에 사용됩니다. iocp에 쓸 클래스는 현 함수를 상속해서 사용해주세요~
// --------------------------------------------------------------------------
#ifndef _ITEM_BASE
#define _ITEM_BASE

//#include <WinDef.h>
#include "../Mutex.h"

namespace YS_THREAD
{
	class ITEM_BASE
	{
		friend class ITEM_BASE_UTILITES;
		friend class JOB_THREAD;
		friend class THREAD_IOCP;
	public:
		//templete 로 만들어서 써도 되지만. 그건 나중에 설계할떄 적용토록하겠음.
		//templete로 만들려면 class를 하나 더만들어서 상속해야됨. 일단 이건 여기까지..
		typedef ITEM_BASE*				OBJECT_PTR;
		typedef const ITEM_BASE*		CONST_OBJECT_PTR;
		enum STATUS
		{
			STATUS_IDLE,
			STATUS_QUEUED,
			STATUS_RUNNING,
			STATUS_FINISHED
		};


		ITEM_BASE() : termination_item(false), status(STATUS_IDLE), rerun_on_completion(false), stop_requested(false){}
		virtual ~ITEM_BASE() {}

		bool is_requested_stop() const															{ return stop_requested; }
		void do_job_item(); 
		void on_change_status(STATUS new_status)	{ status = new_status; }
		STATUS get_status();

		void reset();
		bool can_run() const { return !is_requested_stop(); }
		OBJECT_PTR get_referenced_object(void)				{ return this; }
		CONST_OBJECT_PTR get_referenced_object(void) const 	{ return this; }
		bool termination_item;
		void request_rerun_on_completion() { rerun_on_completion = true; }
		void rerun(THREAD_IOCP& thread_iocp);

		virtual void do_work() = 0; //실질적으로 이놈 호출할끄임..

	private:
		
		bool stop_requested;
		STATUS status;
		bool rerun_on_completion;
		CMuetx	m_iocp_lock;
	};

	class ITEM_BASE_UTILITES
	{
	public:
		static void requested_stop(ITEM_BASE& item)	{ item.stop_requested = true; }
	};
}



#endif //_ITEM_BASE