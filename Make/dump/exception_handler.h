// --------------------------------------------------------------------------
// 2014-06-12¿œ ¡¶¿€. youn-sung.
// 
// --------------------------------------------------------------------------
#pragma once

#ifndef EXCEPTION_HANDLER_H
#define EXCEPTION_HANDLER_H

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------
#include <windows.h>
#include <dbghelp.h>
#include "../Mutex.h"
#include <string>   // FOR basic_string
using namespace std;     // FOR basic_string

typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstring;
// --------------------------------------------------------------------------

namespace YS_DUMP
{
	// --------------------------------------------------------------------------
	// Required Classes
	// --------------------------------------------------------------------------

	// --------------------------------------------------------------------------
	//! The mini dump types that are supported by the exception handler.
	// --------------------------------------------------------------------------

	enum MINI_DUMP_TYPE
	{
		MINI_DUMP_NORMAL,				//!< A normal mini dump.
		MINI_DUMP_WITH_DATA_SECTIONS,	//!< A mini dump with data sections.
		MINI_DUMP_WITH_FULL_MEMORY		//!< A mini dump with full memory.
	};

	class EXCEPTION_HANDLER
	{
	public:
		EXCEPTION_HANDLER(const TCHAR* filename, const MINI_DUMP_TYPE dump_type = MINI_DUMP_WITH_DATA_SECTIONS);
		~EXCEPTION_HANDLER() {}

		void initialise_procedure_pointers(void);

		void set_windows_exception_data(_EXCEPTION_POINTERS* _windows_exception_data) { windows_exception_data = _windows_exception_data; }
		void get_back_trace(const HANDLE thread, tstring& exception_trace_string, CONTEXT& context) const;
		int make_minidump_file();
		tstring make_miniudump_log_file();

		void initialise_frame(const CONTEXT& context, STACKFRAME64& frame_out) const;

		BOOL get_next_frame(HANDLE processed, HANDLE thread, CONTEXT& context, STACKFRAME64& frame) const;
		tstring get_routine_name(HANDLE process, const DWORD64 dwaddress) const;
		tstring get_routine_filename_and_linenumber(HANDLE process, const DWORD64 dwaddress, int& line_number) const;

	private:
		_EXCEPTION_POINTERS*	windows_exception_data;
		TCHAR			m_file_name[100];
		MINI_DUMP_TYPE	m_dump_type;
		mutable CMuetx	m_Exception_Lock;
		DWORD			m_dwSymOptions;
	};
}

#endif // EXCEPTION_HANDLER_BASE_H

