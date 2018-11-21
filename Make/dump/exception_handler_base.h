// --------------------------------------------------------------------------
// 2014-06-12¿œ ¡¶¿€. youn-sung.
// 
// --------------------------------------------------------------------------
#pragma once

#ifndef EXCEPTION_HANDLER_BASE_H
#define EXCEPTION_HANDLER_BASE_H

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------
#include <windows.h>
#include <dbghelp.h>

namespace YS_DUMP
{
	// --------------------------------------------------------------------------
	// Required Classes
	// --------------------------------------------------------------------------
	class EXCEPTION_HANDLER; 
	class EXCEPTION_HANDLER_BASE
	{
	public:
		EXCEPTION_HANDLER_BASE();
		~EXCEPTION_HANDLER_BASE() {}
		static void start_and_set_handler(EXCEPTION_HANDLER* new_exception_handler);
		static void	handle_windows_exception_without_exception_data(void);
	private:
		static long	WINAPI handle_windows_exception(EXCEPTION_POINTERS* exception_data);


		_EXCEPTION_POINTERS*	windows_exception_data;
	};
}

#endif // EXCEPTION_HANDLER_BASE_H

