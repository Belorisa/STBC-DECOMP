/*
===============================================================================

	ConsoleWindow.cpp

===============================================================================
*/

#include "ConsoleWindow.h"
#include "TGConsole.h"
#include "TGEventHandlerTable.h"
#include "TGIcon.h"

/*
================
ConsoleWindow::ConsoleWindow

Hidden at first; the console pane over its background icon.
================
*/
// FUNCTION: STBC 0x0050ebc0
ConsoleWindow::ConsoleWindow( void ) : MainWindow( MWT_CONSOLE, 1.0f, 0.5f ) {
	SetNotVisible();

	TGConsole *console = new TGConsole;
	console->SetConsoleFont( "Tahoma", 14.0f );
	AddChild( console );

	TGIcon *background = new TGIcon( "Console", 0 );
	background->Resize( GetWidth(), GetHeight() );
	AddChild( background );
}

/*
================
ConsoleWindow::GetObjType
================
*/
// FUNCTION: STBC 0x0050ecd0
int ConsoleWindow::GetObjType( void ) {
	return CONSOLEWINDOW_TYPE;
}

/*
================
ConsoleWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x0050ece0
bool ConsoleWindow::IsTypeOf( int type ) {
	if ( type == CONSOLEWINDOW_TYPE ) {
		return true;
	}
	return MainWindow::IsTypeOf( type );
}

/*
================
ConsoleWindow::GetClassName
================
*/
// FUNCTION: STBC 0x0050ed40
char *ConsoleWindow::GetClassName( void ) {
	return "ConsoleWindow";
}

/*
================
ConsoleWindow::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0050ed50
char *ConsoleWindow::GetSwigTypeName( void ) {
	return "_p_ConsoleWindow";
}

/*
================
ConsoleWindow::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0050ed60
char *ConsoleWindow::GetPythonClassName( void ) {
	return "ConsoleWindowPtr";
}

/*
================
ConsoleWindow::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0050ed70
TGEventHandlerTable *ConsoleWindow::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
ConsoleWindow::SetFocus
================
*/
// FUNCTION: STBC 0x0050edc0
void ConsoleWindow::SetFocus( TGUIObject *child, bool layout ) {
	TGUIObject *console = GetNthChild( 0 );
	TGPane::SetFocus( console, layout );
}

/*
================
ConsoleWindow::GetConsole
================
*/
// FUNCTION: STBC 0x0050ede0
TGConsole *ConsoleWindow::GetConsole( void ) {
	int numChildren = GetNumChildren();
	for ( int i = 0; i < numChildren; i++ ) {
		TGUIObject *child = GetNthChild( i );
		if ( child != NULL && child->IsTypeOf( TGCONSOLE_TYPE ) ) {
			return (TGConsole *)child;
		}
	}
	return NULL;
}
