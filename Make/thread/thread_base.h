// --------------------------------------------------------------------------
// 2014-06-17일 제작. youn-sung.
// 
// --------------------------------------------------------------------------
#ifndef _THREAD_BASE
#define _THREAD_BASE
#include <Windows.h>
//나중에 header를 따로 만들어서 관리하는것이 좋음.
#include <string>   // FOR basic_string

#include "atomic_counter.h"
using namespace std;     // FOR basic_string
typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstring;

namespace YS_THREAD
{
	class THREAD_BASE
	{
	public:
		static long	get_thread_count(void);
		virtual		~THREAD_BASE();

		enum STATUS
		{
			NOT_STARTED,
			CREATED,
			RUNNING,
			FINISHED,
			FAILED
		};

		THREAD_BASE(const tstring& name_in);

		void set_status(STATUS new_status) { status = new_status; }
		tstring get_name() { return name; }

		//thread 제어
		void	run(void);
		bool	end(void);

		//상속받은 thread를 사용하게 만듭니다.
		virtual bool do_work(void) = 0;
		
	protected:

		bool thread_run_internal(void);

		//thread 관련.
		bool						create_os_thread(void);
		bool						dispose_os_thread(void);
		static unsigned int WINAPI	os_thread_callback(void* parameter);

	private:

		size_t id;		//thread id
		HANDLE hThread;	// thread handle
		STATUS status;
		tstring name; // thread name;
		static ATOMIC_COUNTER thread_counter;
	};
}

#endif //_THREAD_BASE