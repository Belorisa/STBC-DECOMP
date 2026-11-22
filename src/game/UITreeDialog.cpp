/*
===============================================================================

	UITreeDialog.cpp

===============================================================================
*/

#include <iostream>
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "UITreeDialog.h"
#include "TGRootPane.h"
#include "PyEmbed.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0055d810 _$E49
// FUNCTION: STBC 0x0055d820 _$E48
// FUNCTION: STBC 0x0055d830 _$E205
// FUNCTION: STBC 0x0055d840 _$E204
// FUNCTION: STBC 0x0055d850 _$E60
// FUNCTION: STBC 0x0055d860 _$E59
// FUNCTION: STBC 0x0055d870 _$E64
// FUNCTION: STBC 0x0055d880 _$E63
// FUNCTION: STBC 0x0055d890 _$E68
// FUNCTION: STBC 0x0055d8a0 _$E67
// FUNCTION: STBC 0x0055d8b0 _$E72
// FUNCTION: STBC 0x0055d8c0 _$E71
// FUNCTION: STBC 0x0055d8d0 _$E192
// FUNCTION: STBC 0x0055d8e0 _$E191
// FUNCTION: STBC 0x0055d8f0 _$E134
// FUNCTION: STBC 0x0055d900 _$E133
// FUNCTION: STBC 0x0055d910 _$E147
// FUNCTION: STBC 0x0055d920 _$E146
// FUNCTION: STBC 0x0055d930 _$E197
// FUNCTION: STBC 0x0055d940 _$E196
// FUNCTION: STBC 0x0055d950 _$E210
// FUNCTION: STBC 0x0055d960 _$E209
// FUNCTION: STBC 0x0055d970 _$E201
// FUNCTION: STBC 0x0055d980 _$E200
// FUNCTION: STBC 0x0055d990 _$E214
// FUNCTION: STBC 0x0055d9a0 _$E213
// FUNCTION: STBC 0x0055d9b0 _$E221
// FUNCTION: STBC 0x0055d9c0 _$E220
// FUNCTION: STBC 0x0055d9d0 _$E284
// FUNCTION: STBC 0x0055d9e0 _$E283
// FUNCTION: STBC 0x0055d9f0 _$E288
// FUNCTION: STBC 0x0055da00 _$E287
// FUNCTION: STBC 0x0055da10 _$E96
// FUNCTION: STBC 0x0055da20 _$E95
// FUNCTION: STBC 0x0055da30 _$E270
// FUNCTION: STBC 0x0055da40 _$E269
// FUNCTION: STBC 0x0055da50 _$E277
// FUNCTION: STBC 0x0055da60 _$E276
// FUNCTION: STBC 0x0055da70 _$E300
// FUNCTION: STBC 0x0055da80 _$E299
// FUNCTION: STBC 0x0055da90 _$E304
// FUNCTION: STBC 0x0055daa0 _$E303
// FUNCTION: STBC 0x0055dab0 _$E344
// FUNCTION: STBC 0x0055dac0 _$E343
// FUNCTION: STBC 0x0055dad0 _$E350
// FUNCTION: STBC 0x0055dae0 _$E349
// FUNCTION: STBC 0x0055daf0 _$E292
// FUNCTION: STBC 0x0055db00 _$E291
// FUNCTION: STBC 0x0055db10 _$E296
// FUNCTION: STBC 0x0055db20 _$E295
// FUNCTION: STBC 0x0055db30 _$E308
// FUNCTION: STBC 0x0055db40 _$E307
// FUNCTION: STBC 0x0055db50 _$E312
// FUNCTION: STBC 0x0055db60 _$E311
// FUNCTION: STBC 0x0055dba0 _$E316
// FUNCTION: STBC 0x0055dbb0 _$E315
// FUNCTION: STBC 0x0055dbc0 _$E320
// FUNCTION: STBC 0x0055dbd0 _$E319
// FUNCTION: STBC 0x0055dbe0 _$E324
// FUNCTION: STBC 0x0055dbf0 _$E323
// FUNCTION: STBC 0x0055dc00 _$E328
// FUNCTION: STBC 0x0055dc10 _$E327
// FUNCTION: STBC 0x0055dc20 _$E332
// FUNCTION: STBC 0x0055dc30 _$E331
// FUNCTION: STBC 0x0055dc40 _$E336
// FUNCTION: STBC 0x0055dc50 _$E335
// FUNCTION: STBC 0x0055dc60 _$E101
// FUNCTION: STBC 0x0055dc70 _$E100
// FUNCTION: STBC 0x0055dc80 _$E340
// FUNCTION: STBC 0x0055dc90 _$E339
// FUNCTION: STBC 0x0055dca0 _$E106
// FUNCTION: STBC 0x0055dcb0 _$E105
// FUNCTION: STBC 0x0055dcc0 _$E354
// FUNCTION: STBC 0x0055dcd0 _$E353
// FUNCTION: STBC 0x0055dce0 _$E358
// FUNCTION: STBC 0x0055dcf0 _$E357
// FUNCTION: STBC 0x0055dd00 _$E362
// FUNCTION: STBC 0x0055dd10 _$E361
// FUNCTION: STBC 0x0055dd20 _$E366
// FUNCTION: STBC 0x0055dd30 _$E365
// FUNCTION: STBC 0x0055dd40 _$E111
// FUNCTION: STBC 0x0055dd50 _$E110
// FUNCTION: STBC 0x0055dd60 _$E370
// FUNCTION: STBC 0x0055dd70 _$E369
// FUNCTION: STBC 0x0055dd80 _$E374
// FUNCTION: STBC 0x0055dd90 _$E373
// FUNCTION: STBC 0x0055dda0 _$E378
// FUNCTION: STBC 0x0055ddb0 _$E377
// FUNCTION: STBC 0x0055ddc0 _$E117
// FUNCTION: STBC 0x0055ddd0 _$E116
// FUNCTION: STBC 0x0055dde0 _$E122
// FUNCTION: STBC 0x0055ddf0 _$E121
// FUNCTION: STBC 0x0055de00 _$E152
// FUNCTION: STBC 0x0055de10 _$E151
// FUNCTION: STBC 0x0055de20 _$E382
// FUNCTION: STBC 0x0055de30 _$E381
// FUNCTION: STBC 0x0055de40 _$E157
// FUNCTION: STBC 0x0055de50 _$E156
// FUNCTION: STBC 0x0055de60 _$E386
// FUNCTION: STBC 0x0055de70 _$E385
// FUNCTION: STBC 0x0055de80 _$E390
// FUNCTION: STBC 0x0055de90 _$E389
// FUNCTION: STBC 0x0055dea0 _$E162
// FUNCTION: STBC 0x0055deb0 _$E161
// FUNCTION: STBC 0x0055dec0 _$E394
// FUNCTION: STBC 0x0055ded0 _$E393
// FUNCTION: STBC 0x0055dee0 _$E127
// FUNCTION: STBC 0x0055def0 _$E126
// FUNCTION: STBC 0x0055df00 _$E398
// FUNCTION: STBC 0x0055df10 _$E397
// FUNCTION: STBC 0x0055df20 _$E402
// FUNCTION: STBC 0x0055df30 _$E401
// FUNCTION: STBC 0x0055df40 _$E167
// FUNCTION: STBC 0x0055df50 _$E166
// FUNCTION: STBC 0x0055df60 _$E172
// FUNCTION: STBC 0x0055df70 _$E171
// FUNCTION: STBC 0x0055df80 _$E406
// FUNCTION: STBC 0x0055df90 _$E405
// FUNCTION: STBC 0x0055dfa0 _$E410
// FUNCTION: STBC 0x0055dfb0 _$E409
// FUNCTION: STBC 0x0055dfc0 _$E414
// FUNCTION: STBC 0x0055dfd0 _$E413
// FUNCTION: STBC 0x0055dfe0 _$E418
// FUNCTION: STBC 0x0055dff0 _$E417
// FUNCTION: STBC 0x0055e000 _$E422
// FUNCTION: STBC 0x0055e010 _$E421
// FUNCTION: STBC 0x0055e020 _$E426
// FUNCTION: STBC 0x0055e030 _$E425
// FUNCTION: STBC 0x0055e040 _$E430
// FUNCTION: STBC 0x0055e050 _$E429
// FUNCTION: STBC 0x0055e060 _$E434
// FUNCTION: STBC 0x0055e070 _$E433
// FUNCTION: STBC 0x0055e080 _$E177
// FUNCTION: STBC 0x0055e090 _$E176
// FUNCTION: STBC 0x0055e0a0 _$E438
// FUNCTION: STBC 0x0055e0b0 _$E437
// FUNCTION: STBC 0x0055e0c0 _$E182
// FUNCTION: STBC 0x0055e0d0 _$E181
// FUNCTION: STBC 0x0055e0e0 _$E187
// FUNCTION: STBC 0x0055e0f0 _$E186
// FUNCTION: STBC 0x0055e100 _$E442
// FUNCTION: STBC 0x0055e110 _$E441
// FUNCTION: STBC 0x0055e120 _$E447
// FUNCTION: STBC 0x0055e130 _$E446
// FUNCTION: STBC 0x0055e140 _$E451
// FUNCTION: STBC 0x0055e150 _$E450

// <iostream>'s statics: the ios_base::Init and _Winit quads and the locale id pair.
// FUNCTION: STBC 0x0055d790 _$E32
// FUNCTION: STBC 0x0055d7a0 _$E29
// FUNCTION: STBC 0x0055d7b0 _$E31
// FUNCTION: STBC 0x0055d7c0 _$E30
// FUNCTION: STBC 0x0055d7d0 _$E38
// FUNCTION: STBC 0x0055d7e0 _$E35
// FUNCTION: STBC 0x0055d7f0 _$E37
// FUNCTION: STBC 0x0055d800 _$E36
// FUNCTION: STBC 0x0055db70 _$E459
// FUNCTION: STBC 0x0055db90 _$E458

// DATA: STBC 0x0098b7c4
UITreeDialog *UITreeDialog::activeDialog;

// The dialog template's resource ID and the tree view's dialog item ID.
#define IDD_TREE_DIALOG		101
#define IDC_TREE			1000

/*
================
UITreeDialog::UITreeDialog

Copies the two names, and makes this the dialog DialogProc forwards to.
================
*/
// FUNCTION: STBC 0x0055d440
// PARKED: register: retail loads instance_ into ecx before the window stores; tried chained, separate, instance-first and initializer-list assignments
UITreeDialog::UITreeDialog( HINSTANCE instance_, HWND parentWindow_, char *module_, char *function_, TGUIObject *owner_ ) {
	window = parentWindow = parentWindow_;
	instance = instance_;
	templateID = IDD_TREE_DIALOG;
	dialogProc = DialogProc;
	treeWindow = NULL;
	rootItem = NULL;
	if ( owner_ != NULL ) {
		owner = owner_;
	} else {
		owner = &g_kRootWindow;
	}
	unknown28 = 0;
	unknown2C = 0;
	if ( module_ != NULL ) {
		module = new char[strlen( module_ ) + 1];
		strcpy( module, module_ );
	} else {
		module = NULL;
	}
	if ( function_ != NULL ) {
		function = new char[strlen( function_ ) + 1];
		strcpy( function, function_ );
	} else {
		function = NULL;
	}
	activeDialog = this;
}

/*
================
UITreeDialog::~UITreeDialog
================
*/
// FUNCTION: STBC 0x0055d530
UITreeDialog::~UITreeDialog( void ) {
	treeWindow = NULL;
	rootItem = NULL;
	if ( module != NULL ) {
		delete[] module;
		module = NULL;
	}
	if ( function != NULL ) {
		delete[] function;
		function = NULL;
	}
	activeDialog = NULL;
}

/*
================
UITreeDialog::DoModal
================
*/
// FUNCTION: STBC 0x0055d570
int UITreeDialog::DoModal( void ) {
	while ( ShowCursor( TRUE ) < 0 ) {
	}
	int result = DialogBoxParam( instance, MAKEINTRESOURCE( templateID ), window, dialogProc, 0 );
	while ( ShowCursor( FALSE ) >= 0 ) {
	}
	return result;
}

/*
================
UITreeDialog::DialogProc
================
*/
// FUNCTION: STBC 0x0055d5c0
BOOL CALLBACK UITreeDialog::DialogProc( HWND window, UINT message, WPARAM wParam, LPARAM lParam ) {
	return activeDialog->HandleMessage( window, message, wParam ) == true;
}

/*
================
UITreeDialog::HandleMessage

OK and closing the dialog end it.
================
*/
// FUNCTION: STBC 0x0055d5f0
bool UITreeDialog::HandleMessage( HWND window_, UINT message, WPARAM wParam ) {
	switch ( message ) {
	case WM_INITDIALOG:
		window = window_;
		return OnInitDialog( wParam );
	case WM_COMMAND:
		if ( LOWORD( wParam ) == IDOK ) {
			EndDialog( window, IDOK );
			return true;
		}
		// fall through
	case WM_CLOSE:
		EndDialog( window, IDOK );
		return true;
	}
	return false;
}

/*
================
UITreeDialog::OnInitDialog
================
*/
// FUNCTION: STBC 0x0055d650
bool UITreeDialog::OnInitDialog( WPARAM wParam ) {
	treeWindow = GetDlgItem( window, IDC_TREE );
	GetWindowLong( treeWindow, GWL_STYLE );	// BUG: redundant: the style is read and never used
	FillTree();
	return true;
}

/*
================
UITreeDialog::FillTree

The script function gets the dialog's shadow instance and the owner's.
================
*/
// FUNCTION: STBC 0x0055d680
void UITreeDialog::FillTree( void ) {
	SendMessage( treeWindow, TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT );
	if ( module != NULL && function != NULL ) {
		PyObject *dialogObject = PyEmbed::Unknown6F9920( this, "UITreeDialog" );
		PyObject *ownerObject = owner->VirtualSlot08();
		PyEmbed::CallFunction( module, function, NULL, NULL, "(OO)", dialogObject, ownerObject );
		Py_DECREF( dialogObject );
		Py_DECREF( ownerObject );
	}
}

/*
================
UITreeDialog::AddTreeElement
================
*/
// FUNCTION: STBC 0x0055d710
int UITreeDialog::AddTreeElement( int parent, char *text ) {
	char buffer[1024];
	TV_INSERTSTRUCT insert;
	HTREEITEM item = NULL;
	buffer[sizeof( buffer ) - 1] = 0;
	if ( text != NULL ) {
		insert.hParent = (HTREEITEM)parent;
		insert.hInsertAfter = TVI_LAST;
		insert.item.mask = TVIF_TEXT | TVIF_PARAM;
		sprintf( buffer, "%s", text );
		insert.item.pszText = buffer;
		item = (HTREEITEM)SendMessage( treeWindow, TVM_INSERTITEM, 0, (LPARAM)&insert );
	}
	if ( parent == 0 ) {
		rootItem = item;
	}
	return (int)item;
}
