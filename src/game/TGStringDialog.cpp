/*
===============================================================================

	TGStringDialog.cpp

===============================================================================
*/

#include <iostream>
#include "TGStringDialog.h"
#include "TGString.h"
#include "TGParagraph.h"
#include "TGEvent.h"
#include "TGIntEvent.h"
#include "TGEventManager.h"
#include "TGEventHandlerTable.h"
#include "TGMouseEvent.h"
#include "TGStream.h"
#include "TGUITheme.h"
#include "TGUIThemeManager.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00737290 _$E32
// FUNCTION: STBC 0x007372a0 _$E29
// FUNCTION: STBC 0x007372b0 _$E31
// FUNCTION: STBC 0x007372c0 _$E30
// FUNCTION: STBC 0x007372d0 _$E38
// FUNCTION: STBC 0x007372e0 _$E35
// FUNCTION: STBC 0x007372f0 _$E37
// FUNCTION: STBC 0x00737300 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00737310 _$E57
// FUNCTION: STBC 0x00737320 _$E56
// FUNCTION: STBC 0x00737330 _$E61
// FUNCTION: STBC 0x00737340 _$E60
// FUNCTION: STBC 0x00737350 _$E72
// FUNCTION: STBC 0x00737360 _$E71
// FUNCTION: STBC 0x00737370 _$E76
// FUNCTION: STBC 0x00737380 _$E75
// FUNCTION: STBC 0x00737390 _$E80
// FUNCTION: STBC 0x007373a0 _$E79
// FUNCTION: STBC 0x007373b0 _$E84
// FUNCTION: STBC 0x007373c0 _$E83
// FUNCTION: STBC 0x007373d0 _$E208
// FUNCTION: STBC 0x007373e0 _$E207
// FUNCTION: STBC 0x007373f0 _$E150
// FUNCTION: STBC 0x00737400 _$E149
// FUNCTION: STBC 0x00737410 _$E163
// FUNCTION: STBC 0x00737420 _$E162
// FUNCTION: STBC 0x00737430 _$E213
// FUNCTION: STBC 0x00737440 _$E212
// FUNCTION: STBC 0x00737450 _$E218
// FUNCTION: STBC 0x00737460 _$E217
// FUNCTION: STBC 0x00737470 _$E222
// FUNCTION: STBC 0x00737480 _$E221
// FUNCTION: STBC 0x00737490 _$E226
// FUNCTION: STBC 0x007374a0 _$E225
// FUNCTION: STBC 0x007374b0 _$E233
// FUNCTION: STBC 0x007374c0 _$E232
// FUNCTION: STBC 0x007374d0 _$E271
// FUNCTION: STBC 0x007374e0 _$E270
// FUNCTION: STBC 0x007374f0 _$E275
// FUNCTION: STBC 0x00737500 _$E274
// FUNCTION: STBC 0x00737510 _$E108
// FUNCTION: STBC 0x00737520 _$E107
// FUNCTION: STBC 0x00737530 _$E257
// FUNCTION: STBC 0x00737540 _$E256
// FUNCTION: STBC 0x00737550 _$E264
// FUNCTION: STBC 0x00737560 _$E263
// FUNCTION: STBC 0x00737570 _$E287
// FUNCTION: STBC 0x00737580 _$E286
// FUNCTION: STBC 0x00737590 _$E291
// FUNCTION: STBC 0x007375a0 _$E290
// FUNCTION: STBC 0x007375b0 _$E331
// FUNCTION: STBC 0x007375c0 _$E330
// FUNCTION: STBC 0x007375d0 _$E337
// FUNCTION: STBC 0x007375e0 _$E336
// FUNCTION: STBC 0x007375f0 _$E279
// FUNCTION: STBC 0x00737600 _$E278
// FUNCTION: STBC 0x00737610 _$E283
// FUNCTION: STBC 0x00737620 _$E282
// FUNCTION: STBC 0x00737630 _$E295
// FUNCTION: STBC 0x00737640 _$E294
// FUNCTION: STBC 0x00737650 _$E299
// FUNCTION: STBC 0x00737660 _$E298
// FUNCTION: STBC 0x00737670 _$E303
// FUNCTION: STBC 0x00737680 _$E302
// FUNCTION: STBC 0x00737690 _$E307
// FUNCTION: STBC 0x007376a0 _$E306
// FUNCTION: STBC 0x007376b0 _$E311
// FUNCTION: STBC 0x007376c0 _$E310
// FUNCTION: STBC 0x007376d0 _$E315
// FUNCTION: STBC 0x007376e0 _$E314
// FUNCTION: STBC 0x007376f0 _$E319
// FUNCTION: STBC 0x00737700 _$E318
// FUNCTION: STBC 0x00737710 _$E323
// FUNCTION: STBC 0x00737720 _$E322
// FUNCTION: STBC 0x00737730 _$E113
// FUNCTION: STBC 0x00737740 _$E112
// FUNCTION: STBC 0x00737750 _$E327
// FUNCTION: STBC 0x00737760 _$E326
// FUNCTION: STBC 0x00737770 _$E118
// FUNCTION: STBC 0x00737780 _$E117
// FUNCTION: STBC 0x00737790 _$E341
// FUNCTION: STBC 0x007377a0 _$E340
// FUNCTION: STBC 0x007377b0 _$E345
// FUNCTION: STBC 0x007377c0 _$E344
// FUNCTION: STBC 0x007377d0 _$E349
// FUNCTION: STBC 0x007377e0 _$E348
// FUNCTION: STBC 0x007377f0 _$E353
// FUNCTION: STBC 0x00737800 _$E352
// FUNCTION: STBC 0x00737810 _$E127
// FUNCTION: STBC 0x00737820 _$E126
// FUNCTION: STBC 0x00737830 _$E357
// FUNCTION: STBC 0x00737840 _$E356
// FUNCTION: STBC 0x00737850 _$E361
// FUNCTION: STBC 0x00737860 _$E360
// FUNCTION: STBC 0x00737870 _$E365
// FUNCTION: STBC 0x00737880 _$E364
// FUNCTION: STBC 0x00737890 _$E133
// FUNCTION: STBC 0x007378a0 _$E132
// FUNCTION: STBC 0x007378b0 _$E138
// FUNCTION: STBC 0x007378c0 _$E137
// FUNCTION: STBC 0x007378d0 _$E168
// FUNCTION: STBC 0x007378e0 _$E167
// FUNCTION: STBC 0x007378f0 _$E369
// FUNCTION: STBC 0x00737900 _$E368
// FUNCTION: STBC 0x00737910 _$E173
// FUNCTION: STBC 0x00737920 _$E172
// FUNCTION: STBC 0x00737930 _$E373
// FUNCTION: STBC 0x00737940 _$E372
// FUNCTION: STBC 0x00737950 _$E377
// FUNCTION: STBC 0x00737960 _$E376
// FUNCTION: STBC 0x00737970 _$E178
// FUNCTION: STBC 0x00737980 _$E177
// FUNCTION: STBC 0x00737990 _$E381
// FUNCTION: STBC 0x007379a0 _$E380
// FUNCTION: STBC 0x007379b0 _$E143
// FUNCTION: STBC 0x007379c0 _$E142
// FUNCTION: STBC 0x007379d0 _$E385
// FUNCTION: STBC 0x007379e0 _$E384
// FUNCTION: STBC 0x007379f0 _$E389
// FUNCTION: STBC 0x00737a00 _$E388
// FUNCTION: STBC 0x00737a10 _$E183
// FUNCTION: STBC 0x00737a20 _$E182
// FUNCTION: STBC 0x00737a30 _$E188
// FUNCTION: STBC 0x00737a40 _$E187
// FUNCTION: STBC 0x00737a50 _$E393
// FUNCTION: STBC 0x00737a60 _$E392
// FUNCTION: STBC 0x00737a70 _$E397
// FUNCTION: STBC 0x00737a80 _$E396
// FUNCTION: STBC 0x00737a90 _$E401
// FUNCTION: STBC 0x00737aa0 _$E400
// FUNCTION: STBC 0x00737ab0 _$E405
// FUNCTION: STBC 0x00737ac0 _$E404
// FUNCTION: STBC 0x00737ad0 _$E409
// FUNCTION: STBC 0x00737ae0 _$E408
// FUNCTION: STBC 0x00737af0 _$E413
// FUNCTION: STBC 0x00737b00 _$E412
// FUNCTION: STBC 0x00737b10 _$E417
// FUNCTION: STBC 0x00737b20 _$E416
// FUNCTION: STBC 0x00737b30 _$E421
// FUNCTION: STBC 0x00737b40 _$E420
// FUNCTION: STBC 0x00737b50 _$E193
// FUNCTION: STBC 0x00737b60 _$E192
// FUNCTION: STBC 0x00737b70 _$E425
// FUNCTION: STBC 0x00737b80 _$E424
// FUNCTION: STBC 0x00737b90 _$E198
// FUNCTION: STBC 0x00737ba0 _$E197
// FUNCTION: STBC 0x00737bb0 _$E203
// FUNCTION: STBC 0x00737bc0 _$E202

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x00737bd0 _$E431
// FUNCTION: STBC 0x00737be0 _$E427
// FUNCTION: STBC 0x00737bf0 _$E430
// FUNCTION: STBC 0x00737c00 _$E428
// DATA: STBC 0x0099dbc8
TGEventHandlerTable TGStringDialog::classHandlers;

/*
================
TGStringDialog::TGStringDialog

A dialog with the label above the paragraph that takes the string,
which has the focus.
================
*/
// FUNCTION: STBC 0x00737c20 ??0TGStringDialog@@QAE@MMPAD0M0M00PAVTGEventHandlerObject@@1@Z
TGStringDialog::TGStringDialog( float width, float height, char *message, char *frameName, float frameSize,
		char *buttonFrameName, float buttonFrameSize, char *label, char *string,
		TGEventHandlerObject *destination, TGEventHandlerObject *stringDestination )
	: TGDialogWindow( width, height, message, frameName, frameSize, buttonFrameName, buttonFrameSize, TGSTRINGDIALOG_DIALOG_FLAGS, destination ),
	paragraph( NULL ), unknown64( stringDestination ) {
	if ( stringDestination == NULL ) {
		unknown64 = this;
	}
	// The label's and the string's TGStrings are never deleted.
	TGString *labelString = new TGString( label );
	TGParagraph *labelText = new TGParagraph( labelString, 1.0f, NULL, "", 0.0f, TGPF_READ_ONLY );
	AddChild( labelText );
	TGString *paragraphString = new TGString( string );
	paragraph = new TGParagraph( paragraphString, 1.0f, NULL, "", 0.0f, 0 );
	AddChild( paragraph, 0.0f, labelText->GetHeight() * 1.1f, false );
	TGWindow::MoveToFront( paragraph, false );
	TGWindow::SetFocus( paragraph, true );
}

/*
================
TGStringDialog::Create
================
*/
// FUNCTION: STBC 0x00737dd0
TGStringDialog *TGStringDialog::Create( float width, float height, char *message, char *frameName, float frameSize,
		char *buttonFrameName, float buttonFrameSize, char *label, char *string,
		TGEventHandlerObject *destination, TGEventHandlerObject *stringDestination ) {
	return new TGStringDialog( width, height, message, frameName, frameSize, buttonFrameName, buttonFrameSize, label, string,
			destination, stringDestination );
}

/*
================
TGStringDialog::~TGStringDialog
================
*/
// FUNCTION: STBC 0x00737e30
TGStringDialog::~TGStringDialog( void ) {
}

/*
================
TGStringDialog::RegisterHandlers

The dialog takes TGDialogWindow's mouse handler and its own dialog event
handler.
================
*/
// FUNCTION: STBC 0x00737e40
void TGStringDialog::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_MOUSE, "TGDialogWindow::MouseHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_DIALOG, "TGStringDialog::DialogEventHandler" );
}

/*
================
TGStringDialog::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00737e70
void TGStringDialog::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGStringDialog::DialogEventHandler, "TGStringDialog::DialogEventHandler" );
}

/*
================
TGStringDialog::DialogEventHandler

Sends a copy of the event, with the dialog's event type, when its value
is 1 or 2; a copy of any other value is never freed.
================
*/
// FUNCTION: STBC 0x00737e90
void TGStringDialog::DialogEventHandler( TGEvent *event ) {
	int value = ( (TGIntEvent *)event )->GetInt();
	TGEvent *reply = event->Duplicate();
	if ( eventType == ET_DIALOG_RESULT ) {
		reply->SetDestination( destination );
		reply->SetSource( unknown64 );
	} else {
		reply->SetDestination( unknown64 );
		reply->SetSource( this );
	}
	reply->SetEventType( eventType );
	if ( value > 0 && value <= 2 ) {
		g_kEventManager.AddEvent( reply );
	}
}

/*
================
TGStringDialog::GotFocus

The paragraph comes to the front.
================
*/
// FUNCTION: STBC 0x00737f00
void TGStringDialog::GotFocus( bool layout ) {
	MoveToFront( paragraph, layout );
}

/*
================
TGStringDialog::GetCString
================
*/
// FUNCTION: STBC 0x00737f20
char *TGStringDialog::GetCString( void ) {
	return paragraph->GetCString();
}

/*
================
TGStringDialog::GetString
================
*/
// FUNCTION: STBC 0x00737f30
void TGStringDialog::GetString( TGString *string ) {
	paragraph->GetString( string );
}

/*
================
TGStringDialog::CreateFromTheme

A string dialog in the current theme's dialog frame and button frames;
NULL when no theme is current. As TGDialogWindow::CreateFromTheme, the
new dialog reaches the caller only through EAX.
================
*/
// FUNCTION: STBC 0x00737f50
TGStringDialog *TGStringDialog::CreateFromTheme( float width, float height, char *message, char *label, char *string,
		TGEventHandlerObject *destination, TGEventHandlerObject *stringDestination ) {
	TGUITheme *theme = g_kUIThemeManager.GetCurrentTheme();
	if ( theme == NULL ) {
		return NULL;
	}
	// BUG: the result is not returned; Create's value is left in EAX
	Create( width, height, message, theme->GetDialogFrameName(), theme->GetDialogFrameScale(),
		theme->GetButtonFrameName(), theme->GetButtonFrameScale(), label, string, destination, stringDestination );
}

/*
================
TGStringDialog::WriteToStream
================
*/
// FUNCTION: STBC 0x00737fa0
bool TGStringDialog::WriteToStream( TGStream *stream ) {
	if ( !TGDialogWindow::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( paragraph->GetObjID() );
	if ( unknown64 != NULL ) {
		stream->WriteID( unknown64->GetObjID() );
	} else {
		stream->WriteID( 0 );
	}
	return true;
}

/*
================
TGStringDialog::ReadFromStream
================
*/
// FUNCTION: STBC 0x00738000
bool TGStringDialog::ReadFromStream( TGStream *stream ) {
	if ( !TGDialogWindow::ReadFromStream( stream ) ) {
		return false;
	}
	paragraph = (TGParagraph *)stream->ReadID();
	unknown64 = (TGEventHandlerObject *)stream->ReadID();
	return true;
}

/*
================
TGStringDialog::FixUpPointers
================
*/
// FUNCTION: STBC 0x00738040
bool TGStringDialog::FixUpPointers( void ) {
	if ( !TGDialogWindow::FixUpPointers() ) {
		return false;
	}
	if ( TGObject::GetTGObjectPtr( (long)paragraph ) == NULL ) {
		return false;
	}
	if ( unknown64 != NULL && TGObject::GetTGObjectPtr( (long)unknown64 ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TGStringDialog::PostFixUp
================
*/
// FUNCTION: STBC 0x00738080
void TGStringDialog::PostFixUp( void ) {
	TGDialogWindow::PostFixUp();
	paragraph = (TGParagraph *)TGObject::GetTGObjectPtr( (long)paragraph );
	if ( unknown64 != NULL ) {
		unknown64 = (TGEventHandlerObject *)TGObject::GetTGObjectPtr( (long)unknown64 );
	}
}
