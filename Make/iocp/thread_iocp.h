// --------------------------------------------------------------------------
// 2014-06-16�� ����. youn-sung.
// 
// --------------------------------------------------------------------------
#ifndef _THREAD_IOCP
#define _THREAD_IOCP
#include <Windows.h>
#include <string>   // FOR basic_string
using namespace std;     // FOR basic_string
typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstring;

//job thread������ �ҷ��� �̰͵� thread�� ������ ���� �����ϴ�. �׳� �ᵵ ������..

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

		//thread ���� �մϴ�.
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