/*
===============================================================================

	TGAssert.cpp

	The assert dialog (the names are invented; see TGAssert.h).

===============================================================================
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TGAssert.h"

// The dialog's controls.
#define IDC_ASSERT_FILE				1000
#define IDC_ASSERT_LINE				1001
#define IDC_ASSERT_EXPRESSION		1002
#define IDC_ASSERT_MESSAGE			1003
#define IDC_ASSERT_MODULE			1004
#define IDC_ASSERT_DEBUG			1005
#define IDC_ASSERT_IGNORE			1006
#define IDC_ASSERT_IGNORE_ALWAYS	1007
#define IDC_ASSERT_EXIT				1008

// The template's size: the dialog and its fourteen controls fit.
#define ASSERT_DIALOG_SIZE			0x480

bool	DefaultAssertHandler( const char *expression, const char *message, const char *file, int line, bool *ignoreAlways );

// DATA: STBC 0x0095c6d0
static assertHandler_t	assertHandler = DefaultAssertHandler;

// What the dialog shows, set by the default handler before it runs it.
// DATA: STBC 0x0099c8ac
static char				assertLine[12];
// DATA: STBC 0x0099c8b9
static bool				assertFullscreen;
// DATA: STBC 0x0099c8bc
static const char *		assertExpression;
// DATA: STBC 0x0099c8c0
static const char *		assertMessage;
// DATA: STBC 0x0099c8c4
static const char *		assertFile;

/*
================
SetAssertHandler
================
*/
// FUNCTION: STBC 0x0071e770
void SetAssertHandler( assertHandler_t handler ) {
	assertHandler = handler;
	if ( handler == NULL ) {
		assertHandler = DefaultAssertHandler;
	}
}

/*
================
Unknown71E790
================
*/
// FUNCTION: STBC 0x0071e790
void Unknown71E790( bool fullscreen ) {
	assertFullscreen = fullscreen;
}

/*
================
AddDialogString

Appends the string to the template as a wide string.
================
*/
// FUNCTION: STBC 0x0071e7a0
void AddDialogString( WORD **cursor, const char *string ) {
	int length = lstrlenA( string );
	WCHAR *out = (WCHAR *)*cursor;
	if ( length != 0 ) {
		length = MultiByteToWideChar( CP_ACP, 0, string, length, out, length );
	}
	out[length] = 0;
	*cursor += length + 1;
}

/*
================
AddDialogItem

Appends a visible child control to the template.
================
*/
// FUNCTION: STBC 0x0071e7e0
void AddDialogItem( WORD **cursor, DWORD style, DWORD exStyle, short x, short y, short cx, short cy, WORD id, const char *className, const char *text ) {
	DLGITEMTEMPLATE *item = (DLGITEMTEMPLATE *)( ( (DWORD)*cursor + 3 ) & ~3 );
	item->style = style | WS_CHILD | WS_VISIBLE;
	item->dwExtendedStyle = exStyle;
	item->x = x;
	item->y = y;
	item->cx = cx;
	item->cy = cy;
	item->id = id;
	*cursor = (WORD *)( item + 1 );
	AddDialogString( cursor, className );
	AddDialogString( cursor, text );
	*cursor += 1;		// no creation data
}

/*
================
CreateAssertDialog

The dialog's template, allocated; NULL when it couldn't be.
================
*/
// FUNCTION: STBC 0x0071e860
DLGTEMPLATE *CreateAssertDialog( void ) {
	DLGTEMPLATE *dialog = (DLGTEMPLATE *)new char[ASSERT_DIALOG_SIZE];
	if ( dialog == NULL ) {
		return NULL;
	}

	memset( dialog, 0, ASSERT_DIALOG_SIZE );
	dialog->style = WS_POPUP | WS_VISIBLE | WS_CAPTION | DS_CENTER | DS_SETFOREGROUND | DS_NOIDLEMSG | DS_MODALFRAME | DS_SETFONT | DS_3DLOOK;
	dialog->dwExtendedStyle = 0;
	dialog->cdit = 14;
	dialog->x = 0;
	dialog->y = 0;
	dialog->cx = 346;
	dialog->cy = 127;

	WORD *cursor = (WORD *)( dialog + 1 );
	*cursor++ = 0;		// no menu
	*cursor++ = 0;		// the dialog class
	AddDialogString( &cursor, "Assert failed!" );
	*cursor++ = 8;		// the font's point size
	AddDialogString( &cursor, "Arial" );

	AddDialogItem( &cursor, SS_RIGHT | SS_CENTERIMAGE, 0, 11, 15, 38, 10, 0xFFFF, "STATIC", "File" );
	AddDialogItem( &cursor, SS_RIGHT | SS_CENTERIMAGE, 0, 11, 31, 38, 10, 0xFFFF, "STATIC", "Line" );
	AddDialogItem( &cursor, SS_RIGHT | SS_CENTERIMAGE, 0, 11, 46, 38, 11, 0xFFFF, "STATIC", "Expression" );
	AddDialogItem( &cursor, SS_RIGHT | SS_CENTERIMAGE, 0, 11, 62, 38, 10, 0xFFFF, "STATIC", "Message" );
	AddDialogItem( &cursor, BS_GROUPBOX | WS_GROUP, 0, 5, 2, 336, 120, IDC_ASSERT_MODULE, "BUTTON", "" );
	AddDialogItem( &cursor, ES_AUTOHSCROLL | ES_READONLY | WS_BORDER, 0, 55, 14, 280, 12, IDC_ASSERT_FILE, "EDIT", "" );
	AddDialogItem( &cursor, ES_AUTOHSCROLL | ES_READONLY | WS_BORDER, 0, 55, 30, 280, 12, IDC_ASSERT_LINE, "EDIT", "" );
	AddDialogItem( &cursor, ES_AUTOHSCROLL | ES_READONLY | WS_BORDER, 0, 55, 46, 280, 12, IDC_ASSERT_EXPRESSION, "EDIT", "" );
	AddDialogItem( &cursor, ES_AUTOHSCROLL | ES_READONLY | WS_BORDER, 0, 55, 61, 280, 12, IDC_ASSERT_MESSAGE, "EDIT", "" );
	AddDialogItem( &cursor, WS_GROUP | WS_TABSTOP, 0, 11, 103, 62, 13, IDC_ASSERT_DEBUG, "BUTTON", "Debug" );
	AddDialogItem( &cursor, WS_TABSTOP, 0, 98, 103, 62, 13, IDC_ASSERT_IGNORE, "BUTTON", "Ignore" );
	AddDialogItem( &cursor, WS_TABSTOP, 0, 185, 103, 62, 13, IDC_ASSERT_IGNORE_ALWAYS, "BUTTON", "Ignore always" );
	AddDialogItem( &cursor, WS_TABSTOP, 0, 272, 103, 62, 13, IDC_ASSERT_EXIT, "BUTTON", "Exit" );
	AddDialogItem( &cursor, WS_GROUP | WS_TABSTOP, WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE, 11, 78, 324, 19, 0xFFFF, "STATIC",
		"Note: All information about this assert is copied into the clipboard as well as into the debuggers's log window if a debugger is present." );
	return dialog;
}

/*
================
AssertDialogProc

Fills the fields and copies the report to the clipboard; a button ends
the dialog with its ID.
================
*/
// FUNCTION: STBC 0x0071eb70
BOOL CALLBACK AssertDialogProc( HWND dialog, UINT message, WPARAM wParam, LPARAM lParam ) {
	char path[MAX_PATH];
	char title[MAX_PATH + 16];
	char text[0x800];

	switch ( message ) {
		case WM_INITDIALOG: {
			SetWindowText( GetDlgItem( dialog, IDC_ASSERT_FILE ), assertFile );
			SetWindowText( GetDlgItem( dialog, IDC_ASSERT_LINE ), assertLine );
			SetWindowText( GetDlgItem( dialog, IDC_ASSERT_EXPRESSION ), assertExpression );
			SetWindowText( GetDlgItem( dialog, IDC_ASSERT_MESSAGE ), assertMessage );

			GetModuleFileName( NULL, path, MAX_PATH );
			char *module = strrchr( path, '\\' );
			if ( module != NULL ) {
				module++;
			} else {
				module = path;
			}
			sprintf( title, "In Module \"%s\"", module );
			SetWindowText( GetDlgItem( dialog, IDC_ASSERT_MODULE ), title );

			if ( OpenClipboard( NULL ) ) {
				sprintf( text, "Application: %s\r\nFilename: %s (%s)\r\nExpression: %s\r\nMessage: %s\r\n", module, assertFile, assertLine, assertExpression, assertMessage != NULL ? assertMessage : "" );
				HGLOBAL memory = GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_DDESHARE, strlen( text ) + 1 );
				if ( memory != NULL ) {
					char *clipboardText = (char *)GlobalLock( memory );
					strcpy( clipboardText, text );
					GlobalUnlock( memory );
					EmptyClipboard();
					SetClipboardData( CF_TEXT, memory );
				}
				CloseClipboard();
			}
			return TRUE;
		}

		case WM_COMMAND: {
			int id = LOWORD( wParam );
			if ( id >= IDC_ASSERT_DEBUG && id <= IDC_ASSERT_EXIT ) {
				EndDialog( dialog, id );
				return TRUE;
			}
			break;
		}
	}
	return FALSE;
}

/*
================
DefaultAssertHandler

Logs the assert to the debugger and runs the dialog. Debug asks the
caller to break, Ignore goes on, Ignore always also sets ignoreAlways, and
Exit ends the program.
================
*/
// FUNCTION: STBC 0x0071ed40
bool DefaultAssertHandler( const char *expression, const char *message, const char *file, int line, bool *ignoreAlways ) {
	char text[0x800];

	DWORD lastError = GetLastError();
	assertFile = file;
	assertExpression = expression;
	assertMessage = message;
	_itoa( line, assertLine, 10 );
	sprintf( text, "%s(%i) : Assertion ( %s ) failed...  %s\n", file, line, expression, message != NULL ? message : "" );
	OutputDebugString( text );

	HWND window = GetActiveWindow();
	DLGTEMPLATE *dialog = CreateAssertDialog();
	if ( dialog != NULL ) {
		if ( window != NULL && assertFullscreen ) {
			SendMessage( window, WM_SYSCOMMAND, SC_MINIMIZE, 0 );
		}
		int result = DialogBoxIndirectParam( NULL, dialog, window, AssertDialogProc, 0 );
		if ( window != NULL && assertFullscreen ) {
			SendMessage( window, WM_SYSCOMMAND, SC_RESTORE, 0 );
		}
		delete[] (char *)dialog;

		switch ( result ) {
			case IDC_ASSERT_DEBUG:
				return true;
			case IDC_ASSERT_IGNORE_ALWAYS:
				*ignoreAlways = true;
				// fall through
			case IDC_ASSERT_IGNORE:
				return false;
			case IDC_ASSERT_EXIT:
				exit( 0 );
		}
	} else {
		MessageBeep( (UINT)-1 );
		OutputDebugString( "Couldn't create Assert Dialog !!!" );
	}

	SetLastError( lastError );
	return true;
}

/*
================
TGAssert
================
*/
// FUNCTION: STBC 0x0071eea0
bool TGAssert( const char *expression, const char *message, const char *file, int line, bool *ignoreAlways ) {
	return assertHandler( expression, message, file, line, ignoreAlways );
}
