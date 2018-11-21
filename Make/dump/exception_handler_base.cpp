// --------------------------------------------------------------------------
// cpp.
//
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------
#include "exception_handler_base.h"
#include "exception_manager.h"

namespace YS_DUMP
{

	// --------------------------------------------------------------------------
	//!
	// --------------------------------------------------------------------------
	EXCEPTION_HANDLER_BASE::EXCEPTION_HANDLER_BASE()
	{
		
	}

	// --------------------------------------------------------------------------
	//!
	// --------------------------------------------------------------------------
	void EXCEPTION_HANDLER_BASE::start_and_set_handler(EXCEPTION_HANDLER* new_exception_handler)
	{
		g_EXCEPTION_MANAGER.set_exception_handler(new_exception_handler);
		SetUnhandledExceptionFilter(&handle_windows_exception);
	}

	// --------------------------------------------------------------------------
	//!
	// ----------7----------------------------------------------------------------
	void EXCEPTION_HANDLER_BASE::handle_windows_exception_without_exception_data(void)
	{
		//덤프를 남기고 싶은곳에 남기세용~~~. :)
		EXCEPTION_HANDLER* exception_handler = g_EXCEPTION_MANAGER.get_exception_handler();
		if (exception_handler != nullptr)
		{
			g_EXCEPTION_MANAGER.running_make_mini_dump(nullptr);
		}
	}

	// --------------------------------------------------------------------------
	//!
	// --------------------------------------------------------------------------
	long EXCEPTION_HANDLER_BASE::handle_windows_exception(EXCEPTION_POINTERS* exception_data)
	{
		//선행으로 exception_handler를 설정해주시기 바랍니다.
		//그냥 매니저에 설정을 해놓을까 ;;
		//그러자.~

		g_EXCEPTION_MANAGER.running_make_mini_dump(exception_data);

		return EXCEPTION_CONTINUE_SEARCH;
	}
}

