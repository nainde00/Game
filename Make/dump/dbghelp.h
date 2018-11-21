#include <poppack.h>
#include <pshpack8.h>
#include <WinDef.h>
//
// Full user-mode dump creation.
//

typedef BOOL(WINAPI *PDBGHELP_CREATE_USER_DUMP_CALLBACK)(
	__in DWORD DataType,
	__in PVOID* Data,
	__out LPDWORD DataLength,
	__in_opt PVOID UserData
	);

BOOL
WINAPI
DbgHelpCreateUserDump(
__in_opt LPCSTR FileName,
__in PDBGHELP_CREATE_USER_DUMP_CALLBACK Callback,
__in_opt PVOID UserData
);

BOOL
WINAPI
DbgHelpCreateUserDumpW(
__in_opt LPCWSTR FileName,
__in PDBGHELP_CREATE_USER_DUMP_CALLBACK Callback,
__in_opt PVOID UserData
);
