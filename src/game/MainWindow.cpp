/*
===============================================================================

	MainWindow.cpp

===============================================================================
*/

#include "MainWindow.h"
#include "InterfaceModule.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include "TopWindow.h"
#include "NiGeometry.h"

/*
================
MainWindow::MainWindow
================
*/
// FUNCTION: STBC 0x0050e920
MainWindow::MainWindow( int type, float width, float height ) : TGPane( width, height ) {
	windowType = type;
}

/*
================
MainWindow::GetObjType
================
*/
// FUNCTION: STBC 0x0050e950
int MainWindow::GetObjType( void ) {
	return MAINWINDOW_TYPE;
}

/*
================
MainWindow::GetClassName
================
*/
// FUNCTION: STBC 0x0050e9c0
char *MainWindow::GetClassName( void ) {
	return "MainWindow";
}

/*
================
MainWindow::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0050e9d0
char *MainWindow::GetSwigTypeName( void ) {
	return "_p_MainWindow";
}

/*
================
MainWindow::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0050e9e0
char *MainWindow::GetPythonClassName( void ) {
	return "MainWindowPtr";
}

/*
================
MainWindow::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0050e9f0
TGEventHandlerTable *MainWindow::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
MainWindow::ToggleVisibility

A window shown in front is hidden and sent to the back; a hidden one is
shown and brought to the front.
================
*/
// FUNCTION: STBC 0x0050ea30
void MainWindow::ToggleVisibility( void ) {
	TGPane *top = parent;
	MainWindow *focus = (MainWindow *)top->GetFocus();
	if ( IsVisible() ) {
		if ( focus != NULL && focus->windowType == windowType ) {
			top->MoveToBack( this );
		}
		SetNotVisible();
	} else {
		SetVisible();
		if ( focus != NULL && focus->windowType != windowType ) {
			top->MoveToFront( this, false );
		}
	}
}

/*
================
MainWindow::IsWindowActive

Whether the window has the top window's focus.
================
*/
// FUNCTION: STBC 0x0050eab0
bool MainWindow::IsWindowActive( void ) {
	TopWindow *topWindow = TopWindow::GetTopWindow();
	if ( topWindow != NULL && this == topWindow->GetFocus() ) {
		return true;
	}
	return false;
}

/*
================
MainWindow::SetVisible
================
*/
// FUNCTION: STBC 0x0050eb00
void MainWindow::SetVisible( bool layout ) {
	TGPane::SetVisible( layout );
	InterfaceModule::DoTheRightThing();
}

/*
================
MainWindow::SetNotVisible
================
*/
// FUNCTION: STBC 0x0050eb20
void MainWindow::SetNotVisible( bool layout ) {
	TGPane::SetNotVisible( layout );
	InterfaceModule::DoTheRightThing();
}

/*
================
MainWindow::GotTrueFocusPath
================
*/
// FUNCTION: STBC 0x0050eb40
void MainWindow::GotTrueFocusPath( bool layout ) {
	TGUIObject::GotTrueFocusPath( layout );
	InterfaceModule::DoTheRightThing();
}

/*
================
MainWindow::WriteToStream
================
*/
// FUNCTION: STBC 0x0050eb60
bool MainWindow::WriteToStream( TGStream *stream ) {
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteLong( windowType );
	return true;
}

/*
================
MainWindow::ReadFromStream
================
*/
// FUNCTION: STBC 0x0050eb90
bool MainWindow::ReadFromStream( TGStream *stream ) {
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	windowType = stream->ReadLong();
	return true;
}
