/*
===============================================================================

	TGAssert.h

	The assert dialog: an assert reports its expression, message, file and
	line, and the dialog ("Assert failed!") offers Debug, Ignore, Ignore
	always and Exit. The report also goes to the debugger's log and the
	clipboard.

	Nothing in retail asserts (the asserts were compiled out of the release
	build), so the names of the unit and of its functions are invented: only
	the dialog's strings and the fullscreen flag's setter, which
	UtopiaApp::Initialize (0x438A44) calls with its fullscreen flag, are
	reached. Evidence: the dialog template is built in memory (0x71E860) and
	run with DialogBoxIndirectParam by the default handler (0x71ED40), which
	minimises the active window around it when the game runs fullscreen.

===============================================================================
*/

#ifndef __TGASSERT_H__
#define __TGASSERT_H__

// Reports a failed assert; true asks the caller to break into the debugger.
// ignoreAlways is set when the assert should not be reported again.
typedef bool ( *assertHandler_t )( const char *expression, const char *message, const char *file, int line, bool *ignoreAlways );

// NULL restores the dialog (invented name).
void	SetAssertHandler( assertHandler_t handler );

// The game runs fullscreen: the dialog minimises the active window while it
// is up (identity-TODO: invented placeholder name kept from UtopiaApp's
// declaration).
void	Unknown71E790( bool fullscreen );

// Hands the assert to the handler (invented name).
bool	TGAssert( const char *expression, const char *message, const char *file, int line, bool *ignoreAlways );

#endif /* !__TGASSERT_H__ */
