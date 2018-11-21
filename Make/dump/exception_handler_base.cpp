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
		//������ ����� �������� ���⼼��~~~. :)
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
		//�������� exception_handler�� �������ֽñ� �ٶ��ϴ�.
		//�׳� �Ŵ����� ������ �س����� ;;
		//�׷���.~

		g_EXCEPTION_MANAGER.running_make_mini_dump(exception_data);

		return EXCEPTION_CONTINUE_SEARCH;
	}
}

