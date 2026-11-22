/*
===============================================================================

	UITreeDialog.h

	A modal dialog with a tree view, for the engine's debugging tools. The
	class name comes from the SWIG wrappers UITreeDialog_Create, _Go and
	_AddTreeElement and from the shadow class name the dialog hands to Python
	("UITreeDialog", 0x55D680). Scripts fill the tree: when the dialog opens,
	it calls the script function named at creation with the dialog and its
	owner, and the function adds the items with AddTreeElement.

	0x30 bytes (UITreeDialog::Create allocates 0x30). No vtable: the
	destructor is called directly. The member and method names other than
	Create, Go and AddTreeElement are invented.

===============================================================================
*/

#ifndef __UITREEDIALOG_H__
#define __UITREEDIALOG_H__

#include <windows.h>
#include <commctrl.h>
#include "NiApplication.h"
#include "NiAppWindow.h"

class TGUIObject;

class UITreeDialog {
public:
					UITreeDialog( HINSTANCE instance, HWND parentWindow, char *module, char *function, TGUIObject *owner );
					~UITreeDialog( void );

	// A new dialog over the application's window, owned by the owner (the
	// root window without one). The SWIG wrapper UITreeDialog_Create calls
	// the copy kept from its unit.
	// FUNCTION: STBC 0x00637350 ?Create@UITreeDialog@@SAPAV1@PAD0PAVTGUIObject@@@Z @game/AppWrap
	static UITreeDialog *	Create( char *module, char *function, TGUIObject *owner = NULL ) {
		return new UITreeDialog( NiApplication::GetInstanceReference(),
			NiApplication::application->GetAppWindow()->GetWindow(), module, function, owner );
	}

	// Runs the dialog until it is closed, then destroys it (the SWIG
	// wrapper UITreeDialog_Go expands it).
	void			Go( void ) { DoModal(); delete this; }

	// Runs the dialog box with the cursor shown and answers EndDialog's result.
	int				DoModal( void );

	// Adds an item with the text under the parent item (0 for the top level)
	// and returns the new item (invented parameter names).
	int				AddTreeElement( int parent, char *text );

protected:
	// The dialog procedure DialogBoxParam calls: it hands the message to the
	// open dialog.
	static BOOL CALLBACK	DialogProc( HWND window, UINT message, WPARAM wParam, LPARAM lParam );

	bool			HandleMessage( HWND window, UINT message, WPARAM wParam );
	bool			OnInitDialog( WPARAM wParam );
	// Empties the tree and has the script fill it.
	void			FillTree( void );

	HWND			window;					// 0x00 the parent window until the dialog opens, then the dialog's own
	HWND			parentWindow;			// 0x04
	HINSTANCE		instance;				// 0x08 the module the dialog template is loaded from
	int				templateID;				// 0x0C the dialog template's resource ID, 101
	DLGPROC			dialogProc;				// 0x10 DialogProc
	HWND			treeWindow;				// 0x14 the tree view control (dialog item 1000)
	HTREEITEM		rootItem;				// 0x18 the last top-level item added
	char *			module;					// 0x1C the script module of the fill function (a heap copy)
	char *			function;				// 0x20 the fill function's name (a heap copy)
	TGUIObject *	owner;					// 0x24 passed to the fill function; g_kRootWindow by default
	int				unknown28;				// 0x28 cleared by the constructor, never read here
	int				unknown2C;				// 0x2C cleared by the constructor, never read here

	static UITreeDialog *	activeDialog;	// the dialog DialogProc forwards to (0x98B7C4)
};

#endif /* !__UITREEDIALOG_H__ */
