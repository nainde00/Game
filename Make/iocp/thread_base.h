// --------------------------------------------------------------------------
// 2014-06-17일 제작. youn-sung.
// 
// --------------------------------------------------------------------------
#ifndef _THREAD_BASE
#define _THREAD_BASE
#include <Windows.h>
//나중에 header를 따로 만들어서 관리하는것이 좋음.
#include <string>   // FOR basic_string

#include "../thread/atomic_counter.h"
using namespace std;     // FOR basic_string
typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstring;

//만들다 보니 YS_IOCP라고 네임스페이스를 명명했지만 이건 thread라 IOCP에 쓰는게 아닙니다..
//흠 만들다가 namespace를 바꾸던가 해야겠네요.
namespace YS_IOCP
{
	class THREAD_BASE
	{
	public:
		static long	get_thread_count(void);

		enum STATUS
		{
			NOT_STARTED,
			CREATED,
			RUNNING,
			FINISHED,
			FAILED
		};
		void set_status(STATUS new_status) { status = new_status; }
		virtual void do_work(void) = 0;


	private:
		STATUS status;
		tstring name; // thread name;
		static YS_ATOMIC::ATOMIC_COUNTER thread_counter;
	};
}

#endif //_THREAD_BASE