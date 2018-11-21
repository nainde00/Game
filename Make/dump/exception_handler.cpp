
// --------------------------------------------------------------------------
// cpp.
//
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------
#include <TCHAR.H>
#include <string>
#include "exception_handler.h"

#define _MAX_STACK_TRACE_COUNT 100
#define _SKIP_STACK_TRACE_COUNT 0

namespace YS_DUMP
{
	// --------------------------------------------------------------------------
	//!
	// --------------------------------------------------------------------------
	MINIDUMP_TYPE convert_mini_dump_type(const MINI_DUMP_TYPE mini_dump_type)
	{
		switch (mini_dump_type)
		{
		case MINI_DUMP_WITH_DATA_SECTIONS:
			return MiniDumpWithDataSegs;

		case MINI_DUMP_WITH_FULL_MEMORY:
			return MiniDumpWithFullMemory;

		case MINI_DUMP_NORMAL:
		default:
			return MiniDumpNormal;
		}
	}

	// --------------------------------------------------------------------------
	//!
	// --------------------------------------------------------------------------
	EXCEPTION_HANDLER::EXCEPTION_HANDLER(const TCHAR* filename, const MINI_DUMP_TYPE dump_type)
	{
		//initialise_procedure_pointers();
		windows_exception_data = NULL;
		_tcscpy(m_file_name,  filename);
		m_dump_type = dump_type;

		m_dwSymOptions = DWORD(
			SymGetOptions() |		// use default settings
			SYMOPT_UNDNAME |		// un-decorate names if possible
			SYMOPT_LOAD_LINES |		// load line information
			SYMOPT_DEFERRED_LOADS	// only load debug symbols when necessary
			);
	}

	// --------------------------------------------------------------------------
	//!
	// --------------------------------------------------------------------------
	int EXCEPTION_HANDLER::make_minidump_file()
	{
		//문제가 생기면 이 함수를 탑니다.
		// set up minidump information structure
		MINIDUMP_EXCEPTION_INFORMATION minidump_exception;
		PMINIDUMP_EXCEPTION_INFORMATION pminidump_exception = nullptr;

		if (windows_exception_data)
		{
			minidump_exception.ThreadId = GetCurrentThreadId();
			minidump_exception.ExceptionPointers = windows_exception_data;
			minidump_exception.ClientPointers = true;
			pminidump_exception = &minidump_exception;
		}


		HANDLE file = CreateFileW((LPCTSTR)m_file_name, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (file != INVALID_HANDLE_VALUE)
		{
			const DWORD process_id = GetCurrentProcessId();
			const MINIDUMP_TYPE actual_dump_type = convert_mini_dump_type(m_dump_type);
			int result = (MiniDumpWriteDump(GetCurrentProcess(), process_id, file, actual_dump_type, pminidump_exception, nullptr, nullptr) == 1);
			CloseHandle(file);
			return result;
		}
		return 0;
	}

	// --------------------------------------------------------------------------
	//!
	// --------------------------------------------------------------------------
	tstring EXCEPTION_HANDLER::make_miniudump_log_file()
	{
		//call stack이니 뿌리던지 뭐하던지 마음대로 ;;
		//밖에서 쓸수있도록 return 시키자.
		tstring exception_trace_string;
		
		const HANDLE thread = GetCurrentThread();
		CONTEXT context;
		if (windows_exception_data == NULL)
		{
			//강제로 콜스텍 남길떄 사용합니다.
			memset(&context, 0, sizeof(CONTEXT));
#if _64bit
			RtlCaptureContext(&context);
#else
			// little bit of inline assembler to get the current context
			__asm
			{
			Label:
				mov[context.Ebp], ebp;
				mov[context.Esp], esp;
				mov eax, [Label];
				mov[context.Eip], eax;
			}
			//현재 이 위치의 exception를 가르킵니다.
#endif
		}
		else
		{
			memcpy(&context, windows_exception_data->ContextRecord, sizeof(CONTEXT));
		}

		get_back_trace(thread, exception_trace_string,context);

		DWORD dwWrite = 0;
		//나중에 변경하세요~~ 파일이름 관리하는건 간단함.
		TCHAR m_log_file_name[1024] = L"c:\\dump\\dump.log";
			
		HANDLE file = CreateFileW((LPCTSTR)(m_log_file_name), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		_tprintf(_T(" exception_trace_string : %s"), exception_trace_string.c_str());

		if (file != INVALID_HANDLE_VALUE)
		{
//				char bom[] = { 0xff, 0xfe };
//				WriteFile(file, bom, sizeof(bom), &dwWrite, 0);
			WriteFile(file, exception_trace_string.c_str(), _tcslen(exception_trace_string.c_str()) *sizeof(TCHAR), &dwWrite, 0);
			CloseHandle(file);
		}

		

		return exception_trace_string;
	}


	// --------------------------------------------------------------------------
	//!
	// --------------------------------------------------------------------------
	void EXCEPTION_HANDLER::get_back_trace(const HANDLE thread, tstring& exception_trace_string, CONTEXT& context) const
	{
		CMuetx::Owner Lock(m_Exception_Lock);
		HANDLE processed = GetCurrentProcess();

		SymInitializeW(processed, NULL, true);

		STACKFRAME64 frame;
		initialise_frame(context,frame);
		//문자 초기화
		exception_trace_string = L"DEBUG START";

		int depth_out = 0;

		
		DWORD64 displacement64 = 0;
		DWORD displacement = 0;
//		printf("에러코드를 잡아봅시다 : %d", GetLastError());

		


		const int total_depth = _MAX_STACK_TRACE_COUNT + _SKIP_STACK_TRACE_COUNT;
		while ((depth_out < total_depth) && get_next_frame(processed,thread,context, frame) == TRUE)
		{
			if (depth_out >= _SKIP_STACK_TRACE_COUNT)
			{
				printf("statck address 0x%p\n", &frame.AddrPC.Offset);
				TCHAR tline_number[32];
				int line_number;
				//심볼이 나를 괴롭힌다..
				
				printf("들어와야되는대. : %d \n", depth_out);
				exception_trace_string += L"\n\t";
				exception_trace_string += get_routine_name(processed,frame.AddrPC.Offset);
				exception_trace_string += L"( [";
				exception_trace_string += get_routine_filename_and_linenumber(processed, frame.AddrPC.Offset, line_number);
				exception_trace_string += L" ] : ";
				_itot(line_number, tline_number, 10);
				exception_trace_string += tline_number;
				exception_trace_string += L" )";
			}
			++depth_out;
		}
		SymCleanup(processed);
	}

	// --------------------------------------------------------------------------
	//!
	// --------------------------------------------------------------------------
	void EXCEPTION_HANDLER::initialise_frame(const CONTEXT& context, STACKFRAME64& frame_out) const
	{
		memset(&frame_out, 0, sizeof(frame_out));
#ifdef _64bit //64비트 준비
		frame_out.AddrPC.Offset = context.Rip;
		frame_out.AddrStack.Offset = context.Rsp;
		frame_out.AddrFrame.Offset = context.Rbp;
#else
		frame_out.AddrPC.Offset = context.Eip;
		frame_out.AddrStack.Offset = context.Esp;
		frame_out.AddrFrame.Offset = context.Ebp;
#endif

		frame_out.AddrPC.Mode = AddrModeFlat;
		frame_out.AddrStack.Mode = AddrModeFlat;
		frame_out.AddrFrame.Mode = AddrModeFlat;
		frame_out.AddrBStore.Mode = AddrModeFlat;
		frame_out.AddrReturn.Mode = AddrModeFlat;
	}

	// --------------------------------------------------------------------------
	//!
	// --------------------------------------------------------------------------
	BOOL EXCEPTION_HANDLER::get_next_frame(HANDLE processed, HANDLE thread, CONTEXT& context, STACKFRAME64& frame) const
	{

#ifdef _64bit //64비트 준비
		const DWORD machine_type = IMAGE_FILE_MACHINE_AMD64;
#else
		const DWORD machine_type = IMAGE_FILE_MACHINE_I386;
#endif
		BOOL is_ok = StackWalk64(machine_type, processed, thread, &frame, &context, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr);
		// StackWalk64 에러처리를 하세요~

		return is_ok;
	}

	// --------------------------------------------------------------------------
	//!
	// --------------------------------------------------------------------------
	tstring EXCEPTION_HANDLER::get_routine_name(HANDLE process, const DWORD64 dwaddress) const
	{
		tstring routine_name;
		const size_t symbol_size = sizeof(SYMBOL_INFOW) + ((MAX_SYM_NAME - 1) * sizeof(TCHAR));
		BYTE buffer[symbol_size];
		SYMBOL_INFOW* symbol_info = reinterpret_cast<SYMBOL_INFOW*>(buffer);

		memset(symbol_info, 0, symbol_size);

		symbol_info->MaxNameLen = MAX_SYM_NAME;
		symbol_info->SizeOfStruct = sizeof(SYMBOL_INFOW);
		DWORD64 displacement = 0;

		if (SymFromAddrW(process, dwaddress, &displacement, symbol_info))
		{
			routine_name = symbol_info->Name;
			_tprintf(_T(" line.FileName : %s"), routine_name);
		}
		return routine_name;
	}

	// --------------------------------------------------------------------------
	//!
	// --------------------------------------------------------------------------
	tstring EXCEPTION_HANDLER::get_routine_filename_and_linenumber(HANDLE process, const DWORD64 dwaddress, int& line_number) const
	{
		tstring name;

		IMAGEHLP_LINEW64 line;
		memset(&line, 0, sizeof(IMAGEHLP_LINEW64));
		line.SizeOfStruct = sizeof(IMAGEHLP_LINEW64);

		DWORD displacement = 0;
		line_number = 0;

		if (SymGetLineFromAddrW64(process, dwaddress, &displacement, &line))
		{
			name = line.FileName;
			line_number = line.LineNumber;
			_tprintf(_T(" line.FileName : %s"), name);
		}
		return name;
	}



	
	
}