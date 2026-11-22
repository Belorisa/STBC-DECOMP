/*
===============================================================================

	TGWindow.cpp

===============================================================================
*/

#include <iostream>
#include "TGWindow.h"
#include "TGEventHandlerTable.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0073dce0 _$E32
// FUNCTION: STBC 0x0073dcf0 _$E29
// FUNCTION: STBC 0x0073dd00 _$E31
// FUNCTION: STBC 0x0073dd10 _$E30
// FUNCTION: STBC 0x0073dd20 _$E38
// FUNCTION: STBC 0x0073dd30 _$E35
// FUNCTION: STBC 0x0073dd40 _$E37
// FUNCTION: STBC 0x0073dd50 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0073dd60 _$E57
// FUNCTION: STBC 0x0073dd70 _$E56
// FUNCTION: STBC 0x0073dd80 _$E61
// FUNCTION: STBC 0x0073dd90 _$E60
// FUNCTION: STBC 0x0073dda0 _$E72
// FUNCTION: STBC 0x0073ddb0 _$E71
// FUNCTION: STBC 0x0073ddc0 _$E76
// FUNCTION: STBC 0x0073ddd0 _$E75
// FUNCTION: STBC 0x0073dde0 _$E80
// FUNCTION: STBC 0x0073ddf0 _$E79
// FUNCTION: STBC 0x0073de00 _$E84
// FUNCTION: STBC 0x0073de10 _$E83
// FUNCTION: STBC 0x0073de20 _$E208
// FUNCTION: STBC 0x0073de30 _$E207
// FUNCTION: STBC 0x0073de40 _$E150
// FUNCTION: STBC 0x0073de50 _$E149
// FUNCTION: STBC 0x0073de60 _$E163
// FUNCTION: STBC 0x0073de70 _$E162
// FUNCTION: STBC 0x0073de80 _$E213
// FUNCTION: STBC 0x0073de90 _$E212
// FUNCTION: STBC 0x0073dea0 _$E218
// FUNCTION: STBC 0x0073deb0 _$E217
// FUNCTION: STBC 0x0073dec0 _$E222
// FUNCTION: STBC 0x0073ded0 _$E221
// FUNCTION: STBC 0x0073dee0 _$E226
// FUNCTION: STBC 0x0073def0 _$E225
// FUNCTION: STBC 0x0073df00 _$E233
// FUNCTION: STBC 0x0073df10 _$E232
// FUNCTION: STBC 0x0073df20 _$E258
// FUNCTION: STBC 0x0073df30 _$E257
// FUNCTION: STBC 0x0073df40 _$E262
// FUNCTION: STBC 0x0073df50 _$E261
// FUNCTION: STBC 0x0073df60 _$E108
// FUNCTION: STBC 0x0073df70 _$E107
// FUNCTION: STBC 0x0073df80 _$E244
// FUNCTION: STBC 0x0073df90 _$E243
// FUNCTION: STBC 0x0073dfa0 _$E251
// FUNCTION: STBC 0x0073dfb0 _$E250
// FUNCTION: STBC 0x0073dfc0 _$E274
// FUNCTION: STBC 0x0073dfd0 _$E273
// FUNCTION: STBC 0x0073dfe0 _$E278
// FUNCTION: STBC 0x0073dff0 _$E277
// FUNCTION: STBC 0x0073e000 _$E318
// FUNCTION: STBC 0x0073e010 _$E317
// FUNCTION: STBC 0x0073e020 _$E324
// FUNCTION: STBC 0x0073e030 _$E323
// FUNCTION: STBC 0x0073e040 _$E266
// FUNCTION: STBC 0x0073e050 _$E265
// FUNCTION: STBC 0x0073e060 _$E270
// FUNCTION: STBC 0x0073e070 _$E269
// FUNCTION: STBC 0x0073e080 _$E282
// FUNCTION: STBC 0x0073e090 _$E281
// FUNCTION: STBC 0x0073e0a0 _$E286
// FUNCTION: STBC 0x0073e0b0 _$E285
// FUNCTION: STBC 0x0073e0c0 _$E290
// FUNCTION: STBC 0x0073e0d0 _$E289
// FUNCTION: STBC 0x0073e0e0 _$E294
// FUNCTION: STBC 0x0073e0f0 _$E293
// FUNCTION: STBC 0x0073e100 _$E298
// FUNCTION: STBC 0x0073e110 _$E297
// FUNCTION: STBC 0x0073e120 _$E302
// FUNCTION: STBC 0x0073e130 _$E301
// FUNCTION: STBC 0x0073e140 _$E306
// FUNCTION: STBC 0x0073e150 _$E305
// FUNCTION: STBC 0x0073e160 _$E310
// FUNCTION: STBC 0x0073e170 _$E309
// FUNCTION: STBC 0x0073e180 _$E113
// FUNCTION: STBC 0x0073e190 _$E112
// FUNCTION: STBC 0x0073e1a0 _$E314
// FUNCTION: STBC 0x0073e1b0 _$E313
// FUNCTION: STBC 0x0073e1c0 _$E118
// FUNCTION: STBC 0x0073e1d0 _$E117
// FUNCTION: STBC 0x0073e1e0 _$E328
// FUNCTION: STBC 0x0073e1f0 _$E327
// FUNCTION: STBC 0x0073e200 _$E332
// FUNCTION: STBC 0x0073e210 _$E331
// FUNCTION: STBC 0x0073e220 _$E336
// FUNCTION: STBC 0x0073e230 _$E335
// FUNCTION: STBC 0x0073e240 _$E340
// FUNCTION: STBC 0x0073e250 _$E339
// FUNCTION: STBC 0x0073e260 _$E127
// FUNCTION: STBC 0x0073e270 _$E126
// FUNCTION: STBC 0x0073e280 _$E344
// FUNCTION: STBC 0x0073e290 _$E343
// FUNCTION: STBC 0x0073e2a0 _$E348
// FUNCTION: STBC 0x0073e2b0 _$E347
// FUNCTION: STBC 0x0073e2c0 _$E352
// FUNCTION: STBC 0x0073e2d0 _$E351
// FUNCTION: STBC 0x0073e2e0 _$E133
// FUNCTION: STBC 0x0073e2f0 _$E132
// FUNCTION: STBC 0x0073e300 _$E138
// FUNCTION: STBC 0x0073e310 _$E137
// FUNCTION: STBC 0x0073e320 _$E168
// FUNCTION: STBC 0x0073e330 _$E167
// FUNCTION: STBC 0x0073e340 _$E356
// FUNCTION: STBC 0x0073e350 _$E355
// FUNCTION: STBC 0x0073e360 _$E173
// FUNCTION: STBC 0x0073e370 _$E172
// FUNCTION: STBC 0x0073e380 _$E360
// FUNCTION: STBC 0x0073e390 _$E359
// FUNCTION: STBC 0x0073e3a0 _$E364
// FUNCTION: STBC 0x0073e3b0 _$E363
// FUNCTION: STBC 0x0073e3c0 _$E178
// FUNCTION: STBC 0x0073e3d0 _$E177
// FUNCTION: STBC 0x0073e3e0 _$E368
// FUNCTION: STBC 0x0073e3f0 _$E367
// FUNCTION: STBC 0x0073e400 _$E143
// FUNCTION: STBC 0x0073e410 _$E142
// FUNCTION: STBC 0x0073e420 _$E372
// FUNCTION: STBC 0x0073e430 _$E371
// FUNCTION: STBC 0x0073e440 _$E376
// FUNCTION: STBC 0x0073e450 _$E375
// FUNCTION: STBC 0x0073e460 _$E183
// FUNCTION: STBC 0x0073e470 _$E182
// FUNCTION: STBC 0x0073e480 _$E188
// FUNCTION: STBC 0x0073e490 _$E187
// FUNCTION: STBC 0x0073e4a0 _$E380
// FUNCTION: STBC 0x0073e4b0 _$E379
// FUNCTION: STBC 0x0073e4c0 _$E384
// FUNCTION: STBC 0x0073e4d0 _$E383
// FUNCTION: STBC 0x0073e4e0 _$E388
// FUNCTION: STBC 0x0073e4f0 _$E387
// FUNCTION: STBC 0x0073e500 _$E392
// FUNCTION: STBC 0x0073e510 _$E391
// FUNCTION: STBC 0x0073e520 _$E396
// FUNCTION: STBC 0x0073e530 _$E395
// FUNCTION: STBC 0x0073e540 _$E400
// FUNCTION: STBC 0x0073e550 _$E399
// FUNCTION: STBC 0x0073e560 _$E404
// FUNCTION: STBC 0x0073e570 _$E403
// FUNCTION: STBC 0x0073e580 _$E408
// FUNCTION: STBC 0x0073e590 _$E407
// FUNCTION: STBC 0x0073e5a0 _$E193
// FUNCTION: STBC 0x0073e5b0 _$E192
// FUNCTION: STBC 0x0073e5c0 _$E412
// FUNCTION: STBC 0x0073e5d0 _$E411
// FUNCTION: STBC 0x0073e5e0 _$E198
// FUNCTION: STBC 0x0073e5f0 _$E197
// FUNCTION: STBC 0x0073e600 _$E203
// FUNCTION: STBC 0x0073e610 _$E202

/*
================
TGWindow::Cast

The object as a window; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0073e670
TGWindow *TGWindow::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGWINDOW_TYPE ) ) {
		return (TGWindow *)object;
	}
	return NULL;
}

/*
================
TGWindow::TGWindow

A window of the given size around subPane, or around a new pane of the
same size, indented by indentX and indentY.
================
*/
// FUNCTION: STBC 0x0073e6a0
TGWindow::TGWindow( float width, float height, float indentX, float indentY, TGPane *subPane ) : TGPane( width, height ) {
	TGRect rect( 0.0f, 0.0f, width, height );		// never used
	if ( subPane == NULL ) {
		subPane = new TGPane( width, height );
	}
	subPane->SetSkipParent();
	TGPane::InsertChild( 0x7FFFFFFF, subPane, 0.0f, 0.0f, true );
	SetIndent( indentX, indentY );
}

/*
================
TGWindow::Create

TGWindow_Create.
================
*/
// FUNCTION: STBC 0x0073e780
TGWindow *TGWindow::Create( float width, float height, float indentX, float indentY, TGPane *subPane ) {
	return new TGWindow( width, height, indentX, indentY, subPane );
}

/*
================
TGWindow::~TGWindow
================
*/
// FUNCTION: STBC 0x0073e7d0
TGWindow::~TGWindow( void ) {
	TGPane::SetFocus( NULL, false );
	TGPane::KillChildren();
}

/*
================
TGWindow::RegisterHandlers

The window's handlers are its pane's.
================
*/
// FUNCTION: STBC 0x0073e830
void TGWindow::RegisterHandlers( void ) {
}

/*
================
TGWindow::MoveToFront
================
*/
// FUNCTION: STBC 0x0073e840
void TGWindow::MoveToFront( TGUIObject *child, bool layout ) {
	TGPane *subPane = GetSubPane();
	if ( subPane != NULL ) {
		subPane->MoveToFront( child, layout );
	}
}

/*
================
TGWindow::MoveToBack
================
*/
// FUNCTION: STBC 0x0073e870
void TGWindow::MoveToBack( TGUIObject *child, bool layout ) {
	TGPane *subPane = GetSubPane();
	if ( subPane != NULL ) {
		subPane->MoveToBack( child, layout );
	}
}

/*
================
TGWindow::GetFocus
================
*/
// FUNCTION: STBC 0x0073e8a0
TGUIObject *TGWindow::GetFocus( void ) {
	TGPane *subPane = GetSubPane();
	if ( subPane != NULL ) {
		return subPane->GetFocus();
	}
	return NULL;
}

/*
================
TGWindow::SetFocus

The sub pane itself takes the window's focus; any other child is the sub
pane's.
================
*/
// FUNCTION: STBC 0x0073e8c0
void TGWindow::SetFocus( TGUIObject *child, bool layout ) {
	if ( child == TGPane::GetFirstChild() ) {
		TGPane::SetFocus( child, layout );
		return;
	}
	TGPane *subPane = GetSubPane();
	if ( subPane != NULL ) {
		subPane->SetFocus( child, layout );
	}
}

/*
================
TGWindow::MoveFocus
================
*/
// FUNCTION: STBC 0x0073e910
void TGWindow::MoveFocus( int direction, bool wrap, bool layout ) {
	TGPane *subPane = GetSubPane();
	if ( subPane != NULL ) {
		subPane->MoveFocus( direction, wrap, layout );
	}
}

/*
================
TGWindow::InsertChild
================
*/
// FUNCTION: STBC 0x0073e940
void TGWindow::InsertChild( int index, TGUIObject *child, float x, float y, bool layout ) {
	TGPane *subPane = GetSubPane();
	if ( subPane != NULL ) {
		subPane->InsertChild( index, child, x, y, layout );
	}
}

/*
================
TGWindow::RemoveChild
================
*/
// FUNCTION: STBC 0x0073e980
void TGWindow::RemoveChild( TGUIObject *child, bool layout ) {
	TGPane *subPane = GetSubPane();
	if ( subPane != NULL ) {
		subPane->RemoveChild( child, layout );
	}
}

/*
================
TGWindow::DeleteChild
================
*/
// FUNCTION: STBC 0x0073e9b0
void TGWindow::DeleteChild( TGUIObject *child, bool layout ) {
	TGPane *subPane = GetSubPane();
	if ( subPane != NULL ) {
		subPane->DeleteChild( child, layout );
	}
}

/*
================
TGWindow::GetNumChildren
================
*/
// FUNCTION: STBC 0x0073e9e0
int TGWindow::GetNumChildren( void ) {
	TGPane *subPane = GetSubPane();
	if ( subPane != NULL ) {
		return subPane->GetNumChildren();
	}
	return 0;
}

/*
================
TGWindow::GetFirstChild
================
*/
// FUNCTION: STBC 0x0073ea00
TGUIObject *TGWindow::GetFirstChild( void ) {
	TGPane *subPane = GetSubPane();
	if ( subPane != NULL ) {
		return subPane->GetFirstChild();
	}
	return NULL;
}

/*
================
TGWindow::GetLastChild
================
*/
// FUNCTION: STBC 0x0073ea20
TGUIObject *TGWindow::GetLastChild( void ) {
	TGPane *subPane = GetSubPane();
	if ( subPane != NULL ) {
		return subPane->GetLastChild();
	}
	return NULL;
}

/*
================
TGWindow::GetNthChild
================
*/
// FUNCTION: STBC 0x0073ea40
TGUIObject *TGWindow::GetNthChild( int index ) {
	TGPane *subPane = GetSubPane();
	if ( subPane != NULL ) {
		return subPane->GetNthChild( index );
	}
	return NULL;
}

/*
================
TGWindow::GetNextChild
================
*/
// FUNCTION: STBC 0x0073ea70
TGUIObject *TGWindow::GetNextChild( TGUIObject *child ) {
	TGPane *subPane = GetSubPane();
	if ( subPane != NULL ) {
		return subPane->GetNextChild( child );
	}
	return NULL;
}

/*
================
TGWindow::GetPrevChild
================
*/
// FUNCTION: STBC 0x0073eaa0
TGUIObject *TGWindow::GetPrevChild( TGUIObject *child ) {
	TGPane *subPane = GetSubPane();
	if ( subPane != NULL ) {
		return subPane->GetPrevChild( child );
	}
	return NULL;
}

/*
================
TGWindow::KillChildren
================
*/
// FUNCTION: STBC 0x0073ead0
void TGWindow::KillChildren( void ) {
	TGPane *subPane = GetSubPane();
	if ( subPane != NULL ) {
		subPane->KillChildren();
	}
}

/*
================
TGWindow::SetExclusive
================
*/
// FUNCTION: STBC 0x0073eaf0
void TGWindow::SetExclusive( bool layout ) {
	TGPane *subPane = GetSubPane();
	if ( subPane != NULL ) {
		subPane->SetExclusive( layout );
	}
}

/*
================
TGWindow::SetNotExclusive
================
*/
// FUNCTION: STBC 0x0073eb10
void TGWindow::SetNotExclusive( bool layout ) {
	TGPane *subPane = GetSubPane();
	if ( subPane != NULL ) {
		subPane->SetNotExclusive( layout );
	}
}

/*
================
TGWindow::SetIndent

Places the sub pane x and y in from the window's top left corner and
shrinks it by as much.
================
*/
// FUNCTION: STBC 0x0073eb30
void TGWindow::SetIndent( float x, float y ) {
	TGRect rect;
	TGPane *subPane = GetSubPane();
	GetBounds( &rect );
	rect.left += x;
	rect.top += y;
	rect.width -= x;
	rect.height -= y;
	subPane->Move( rect.left, rect.top, true );
	subPane->Resize( rect.width, rect.height, true );
}

/*
================
TGWindow::Resize

Resizes the sub pane by as much as the window, unless the window is set
to TGWINDOW_SIMPLE_RESIZE.
================
*/
// FUNCTION: STBC 0x0073ec10
void TGWindow::Resize( float width, float height, bool layout ) {
	if ( !IsSimpleResize() ) {
		TGRect windowBounds;
		TGRect subBounds;
		TGPane *subPane = GetSubPane();
		GetBounds( &windowBounds );
		subPane->GetBounds( &subBounds );
		float dWidth = windowBounds.width - width;
		float dHeight = windowBounds.height - height;
		subPane->Resize( subBounds.width - dWidth, subBounds.height - dHeight, false );
	}
	TGPane::Resize( width, height, layout );
}

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x0073e620 _$E418
// FUNCTION: STBC 0x0073e630 _$E414
// FUNCTION: STBC 0x0073e640 _$E417
// FUNCTION: STBC 0x0073e650 _$E415
// DATA: STBC 0x0099e0e0
TGEventHandlerTable TGWindow::classHandlers;
