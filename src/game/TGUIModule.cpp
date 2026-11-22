/*
===============================================================================

	TGUIModule.cpp

===============================================================================
*/

#include "TGUIModule.h"
#include "TGInputModule.h"
#include "TGKeyboardEvent.h"
#include "TGMouseEvent.h"
#include "TGUIObject.h"
#include "TGPane.h"
#include "TGRootPane.h"
#include "TGFrame.h"
#include "TGWindow.h"
#include "TGFrameWindow.h"
#include "TGParagraph.h"
#include "TGDialogWindow.h"
#include "TGStringDialog.h"
#include "TGConsole.h"
#include "TGPrompt.h"
#include "TGIcon.h"
#include "TGIconManager.h"
#include "TGButtonBase.h"
#include "TGButton.h"
#include "TGTextButton.h"
#include "TGEventHandlerTable.h"
#include "PyEmbed.h"
#include "NiMain.h"
#include <iostream>

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00724c90 _$E448
// FUNCTION: STBC 0x00724ca0 _$E445
// FUNCTION: STBC 0x00724cb0 _$E447
// FUNCTION: STBC 0x00724cc0 _$E446
// FUNCTION: STBC 0x00724cd0 _$E454
// FUNCTION: STBC 0x00724ce0 _$E451
// FUNCTION: STBC 0x00724cf0 _$E453
// FUNCTION: STBC 0x00724d00 _$E452

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00724d10 _$E23
// FUNCTION: STBC 0x00724d20 _$E22
// FUNCTION: STBC 0x00724d30 _$E30
// FUNCTION: STBC 0x00724d40 _$E29
// FUNCTION: STBC 0x00724d50 _$E41
// FUNCTION: STBC 0x00724d60 _$E40
// FUNCTION: STBC 0x00724d70 _$E45
// FUNCTION: STBC 0x00724d80 _$E44
// FUNCTION: STBC 0x00724d90 _$E49
// FUNCTION: STBC 0x00724da0 _$E48
// FUNCTION: STBC 0x00724db0 _$E53
// FUNCTION: STBC 0x00724dc0 _$E52
// FUNCTION: STBC 0x00724dd0 _$E177
// FUNCTION: STBC 0x00724de0 _$E176
// FUNCTION: STBC 0x00724df0 _$E119
// FUNCTION: STBC 0x00724e00 _$E118
// FUNCTION: STBC 0x00724e10 _$E132
// FUNCTION: STBC 0x00724e20 _$E131
// FUNCTION: STBC 0x00724e50 _$E187
// FUNCTION: STBC 0x00724e60 _$E186
// FUNCTION: STBC 0x00724e90 _$E195
// FUNCTION: STBC 0x00724ea0 _$E194
// FUNCTION: STBC 0x00724eb0 _$E202
// FUNCTION: STBC 0x00724ec0 _$E201
// FUNCTION: STBC 0x00724f10 _$E77
// FUNCTION: STBC 0x00724f20 _$E76
// FUNCTION: STBC 0x00725130 _$E82
// FUNCTION: STBC 0x00725140 _$E81
// FUNCTION: STBC 0x00725170 _$E87
// FUNCTION: STBC 0x00725180 _$E86
// FUNCTION: STBC 0x00725210 _$E96
// FUNCTION: STBC 0x00725220 _$E95
// FUNCTION: STBC 0x00725290 _$E102
// FUNCTION: STBC 0x007252a0 _$E101
// FUNCTION: STBC 0x007252b0 _$E107
// FUNCTION: STBC 0x007252c0 _$E106
// FUNCTION: STBC 0x007252d0 _$E137
// FUNCTION: STBC 0x007252e0 _$E136
// FUNCTION: STBC 0x00725310 _$E142
// FUNCTION: STBC 0x00725320 _$E141
// FUNCTION: STBC 0x00725370 _$E147
// FUNCTION: STBC 0x00725380 _$E146
// FUNCTION: STBC 0x007253b0 _$E112
// FUNCTION: STBC 0x007253c0 _$E111
// FUNCTION: STBC 0x00725410 _$E152
// FUNCTION: STBC 0x00725420 _$E151
// FUNCTION: STBC 0x00725430 _$E157
// FUNCTION: STBC 0x00725440 _$E156
// FUNCTION: STBC 0x00725550 _$E162
// FUNCTION: STBC 0x00725560 _$E161
// FUNCTION: STBC 0x00725590 _$E167
// FUNCTION: STBC 0x007255a0 _$E166
// FUNCTION: STBC 0x007255b0 _$E172
// FUNCTION: STBC 0x007255c0 _$E171
// FUNCTION: STBC 0x00724e30 _$E182
// FUNCTION: STBC 0x00724e40 _$E181
// FUNCTION: STBC 0x00724e70 _$E191
// FUNCTION: STBC 0x00724e80 _$E190
// FUNCTION: STBC 0x00724ed0 _$E289
// FUNCTION: STBC 0x00724ee0 _$E288
// FUNCTION: STBC 0x00724ef0 _$E293
// FUNCTION: STBC 0x00724f00 _$E292
// FUNCTION: STBC 0x00724f30 _$E275
// FUNCTION: STBC 0x00724f40 _$E274
// FUNCTION: STBC 0x00724f50 _$E282
// FUNCTION: STBC 0x00724f60 _$E281
// FUNCTION: STBC 0x00724f70 _$E305
// FUNCTION: STBC 0x00724f80 _$E304
// FUNCTION: STBC 0x00724f90 _$E309
// FUNCTION: STBC 0x00724fa0 _$E308
// FUNCTION: STBC 0x00724fb0 _$E349
// FUNCTION: STBC 0x00724fc0 _$E348
// FUNCTION: STBC 0x00724fd0 _$E355
// FUNCTION: STBC 0x00724fe0 _$E354
// FUNCTION: STBC 0x00724ff0 _$E297
// FUNCTION: STBC 0x00725000 _$E296
// FUNCTION: STBC 0x00725010 _$E301
// FUNCTION: STBC 0x00725020 _$E300
// FUNCTION: STBC 0x00725030 _$E313
// FUNCTION: STBC 0x00725040 _$E312
// FUNCTION: STBC 0x00725050 _$E317
// FUNCTION: STBC 0x00725060 _$E316
// FUNCTION: STBC 0x00725070 _$E321
// FUNCTION: STBC 0x00725080 _$E320
// FUNCTION: STBC 0x00725090 _$E325
// FUNCTION: STBC 0x007250a0 _$E324
// FUNCTION: STBC 0x007250b0 _$E329
// FUNCTION: STBC 0x007250c0 _$E328
// FUNCTION: STBC 0x007250d0 _$E333
// FUNCTION: STBC 0x007250e0 _$E332
// FUNCTION: STBC 0x007250f0 _$E337
// FUNCTION: STBC 0x00725100 _$E336
// FUNCTION: STBC 0x00725110 _$E341
// FUNCTION: STBC 0x00725120 _$E340
// FUNCTION: STBC 0x00725150 _$E345
// FUNCTION: STBC 0x00725160 _$E344
// FUNCTION: STBC 0x00725190 _$E359
// FUNCTION: STBC 0x007251a0 _$E358
// FUNCTION: STBC 0x007251b0 _$E363
// FUNCTION: STBC 0x007251c0 _$E362
// FUNCTION: STBC 0x007251d0 _$E367
// FUNCTION: STBC 0x007251e0 _$E366
// FUNCTION: STBC 0x007251f0 _$E371
// FUNCTION: STBC 0x00725200 _$E370
// FUNCTION: STBC 0x00725230 _$E375
// FUNCTION: STBC 0x00725240 _$E374
// FUNCTION: STBC 0x00725250 _$E379
// FUNCTION: STBC 0x00725260 _$E378
// FUNCTION: STBC 0x00725270 _$E383
// FUNCTION: STBC 0x00725280 _$E382
// FUNCTION: STBC 0x007252f0 _$E387
// FUNCTION: STBC 0x00725300 _$E386
// FUNCTION: STBC 0x00725330 _$E391
// FUNCTION: STBC 0x00725340 _$E390
// FUNCTION: STBC 0x00725350 _$E395
// FUNCTION: STBC 0x00725360 _$E394
// FUNCTION: STBC 0x00725390 _$E399
// FUNCTION: STBC 0x007253a0 _$E398
// FUNCTION: STBC 0x007253d0 _$E403
// FUNCTION: STBC 0x007253e0 _$E402
// FUNCTION: STBC 0x007253f0 _$E407
// FUNCTION: STBC 0x00725400 _$E406
// FUNCTION: STBC 0x00725450 _$E411
// FUNCTION: STBC 0x00725460 _$E410
// FUNCTION: STBC 0x00725470 _$E415
// FUNCTION: STBC 0x00725480 _$E414
// FUNCTION: STBC 0x00725490 _$E419
// FUNCTION: STBC 0x007254a0 _$E418
// FUNCTION: STBC 0x007254b0 _$E423
// FUNCTION: STBC 0x007254c0 _$E422
// FUNCTION: STBC 0x007254d0 _$E427
// FUNCTION: STBC 0x007254e0 _$E426
// FUNCTION: STBC 0x007254f0 _$E431
// FUNCTION: STBC 0x00725500 _$E430
// FUNCTION: STBC 0x00725510 _$E435
// FUNCTION: STBC 0x00725520 _$E434
// FUNCTION: STBC 0x00725530 _$E439
// FUNCTION: STBC 0x00725540 _$E438
// FUNCTION: STBC 0x00725570 _$E443
// FUNCTION: STBC 0x00725580 _$E442

// The UI module's constructor and its exit-time destructor.
// FUNCTION: STBC 0x007255d0 _$E459
// FUNCTION: STBC 0x007255e0 _$E456
// FUNCTION: STBC 0x007255f0 _$E458
// FUNCTION: STBC 0x00725600 _$E457

// DATA: STBC 0x0099cf38
TGUIModule uiModule;		// invented name

/*
================
TGUIModule::VirtualSlot00

The startup: the Python method, the console's output, the classes and the
handler names.
================
*/
// FUNCTION: STBC 0x00725640
void TGUIModule::VirtualSlot00( void ) {
	PyEmbed::AddClassMethod( TGParagraph_GetCString, "TGParagraph", "GetCString" );
	TGConsole::RedirectOutput();
	RegisterClasses();
	RegisterHandlerNames();
}

/*
================
TGUIModule::VirtualSlot01
================
*/
// FUNCTION: STBC 0x00725670
void TGUIModule::VirtualSlot01( void ) {
	RegisterHandlers();
	TGParagraph::CreateCursorTimer();
}

/*
================
TGUIModule::VirtualSlot02
================
*/
// FUNCTION: STBC 0x00725680
void TGUIModule::VirtualSlot02( void ) {
}

/*
================
TGUIModule::VirtualSlot04

The root window loses its children.
================
*/
// FUNCTION: STBC 0x00725690
void TGUIModule::VirtualSlot04( void ) {
	TGConsole::Unknown736660();
	TGParagraph::DeleteCursorTimer();
	g_kRootWindow.KillChildren();
}

/*
================
TGUIModule::VirtualSlot05
================
*/
// FUNCTION: STBC 0x007256b0
void TGUIModule::VirtualSlot05( void ) {
	g_kRootWindow.Shutdown();
	g_kIconManager.Shutdown();
}

/*
================
TGUIModule::RegisterClasses

Each class's creator, then its handler table: the table's parent is its
base class's, and the table joins the array of the class tables. The
creators go through RegisterCreator and the tables through the classes'
GetClassTable: with NiTMap::SetAt and NiTArray::Add one inline level
deeper, the compiler's inline budget runs out where retail's does (SetAt
is never expanded, SetAtGrow only in the last three Adds).
================
*/
// FUNCTION: STBC 0x007256d0
void TGUIModule::RegisterClasses( void ) {
	TGObject::RegisterCreator( TGKEYBOARDEVENT_TYPE, TGInputModule::CreateTGKeyboardEvent );
	TGObject::RegisterCreator( TGMOUSEEVENT_TYPE, TGInputModule::CreateTGMouseEvent );

	TGObject::RegisterCreator( TGUIOBJECT_TYPE, CreateTGUIObject );
	TGUIObject::GetClassTable()->SetParent( TGEventHandlerObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGUIObject::GetClassTable() );

	TGObject::RegisterCreator( TGICON_TYPE, CreateTGIcon );
	TGIcon::GetClassTable()->SetParent( TGUIObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGIcon::GetClassTable() );

	TGObject::RegisterCreator( TGFRAME_TYPE, CreateTGFrame );
	TGFrame::GetClassTable()->SetParent( TGUIObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGFrame::GetClassTable() );

	TGObject::RegisterCreator( TGPANE_TYPE, CreateTGPane );
	TGPane::GetClassTable()->SetParent( TGUIObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGPane::GetClassTable() );

	TGObject::RegisterCreator( TGWINDOW_TYPE, CreateTGWindow );
	TGWindow::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGWindow::GetClassTable() );

	TGObject::RegisterCreator( TGFRAMEWINDOW_TYPE, CreateTGFrameWindow );
	TGFrameWindow::GetClassTable()->SetParent( TGWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGFrameWindow::GetClassTable() );

	TGObject::RegisterCreator( TGDIALOGWINDOW_TYPE, CreateTGDialogWindow );
	TGDialogWindow::GetClassTable()->SetParent( TGFrameWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGDialogWindow::GetClassTable() );

	TGObject::RegisterCreator( TGSTRINGDIALOG_TYPE, CreateTGStringDialog );
	TGStringDialog::GetClassTable()->SetParent( TGDialogWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGStringDialog::GetClassTable() );

	TGObject::RegisterCreator( TGCONSOLE_TYPE, CreateTGConsole );
	TGConsole::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGConsole::GetClassTable() );

	TGObject::RegisterCreator( TGBUTTONBASE_TYPE, CreateTGButtonBase );
	TGButtonBase::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGButtonBase::GetClassTable() );

	TGObject::RegisterCreator( TGBUTTON_TYPE, CreateTGButton );
	TGButton::GetClassTable()->SetParent( TGButtonBase::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGButton::GetClassTable() );

	TGObject::RegisterCreator( TGTEXTBUTTON_TYPE, CreateTGTextButton );
	TGTextButton::GetClassTable()->SetParent( TGButtonBase::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGTextButton::GetClassTable() );

	TGObject::RegisterCreator( TGROOTPANE_TYPE, CreateTGRootPane );
	TGRootPane::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGRootPane::GetClassTable() );

	TGObject::RegisterCreator( TGPARAGRAPH_TYPE, CreateTGParagraph );
	TGParagraph::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGParagraph::GetClassTable() );

	TGObject::RegisterCreator( TGPROMPT_TYPE, CreateTGPrompt );
	TGPrompt::GetClassTable()->SetParent( TGParagraph::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TGPrompt::GetClassTable() );
}

/*
================
TGUIModule::CreateTGUIObject
================
*/
// FUNCTION: STBC 0x00725b30
TGObject *TGUIModule::CreateTGUIObject( long id ) {
	return new TGUIObject( id );
}

/*
================
TGUIModule::CreateTGPane
================
*/
// FUNCTION: STBC 0x00725b60
TGObject *TGUIModule::CreateTGPane( long id ) {
	return new TGPane( id );
}

/*
================
TGUIModule::CreateTGRootPane
================
*/
// FUNCTION: STBC 0x00725b90
TGObject *TGUIModule::CreateTGRootPane( long id ) {
	return new TGRootPane( id );
}

/*
================
TGUIModule::CreateTGFrame
================
*/
// FUNCTION: STBC 0x00725bd0
TGObject *TGUIModule::CreateTGFrame( long id ) {
	return new TGFrame( id );
}

/*
================
TGUIModule::CreateTGWindow
================
*/
// FUNCTION: STBC 0x00725d60
TGObject *TGUIModule::CreateTGWindow( long id ) {
	return new TGWindow( id );
}

/*
================
TGUIModule::CreateTGFrameWindow
================
*/
// FUNCTION: STBC 0x00725db0
TGObject *TGUIModule::CreateTGFrameWindow( long id ) {
	return new TGFrameWindow( id );
}

/*
================
TGUIModule::CreateTGParagraph
================
*/
// FUNCTION: STBC 0x00725ee0
TGObject *TGUIModule::CreateTGParagraph( long id ) {
	return new TGParagraph( id );
}

/*
================
TGUIModule::CreateTGDialogWindow
================
*/
// FUNCTION: STBC 0x007260b0
TGObject *TGUIModule::CreateTGDialogWindow( long id ) {
	return new TGDialogWindow( id );
}

/*
================
TGUIModule::CreateTGStringDialog
================
*/
// FUNCTION: STBC 0x007261f0
TGObject *TGUIModule::CreateTGStringDialog( long id ) {
	return new TGStringDialog( id );
}

/*
================
TGUIModule::CreateTGConsole
================
*/
// FUNCTION: STBC 0x00726340
TGObject *TGUIModule::CreateTGConsole( long id ) {
	return new TGConsole( id );
}

/*
================
TGUIModule::CreateTGPrompt
================
*/
// FUNCTION: STBC 0x00726490
TGObject *TGUIModule::CreateTGPrompt( long id ) {
	return new TGPrompt( id );
}

/*
================
TGUIModule::CreateTGIcon
================
*/
// FUNCTION: STBC 0x00726640
TGObject *TGUIModule::CreateTGIcon( long id ) {
	return new TGIcon( id );
}

/*
================
TGUIModule::CreateTGButtonBase
================
*/
// FUNCTION: STBC 0x00726760
TGObject *TGUIModule::CreateTGButtonBase( long id ) {
	return new TGButtonBase( id );
}

/*
================
TGUIModule::CreateTGButton
================
*/
// FUNCTION: STBC 0x00726890
TGObject *TGUIModule::CreateTGButton( long id ) {
	return new TGButton( id );
}

/*
================
TGUIModule::CreateTGTextButton
================
*/
// FUNCTION: STBC 0x007269c0
TGObject *TGUIModule::CreateTGTextButton( long id ) {
	return new TGTextButton( id );
}

/*
================
TGUIModule::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00726b70
void TGUIModule::RegisterHandlerNames( void ) {
	TGUIObject::RegisterHandlerNames();
	TGPane::RegisterHandlerNames();
	TGRootPane::RegisterHandlerNames();
	TGParagraph::RegisterHandlerNames();
	TGConsole::RegisterHandlerNames();
	TGButtonBase::RegisterHandlerNames();
	TGDialogWindow::RegisterHandlerNames();
	TGStringDialog::RegisterHandlerNames();
}

/*
================
TGUIModule::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00726ba0
void TGUIModule::RegisterHandlers( void ) {
	TGUIObject::RegisterHandlers();
	TGPane::RegisterHandlers();
	TGWindow::RegisterHandlers();
	TGDialogWindow::RegisterHandlers();
	TGStringDialog::RegisterHandlers();
	TGRootPane::RegisterHandlers();
	TGParagraph::RegisterHandlers();
	TGConsole::RegisterHandlers();
	TGButtonBase::RegisterHandlers();
}

// The standard library's locale facet id (<iostream>): a test-and-set initialiser and its exit-time destructor.
// FUNCTION: STBC 0x00726c20 _$E467
// FUNCTION: STBC 0x00726c40 _$E466
