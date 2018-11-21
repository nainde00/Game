// --------------------------------------------------------------------------
// 2014-06-17�� ����. youn-sung.
// 
// --------------------------------------------------------------------------
#ifndef _THREAD_BASE
#define _THREAD_BASE
#include <Windows.h>
//���߿� header�� ���� ���� �����ϴ°��� ����.
#include <string>   // FOR basic_string

#include "../thread/atomic_counter.h"
using namespace std;     // FOR basic_string
typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstring;

//����� ���� YS_IOCP��� ���ӽ����̽��� ��������� �̰� thread�� IOCP�� ���°� �ƴմϴ�..
//�� ����ٰ� namespace�� �ٲٴ��� �ؾ߰ڳ׿�.
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