/*
===============================================================================

	MSJExceptionHandler.cpp

	Matt Pietrek's crash reporter (MSJ, April 1998), as Totally Games
	extended it; see MSJExceptionHandler.h.

===============================================================================
*/

#include <string.h>
#include <time.h>

#include "MSJExceptionHandler.h"

// The instance's construction and its destruction at exit.
// FUNCTION: STBC 0x00716a10 _$E22
// FUNCTION: STBC 0x00716a20 _$E19
// FUNCTION: STBC 0x00716a30 _$E21
// FUNCTION: STBC 0x00716a40 _$E20

// DATA: STBC 0x0099c15c
MSJExceptionHandler g_MSJExceptionHandler;

// DATA: STBC 0x0099c050
TCHAR MSJExceptionHandler::logFileName[MAX_PATH];
// DATA: STBC 0x0099c154
LPTOP_LEVEL_EXCEPTION_FILTER MSJExceptionHandler::previousFilter;
// DATA: STBC 0x0099c158
HANDLE MSJExceptionHandler::reportFile;

/*
================
MSJExceptionHandler::MSJExceptionHandler

Installs the filter and names the report after the executable, with the
extension RPT.
================
*/
// FUNCTION: STBC 0x00716a50
MSJExceptionHandler::MSJExceptionHandler( void ) {
	previousFilter = SetUnhandledExceptionFilter( MSJUnhandledExceptionFilter );

	GetModuleFileName( 0, logFileName, MAX_PATH );

	PTSTR pszDot = _tcsrchr( logFileName, _T( '.' ) );
	if ( pszDot ) {
		pszDot++;
		if ( _tcslen( pszDot ) >= 3 ) {
			_tcscpy( pszDot, _T( "RPT" ) );
		}
	}
}

/*
================
MSJExceptionHandler::~MSJExceptionHandler
================
*/
// FUNCTION: STBC 0x00716ad0
MSJExceptionHandler::~MSJExceptionHandler( void ) {
	SetUnhandledExceptionFilter( previousFilter );
}

/*
================
MSJExceptionHandler::SetLogFileName
================
*/
// FUNCTION: STBC 0x00716ae0
void MSJExceptionHandler::SetLogFileName( PTSTR pszLogFileName ) {
	_tcscpy( logFileName, pszLogFileName );
}

/*
================
MSJExceptionHandler::MSJUnhandledExceptionFilter

Appends the report, then passes the exception to the filter it replaced.
================
*/
// FUNCTION: STBC 0x00716b10
LONG WINAPI MSJExceptionHandler::MSJUnhandledExceptionFilter( PEXCEPTION_POINTERS pExceptionInfo ) {
	reportFile = CreateFile( logFileName, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH, 0 );

	if ( reportFile ) {
		SetFilePointer( reportFile, 0, 0, FILE_END );

		GenerateExceptionReport( pExceptionInfo );

		CloseHandle( reportFile );
		reportFile = 0;
	}

	if ( previousFilter ) {
		return previousFilter( pExceptionInfo );
	} else {
		return EXCEPTION_CONTINUE_SEARCH;
	}
}

/*
================
MSJExceptionHandler::GenerateExceptionReport

The report: the exception and where it happened, the registers, then the
call stack, walked with DBGHELP's symbols when they load and frame by frame
when they don't.
================
*/
// FUNCTION: STBC 0x00716b80
void MSJExceptionHandler::GenerateExceptionReport( PEXCEPTION_POINTERS pExceptionInfo ) {
	PEXCEPTION_RECORD pExceptionRecord = pExceptionInfo->ExceptionRecord;
	PCONTEXT pCtx = pExceptionInfo->ContextRecord;

	_tprintf( _T( "//=====================================================\n" ) );

	time_t now;
	time( &now );
	_tprintf( _T( "Current Time: %s" ), ctime( &now ) );

	_tprintf( _T( "Exception code: %08X %s\n" ), pExceptionRecord->ExceptionCode,
		GetExceptionString( pExceptionRecord->ExceptionCode ) );

	TCHAR szFaultingModule[MAX_PATH];
	DWORD section, offset;
	GetLogicalAddress( pExceptionRecord->ExceptionAddress, szFaultingModule, sizeof( szFaultingModule ), section, offset );

	_tprintf( _T( "Fault address:  %08X %02X:%08X %s\n" ), pExceptionRecord->ExceptionAddress, section, offset,
		szFaultingModule );

	_tprintf( _T( "\nRegisters:\n" ) );

	_tprintf( _T( "EAX:%08X\nEBX:%08X\nECX:%08X\nEDX:%08X\nESI:%08X\nEDI:%08X\n" ),
		pCtx->Eax, pCtx->Ebx, pCtx->Ecx, pCtx->Edx, pCtx->Esi, pCtx->Edi );

	_tprintf( _T( "CS:EIP:%04X:%08X\n" ), pCtx->SegCs, pCtx->Eip );
	_tprintf( _T( "SS:ESP:%04X:%08X  EBP:%08X\n" ), pCtx->SegSs, pCtx->Esp, pCtx->Ebp );
	_tprintf( _T( "DS:%04X  ES:%04X  FS:%04X  GS:%04X\n" ), pCtx->SegDs, pCtx->SegEs, pCtx->SegFs, pCtx->SegGs );
	_tprintf( _T( "Flags:%08X\n" ), pCtx->EFlags );

	HANDLE process = GetCurrentProcess();
	TGSymbolEngine symbolEngine;
	symbolEngine.process = process;

	if ( !symbolEngine.LoadFunctions() ) {
		_tprintf( _T( "Couldn't initialize symbol reader, no symbol info available\n" ) );
		IntelStackWalk( pCtx );
		return;
	}
	if ( !symbolEngine.symInitialize( process, NULL, FALSE ) ) {
		_tprintf( _T( "Couldn't initialize symbol reader, no symbol info available\n" ) );
		IntelStackWalk( pCtx );
		return;
	}

	symbolEngine.symSetOptions( symbolEngine.symGetOptions() | SYMOPT_LOAD_LINES | SYMOPT_OMAP_FIND_NEAREST );

	TCHAR szModule[MAX_PATH];
	GetModuleFileName( 0, szModule, MAX_PATH );
	if ( symbolEngine.symLoadModule( symbolEngine.process, 0, szModule, 0, 0, 0 ) ) {
		MEMORY_BASIC_INFORMATION mbi;
		if ( VirtualQuery( (PVOID)pCtx->Eip, &mbi, sizeof( mbi ) ) ) {
			time_t timestamp = symbolEngine.getTimestampForLoadedLibrary( (HMODULE)mbi.AllocationBase );
			_tprintf( _T( "\nApplication Timestamp: %s" ), ctime( &timestamp ) );
		}
		_tprintf( _T( "Doing stack walk\n" ) );
		ImagehlpStackWalk( pCtx, &symbolEngine, process );
	} else {
		_tprintf( _T( "Couldn't load module\n" ) );
	}

	if ( !symbolEngine.symCleanup( symbolEngine.process ) ) {
		_tprintf( _T( "Couldn't cleanup TGSymbolEngine\n" ) );
	}

	_tprintf( _T( "\n" ) );
}

/*
================
MSJExceptionHandler::GetExceptionString

The name of an exception code; codes it doesn't know are looked up in
NTDLL.DLL's message table.
================
*/
// FUNCTION: STBC 0x00716e50
LPTSTR MSJExceptionHandler::GetExceptionString( DWORD dwCode ) {
	#define EXCEPTION( x ) case EXCEPTION_##x: return _T( #x );

	switch ( dwCode ) {
		EXCEPTION( ACCESS_VIOLATION )
		EXCEPTION( DATATYPE_MISALIGNMENT )
		EXCEPTION( BREAKPOINT )
		EXCEPTION( SINGLE_STEP )
		EXCEPTION( ARRAY_BOUNDS_EXCEEDED )
		EXCEPTION( FLT_DENORMAL_OPERAND )
		EXCEPTION( FLT_DIVIDE_BY_ZERO )
		EXCEPTION( FLT_INEXACT_RESULT )
		EXCEPTION( FLT_INVALID_OPERATION )
		EXCEPTION( FLT_OVERFLOW )
		EXCEPTION( FLT_STACK_CHECK )
		EXCEPTION( FLT_UNDERFLOW )
		EXCEPTION( INT_DIVIDE_BY_ZERO )
		EXCEPTION( INT_OVERFLOW )
		EXCEPTION( PRIV_INSTRUCTION )
		EXCEPTION( IN_PAGE_ERROR )
		EXCEPTION( ILLEGAL_INSTRUCTION )
		EXCEPTION( NONCONTINUABLE_EXCEPTION )
		EXCEPTION( STACK_OVERFLOW )
		EXCEPTION( INVALID_DISPOSITION )
		EXCEPTION( GUARD_PAGE )
		EXCEPTION( INVALID_HANDLE )
	}

	static TCHAR szBuffer[512] = { 0 };

	FormatMessage( FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE, GetModuleHandle( _T( "NTDLL.DLL" ) ),
		dwCode, 0, szBuffer, sizeof( szBuffer ), 0 );

	return szBuffer;
}

/*
================
MSJExceptionHandler::GetLogicalAddress

The module, section and offset of a linear address.
================
*/
// FUNCTION: STBC 0x007170d0
BOOL MSJExceptionHandler::GetLogicalAddress( PVOID addr, PTSTR szModule, DWORD len, DWORD &section, DWORD &offset ) {
	MEMORY_BASIC_INFORMATION mbi;

	if ( !VirtualQuery( addr, &mbi, sizeof( mbi ) ) ) {
		return FALSE;
	}

	DWORD hMod = (DWORD)mbi.AllocationBase;

	if ( !GetModuleFileName( (HMODULE)hMod, szModule, len ) ) {
		return FALSE;
	}

	PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)hMod;
	PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)( hMod + pDosHdr->e_lfanew );
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION( pNtHdr );

	DWORD rva = (DWORD)addr - hMod;

	for ( unsigned i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++, pSection++ ) {
		DWORD sectionStart = pSection->VirtualAddress;
		DWORD sectionEnd = sectionStart + max( pSection->SizeOfRawData, pSection->Misc.VirtualSize );

		if ( ( rva >= sectionStart ) && ( rva <= sectionEnd ) ) {
			section = i + 1;
			offset = rva - sectionStart;
			return TRUE;
		}
	}

	return FALSE;
}

/*
================
MSJExceptionHandler::IntelStackWalk

The call stack without symbols: follows the chain of EBP frames for as long
as it looks sane.
================
*/
// FUNCTION: STBC 0x00717170
void MSJExceptionHandler::IntelStackWalk( PCONTEXT pContext ) {
	_tprintf( _T( "\nCall stack:\n" ) );

	_tprintf( _T( "Address   Frame     Logical addr  Module\n" ) );

	DWORD pc = pContext->Eip;
	PDWORD pFrame, pPrevFrame;

	pFrame = (PDWORD)pContext->Ebp;

	do {
		TCHAR szModule[MAX_PATH] = _T( "" );
		DWORD section = 0, offset = 0;

		GetLogicalAddress( (PVOID)pc, szModule, sizeof( szModule ), section, offset );

		_tprintf( _T( "%08X  %08X  %04X:%08X %s\n" ), pc, pFrame, section, offset, szModule );

		pc = pFrame[1];

		pPrevFrame = pFrame;

		pFrame = (PDWORD)pFrame[0];

		if ( (DWORD)pFrame & 3 ) {
			break;
		}

		if ( pFrame <= pPrevFrame ) {
			break;
		}

		if ( IsBadWritePtr( pFrame, sizeof( PVOID ) * 2 ) ) {
			break;
		}
	} while ( 1 );
}

/*
================
MSJExceptionHandler::ImagehlpStackWalk

The call stack with DBGHELP's StackWalk: each frame's function and
displacement, its source line when one is found within 100 bytes before
the address, and the frame's first four parameters.
================
*/
// FUNCTION: STBC 0x007172a0
// PARKED: register: the no-line print loads its four arguments into other registers (eax, ecx, edx in retail; ecx, edx, eax here); the line search leaves for it with a goto as retail does
void MSJExceptionHandler::ImagehlpStackWalk( PCONTEXT pContext, TGSymbolEngine *symbolEngine, HANDLE process ) {
	_tprintf( _T( "\nCall stack:\n" ) );

	_tprintf( _T( "Address   Frame\n" ) );

	STACKFRAME sf;
	memset( &sf, 0, sizeof( sf ) );

	sf.AddrPC.Offset = pContext->Eip;
	sf.AddrPC.Mode = AddrModeFlat;
	sf.AddrStack.Offset = pContext->Esp;
	sf.AddrStack.Mode = AddrModeFlat;
	sf.AddrFrame.Offset = pContext->Ebp;
	sf.AddrFrame.Mode = AddrModeFlat;

	while ( 1 ) {
		HANDLE thread = GetCurrentThread();
		if ( !symbolEngine->stackWalk( IMAGE_FILE_MACHINE_I386, symbolEngine->process, thread, &sf, NULL, NULL,
				symbolEngine->symFunctionTableAccess, symbolEngine->symGetModuleBase, NULL ) ) {
			break;
		}

		if ( 0 == sf.AddrFrame.Offset ) {
			break;
		}

		_tprintf( _T( "%08X  %08X  " ), sf.AddrPC.Offset, sf.AddrFrame.Offset );

		IMAGEHLP_MODULE moduleInfo;
		memset( &moduleInfo, 0, sizeof( moduleInfo ) );

		BYTE symbolBuffer[sizeof( IMAGEHLP_SYMBOL ) + 512];
		PIMAGEHLP_SYMBOL pSymbol = (PIMAGEHLP_SYMBOL)symbolBuffer;
		pSymbol->SizeOfStruct = sizeof( symbolBuffer );
		pSymbol->MaxNameLength = 512;

		DWORD symDisplacement = 0;

		moduleInfo.SizeOfStruct = sizeof( moduleInfo );

		bool haveModule = false;
		if ( symbolEngine->symGetModuleInfo( symbolEngine->process, sf.AddrPC.Offset, &moduleInfo ) ) {
			haveModule = true;
			if ( symbolEngine->symGetSymFromAddr( symbolEngine->process, sf.AddrPC.Offset, &symDisplacement, pSymbol ) ) {
				IMAGEHLP_LINE line;
				DWORD lineDisplacement;
				line.SizeOfStruct = sizeof( line );

				DWORD address = sf.AddrPC.Offset;
				DWORD back = 0;
				while ( !symbolEngine->symGetLineFromAddr( symbolEngine->process, address - back, &lineDisplacement, &line ) ) {
					back++;
					if ( back == 100 ) {
						goto noLine;
					}
				}

				if ( back != 0 ) {
					lineDisplacement = back;
				}
				_tprintf( _T( "%hs+%X (%s:%d) (Possible Params: %X %X %X %X)\n" ),
					pSymbol->Name, symDisplacement, line.FileName, line.LineNumber,
					sf.Params[0], sf.Params[1], sf.Params[2], sf.Params[3] );
				continue;
noLine:
				_tprintf( _T( "%hs+%X (Line Info Not Available) (Possible Params: %X %X %X %X)\n" ),
					pSymbol->Name, symDisplacement, sf.Params[0], sf.Params[1], sf.Params[2], sf.Params[3] );
				continue;
			}
		}

		TCHAR szModule[MAX_PATH] = _T( "" );
		DWORD section = 0, offset = 0;

		GetLogicalAddress( (PVOID)sf.AddrPC.Offset, szModule, sizeof( szModule ), section, offset );

		if ( !haveModule ) {
			_tprintf( _T( "%04X:%08X %s (No symbol info found)\n" ), section, offset, szModule );
		} else {
			_tprintf( _T( "%04X:%08X %s\n" ), section, offset, szModule );
		}
	}
}

/*
================
MSJExceptionHandler::_tprintf

printf into the report file.
================
*/
// FUNCTION: STBC 0x00717520 ?printf@MSJExceptionHandler@@CAHPBDZZ
int __cdecl MSJExceptionHandler::_tprintf( const TCHAR *format, ... ) {
	TCHAR szBuff[1024];
	int retValue;
	DWORD cbWritten;
	va_list argptr;

	va_start( argptr, format );
	retValue = wvsprintf( szBuff, format, argptr );
	va_end( argptr );

	WriteFile( reportFile, szBuff, retValue * sizeof( TCHAR ), &cbWritten, 0 );

	return retValue;
}

/*
================
TGSymbolEngine::LoadFunctions

Loads DBGHELP.DLL and looks up its functions; false when one is missing.
The library is never freed. (Invented name.)
================
*/
// FUNCTION: STBC 0x00717570
BOOL TGSymbolEngine::LoadFunctions( void ) {
	HMODULE dbgHelp = LoadLibrary( "DBGHELP.DLL" );
	if ( !dbgHelp ) {
		return FALSE;
	}

	symInitialize = (SymInitializeProc)GetProcAddress( dbgHelp, "SymInitialize" );
	if ( !symInitialize ) {
		return FALSE;
	}
	symCleanup = (SymCleanupProc)GetProcAddress( dbgHelp, "SymCleanup" );
	if ( !symCleanup ) {
		return FALSE;
	}
	stackWalk = (StackWalkProc)GetProcAddress( dbgHelp, "StackWalk" );
	if ( !stackWalk ) {
		return FALSE;
	}
	symFunctionTableAccess = (PFUNCTION_TABLE_ACCESS_ROUTINE)GetProcAddress( dbgHelp, "SymFunctionTableAccess" );
	if ( !symFunctionTableAccess ) {
		return FALSE;
	}
	symGetModuleBase = (PGET_MODULE_BASE_ROUTINE)GetProcAddress( dbgHelp, "SymGetModuleBase" );
	if ( !symGetModuleBase ) {
		return FALSE;
	}
	symGetSymFromAddr = (SymGetSymFromAddrProc)GetProcAddress( dbgHelp, "SymGetSymFromAddr" );
	if ( !symGetSymFromAddr ) {
		return FALSE;
	}
	symGetLineFromAddr = (SymGetLineFromAddrProc)GetProcAddress( dbgHelp, "SymGetLineFromAddr" );
	if ( !symGetLineFromAddr ) {
		return FALSE;
	}
	symGetOptions = (SymGetOptionsProc)GetProcAddress( dbgHelp, "SymGetOptions" );
	if ( !symGetOptions ) {
		return FALSE;
	}
	symSetOptions = (SymSetOptionsProc)GetProcAddress( dbgHelp, "SymSetOptions" );
	if ( !symSetOptions ) {
		return FALSE;
	}
	getTimestampForLoadedLibrary = (GetTimestampForLoadedLibraryProc)GetProcAddress( dbgHelp, "GetTimestampForLoadedLibrary" );
	if ( !getTimestampForLoadedLibrary ) {
		return FALSE;
	}
	symEnumerateModules = (SymEnumerateModulesProc)GetProcAddress( dbgHelp, "SymEnumerateModules" );
	if ( !symEnumerateModules ) {
		return FALSE;
	}
	symLoadModule = (SymLoadModuleProc)GetProcAddress( dbgHelp, "SymLoadModule" );
	if ( !symLoadModule ) {
		return FALSE;
	}
	enumerateLoadedModules = (EnumerateLoadedModulesProc)GetProcAddress( dbgHelp, "EnumerateLoadedModules" );
	if ( !enumerateLoadedModules ) {
		return FALSE;
	}
	symUnloadModule = (SymUnloadModuleProc)GetProcAddress( dbgHelp, "SymUnloadModule" );
	if ( !symUnloadModule ) {
		return FALSE;
	}
	symGetModuleInfo = (SymGetModuleInfoProc)GetProcAddress( dbgHelp, "SymGetModuleInfo" );
	if ( !symGetModuleInfo ) {
		return FALSE;
	}
	symEnumerateSymbols = (SymEnumerateSymbolsProc)GetProcAddress( dbgHelp, "SymEnumerateSymbols" );
	if ( !symEnumerateSymbols ) {
		return FALSE;
	}
	symGetSymFromName = (SymGetSymFromNameProc)GetProcAddress( dbgHelp, "SymGetSymFromName" );
	if ( !symGetSymFromName ) {
		return FALSE;
	}
	symGetSymNext = (SymGetSymNextProc)GetProcAddress( dbgHelp, "SymGetSymNext" );
	if ( !symGetSymNext ) {
		return FALSE;
	}
	symGetSymPrev = (SymGetSymPrevProc)GetProcAddress( dbgHelp, "SymGetSymPrev" );
	if ( !symGetSymPrev ) {
		return FALSE;
	}
	symGetLineFromName = (SymGetLineFromNameProc)GetProcAddress( dbgHelp, "SymGetLineFromName" );
	if ( !symGetLineFromName ) {
		return FALSE;
	}
	symGetLineNext = (SymGetLineNextProc)GetProcAddress( dbgHelp, "SymGetLineNext" );
	if ( !symGetLineNext ) {
		return FALSE;
	}
	symGetLinePrev = (SymGetLinePrevProc)GetProcAddress( dbgHelp, "SymGetLinePrev" );
	if ( !symGetLinePrev ) {
		return FALSE;
	}
	symMatchFileName = (SymMatchFileNameProc)GetProcAddress( dbgHelp, "SymMatchFileName" );
	if ( !symMatchFileName ) {
		return FALSE;
	}
	symGetSearchPath = (SymGetSearchPathProc)GetProcAddress( dbgHelp, "SymGetSearchPath" );
	if ( !symGetSearchPath ) {
		return FALSE;
	}
	symSetSearchPath = (SymSetSearchPathProc)GetProcAddress( dbgHelp, "SymSetSearchPath" );
	if ( !symSetSearchPath ) {
		return FALSE;
	}
	symRegisterCallback = (SymRegisterCallbackProc)GetProcAddress( dbgHelp, "SymRegisterCallback" );
	if ( !symRegisterCallback ) {
		return FALSE;
	}
	// BUG: redundant: looks StackWalk up a second time
	stackWalk = (StackWalkProc)GetProcAddress( dbgHelp, "StackWalk" );
	return stackWalk != NULL;
}
