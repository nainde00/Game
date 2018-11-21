// --------------------------------------------------------------------------
// 
// 
// --------------------------------------------------------------------------

#pragma once

#ifndef EXCEPTION_HANDLER_MANAGER_WINDOWS_H
#define EXCEPTION_HANDLER_MANAGER_WINDOWS_H

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "sicore/generic/debugging/exception_handler_manager_base.h"
#include "sicore/generic/threads/sync_mutex.h"
#include <Windows.h>

// --------------------------------------------------------------------------

namespace SICORE
{
	// --------------------------------------------------------------------------
	// Windows-specific exception handler manager.
	// --------------------------------------------------------------------------

	class EXCEPTION_HANDLER_MANAGER_WINDOWS : public EXCEPTION_HANDLER_MANAGER_BASE
	{
	public:
		static void			set_exception_handler(EXCEPTION_HANDLER* new_exception_handler);
		static void			handle_windows_exception_without_exception_data(void);

	private:
		static long WINAPI	handle_windows_exception(EXCEPTION_POINTERS* exception_data);

		EXCEPTION_HANDLER_MANAGER_WINDOWS(void);
		~EXCEPTION_HANDLER_MANAGER_WINDOWS(void);

		EXCEPTION_HANDLER_MANAGER_WINDOWS(const EXCEPTION_HANDLER_MANAGER_WINDOWS& other);
		EXCEPTION_HANDLER_MANAGER_WINDOWS& operator=(const EXCEPTION_HANDLER_MANAGER_WINDOWS& other);

		static SYNC_MUTEX	exception_mutex;	//!< Mutex to prevent multiple threads processing exceptions at the same time
	};
}

// --------------------------------------------------------------------------

#endif // SICORE_EXCEPTION_HANDLER_MANAGER_WINDOWS_H
