/*
===============================================================================

	STSaveDialog.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>
#include <io.h>

#include "STSaveDialog.h"
#include "STButton.h"
#include "STColors.h"
#include "STFileMenu.h"
#include "STRoundedButton.h"
#include "FocusManager.h"
#include "GraphicsModeInfo.h"
#include "InterfaceModule.h"
#include "KeyboardBinding.h"
#include "ModalDialogWindow.h"
#include "PyEmbed.h"
#include "TGEvent.h"
#include "TGEventHandlerTable.h"
#include "TGIntEvent.h"
#include "TGEventManager.h"
#include "TGKeyboardEvent.h"
#include "TGMouseEvent.h"
#include "TGStringEvent.h"
#include "TGParagraph.h"
#include "TGIcon.h"
#include "TGPane.h"
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
// FUNCTION: STBC 0x005303b0 _$E57
// FUNCTION: STBC 0x005303c0 _$E56
// FUNCTION: STBC 0x005303d0 _$E61
// FUNCTION: STBC 0x005303e0 _$E60
// FUNCTION: STBC 0x005303f0 _$E72
// FUNCTION: STBC 0x00530400 _$E71
// FUNCTION: STBC 0x00530410 _$E76
// FUNCTION: STBC 0x00530420 _$E75
// FUNCTION: STBC 0x00530430 _$E80
// FUNCTION: STBC 0x00530440 _$E79
// FUNCTION: STBC 0x00530450 _$E84
// FUNCTION: STBC 0x00530460 _$E83
// FUNCTION: STBC 0x00530470 _$E208
// FUNCTION: STBC 0x00530480 _$E207
// FUNCTION: STBC 0x00530490 _$E150
// FUNCTION: STBC 0x005304a0 _$E149
// FUNCTION: STBC 0x005304b0 _$E163
// FUNCTION: STBC 0x005304c0 _$E162
// FUNCTION: STBC 0x005304d0 _$E213
// FUNCTION: STBC 0x005304e0 _$E212
// FUNCTION: STBC 0x005304f0 _$E218
// FUNCTION: STBC 0x00530500 _$E217
// FUNCTION: STBC 0x00530510 _$E222
// FUNCTION: STBC 0x00530520 _$E221
// FUNCTION: STBC 0x00530530 _$E226
// FUNCTION: STBC 0x00530540 _$E225
// FUNCTION: STBC 0x00530550 _$E233
// FUNCTION: STBC 0x00530560 _$E232
// FUNCTION: STBC 0x00530570 _$E317
// FUNCTION: STBC 0x00530580 _$E316
// FUNCTION: STBC 0x00530590 _$E321
// FUNCTION: STBC 0x005305a0 _$E320
// FUNCTION: STBC 0x005305b0 _$E108
// FUNCTION: STBC 0x005305c0 _$E107
// FUNCTION: STBC 0x005305d0 _$E303
// FUNCTION: STBC 0x005305e0 _$E302
// FUNCTION: STBC 0x005305f0 _$E310
// FUNCTION: STBC 0x00530600 _$E309
// FUNCTION: STBC 0x00530610 _$E333
// FUNCTION: STBC 0x00530620 _$E332
// FUNCTION: STBC 0x00530630 _$E337
// FUNCTION: STBC 0x00530640 _$E336
// FUNCTION: STBC 0x00530650 _$E377
// FUNCTION: STBC 0x00530660 _$E376
// FUNCTION: STBC 0x00530670 _$E383
// FUNCTION: STBC 0x00530680 _$E382
// FUNCTION: STBC 0x00530690 _$E325
// FUNCTION: STBC 0x005306a0 _$E324
// FUNCTION: STBC 0x005306b0 _$E329
// FUNCTION: STBC 0x005306c0 _$E328
// FUNCTION: STBC 0x005306d0 _$E341
// FUNCTION: STBC 0x005306e0 _$E340
// FUNCTION: STBC 0x005306f0 _$E345
// FUNCTION: STBC 0x00530700 _$E344
// FUNCTION: STBC 0x00530710 _$E349
// FUNCTION: STBC 0x00530720 _$E348
// FUNCTION: STBC 0x00530730 _$E353
// FUNCTION: STBC 0x00530740 _$E352
// FUNCTION: STBC 0x00530750 _$E357
// FUNCTION: STBC 0x00530760 _$E356
// FUNCTION: STBC 0x00530770 _$E361
// FUNCTION: STBC 0x00530780 _$E360
// FUNCTION: STBC 0x00530790 _$E365
// FUNCTION: STBC 0x005307a0 _$E364
// FUNCTION: STBC 0x005307b0 _$E369
// FUNCTION: STBC 0x005307c0 _$E368
// FUNCTION: STBC 0x005307d0 _$E113
// FUNCTION: STBC 0x005307e0 _$E112
// FUNCTION: STBC 0x005307f0 _$E373
// FUNCTION: STBC 0x00530800 _$E372
// FUNCTION: STBC 0x00530810 _$E118
// FUNCTION: STBC 0x00530820 _$E117
// FUNCTION: STBC 0x00530830 _$E387
// FUNCTION: STBC 0x00530840 _$E386
// FUNCTION: STBC 0x00530850 _$E391
// FUNCTION: STBC 0x00530860 _$E390
// FUNCTION: STBC 0x00530870 _$E395
// FUNCTION: STBC 0x00530880 _$E394
// FUNCTION: STBC 0x00530890 _$E399
// FUNCTION: STBC 0x005308a0 _$E398
// FUNCTION: STBC 0x005308b0 _$E127
// FUNCTION: STBC 0x005308c0 _$E126
// FUNCTION: STBC 0x005308d0 _$E403
// FUNCTION: STBC 0x005308e0 _$E402
// FUNCTION: STBC 0x005308f0 _$E407
// FUNCTION: STBC 0x00530900 _$E406
// FUNCTION: STBC 0x00530910 _$E411
// FUNCTION: STBC 0x00530920 _$E410
// FUNCTION: STBC 0x00530930 _$E133
// FUNCTION: STBC 0x00530940 _$E132
// FUNCTION: STBC 0x00530950 _$E138
// FUNCTION: STBC 0x00530960 _$E137
// FUNCTION: STBC 0x00530970 _$E168
// FUNCTION: STBC 0x00530980 _$E167
// FUNCTION: STBC 0x005309c0 _$E415
// FUNCTION: STBC 0x005309d0 _$E414
// FUNCTION: STBC 0x005309e0 _$E173
// FUNCTION: STBC 0x005309f0 _$E172
// FUNCTION: STBC 0x00530a00 _$E419
// FUNCTION: STBC 0x00530a10 _$E418
// FUNCTION: STBC 0x00530a20 _$E423
// FUNCTION: STBC 0x00530a30 _$E422
// FUNCTION: STBC 0x00530a40 _$E178
// FUNCTION: STBC 0x00530a50 _$E177
// FUNCTION: STBC 0x00530a60 _$E427
// FUNCTION: STBC 0x00530a70 _$E426
// FUNCTION: STBC 0x00530a80 _$E143
// FUNCTION: STBC 0x00530a90 _$E142
// FUNCTION: STBC 0x00530aa0 _$E431
// FUNCTION: STBC 0x00530ab0 _$E430
// FUNCTION: STBC 0x00530ac0 _$E435
// FUNCTION: STBC 0x00530ad0 _$E434
// FUNCTION: STBC 0x00530ae0 _$E183
// FUNCTION: STBC 0x00530af0 _$E182
// FUNCTION: STBC 0x00530b00 _$E188
// FUNCTION: STBC 0x00530b10 _$E187
// FUNCTION: STBC 0x00530b20 _$E439
// FUNCTION: STBC 0x00530b30 _$E438
// FUNCTION: STBC 0x00530b40 _$E443
// FUNCTION: STBC 0x00530b50 _$E442
// FUNCTION: STBC 0x00530b60 _$E447
// FUNCTION: STBC 0x00530b70 _$E446
// FUNCTION: STBC 0x00530b80 _$E451
// FUNCTION: STBC 0x00530b90 _$E450
// FUNCTION: STBC 0x00530ba0 _$E455
// FUNCTION: STBC 0x00530bb0 _$E454
// FUNCTION: STBC 0x00530bc0 _$E459
// FUNCTION: STBC 0x00530bd0 _$E458
// FUNCTION: STBC 0x00530be0 _$E463
// FUNCTION: STBC 0x00530bf0 _$E462
// FUNCTION: STBC 0x00530c00 _$E467
// FUNCTION: STBC 0x00530c10 _$E466
// FUNCTION: STBC 0x00530c20 _$E193
// FUNCTION: STBC 0x00530c30 _$E192
// FUNCTION: STBC 0x00530c40 _$E471
// FUNCTION: STBC 0x00530c50 _$E470
// FUNCTION: STBC 0x00530c60 _$E198
// FUNCTION: STBC 0x00530c70 _$E197
// FUNCTION: STBC 0x00530c80 _$E203
// FUNCTION: STBC 0x00530c90 _$E202
// FUNCTION: STBC 0x00530ca0 _$E475
// FUNCTION: STBC 0x00530cb0 _$E474
// FUNCTION: STBC 0x00530cc0 _$E480
// FUNCTION: STBC 0x00530cd0 _$E479
// FUNCTION: STBC 0x00530ce0 _$E484
// FUNCTION: STBC 0x00530cf0 _$E483

// The guard of std::ctype<unsigned short>::id and its exit-time thunk (<iostream>).
// FUNCTION: STBC 0x00530990 _$E498
// FUNCTION: STBC 0x005309b0 _$E497

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0052f5c0 _$E490
// FUNCTION: STBC 0x0052f5d0 _$E486
// FUNCTION: STBC 0x0052f5e0 _$E489
// FUNCTION: STBC 0x0052f5f0 _$E487

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00530330 _$E32
// FUNCTION: STBC 0x00530340 _$E29
// FUNCTION: STBC 0x00530350 _$E31
// FUNCTION: STBC 0x00530360 _$E30
// FUNCTION: STBC 0x00530370 _$E38
// FUNCTION: STBC 0x00530380 _$E35
// FUNCTION: STBC 0x00530390 _$E37
// FUNCTION: STBC 0x005303a0 _$E36

// DATA: STBC 0x00989820
TGEventHandlerTable STSaveDialog::classHandlers;

/*
================
STSaveDialog::STSaveDialog
================
*/
// FUNCTION: STBC 0x0052f610 ??0STSaveDialog@@QAE@MM@Z
STSaveDialog::STSaveDialog( float width, float height ) : STFileDialog( width, height ) {
	CreateControls();
}

/*
================
STSaveDialog::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0052f760
void STSaveDialog::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STSaveDialog::HandleFileSelection, "STSaveDialog::HandleFileSelection" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STSaveDialog::HandleSaveButton, "STSaveDialog::HandleSaveButton" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STSaveDialog::HandleDeleteButton, "STSaveDialog::HandleDeleteButton" );
	g_kEventManager.RegisterHandlerFunction( STSaveDialog::HandleSaveNameMouse, "STSaveDialog::HandleSaveNameMouse" );
}

/*
================
STSaveDialog::RegisterHandlers

The text field's mouse handler is the field's own, not the class's.
================
*/
// FUNCTION: STBC 0x0052f7c0
void STSaveDialog::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_ST_FILE_DIALOG_FILE_CHOSEN, "STSaveDialog::HandleFileSelection" );
	AddMethodHandlerForClass( &classHandlers, ET_ST_SAVE_DIALOG_SAVE, "STSaveDialog::HandleSaveButton" );
	AddMethodHandlerForClass( &classHandlers, ET_ST_FILE_DIALOG_DELETE, "STSaveDialog::HandleDeleteButton" );
}

/*
================
STSaveDialog::CreateControls

Fills the dialog's second pane with the Save Game and Delete buttons, below
one another (Delete disabled until a file is chosen), and its first pane with
the save name's label and text field over the icon; a click on the pane puts
the focus in the field (name invented).
================
*/
// FUNCTION: STBC 0x0052f800
void STSaveDialog::CreateControls( void ) {
	char *lcars = GraphicsModeInfo::lcarsStrings[GraphicsModeInfo::currentMode->GetCurrentResolution()];
	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Options.tgl", true );
	NiColorA iconColor = g_kSTMenu2HighlightedBase;
	TGPane *buttonPane = TGPane::Cast( ( (TGPane *)GetNthChild( 0 ) )->GetNthChild( 1 ) );
	float buttonWidth;
	float buttonHeight;
	float verticalSpacing;
	PyEmbed::GetValue( lcars, "MAIN_MENU_TOP_BUTTON_WIDTH", "f", &buttonWidth );
	PyEmbed::GetValue( lcars, "MAIN_MENU_TOP_BUTTON_HEIGHT", "f", &buttonHeight );
	PyEmbed::GetValue( "MainMenu.mainmenu", "g_fVerticalSpacing", "f", &verticalSpacing );

	TGEvent *saveEvent = new TGEvent;
	saveEvent->SetDestination( this );
	saveEvent->SetEventType( ET_ST_SAVE_DIALOG_SAVE );
	STRoundedButton *saveButton = STRoundedButton::CreateW( database->GetString( "Save Game" ), saveEvent, buttonWidth, buttonHeight, false );
	saveButton->SetNormalColor( &g_kMainMenuButtonColor );
	saveButton->SetHighlightedColor( &g_kMainMenuButtonHighlightedColor );
	saveButton->SetSelectedColor( &g_kMainMenuButtonSelectedColor );
	saveButton->SetDisabledColor( &g_kSTMenu1Disabled );
	saveButton->SetColorBasedOnFlags();
	saveEvent->SetSource( saveButton );
	buttonPane->AddChild( saveButton, 0.0f, 0.0f, false );

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
	buttonPane->AddChild( deleteButton, 0.0f, verticalSpacing + buttonHeight, false );
	deleteButton->SetDisabled( true );

	TGPane *namePane = TGPane::Cast( ( (TGPane *)GetNthChild( 0 ) )->GetNthChild( 0 ) );
	TGIcon *icon = new TGIcon( lcars, 200, &iconColor );
	icon->Resize( namePane->GetWidth(), namePane->GetHeight(), false );
	namePane->AddFuncHandlerForInstance( ET_MOUSE, "STSaveDialog::HandleSaveNameMouse" );

	TGParagraph *label = new TGParagraph( database->GetString( "Saved Game Name:" ), 1.0f, &g_kSTMenuTextColor, "", 0.0f, 0x80000000 );
	label->SetReadOnly( true );
	TGParagraph *nameField = new TGParagraph( "", 1.0f, &g_kSTMenuTextColor, "", 0.0f, 0x80000000 );
	nameField->SetReadOnly( false );
	nameField->SetIgnoreString( "*?\r\n\t\\/.,<>\"|:^" );
	namePane->AddChild( label, DEFAULT_ST_INDENT_HORIZ, DEFAULT_ST_INDENT_VERT, false );
	namePane->AddChild( nameField, DEFAULT_ST_INDENT_HORIZ, label->GetBottom() + DEFAULT_ST_INDENT_VERT, false );
	namePane->InsertChild( 0x7FFFFFFF, icon, 0.0f, 0.0f, false );
	namePane->SetFocus( nameField, false );
	SetFocus( namePane, true );
	g_kLocalizationManager.Unload( database );
}

/*
================
STSaveDialog::HandleFileSelection

A chosen file's name goes into the save name field, and the Delete button
works.
================
*/
// FUNCTION: STBC 0x0052fd80
void STSaveDialog::HandleFileSelection( TGEvent *event ) {
	STButton *chosen = STButton::Cast( GetFileMenu()->GetFocus() );
	TGPane *buttonPane = TGPane::Cast( ( (TGPane *)GetNthChild( 0 ) )->GetNthChild( 1 ) );
	STRoundedButton *deleteButton = STRoundedButton::Cast( buttonPane->GetNthChild( 1 ) );
	TGPane *namePane = TGPane::Cast( ( (TGPane *)GetNthChild( 0 ) )->GetNthChild( 0 ) );
	TGParagraph *nameField = TGParagraph::Cast( namePane->GetNthChild( 1 ) );
	if ( chosen != NULL ) {
		TGString name( "" );
		chosen->GetName( &name );
		nameField->SetString( &name, true );
		deleteButton->SetEnabled( true );
		chosen->SetChosen( true, false );
	} else {
		deleteButton->SetDisabled( true );
	}
	CallNextHandler( event );
}

/*
================
STSaveDialog::HandleSaveButton

Posts the save event for the name in the field: the menu's directory, a
backslash and the name, or "UntitledSave" when the field is empty. The Delete
button is disabled afterwards.
================
*/
// FUNCTION: STBC 0x0052fec0
void STSaveDialog::HandleSaveButton( TGEvent *event ) {
	STFileMenu *menu = GetFileMenu();
	if ( menu == NULL ) {
		CallNextHandler( event );
		return;
	}
	MainWindow *options = TopWindow::GetTopWindow()->FindMainWindow( MWT_OPTIONS );
	TGPane *firstPane = TGPane::Cast( GetNthChild( 0 ) );
	TGParagraph *nameField = TGParagraph::Cast( firstPane->GetNthChild( 1 ) );
	TGPane *buttonPane = TGPane::Cast( ( (TGPane *)GetNthChild( 0 ) )->GetNthChild( 1 ) );
	STRoundedButton *deleteButton = STRoundedButton::Cast( buttonPane->GetNthChild( 1 ) );
	if ( nameField != NULL ) {
		TGString name( "" );
		nameField->GetString( &name );
		if ( name.GetLength() <= 0 ) {
			name.SetString( "UntitledSave" );
		}
		TGString path = menu->GetDir() + TGString( "\\" ) + name;
		char *fileName = path.Unknown6F56F0();
		TGStringEvent *saveEvent = new TGStringEvent;
		saveEvent->SetSource( this );
		saveEvent->SetDestination( options );
		saveEvent->SetEventType( ET_ST_SAVE_DIALOG_SAVE );
		saveEvent->SetString( fileName );
		g_kEventManager.AddEvent( saveEvent );
		delete fileName;
	}
	deleteButton->SetDisabled( true );
	CallNextHandler( event );
}

/*
================
STSaveDialog::HandleDeleteButton

Deletes the chosen file.
================
*/
// FUNCTION: STBC 0x00530120
void STSaveDialog::HandleDeleteButton( TGEvent *event ) {
	STFileMenu *menu = GetFileMenu();
	if ( menu == NULL ) {
		CallNextHandler( event );
		return;
	}
	STButton *chosen = STButton::Cast( menu->GetFocus() );
	TGPane *buttonPane = TGPane::Cast( ( (TGPane *)GetNthChild( 0 ) )->GetNthChild( 1 ) );
	STRoundedButton *deleteButton = STRoundedButton::Cast( buttonPane->GetNthChild( 1 ) );
	if ( chosen != NULL ) {
		TGString name( "" );
		chosen->GetName( &name );
		TGString path = menu->GetDir() + TGString( "\\" ) + name;
		char *fileName = path.Unknown6F56F0();
		_unlink( fileName );
		delete fileName;
		RefreshFileList();
		deleteButton->SetDisabled( true );
	}
	CallNextHandler( event );
}

/*
================
STSaveDialog::HandleSaveNameMouse

A press on the save name's pane puts the focus in its text field, and the
dialog's own focus on the pane.
================
*/
// FUNCTION: STBC 0x005302d0
void STSaveDialog::HandleSaveNameMouse( TGEventHandlerObject *object, TGEvent *event ) {
	TGPane *pane = (TGPane *)object;
	TGMouseEvent *mouse = (TGMouseEvent *)event;
	if ( mouse->IsButtonEvent() && ( mouse->GetFlags() & MEF_BUTTON_DOWN ) ) {
		TGUIObject *nameField = pane->GetNthChild( 1 );
		pane->SetFocus( nameField, false );
		pane->GetParent()->SetFocus( pane, true );
		mouse->SetHandled();
	}
	if ( !mouse->EventHandled() ) {
		object->CallNextHandler( event );
	}
}
