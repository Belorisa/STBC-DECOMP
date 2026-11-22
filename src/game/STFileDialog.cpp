/*
===============================================================================

	STFileDialog.cpp

===============================================================================
*/

#include "STFileDialog.h"
#include "STFileMenu.h"
#include "STButton.h"
#include "STStylizedWindow.h"
#include "TGEvent.h"
#include "TGEventHandlerTable.h"
#include "FocusManager.h"
#include "GraphicsModeInfo.h"
#include "PyEmbed.h"
#include "STColors.h"
#include "InterfaceModule.h"
#include "TGIcon.h"
#include "TGIconManager.h"
#include "TGLocalizationManager.h"
#include "TGLocalizationDatabase.h"
#include "STSubPane.h"
#include "TGPane.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include <iostream>
#include "NiMain.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005204c0 _$E33
// FUNCTION: STBC 0x005204d0 _$E32
// FUNCTION: STBC 0x005204e0 _$E37
// FUNCTION: STBC 0x005204f0 _$E36
// FUNCTION: STBC 0x00520500 _$E48
// FUNCTION: STBC 0x00520510 _$E47
// FUNCTION: STBC 0x00520520 _$E52
// FUNCTION: STBC 0x00520530 _$E51
// FUNCTION: STBC 0x00520540 _$E56
// FUNCTION: STBC 0x00520550 _$E55
// FUNCTION: STBC 0x00520560 _$E60
// FUNCTION: STBC 0x00520570 _$E59
// FUNCTION: STBC 0x00520580 _$E184
// FUNCTION: STBC 0x00520590 _$E183
// FUNCTION: STBC 0x005205a0 _$E126
// FUNCTION: STBC 0x005205b0 _$E125
// FUNCTION: STBC 0x005205c0 _$E139
// FUNCTION: STBC 0x005205d0 _$E138
// FUNCTION: STBC 0x005205e0 _$E189
// FUNCTION: STBC 0x005205f0 _$E188
// FUNCTION: STBC 0x00520600 _$E194
// FUNCTION: STBC 0x00520610 _$E193
// FUNCTION: STBC 0x00520620 _$E198
// FUNCTION: STBC 0x00520630 _$E197
// FUNCTION: STBC 0x00520640 _$E202
// FUNCTION: STBC 0x00520650 _$E201
// FUNCTION: STBC 0x00520660 _$E209
// FUNCTION: STBC 0x00520670 _$E208
// FUNCTION: STBC 0x00520680 _$E317
// FUNCTION: STBC 0x00520690 _$E316
// FUNCTION: STBC 0x005206a0 _$E321
// FUNCTION: STBC 0x005206b0 _$E320
// FUNCTION: STBC 0x005206c0 _$E84
// FUNCTION: STBC 0x005206d0 _$E83
// FUNCTION: STBC 0x005206e0 _$E303
// FUNCTION: STBC 0x005206f0 _$E302
// FUNCTION: STBC 0x00520700 _$E310
// FUNCTION: STBC 0x00520710 _$E309
// FUNCTION: STBC 0x00520720 _$E333
// FUNCTION: STBC 0x00520730 _$E332
// FUNCTION: STBC 0x00520740 _$E337
// FUNCTION: STBC 0x00520750 _$E336
// FUNCTION: STBC 0x00520760 _$E377
// FUNCTION: STBC 0x00520770 _$E376
// FUNCTION: STBC 0x00520780 _$E383
// FUNCTION: STBC 0x00520790 _$E382
// FUNCTION: STBC 0x005207a0 _$E325
// FUNCTION: STBC 0x005207b0 _$E324
// FUNCTION: STBC 0x005207c0 _$E329
// FUNCTION: STBC 0x005207d0 _$E328
// FUNCTION: STBC 0x005207e0 _$E341
// FUNCTION: STBC 0x005207f0 _$E340
// FUNCTION: STBC 0x00520800 _$E345
// FUNCTION: STBC 0x00520810 _$E344
// FUNCTION: STBC 0x00520820 _$E349
// FUNCTION: STBC 0x00520830 _$E348
// FUNCTION: STBC 0x00520840 _$E353
// FUNCTION: STBC 0x00520850 _$E352
// FUNCTION: STBC 0x00520860 _$E357
// FUNCTION: STBC 0x00520870 _$E356
// FUNCTION: STBC 0x00520880 _$E361
// FUNCTION: STBC 0x00520890 _$E360
// FUNCTION: STBC 0x005208a0 _$E365
// FUNCTION: STBC 0x005208b0 _$E364
// FUNCTION: STBC 0x005208c0 _$E369
// FUNCTION: STBC 0x005208d0 _$E368
// FUNCTION: STBC 0x005208e0 _$E89
// FUNCTION: STBC 0x005208f0 _$E88
// FUNCTION: STBC 0x00520900 _$E373
// FUNCTION: STBC 0x00520910 _$E372
// FUNCTION: STBC 0x00520920 _$E94
// FUNCTION: STBC 0x00520930 _$E93
// FUNCTION: STBC 0x00520940 _$E387
// FUNCTION: STBC 0x00520950 _$E386
// FUNCTION: STBC 0x00520960 _$E391
// FUNCTION: STBC 0x00520970 _$E390
// FUNCTION: STBC 0x00520980 _$E395
// FUNCTION: STBC 0x00520990 _$E394
// FUNCTION: STBC 0x005209a0 _$E399
// FUNCTION: STBC 0x005209b0 _$E398
// FUNCTION: STBC 0x005209c0 _$E103
// FUNCTION: STBC 0x005209d0 _$E102
// FUNCTION: STBC 0x005209e0 _$E403
// FUNCTION: STBC 0x005209f0 _$E402
// FUNCTION: STBC 0x00520a00 _$E407
// FUNCTION: STBC 0x00520a10 _$E406
// FUNCTION: STBC 0x00520a20 _$E411
// FUNCTION: STBC 0x00520a30 _$E410
// FUNCTION: STBC 0x00520a40 _$E109
// FUNCTION: STBC 0x00520a50 _$E108
// FUNCTION: STBC 0x00520a60 _$E114
// FUNCTION: STBC 0x00520a70 _$E113
// FUNCTION: STBC 0x00520a80 _$E144
// FUNCTION: STBC 0x00520a90 _$E143
// FUNCTION: STBC 0x00520ad0 _$E415
// FUNCTION: STBC 0x00520ae0 _$E414
// FUNCTION: STBC 0x00520af0 _$E149
// FUNCTION: STBC 0x00520b00 _$E148
// FUNCTION: STBC 0x00520b10 _$E419
// FUNCTION: STBC 0x00520b20 _$E418
// FUNCTION: STBC 0x00520b30 _$E423
// FUNCTION: STBC 0x00520b40 _$E422
// FUNCTION: STBC 0x00520b50 _$E154
// FUNCTION: STBC 0x00520b60 _$E153
// FUNCTION: STBC 0x00520b70 _$E427
// FUNCTION: STBC 0x00520b80 _$E426
// FUNCTION: STBC 0x00520b90 _$E119
// FUNCTION: STBC 0x00520ba0 _$E118
// FUNCTION: STBC 0x00520bb0 _$E431
// FUNCTION: STBC 0x00520bc0 _$E430
// FUNCTION: STBC 0x00520bd0 _$E435
// FUNCTION: STBC 0x00520be0 _$E434
// FUNCTION: STBC 0x00520bf0 _$E159
// FUNCTION: STBC 0x00520c00 _$E158
// FUNCTION: STBC 0x00520c10 _$E164
// FUNCTION: STBC 0x00520c20 _$E163
// FUNCTION: STBC 0x00520c30 _$E439
// FUNCTION: STBC 0x00520c40 _$E438
// FUNCTION: STBC 0x00520c50 _$E443
// FUNCTION: STBC 0x00520c60 _$E442
// FUNCTION: STBC 0x00520c70 _$E447
// FUNCTION: STBC 0x00520c80 _$E446
// FUNCTION: STBC 0x00520c90 _$E451
// FUNCTION: STBC 0x00520ca0 _$E450
// FUNCTION: STBC 0x00520cb0 _$E455
// FUNCTION: STBC 0x00520cc0 _$E454
// FUNCTION: STBC 0x00520cd0 _$E459
// FUNCTION: STBC 0x00520ce0 _$E458
// FUNCTION: STBC 0x00520cf0 _$E463
// FUNCTION: STBC 0x00520d00 _$E462
// FUNCTION: STBC 0x00520d10 _$E467
// FUNCTION: STBC 0x00520d20 _$E466
// FUNCTION: STBC 0x00520d30 _$E169
// FUNCTION: STBC 0x00520d40 _$E168
// FUNCTION: STBC 0x00520d50 _$E471
// FUNCTION: STBC 0x00520d60 _$E470
// FUNCTION: STBC 0x00520d70 _$E174
// FUNCTION: STBC 0x00520d80 _$E173
// FUNCTION: STBC 0x00520d90 _$E179
// FUNCTION: STBC 0x00520da0 _$E178
// FUNCTION: STBC 0x00520db0 _$E275
// FUNCTION: STBC 0x00520dc0 _$E274
// FUNCTION: STBC 0x00520dd0 _$E280
// FUNCTION: STBC 0x00520de0 _$E279
// FUNCTION: STBC 0x00520df0 _$E284
// FUNCTION: STBC 0x00520e00 _$E283

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00520440 _$E289
// FUNCTION: STBC 0x00520450 _$E286
// FUNCTION: STBC 0x00520460 _$E288
// FUNCTION: STBC 0x00520470 _$E287
// FUNCTION: STBC 0x00520480 _$E295
// FUNCTION: STBC 0x00520490 _$E292
// FUNCTION: STBC 0x005204a0 _$E294
// FUNCTION: STBC 0x005204b0 _$E293

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0051f580 _$E477
// FUNCTION: STBC 0x0051f590 _$E473
// FUNCTION: STBC 0x0051f5a0 _$E476
// FUNCTION: STBC 0x0051f5b0 _$E474
// DATA: STBC 0x00988c78
TGEventHandlerTable STFileDialog::classHandlers;

/*
================
STFileDialog::STFileDialog
================
*/
// FUNCTION: STBC 0x0051f5d0 ??0STFileDialog@@QAE@MM@Z
STFileDialog::STFileDialog( float width, float height ) : TGPane( width, height ) {
	CreateControls();
}

/*
================
STFileDialog::RegisterHandlerNames

The dialog has no handler methods of its own.
================
*/
// FUNCTION: STBC 0x0051f720
void STFileDialog::RegisterHandlerNames( void ) {
}

/*
================
STFileDialog::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0051f730
void STFileDialog::RegisterHandlers( void ) {
}
/*
================
STFileDialog::CreateControls

One pane holds the top area, the content area, the file list window and the
screenshot area, each at the place the lcars group's Python values give;
the file list gets the first tab stop.
================
*/
// FUNCTION: STBC 0x0051f740
void STFileDialog::CreateControls( void ) {
	char *lcars = GraphicsModeInfo::lcarsStrings[GraphicsModeInfo::currentMode->GetCurrentResolution()];
	TGPane *topArea = CreateTopArea( lcars );
	TGPane *contentArea = CreateContentArea( lcars );
	STStylizedWindow *fileList = CreateFileListWindow( lcars );
	TGPane *screenshotArea = CreateScreenshotArea( lcars );

	float topX = 0.0f;
	float topY = 0.0f;
	float buttonAreaX = 0.0f;
	float buttonAreaY = 0.0f;
	float listX = 0.0f;
	float listY = 0.0f;
	float screenshotX = 0.0f;
	float screenshotY = 0.0f;
	float middleX;
	float middleY;
	float middleWidth;
	float middleHeight;
	float tabX;
	float tabY;
	PyEmbed::GetValue( lcars, "FILE_TOP_AREA_X", "f", &topX );
	PyEmbed::GetValue( lcars, "FILE_TOP_AREA_Y", "f", &topY );
	// The button area's place is read and not used.
	PyEmbed::GetValue( lcars, "FILE_BUTTON_AREA_X", "f", &buttonAreaX );
	PyEmbed::GetValue( lcars, "FILE_BUTTON_AREA_Y", "f", &buttonAreaY );
	PyEmbed::GetValue( lcars, "FILE_LIST_AREA_X", "f", &listX );
	PyEmbed::GetValue( lcars, "FILE_LIST_AREA_Y", "f", &listY );
	PyEmbed::GetValue( lcars, "FILE_SCREENSHOT_AREA_X", "f", &screenshotX );
	PyEmbed::GetValue( lcars, "FILE_SCREENSHOT_AREA_Y", "f", &screenshotY );
	PyEmbed::GetValue( lcars, "MAIN_MENU_MIDDLE_CONTENT_X", "f", &middleX );
	PyEmbed::GetValue( lcars, "MAIN_MENU_MIDDLE_CONTENT_Y", "f", &middleY );
	PyEmbed::GetValue( lcars, "MAIN_MENU_MIDDLE_CONTENT_WIDTH", "f", &middleWidth );
	PyEmbed::GetValue( lcars, "MAIN_MENU_MIDDLE_CONTENT_HEIGHT", "f", &middleHeight );
	PyEmbed::GetValue( lcars, "MAIN_MENU_CONFIGURE_TAB_X", "f", &tabX );
	PyEmbed::GetValue( lcars, "MAIN_MENU_CONFIGURE_TAB_Y", "f", &tabY );

	TGPane *pane = new TGPane( middleWidth, middleHeight );
	AddChild( pane, middleX, middleY, false );
	pane->AddChild( topArea, topX, topY, false );
	pane->AddChild( contentArea, tabX, tabY, false );
	pane->AddChild( fileList, listX, listY, false );
	pane->AddChild( screenshotArea, screenshotX, screenshotY, false );
	g_kFocusManager.AddObjectToTabOrder( fileList );
}

/*
================
STFileDialog::GetFileMenu

The file menu in the window that is the dialog's third pane; NULL when
that is not a stylized window.
================
*/
// FUNCTION: STBC 0x0051fa00
STFileMenu *STFileDialog::GetFileMenu( void ) {
	STStylizedWindow *window = STStylizedWindow::Cast( TGPane::Cast( GetFirstChild() )->GetNthChild( 2 ) );
	if ( window != NULL ) {
		return STFileMenu::Cast( window->GetFirstChild() );
	}
	return NULL;
}

/*
================
STFileDialog::RefreshFileList

Lists the directory again and makes each file's button send
ET_ST_FILE_DIALOG_FILE_CHOSEN to the dialog when it is activated.
================
*/
// FUNCTION: STBC 0x0051fa50
void STFileDialog::RefreshFileList( void ) {
	STFileMenu *menu = GetFileMenu();
	if ( menu != NULL ) {
		menu->Refresh();
		for ( int i = 0; i < menu->GetNumChildren(); i++ ) {
			STButton *button = STButton::Cast( menu->GetNthChild( i ) );
			TGEvent *event = new TGEvent;
			event->SetSource( button );
			event->SetDestination( this );
			event->SetEventType( ET_ST_FILE_DIALOG_FILE_CHOSEN );
			button->SetActivationEvent( event );
		}
		STStylizedWindow *window = STStylizedWindow::Cast( TGPane::Cast( GetFirstChild() )->GetNthChild( 2 ) );
		window->InteriorChangedSize( false );
	}
}

/*
================
STFileDialog::CreateTopArea
================
*/
// FUNCTION: STBC 0x0051fb40
TGPane *STFileDialog::CreateTopArea( char *lcars ) {
	float width = 0.0f;
	float height = 0.0f;
	PyEmbed::GetValue( lcars, "FILE_TOP_AREA_WIDTH", "f", &width );
	PyEmbed::GetValue( lcars, "FILE_TOP_AREA_HEIGHT", "f", &height );
	return new TGPane( width, height );
}

/*
================
STFileDialog::CreateContentArea
================
*/
// FUNCTION: STBC 0x0051fbd0
TGPane *STFileDialog::CreateContentArea( char *lcars ) {
	float width;
	float height;
	PyEmbed::GetValue( lcars, "MAIN_MENU_MIDDLE_CONTENT_WIDTH", "f", &width );
	PyEmbed::GetValue( lcars, "MAIN_MENU_MIDDLE_CONTENT_HEIGHT", "f", &height );
	return TGPane::Create( width, height );
}

/*
================
STFileDialog::CreateFileListWindow

The "Files" window: a stylized window of the lcars group's list size
holding the file menu, whose files send ET_ST_FILE_DIALOG_FILE_CHOSEN to
the dialog; its buttons are a radio group.
================
*/
// FUNCTION: STBC 0x0051fc60 ?CreateFileListWindow@STFileDialog@@IAEPAVSTStylizedWindow@@PAD@Z
STStylizedWindow *STFileDialog::CreateFileListWindow( char *lcars ) {
	float width = 0.0f;
	float height = 0.0f;
	PyEmbed::GetValue( lcars, "FILE_LIST_AREA_WIDTH", "f", &width );
	PyEmbed::GetValue( lcars, "FILE_LIST_AREA_HEIGHT", "f", &height );

	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Options.tgl", true );
	TGString *title = database->GetString( "Files" );
	STStylizedWindow *window = new STStylizedWindow( "StylizedWindow", "NoMinimize", title, 0.0f, 0.0f, NULL, false, 0.0f, 0.0f, &g_kInterfaceBorderColor );
	window->SetFixedSize( width, height * 0.9f, true );
	window->SetTitleBarThickness( 0.0291667f, true );

	STFileMenu *menu = new STFileMenu( "Files", ".", "*.*" );
	TGEvent *event = new TGEvent;
	event->SetSource( menu );
	event->SetDestination( this );
	event->SetEventType( ET_ST_FILE_DIALOG_FILE_CHOSEN );
	menu->SetActivationEvent( event );
	window->AddChild( menu, 0.0f, 0.0f, false );
	menu->Open( true );
	menu->SetNotCloseable();
	menu->SetFocus( menu->GetFirstChild(), true );
	STSubPane *items = STSubPane::Cast( menu->GetSubPane() );
	items->SetRadioGroup( true );
	g_kLocalizationManager.Unload( database );
	return window;
}

/*
================
STFileDialog::CreateScreenshotArea

A pane of the lcars group's screenshot size whose top and bottom edges are
rounded off: the scripts build a curve for each corner, and a coloured bar
between the two curves of each edge fills it up.
================
*/
// FUNCTION: STBC 0x0051fe50 ?CreateScreenshotArea@STFileDialog@@IAEPAVTGPane@@PAD@Z
// DRAFT: the frame agrees; the bars' x (right + 2 * indent) is summed indent first where retail adds the right edge first, and the bar width's operands load in another order
TGPane *STFileDialog::CreateScreenshotArea( char *lcars ) {
	float width = 0.0f;
	float height = 0.0f;
	float blockHeight = 0.0f;
	PyEmbed::GetValue( lcars, "FILE_SCREENSHOT_AREA_WIDTH", "f", &width );
	PyEmbed::GetValue( lcars, "FILE_SCREENSHOT_AREA_HEIGHT", "f", &height );
	PyEmbed::GetValue( lcars, "MAIN_MENU_HORIZONTAL_BLOCK_HEIGHT", "f", &blockHeight );
	TGPane *area = new TGPane( width, height );

	NiColorA barColor = g_kSTMenu2HighlightedBase;
	// The icon group is looked up and not used.
	g_kIconManager.GetIconGroup( lcars );
	// BUG: the colour object is never released.
	PyObject *curveColor = PyEmbed::Unknown6F9920( &g_kSTMenu1HighlightedBase, "NiColorA" );

	int upperLeft;
	int upperRight;
	int lowerLeft;
	int lowerRight;
	float smallWidth;
	float smallHeight;
	float innerWidth;
	float innerHeight;
	float innerIconWidth;
	float innerIconHeight;
	float outerIconWidth;
	float outerIconHeight;
	PyEmbed::GetValue( "UIHelpers", "UPPER_LEFT_CURVE", "i", &upperLeft );
	PyEmbed::GetValue( "UIHelpers", "UPPER_RIGHT_CURVE", "i", &upperRight );
	PyEmbed::GetValue( "UIHelpers", "LOWER_LEFT_CURVE", "i", &lowerLeft );
	PyEmbed::GetValue( "UIHelpers", "LOWER_RIGHT_CURVE", "i", &lowerRight );
	PyEmbed::GetValue( lcars, "MAIN_MENU_SMALL_CURVE_WIDTH", "f", &smallWidth );
	PyEmbed::GetValue( lcars, "MAIN_MENU_SMALL_CURVE_HEIGHT", "f", &smallHeight );
	PyEmbed::GetValue( lcars, "MAIN_MENU_SMALL_CURVE_INNER_WIDTH", "f", &innerWidth );
	PyEmbed::GetValue( lcars, "MAIN_MENU_SMALL_CURVE_INNER_HEIGHT", "f", &innerHeight );
	PyEmbed::GetValue( lcars, "MAIN_MENU_SMALL_CURVE_INNER_ICON_WIDTH", "f", &innerIconWidth );
	PyEmbed::GetValue( lcars, "MAIN_MENU_SMALL_CURVE_INNER_ICON_HEIGHT", "f", &innerIconHeight );
	PyEmbed::GetValue( lcars, "MAIN_MENU_SMALL_CURVE_OUTER_ICON_WIDTH", "f", &outerIconWidth );
	PyEmbed::GetValue( lcars, "MAIN_MENU_SMALL_CURVE_OUTER_ICON_HEIGHT", "f", &outerIconHeight );

	PyObject *curve;
	PyEmbed::CallFunction( "UIHelpers", "CreateCurve", "O", &curve, "iffffOf", upperLeft, innerWidth, smallWidth, innerHeight, smallHeight, curveColor, outerIconWidth );
	TGPane *upperLeftCurve = (TGPane *)PyEmbed::GetObjectPointer( curve );
	Py_DECREF( curve );
	PyEmbed::CallFunction( "UIHelpers", "CreateCurve", "O", &curve, "iffffOf", upperRight, innerWidth, smallWidth, innerHeight, smallHeight, curveColor, outerIconWidth );
	TGPane *upperRightCurve = (TGPane *)PyEmbed::GetObjectPointer( curve );
	Py_DECREF( curve );

	TGIcon *upperBar = new TGIcon( lcars, 200, &barColor );
	upperBar->Resize( area->GetWidth() - ( upperLeftCurve->GetWidth() + upperRightCurve->GetWidth() + DEFAULT_ST_INDENT_HORIZ * 4.0f ), blockHeight, false );
	area->AddChild( upperLeftCurve, 0.0f, 0.0f, false );
	area->AddChild( upperBar, upperLeftCurve->GetRight() + DEFAULT_ST_INDENT_HORIZ * 2.0f, 0.0f, false );
	area->AddChild( upperRightCurve, upperBar->GetRight() + DEFAULT_ST_INDENT_HORIZ * 2.0f, 0.0f, false );

	PyEmbed::CallFunction( "UIHelpers", "CreateCurve", "O", &curve, "iffffOf", lowerLeft, innerWidth, smallWidth, innerHeight, smallHeight, curveColor, outerIconWidth );
	TGPane *lowerLeftCurve = (TGPane *)PyEmbed::GetObjectPointer( curve );
	Py_DECREF( curve );
	PyEmbed::CallFunction( "UIHelpers", "CreateCurve", "O", &curve, "iffffOf", lowerRight, innerWidth, smallWidth, innerHeight, smallHeight, curveColor, outerIconWidth );
	TGPane *lowerRightCurve = (TGPane *)PyEmbed::GetObjectPointer( curve );
	Py_DECREF( curve );

	TGIcon *lowerBar = new TGIcon( lcars, 200, &barColor );
	lowerBar->Resize( area->GetWidth() - ( lowerLeftCurve->GetWidth() + lowerRightCurve->GetWidth() + DEFAULT_ST_INDENT_HORIZ * 4.0f ), blockHeight, false );
	area->AddChild( lowerLeftCurve, 0.0f, area->GetHeight() - lowerLeftCurve->GetHeight(), false );
	area->AddChild( lowerBar, lowerLeftCurve->GetRight() + DEFAULT_ST_INDENT_HORIZ * 2.0f, area->GetHeight() - lowerBar->GetHeight(), false );
	area->AddChild( lowerRightCurve, lowerBar->GetRight() + DEFAULT_ST_INDENT_HORIZ * 2.0f, area->GetHeight() - lowerRightCurve->GetHeight(), false );
	return area;
}

