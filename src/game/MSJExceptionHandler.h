/*
===============================================================================

	MSJExceptionHandler.h

	The crash reporter: an unhandled exception filter that appends a report
	(exception, registers, call stack) to the executable's name with the
	extension RPT. The code is Matt Pietrek's MSJExceptionHandler ("Under
	the Hood", Microsoft Systems Journal, April 1997 and April 1998): its
	banner and format strings, the RPT extension and its static members are
	the article's. No string names the class or its functions: the names are
	the article's.

	Totally Games added the time of the crash and the application's time
	stamp to the report, and replaced the article's statically bound
	IMAGEHLP functions with TGSymbolEngine, which loads DBGHELP.DLL at run
	time and adds line numbers and the frames' first parameters to the stack
	walk.

===============================================================================
*/

#ifndef __MSJEXCEPTIONHANDLER_H__
#define __MSJEXCEPTIONHANDLER_H__

#include <windows.h>
#include <tchar.h>
#include <imagehlp.h>

/*
===============================================================================

	TGSymbolEngine

	The DBGHELP.DLL functions, looked up by name. The class name comes from
	the report's "Couldn't cleanup TGSymbolEngine"; the members are named
	after the functions they hold. 0x70 bytes; vtable 0x896AC8.

===============================================================================
*/

typedef BOOL ( __stdcall *SymInitializeProc )( HANDLE, LPSTR, BOOL );
typedef BOOL ( __stdcall *SymCleanupProc )( HANDLE );
typedef BOOL ( __stdcall *StackWalkProc )( DWORD, HANDLE, HANDLE, LPSTACKFRAME, LPVOID, PREAD_PROCESS_MEMORY_ROUTINE,
	PFUNCTION_TABLE_ACCESS_ROUTINE, PGET_MODULE_BASE_ROUTINE, PTRANSLATE_ADDRESS_ROUTINE );
typedef DWORD ( __stdcall *SymGetModuleBaseProc )( HANDLE, DWORD );
typedef BOOL ( __stdcall *SymGetSymFromAddrProc )( HANDLE, DWORD, PDWORD, PIMAGEHLP_SYMBOL );
typedef BOOL ( __stdcall *SymGetLineFromAddrProc )( HANDLE, DWORD, PDWORD, PIMAGEHLP_LINE );
typedef DWORD ( __stdcall *SymGetOptionsProc )( void );
typedef DWORD ( __stdcall *SymSetOptionsProc )( DWORD );
typedef DWORD ( __stdcall *GetTimestampForLoadedLibraryProc )( HMODULE );
typedef BOOL ( __stdcall *SymEnumerateModulesProc )( HANDLE, PSYM_ENUMMODULES_CALLBACK, PVOID );
typedef DWORD ( __stdcall *SymLoadModuleProc )( HANDLE, HANDLE, PSTR, PSTR, DWORD, DWORD );
typedef BOOL ( __stdcall *EnumerateLoadedModulesProc )( HANDLE, PENUMLOADED_MODULES_CALLBACK, PVOID );
typedef BOOL ( __stdcall *SymUnloadModuleProc )( HANDLE, DWORD );
typedef BOOL ( __stdcall *SymGetModuleInfoProc )( HANDLE, DWORD, PIMAGEHLP_MODULE );
typedef BOOL ( __stdcall *SymEnumerateSymbolsProc )( HANDLE, DWORD, PSYM_ENUMSYMBOLS_CALLBACK, PVOID );
typedef BOOL ( __stdcall *SymGetSymFromNameProc )( HANDLE, LPSTR, PIMAGEHLP_SYMBOL );
typedef BOOL ( __stdcall *SymGetSymNextProc )( HANDLE, PIMAGEHLP_SYMBOL );
typedef BOOL ( __stdcall *SymGetSymPrevProc )( HANDLE, PIMAGEHLP_SYMBOL );
typedef BOOL ( __stdcall *SymGetLineFromNameProc )( HANDLE, LPSTR, LPSTR, DWORD, PLONG, PIMAGEHLP_LINE );
typedef BOOL ( __stdcall *SymGetLineNextProc )( HANDLE, PIMAGEHLP_LINE );
typedef BOOL ( __stdcall *SymGetLinePrevProc )( HANDLE, PIMAGEHLP_LINE );
typedef BOOL ( __stdcall *SymMatchFileNameProc )( LPSTR, LPSTR, LPSTR *, LPSTR * );
typedef BOOL ( __stdcall *SymGetSearchPathProc )( HANDLE, LPSTR, DWORD );
typedef BOOL ( __stdcall *SymSetSearchPathProc )( HANDLE, LPSTR );
typedef BOOL ( __stdcall *SymRegisterCallbackProc )( HANDLE, PSYMBOL_REGISTERED_CALLBACK, PVOID );

// FUNCTION: STBC 0x007177b0 ??_GTGSymbolEngine@@UAEPAXI@Z
class TGSymbolEngine {
public:
	// FUNCTION: STBC 0x007177a0 ??1TGSymbolEngine@@UAE@XZ
	virtual					~TGSymbolEngine( void ) {}

	BOOL					LoadFunctions( void );

	SymInitializeProc					symInitialize;					// 0x04
	SymCleanupProc						symCleanup;						// 0x08
	PFUNCTION_TABLE_ACCESS_ROUTINE		symFunctionTableAccess;			// 0x0C
	PGET_MODULE_BASE_ROUTINE			symGetModuleBase;				// 0x10
	SymGetSymFromAddrProc				symGetSymFromAddr;				// 0x14
	SymGetLineFromAddrProc				symGetLineFromAddr;				// 0x18
	SymGetOptionsProc					symGetOptions;					// 0x1C
	SymSetOptionsProc					symSetOptions;					// 0x20
	GetTimestampForLoadedLibraryProc	getTimestampForLoadedLibrary;	// 0x24
	SymEnumerateModulesProc				symEnumerateModules;			// 0x28
	SymLoadModuleProc					symLoadModule;					// 0x2C
	EnumerateLoadedModulesProc			enumerateLoadedModules;			// 0x30
	SymUnloadModuleProc					symUnloadModule;				// 0x34
	SymGetModuleInfoProc				symGetModuleInfo;				// 0x38
	SymEnumerateSymbolsProc				symEnumerateSymbols;			// 0x3C
	SymGetSymFromNameProc				symGetSymFromName;				// 0x40
	SymGetSymNextProc					symGetSymNext;					// 0x44
	SymGetSymPrevProc					symGetSymPrev;					// 0x48
	SymGetLineFromNameProc				symGetLineFromName;				// 0x4C
	SymGetLineNextProc					symGetLineNext;					// 0x50
	SymGetLinePrevProc					symGetLinePrev;					// 0x54
	SymMatchFileNameProc				symMatchFileName;				// 0x58
	SymGetSearchPathProc				symGetSearchPath;				// 0x5C
	SymSetSearchPathProc				symSetSearchPath;				// 0x60
	SymRegisterCallbackProc				symRegisterCallback;			// 0x64
	StackWalkProc						stackWalk;						// 0x68
	HANDLE								process;						// 0x6C the process whose symbols are read
};

/*
===============================================================================

	MSJExceptionHandler

	No data members: its one instance, g_MSJExceptionHandler, exists for its
	constructor, which installs the filter, and its destructor, which
	removes it. The state is static.

===============================================================================
*/

class MSJExceptionHandler {
public:
							MSJExceptionHandler( void );
							~MSJExceptionHandler( void );

	void					SetLogFileName( PTSTR pszLogFileName );

private:
	static LONG WINAPI		MSJUnhandledExceptionFilter( PEXCEPTION_POINTERS pExceptionInfo );
	static void				GenerateExceptionReport( PEXCEPTION_POINTERS pExceptionInfo );
	static LPTSTR			GetExceptionString( DWORD dwCode );
	static BOOL				GetLogicalAddress( PVOID addr, PTSTR szModule, DWORD len, DWORD &section, DWORD &offset );
	static void				IntelStackWalk( PCONTEXT pContext );
	static void				ImagehlpStackWalk( PCONTEXT pContext, TGSymbolEngine *symbolEngine, HANDLE process );
	static int __cdecl		_tprintf( const TCHAR *format, ... );

	static TCHAR			logFileName[MAX_PATH];
	static LPTOP_LEVEL_EXCEPTION_FILTER previousFilter;
	static HANDLE			reportFile;
};

extern MSJExceptionHandler g_MSJExceptionHandler;

#endif /* !__MSJEXCEPTIONHANDLER_H__ */
