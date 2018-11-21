
// --------------------------------------------------------------------------
// cpp.
//
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------
#include "exception_manager.h"
#include "exception_handler.h"

namespace YS_DUMP
{
	// --------------------------------------------------------------------------
	//!
	// --------------------------------------------------------------------------
	EXCEPTION_MANAGER::EXCEPTION_MANAGER()
	{
		exception_handler = NULL;
	}

	// --------------------------------------------------------------------------
	//!
	// --------------------------------------------------------------------------
	EXCEPTION_MANAGER::~EXCEPTION_MANAGER()
	{
		if (exception_handler != NULL)
		{
			delete exception_handler;
		}
	}

	// --------------------------------------------------------------------------
	//!
	// --------------------------------------------------------------------------
	void EXCEPTION_MANAGER::set_exception_handler(EXCEPTION_HANDLER* _exception_handler)
	{
		CMuetx::Owner Lock(m_MutexLock);
		if (exception_handler != nullptr)
		{
			delete exception_handler;
		}
		exception_handler = _exception_handler;
	}

	// --------------------------------------------------------------------------
	//!
	// --------------------------------------------------------------------------
	void EXCEPTION_MANAGER::running_make_mini_dump(EXCEPTION_POINTERS* exception_data)
	{
		CMuetx::Owner Lock(m_MutexLock);

		if (exception_handler != nullptr)
		{
			exception_handler->set_windows_exception_data(exception_data);
			exception_handler->make_miniudump_log_file();
			exception_handler->make_minidump_file();
			//만약 winapi라면 여기다가 MessageBox를 호출하세요.
			
		}
	}

}