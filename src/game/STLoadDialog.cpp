/*
===============================================================================

	STLoadDialog.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>
#include <io.h>

#include "STLoadDialog.h"
#include "STButton.h"
#include "STColors.h"
#include "STFileMenu.h"
#include "STRoundedButton.h"
#include "FocusManager.h"
#include "GraphicsModeInfo.h"
#include "KeyboardBinding.h"
#include "ModalDialogWindow.h"
#include "PyEmbed.h"
#include "TGEvent.h"
#include "TGEventHandlerTable.h"
#include "TGIntEvent.h"
#include "TGEventManager.h"
#include "TGKeyboardEvent.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "TGString.h"
#include "TopWindow.h"
#include "UtopiaModule.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00524bc0 _$E57
// FUNCTION: STBC 0x00524bd0 _$E56
// FUNCTION: STBC 0x00524be0 _$E61
// FUNCTION: STBC 0x00524bf0 _$E60
// FUNCTION: STBC 0x00524c00 _$E72
// FUNCTION: STBC 0x00524c10 _$E71
// FUNCTION: STBC 0x00524c20 _$E76
// FUNCTION: STBC 0x00524c30 _$E75
// FUNCTION: STBC 0x00524c40 _$E80
// FUNCTION: STBC 0x00524c50 _$E79
// FUNCTION: STBC 0x00524c60 _$E84
// FUNCTION: STBC 0x00524c70 _$E83
// FUNCTION: STBC 0x00524c80 _$E208
// FUNCTION: STBC 0x00524c90 _$E207
// FUNCTION: STBC 0x00524ca0 _$E150
// FUNCTION: STBC 0x00524cb0 _$E149
// FUNCTION: STBC 0x00524cc0 _$E163
// FUNCTION: STBC 0x00524cd0 _$E162
// FUNCTION: STBC 0x00524ce0 _$E213
// FUNCTION: STBC 0x00524cf0 _$E212
// FUNCTION: STBC 0x00524d00 _$E218
// FUNCTION: STBC 0x00524d10 _$E217
// FUNCTION: STBC 0x00524d20 _$E222
// FUNCTION: STBC 0x00524d30 _$E221
// FUNCTION: STBC 0x00524d40 _$E226
// FUNCTION: STBC 0x00524d50 _$E225
// FUNCTION: STBC 0x00524d60 _$E233
// FUNCTION: STBC 0x00524d70 _$E232
// FUNCTION: STBC 0x00524d80 _$E316
// FUNCTION: STBC 0x00524d90 _$E315
// FUNCTION: STBC 0x00524da0 _$E320
// FUNCTION: STBC 0x00524db0 _$E319
// FUNCTION: STBC 0x00524dc0 _$E108
// FUNCTION: STBC 0x00524dd0 _$E107
// FUNCTION: STBC 0x00524de0 _$E302
// FUNCTION: STBC 0x00524df0 _$E301
// FUNCTION: STBC 0x00524e00 _$E309
// FUNCTION: STBC 0x00524e10 _$E308
// FUNCTION: STBC 0x00524e20 _$E332
// FUNCTION: STBC 0x00524e30 _$E331
// FUNCTION: STBC 0x00524e40 _$E336
// FUNCTION: STBC 0x00524e50 _$E335
// FUNCTION: STBC 0x00524e60 _$E376
// FUNCTION: STBC 0x00524e70 _$E375
// FUNCTION: STBC 0x00524e80 _$E382
// FUNCTION: STBC 0x00524e90 _$E381
// FUNCTION: STBC 0x00524ea0 _$E324
// FUNCTION: STBC 0x00524eb0 _$E323
// FUNCTION: STBC 0x00524ec0 _$E328
// FUNCTION: STBC 0x00524ed0 _$E327
// FUNCTION: STBC 0x00524ee0 _$E340
// FUNCTION: STBC 0x00524ef0 _$E339
// FUNCTION: STBC 0x00524f00 _$E344
// FUNCTION: STBC 0x00524f10 _$E343
// FUNCTION: STBC 0x00524f20 _$E348
// FUNCTION: STBC 0x00524f30 _$E347
// FUNCTION: STBC 0x00524f40 _$E352
// FUNCTION: STBC 0x00524f50 _$E351
// FUNCTION: STBC 0x00524f60 _$E356
// FUNCTION: STBC 0x00524f70 _$E355
// FUNCTION: STBC 0x00524f80 _$E360
// FUNCTION: STBC 0x00524f90 _$E359
// FUNCTION: STBC 0x00524fa0 _$E364
// FUNCTION: STBC 0x00524fb0 _$E363
// FUNCTION: STBC 0x00524fc0 _$E368
// FUNCTION: STBC 0x00524fd0 _$E367
// FUNCTION: STBC 0x00524fe0 _$E113
// FUNCTION: STBC 0x00524ff0 _$E112
// FUNCTION: STBC 0x00525000 _$E372
// FUNCTION: STBC 0x00525010 _$E371
// FUNCTION: STBC 0x00525020 _$E118
// FUNCTION: STBC 0x00525030 _$E117
// FUNCTION: STBC 0x00525040 _$E386
// FUNCTION: STBC 0x00525050 _$E385
// FUNCTION: STBC 0x00525060 _$E390
// FUNCTION: STBC 0x00525070 _$E389
// FUNCTION: STBC 0x00525080 _$E394
// FUNCTION: STBC 0x00525090 _$E393
// FUNCTION: STBC 0x005250a0 _$E398
// FUNCTION: STBC 0x005250b0 _$E397
// FUNCTION: STBC 0x005250c0 _$E127
// FUNCTION: STBC 0x005250d0 _$E126
// FUNCTION: STBC 0x005250e0 _$E402
// FUNCTION: STBC 0x005250f0 _$E401
// FUNCTION: STBC 0x00525100 _$E406
// FUNCTION: STBC 0x00525110 _$E405
// FUNCTION: STBC 0x00525120 _$E410
// FUNCTION: STBC 0x00525130 _$E409
// FUNCTION: STBC 0x00525140 _$E133
// FUNCTION: STBC 0x00525150 _$E132
// FUNCTION: STBC 0x00525160 _$E138
// FUNCTION: STBC 0x00525170 _$E137
// FUNCTION: STBC 0x00525180 _$E168
// FUNCTION: STBC 0x00525190 _$E167
// FUNCTION: STBC 0x005251a0 _$E414
// FUNCTION: STBC 0x005251b0 _$E413
// FUNCTION: STBC 0x005251c0 _$E173
// FUNCTION: STBC 0x005251d0 _$E172
// FUNCTION: STBC 0x00525210 _$E418
// FUNCTION: STBC 0x00525220 _$E417
// FUNCTION: STBC 0x00525230 _$E422
// FUNCTION: STBC 0x00525240 _$E421
// FUNCTION: STBC 0x00525250 _$E178
// FUNCTION: STBC 0x00525260 _$E177
// FUNCTION: STBC 0x00525270 _$E426
// FUNCTION: STBC 0x00525280 _$E425
// FUNCTION: STBC 0x00525290 _$E143
// FUNCTION: STBC 0x005252a0 _$E142
// FUNCTION: STBC 0x005252b0 _$E430
// FUNCTION: STBC 0x005252c0 _$E429
// FUNCTION: STBC 0x005252d0 _$E434
// FUNCTION: STBC 0x005252e0 _$E433
// FUNCTION: STBC 0x005252f0 _$E183
// FUNCTION: STBC 0x00525300 _$E182
// FUNCTION: STBC 0x00525310 _$E188
// FUNCTION: STBC 0x00525320 _$E187
// FUNCTION: STBC 0x00525330 _$E438
// FUNCTION: STBC 0x00525340 _$E437
// FUNCTION: STBC 0x00525350 _$E442
// FUNCTION: STBC 0x00525360 _$E441
// FUNCTION: STBC 0x00525370 _$E446
// FUNCTION: STBC 0x00525380 _$E445
// FUNCTION: STBC 0x00525390 _$E450
// FUNCTION: STBC 0x005253a0 _$E449
// FUNCTION: STBC 0x005253b0 _$E454
// FUNCTION: STBC 0x005253c0 _$E453
// FUNCTION: STBC 0x005253d0 _$E458
// FUNCTION: STBC 0x005253e0 _$E457
// FUNCTION: STBC 0x005253f0 _$E462
// FUNCTION: STBC 0x00525400 _$E461
// FUNCTION: STBC 0x00525410 _$E466
// FUNCTION: STBC 0x00525420 _$E465
// FUNCTION: STBC 0x00525430 _$E193
// FUNCTION: STBC 0x00525440 _$E192
// FUNCTION: STBC 0x00525450 _$E470
// FUNCTION: STBC 0x00525460 _$E469
// FUNCTION: STBC 0x00525470 _$E198
// FUNCTION: STBC 0x00525480 _$E197
// FUNCTION: STBC 0x00525490 _$E203
// FUNCTION: STBC 0x005254a0 _$E202
// FUNCTION: STBC 0x005254b0 _$E474
// FUNCTION: STBC 0x005254c0 _$E473
// FUNCTION: STBC 0x005254d0 _$E479
// FUNCTION: STBC 0x005254e0 _$E478
// FUNCTION: STBC 0x005254f0 _$E483
// FUNCTION: STBC 0x00525500 _$E482

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00523dd0 _$E489
// FUNCTION: STBC 0x00523de0 _$E485
// FUNCTION: STBC 0x00523df0 _$E488
// FUNCTION: STBC 0x00523e00 _$E486

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00524a80 _$E32
// FUNCTION: STBC 0x00524a90 _$E29
// FUNCTION: STBC 0x00524aa0 _$E31
// FUNCTION: STBC 0x00524ab0 _$E30
// FUNCTION: STBC 0x00524ac0 _$E38
// FUNCTION: STBC 0x00524ad0 _$E35
// FUNCTION: STBC 0x00524ae0 _$E37
// FUNCTION: STBC 0x00524af0 _$E36
// FUNCTION: STBC 0x00523a40 _$E497
// FUNCTION: STBC 0x00523a60 _$E496

// DATA: STBC 0x009890a0
TGEventHandlerTable STLoadDialog::classHandlers;

/*
================
STLoadDialog::STLoadDialog
================
*/
// FUNCTION: STBC 0x00523e20 ??0STLoadDialog@@QAE@MM@Z
STLoadDialog::STLoadDialog( float width, float height ) : STFileDialog( width, height ) {
	CreateButtons();
}

/*
================
STLoadDialog::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00523f70
void STLoadDialog::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STLoadDialog::KeyboardHandler, "STLoadDialog::KeyboardHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STLoadDialog::HandleFileSelection, "STLoadDialog::HandleFileSelection" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STLoadDialog::HandleLoadButton, "STLoadDialog::HandleLoadButton" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STLoadDialog::HandleDeleteButton, "STLoadDialog::HandleDeleteButton" );
}

/*
================
STLoadDialog::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00523fd0
void STLoadDialog::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "STLoadDialog::KeyboardHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_ST_FILE_DIALOG_FILE_CHOSEN, "STLoadDialog::HandleFileSelection" );
	AddMethodHandlerForClass( &classHandlers, ET_ST_LOAD_DIALOG_LOAD, "STLoadDialog::HandleLoadButton" );
	AddMethodHandlerForClass( &classHandlers, ET_ST_FILE_DIALOG_DELETE, "STLoadDialog::HandleDeleteButton" );
	AddMethodHandlerForClass( &classHandlers, ET_ST_FILE_DIALOG_DELETE2, "STLoadDialog::HandleDeleteButton" );
}

/*
================
STLoadDialog::CreateButtons

Adds the Load Game and Delete buttons, below one another and disabled until
a file is chosen, to the dialog's second pane (invented name).
================
*/
// FUNCTION: STBC 0x00524040
void STLoadDialog::CreateButtons( void ) {
	TGPane *buttonPane = TGPane::Cast( ( (TGPane *)GetNthChild( 0 ) )->GetNthChild( 1 ) );
	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Options.tgl", true );
	char *lcars = GraphicsModeInfo::lcarsStrings[GraphicsModeInfo::currentMode->GetCurrentResolution()];
	float buttonWidth;
	float buttonHeight;
	float verticalSpacing;
	PyEmbed::GetValue( lcars, "MAIN_MENU_TOP_BUTTON_WIDTH", "f", &buttonWidth );
	PyEmbed::GetValue( lcars, "MAIN_MENU_TOP_BUTTON_HEIGHT", "f", &buttonHeight );
	PyEmbed::GetValue( "MainMenu.mainmenu", "g_fVerticalSpacing", "f", &verticalSpacing );

	TGEvent *loadEvent = new TGEvent;
	loadEvent->SetDestination( this );
	loadEvent->SetEventType( ET_ST_LOAD_DIALOG_LOAD );
	STRoundedButton *loadButton = STRoundedButton::CreateW( database->GetString( "Load Game" ), loadEvent, buttonWidth, buttonHeight, false );
	loadButton->SetNormalColor( &g_kMainMenuButtonColor );
	loadButton->SetHighlightedColor( &g_kMainMenuButtonHighlightedColor );
	loadButton->SetSelectedColor( &g_kMainMenuButtonSelectedColor );
	loadButton->SetDisabledColor( &g_kSTMenu1Disabled );
	loadButton->SetColorBasedOnFlags();
	loadEvent->SetSource( loadButton );
	buttonPane->AddChild( loadButton, 0.0f, 0.0f, false );
	buttonPane->SetFocus( loadButton, true );
	g_kFocusManager.AddObjectToTabOrder( loadButton );

	TGEvent *deleteEvent = new TGEvent;
	deleteEvent->SetDestination( this );
	deleteEvent->SetEventType( ET_ST_FILE_DIALOG_DELETE );
	STRoundedButton *deleteButton = STRoundedButton::CreateW( database->GetString( "Delete" ), deleteEvent, buttonWidth, buttonHeight, false );
	deleteButton->SetNormalColor( &g_kMainMenuButtonColor );
	deleteButton->SetHighlightedColor( &g_kMainMenuButtonHighlightedColor );
	deleteButton->SetSelectedColor( &g_kMainMenuButtonSelectedColor );
	deleteButton->SetDisabledColor( &g_kSTMenu1Disabled );
	deleteButton->SetColorBasedOnFlags();
	deleteEvent->SetSource( deleteButton );
	g_kFocusManager.AddObjectToTabOrder( deleteButton );
	buttonPane->AddChild( deleteButton, 0.0f, verticalSpacing + buttonHeight, false );

	loadButton->SetDisabled( true );
	deleteButton->SetDisabled( true );
	g_kLocalizationManager.Unload( database );
}

/*
================
STLoadDialog::HandleFileSelection

The buttons work once a file is chosen.
================
*/
// FUNCTION: STBC 0x005243f0
void STLoadDialog::HandleFileSelection( TGEvent *event ) {
	STButton *chosen = STButton::Cast( GetFileMenu()->GetFocus() );
	TGPane *buttonPane = TGPane::Cast( ( (TGPane *)GetNthChild( 0 ) )->GetNthChild( 1 ) );
	STRoundedButton *loadButton = STRoundedButton::Cast( buttonPane->GetNthChild( 0 ) );
	STRoundedButton *deleteButton = STRoundedButton::Cast( buttonPane->GetNthChild( 1 ) );
	if ( chosen != NULL ) {
		loadButton->SetEnabled( true );
		deleteButton->SetEnabled( true );
		chosen->SetChosen( true, false );
	} else {
		loadButton->SetDisabled( true );
		deleteButton->SetDisabled( true );
	}
	CallNextHandler( event );
}

/*
================
STLoadDialog::HandleLoadButton

Loads the game of the chosen file: the menu's directory, a backslash, the
button's name and the filter's extension.
================
*/
// FUNCTION: STBC 0x005244c0
void STLoadDialog::HandleLoadButton( TGEvent *event ) {
	STFileMenu *menu = GetFileMenu();
	if ( menu == NULL ) {
		CallNextHandler( event );
		return;
	}
	STButton *chosen = STButton::Cast( menu->GetFocus() );
	TGPane *buttonPane = TGPane::Cast( ( (TGPane *)GetNthChild( 0 ) )->GetNthChild( 1 ) );
	STRoundedButton *loadButton = STRoundedButton::Cast( buttonPane->GetNthChild( 0 ) );
	STRoundedButton *deleteButton = STRoundedButton::Cast( buttonPane->GetNthChild( 1 ) );
	if ( chosen != NULL ) {
		TGString name( "" );
		chosen->GetName( &name );
		TGString path = menu->GetDir() + TGString( "\\" ) + name;
		char *fileName = path.Unknown6F56F0();
		char fullName[1024];
		strcpy( fullName, fileName );
		char *filter = menu->GetFilter();
		if ( filter != NULL ) {
			char extension[256];
			strcpy( extension, filter );
			strcat( fullName, extension + 1 );
		}
		g_kUtopiaModule.LoadFromFile( fullName );
		delete fileName;
		loadButton->SetDisabled( true );
		deleteButton->SetDisabled( true );
	}
	CallNextHandler( event );
}

/*
================
STLoadDialog::HandleDeleteButton

The first press asks to confirm; the confirmation deletes the chosen file.
================
*/
// FUNCTION: STBC 0x00524740
void STLoadDialog::HandleDeleteButton( TGEvent *event ) {
	if ( event->GetEventType() == ET_ST_FILE_DIALOG_DELETE ) {
		TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Options.TGL", true );
		TGString *message = database->GetString( "Delete Prompt" );
		MainWindow *modalDialog = TopWindow::GetTopWindow()->FindMainWindow( MWT_MODAL_DIALOG );
		TGIntEvent *reply = new TGIntEvent;
		reply->SetEventType( ET_ST_FILE_DIALOG_DELETE2 );
		reply->SetDestination( this );
		( (ModalDialogWindow *)modalDialog )->Run( NULL, message, database->GetString( "OK" ), reply, database->GetString( "Cancel" ), NULL );
		g_kLocalizationManager.Unload( database );
	} else {
		STFileMenu *menu = GetFileMenu();
		if ( menu == NULL ) {
			CallNextHandler( event );
			return;
		}
		STButton *chosen = STButton::Cast( menu->GetFocus() );
		TGPane *buttonPane = TGPane::Cast( ( (TGPane *)GetNthChild( 0 ) )->GetNthChild( 1 ) );
		STRoundedButton *loadButton = STRoundedButton::Cast( buttonPane->GetNthChild( 0 ) );
		STRoundedButton *deleteButton = STRoundedButton::Cast( buttonPane->GetNthChild( 1 ) );
		if ( chosen != NULL ) {
			TGString name( "" );
			chosen->GetName( &name );
			TGString path = menu->GetDir() + TGString( "\\" ) + name;
			char *fileName = path.Unknown6F56F0();
			char fullName[1024];
			strcpy( fullName, fileName );
			char *filter = menu->GetFilter();
			if ( filter != NULL ) {
				char extension[256];
				strcpy( extension, filter );
				strcat( fullName, extension + 1 );
			}
			_unlink( fullName );
			delete fileName;
			RefreshFileList();
			loadButton->SetDisabled( true );
			deleteButton->SetDisabled( true );
		}
	}
	CallNextHandler( event );
}

/*
================
STLoadDialog::KeyboardHandler

A key bound to an event goes to the file menu; Enter presses the focused
button, or the first.
================
*/
// FUNCTION: STBC 0x00524b00
void STLoadDialog::KeyboardHandler( TGKeyboardEvent *event ) {
	STFileMenu *menu = GetFileMenu();
	if ( menu->HasFocus() ) {
		if ( g_kKeyboardBinding.LaunchEvent( event->GetUnicode(), event->GetKeyState(), menu, NULL, false ) ) {
			event->SetHandled();
			return;
		}
	}
	TGPane *buttonPane = TGPane::Cast( ( (TGPane *)GetNthChild( 0 ) )->GetNthChild( 1 ) );
	if ( event->GetUnicode() == 13 && event->GetKeyState() == KS_NORMAL ) {
		STRoundedButton *button = STRoundedButton::Cast( buttonPane->GetFocus() );
		if ( button == NULL || !button->IsHighlighted() ) {
			button = STRoundedButton::Cast( buttonPane->GetFirstChild() );
		}
		if ( button != NULL ) {
			button->SendActivationEvent();
		}
	}
}
