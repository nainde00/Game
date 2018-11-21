// --------------------------------------------------------------------------
// 2014-06-16일 제작. youn-sung.
// 
// --------------------------------------------------------------------------
#ifndef _THREAD_IOCP
#define _THREAD_IOCP
#include <Windows.h>
#include <string>   // FOR basic_string
using namespace std;     // FOR basic_string
typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstring;

//job thread관리를 할려면 이것도 thread를 돌리는 편이 좋습니다. 그냥 써도 되지만..

namespace YS_THREAD
{
	// --------------------------------------------------------------------------
	// Required Classes
	// --------------------------------------------------------------------------
	class ITEM_BASE;
	class JOB_THREAD;

	class THREAD_IOCP
	{
	public:
		THREAD_IOCP();
		~THREAD_IOCP();

		bool post_job(ITEM_BASE& item);
		bool get_job(ITEM_BASE*& item, LPOVERLAPPED& overlapped_ptr);
		void run_item(ITEM_BASE* item);

		//thread 시작 합니다.
		void create_workers(tstring _name);
		void pause_thread();
		void resume_thread();

		void iocp_end();

	private:
		typedef void* IOCP_HANDLE;
		void create_completion_port();
		IOCP_HANDLE completion_port;
		JOB_THREAD* job_thread;

	};


}


#endif