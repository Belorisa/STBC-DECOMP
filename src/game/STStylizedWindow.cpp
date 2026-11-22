/*
===============================================================================

	STStylizedWindow.cpp

===============================================================================
*/

#include <iostream>
#include "STStylizedWindow.h"
#include "STMenu.h"
#include "GraphicsModeInfo.h"
#include "TGBoolEvent.h"
#include "TGIntEvent.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGIconManager.h"
#include "TGParagraph.h"
#include "TGStream.h"
#include "TGSoundManager.h"
#include "TGSound.h"
#include "TGTimer.h"
#include "TGTimerManager.h"
#include "PyEmbed.h"
#include "NiApplication.h"
#include "NiPoint2.h"
#include <math.h>
#include <string.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00532500 _$E32
// FUNCTION: STBC 0x00532510 _$E29
// FUNCTION: STBC 0x00532520 _$E31
// FUNCTION: STBC 0x00532530 _$E30
// FUNCTION: STBC 0x00532540 _$E38
// FUNCTION: STBC 0x00532550 _$E35
// FUNCTION: STBC 0x00532560 _$E37
// FUNCTION: STBC 0x00532570 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00532580 _$E57
// FUNCTION: STBC 0x00532590 _$E56
// FUNCTION: STBC 0x005325a0 _$E61
// FUNCTION: STBC 0x005325b0 _$E60
// FUNCTION: STBC 0x005325c0 _$E72
// FUNCTION: STBC 0x005325d0 _$E71
// FUNCTION: STBC 0x005325e0 _$E76
// FUNCTION: STBC 0x005325f0 _$E75
// FUNCTION: STBC 0x00532600 _$E80
// FUNCTION: STBC 0x00532610 _$E79
// FUNCTION: STBC 0x00532620 _$E84
// FUNCTION: STBC 0x00532630 _$E83
// FUNCTION: STBC 0x00532640 _$E208
// FUNCTION: STBC 0x00532650 _$E207
// FUNCTION: STBC 0x00532660 _$E150
// FUNCTION: STBC 0x00532670 _$E149
// FUNCTION: STBC 0x00532680 _$E163
// FUNCTION: STBC 0x00532690 _$E162
// FUNCTION: STBC 0x005326a0 _$E213
// FUNCTION: STBC 0x005326b0 _$E212
// FUNCTION: STBC 0x005326c0 _$E218
// FUNCTION: STBC 0x005326d0 _$E217
// FUNCTION: STBC 0x005326e0 _$E222
// FUNCTION: STBC 0x005326f0 _$E221
// FUNCTION: STBC 0x00532700 _$E226
// FUNCTION: STBC 0x00532710 _$E225
// FUNCTION: STBC 0x00532720 _$E233
// FUNCTION: STBC 0x00532730 _$E232
// FUNCTION: STBC 0x00532740 _$E310
// FUNCTION: STBC 0x00532750 _$E309
// FUNCTION: STBC 0x00532760 _$E314
// FUNCTION: STBC 0x00532770 _$E313
// FUNCTION: STBC 0x00532780 _$E108
// FUNCTION: STBC 0x00532790 _$E107
// FUNCTION: STBC 0x005327a0 _$E296
// FUNCTION: STBC 0x005327b0 _$E295
// FUNCTION: STBC 0x005327c0 _$E303
// FUNCTION: STBC 0x005327d0 _$E302
// FUNCTION: STBC 0x005327e0 _$E326
// FUNCTION: STBC 0x005327f0 _$E325
// FUNCTION: STBC 0x00532800 _$E330
// FUNCTION: STBC 0x00532810 _$E329
// FUNCTION: STBC 0x00532820 _$E370
// FUNCTION: STBC 0x00532830 _$E369
// FUNCTION: STBC 0x00532840 _$E376
// FUNCTION: STBC 0x00532850 _$E375
// FUNCTION: STBC 0x00532860 _$E318
// FUNCTION: STBC 0x00532870 _$E317
// FUNCTION: STBC 0x00532880 _$E322
// FUNCTION: STBC 0x00532890 _$E321
// FUNCTION: STBC 0x005328a0 _$E334
// FUNCTION: STBC 0x005328b0 _$E333
// FUNCTION: STBC 0x005328c0 _$E338
// FUNCTION: STBC 0x005328d0 _$E337
// FUNCTION: STBC 0x005328e0 _$E342
// FUNCTION: STBC 0x005328f0 _$E341
// FUNCTION: STBC 0x00532900 _$E346
// FUNCTION: STBC 0x00532910 _$E345
// FUNCTION: STBC 0x00532920 _$E350
// FUNCTION: STBC 0x00532930 _$E349
// FUNCTION: STBC 0x00532940 _$E354
// FUNCTION: STBC 0x00532950 _$E353
// FUNCTION: STBC 0x00532960 _$E358
// FUNCTION: STBC 0x00532970 _$E357
// FUNCTION: STBC 0x00532980 _$E362
// FUNCTION: STBC 0x00532990 _$E361
// FUNCTION: STBC 0x005329a0 _$E113
// FUNCTION: STBC 0x005329b0 _$E112
// FUNCTION: STBC 0x005329c0 _$E366
// FUNCTION: STBC 0x005329d0 _$E365
// FUNCTION: STBC 0x005329e0 _$E118
// FUNCTION: STBC 0x005329f0 _$E117
// FUNCTION: STBC 0x00532a00 _$E380
// FUNCTION: STBC 0x00532a10 _$E379
// FUNCTION: STBC 0x00532a20 _$E384
// FUNCTION: STBC 0x00532a30 _$E383
// FUNCTION: STBC 0x00532a40 _$E388
// FUNCTION: STBC 0x00532a50 _$E387
// FUNCTION: STBC 0x00532a60 _$E392
// FUNCTION: STBC 0x00532a70 _$E391
// FUNCTION: STBC 0x00532a80 _$E127
// FUNCTION: STBC 0x00532a90 _$E126
// FUNCTION: STBC 0x00532aa0 _$E396
// FUNCTION: STBC 0x00532ab0 _$E395
// FUNCTION: STBC 0x00532ac0 _$E400
// FUNCTION: STBC 0x00532ad0 _$E399
// FUNCTION: STBC 0x00532ae0 _$E404
// FUNCTION: STBC 0x00532af0 _$E403
// FUNCTION: STBC 0x00532b00 _$E133
// FUNCTION: STBC 0x00532b10 _$E132
// FUNCTION: STBC 0x00532b20 _$E138
// FUNCTION: STBC 0x00532b30 _$E137
// FUNCTION: STBC 0x00532b40 _$E168
// FUNCTION: STBC 0x00532b50 _$E167
// FUNCTION: STBC 0x00532b90 _$E408
// FUNCTION: STBC 0x00532ba0 _$E407
// FUNCTION: STBC 0x00532bb0 _$E173
// FUNCTION: STBC 0x00532bc0 _$E172
// FUNCTION: STBC 0x00532bd0 _$E412
// FUNCTION: STBC 0x00532be0 _$E411
// FUNCTION: STBC 0x00532bf0 _$E416
// FUNCTION: STBC 0x00532c00 _$E415
// FUNCTION: STBC 0x00532c10 _$E178
// FUNCTION: STBC 0x00532c20 _$E177
// FUNCTION: STBC 0x00532c30 _$E420
// FUNCTION: STBC 0x00532c40 _$E419
// FUNCTION: STBC 0x00532c50 _$E143
// FUNCTION: STBC 0x00532c60 _$E142
// FUNCTION: STBC 0x00532c70 _$E424
// FUNCTION: STBC 0x00532c80 _$E423
// FUNCTION: STBC 0x00532c90 _$E428
// FUNCTION: STBC 0x00532ca0 _$E427
// FUNCTION: STBC 0x00532cb0 _$E183
// FUNCTION: STBC 0x00532cc0 _$E182
// FUNCTION: STBC 0x00532cd0 _$E188
// FUNCTION: STBC 0x00532ce0 _$E187
// FUNCTION: STBC 0x00532cf0 _$E432
// FUNCTION: STBC 0x00532d00 _$E431
// FUNCTION: STBC 0x00532d10 _$E436
// FUNCTION: STBC 0x00532d20 _$E435
// FUNCTION: STBC 0x00532d30 _$E440
// FUNCTION: STBC 0x00532d40 _$E439
// FUNCTION: STBC 0x00532d50 _$E444
// FUNCTION: STBC 0x00532d60 _$E443
// FUNCTION: STBC 0x00532d70 _$E448
// FUNCTION: STBC 0x00532d80 _$E447
// FUNCTION: STBC 0x00532d90 _$E452
// FUNCTION: STBC 0x00532da0 _$E451
// FUNCTION: STBC 0x00532db0 _$E456
// FUNCTION: STBC 0x00532dc0 _$E455
// FUNCTION: STBC 0x00532dd0 _$E460
// FUNCTION: STBC 0x00532de0 _$E459
// FUNCTION: STBC 0x00532df0 _$E193
// FUNCTION: STBC 0x00532e00 _$E192
// FUNCTION: STBC 0x00532e10 _$E464
// FUNCTION: STBC 0x00532e20 _$E463
// FUNCTION: STBC 0x00532e30 _$E198
// FUNCTION: STBC 0x00532e40 _$E197
// FUNCTION: STBC 0x00532e50 _$E203
// FUNCTION: STBC 0x00532e60 _$E202
// FUNCTION: STBC 0x00532e70 _$E468
// FUNCTION: STBC 0x00532e80 _$E467
// FUNCTION: STBC 0x00532e90 _$E473
// FUNCTION: STBC 0x00532ea0 _$E472
// FUNCTION: STBC 0x00532eb0 _$E259
// FUNCTION: STBC 0x00532ec0 _$E258

// Further static initialisers of the unit.
// FUNCTION: STBC 0x00530d00 _$E479
// FUNCTION: STBC 0x00530d10 _$E475
// FUNCTION: STBC 0x00530d20 _$E478
// FUNCTION: STBC 0x00530d30 _$E476

// DATA: STBC 0x00989a30
TGEventHandlerTable STStylizedWindow::classHandlers;

/*
================
STStylizedWindow::Cast

The object as a stylized window; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00530d50
STStylizedWindow *STStylizedWindow::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STSTYLIZEDWINDOW_TYPE ) ) {
		return (STStylizedWindow *)object;
	}
	return NULL;
}

/*
================
STStylizedWindow::STStylizedWindow

The window holds a sub window around the interior; the script object made
by the module's function draws the frame in the exterior pane.
================
*/
// FUNCTION: STBC 0x00530d80 ??0STStylizedWindow@@QAE@PAD0PAVTGString@@MMPAVTGPane@@_NMMPAVNiColorA@@@Z
STStylizedWindow::STStylizedWindow( char *module, char *function, TGString *name_, float indentX, float indentY, TGPane *interior,
									bool fixedSize_, float maximumWidth_, float maximumHeight_, NiColorA *color_ )
	: TGWindow( 0.0f, 0.0f, indentX, indentY, new TGWindow( 0.0f, 0.0f, 0.0f, 0.0f, interior ) ), name( "" ) {
	color = *color_;
	SetSimpleResize();
	( (TGWindow *)TGPane::GetFirstChild() )->SetSimpleResize();

	pythonModule = new char[strlen( module ) + 1];
	strcpy( pythonModule, module );
	pythonFunction = new char[strlen( function ) + 1];
	strcpy( pythonFunction, function );
	if ( PyEmbed::CallFunction( pythonModule, pythonFunction, "O", &pythonObject, "()" ) == -1 ) {
		PyErr_Print();
		pythonObject = NULL;
	}
	if ( name_ != NULL ) {
		name = *name_;
	}

	scrollViewHeight = -1.0f;
	lastInteriorWidth = -1.0f;
	lastInteriorHeight = -1.0f;
	hasMaximumSize = false;
	scrolling = 0;
	scrollSpeed = 0.5f;
	maximumWidth = maximumWidth_;
	maximumHeight = maximumHeight_;
	borderWidth = 0.0f;
	borderHeight = 0.0f;
	useScrolling = true;
	minimizable = true;
	useFocusGlass = false;
	titleBarThickness = 0.0f;

	TGPane *exterior = new TGPane;
	exterior->SetBatchChildPolys( false );
	TGPane::InsertChild( 0x7FFFFFFF, exterior, 0.0f, 0.0f, false );
	CallPythonMethod( methodNames[STSW_METHOD_SETUP] );

	fixedSize = fixedSize_;
	if ( fixedSize_ ) {
		SetFixedSize( maximumWidth_, maximumHeight_, true );
	}

	TGPane *subWindow = (TGPane *)TGPane::GetFirstChild();
	TGPane::SetFocus( subWindow, true );
	subWindow->TGPane::SetFocus( GetInteriorPane(), true );
	subWindow->SetBatchChildPolys( false );
	UpdateBorderWidth();
}

/*
================
STStylizedWindow::~STStylizedWindow
================
*/
// FUNCTION: STBC 0x00531120
STStylizedWindow::~STStylizedWindow( void ) {
	if ( pythonModule != NULL ) {
		delete[] pythonModule;
	}
	if ( pythonFunction != NULL ) {
		delete[] pythonFunction;
	}
	Py_XDECREF( pythonObject );
}

/*
================
STStylizedWindow::Layout

Lays the window out once; a layout the script object starts from inside is
skipped.
================
*/
// FUNCTION: STBC 0x005311c0
void STStylizedWindow::Layout( void ) {
	if ( !( flags & STSW_IN_LAYOUT ) ) {
		flags |= STSW_IN_LAYOUT;
		TGPane::Layout();
		flags &= ~STSW_IN_LAYOUT;
	}
}

/*
================
STStylizedWindow::SetFocus
================
*/
// FUNCTION: STBC 0x005311f0
void STStylizedWindow::SetFocus( TGUIObject *child, bool layout ) {
	GetInteriorPane()->SetFocus( child, layout );
}

/*
================
STStylizedWindow::SetUseFocusGlass
================
*/
// FUNCTION: STBC 0x00531220
void STStylizedWindow::SetUseFocusGlass( bool useFocusGlass ) {
	this->useFocusGlass = useFocusGlass;
	CallPythonMethod( methodNames[STSW_METHOD_UPDATE_GLASS] );
}

/*
================
STStylizedWindow::GotTrueFocusPath
================
*/
// FUNCTION: STBC 0x00531240
void STStylizedWindow::GotTrueFocusPath( bool layout ) {
	TGUIObject::GotTrueFocusPath( false );
	if ( useFocusGlass == true ) {
		CallPythonMethod( methodNames[STSW_METHOD_UPDATE_GLASS] );
	}
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STStylizedWindow::LostTrueFocusPath
================
*/
// FUNCTION: STBC 0x00531280
void STStylizedWindow::LostTrueFocusPath( bool layout ) {
	TGUIObject::LostTrueFocusPath( false );
	if ( useFocusGlass == true ) {
		CallPythonMethod( methodNames[STSW_METHOD_UPDATE_GLASS] );
	}
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STStylizedWindow::InteriorChangedSize

The interior is as big as its visible children reach, at least the room
inside a fixed size, and no taller than nothing while minimized. A change
(or force) resizes the interior and the sub pane and tells the script
object.
================
*/
// FUNCTION: STBC 0x005312c0
void STStylizedWindow::InteriorChangedSize( bool force ) {
	TGPane *interior = GetInteriorPane();
	float width = 0.0f;
	float height = 0.0f;
	for ( TGUIObject *child = interior->GetFirstChild(); child != NULL; child = interior->GetNextChild( child ) ) {
		if ( child->IsVisible() ) {
			if ( child->GetRight() > width ) {
				width = child->GetRight();
			}
			if ( child->GetBottom() > height ) {
				height = child->GetBottom();
			}
		}
	}
	if ( fixedSize ) {
		if ( width < GetMaximumInteriorWidth() ) {
			width = GetMaximumInteriorWidth();
		}
		if ( height < GetMaximumInteriorHeight() ) {
			height = GetMaximumInteriorHeight();
		}
	}
	if ( IsMinimized() ) {
		height = 0.0f;
	}
	if ( lastInteriorWidth != width || lastInteriorHeight != height || force ) {
		lastInteriorWidth = width;
		lastInteriorHeight = height;
		interior->Resize( width, height, false );
		TGPane *subPane = GetSubPane();
		subPane->Resize( width, height, false );
		CallPythonMethod( methodNames[STSW_METHOD_RESIZE] );
		if ( scrollViewHeight >= 0.0f ) {
			height = scrollViewHeight;
		} else if ( interior->GetLeft() != 0.0f || interior->GetTop() != 0.0f ) {
			interior->SetPosition( 0.0f, 0.0f, false );
		}
		if ( subPane->GetWidth() != width || subPane->GetHeight() != height ) {
			subPane->Resize( width, height, false );
		}
	}
	UpdateScrollArrows();
}

/*
================
STStylizedWindow::Unknown5314F0
================
*/
// FUNCTION: STBC 0x005314f0
void STStylizedWindow::Unknown5314F0( void ) {
	lastInteriorWidth = -1.0f;
	lastInteriorHeight = -1.0f;
}

/*
================
STStylizedWindow::EnsureFocusIsVisible

Scrolls the interior until the focused object shows. Without a focus, the
view is scrolled back by one screen of the sub pane.
================
*/
// FUNCTION: STBC 0x00531510 ?EnsureFocusIsVisible@STStylizedWindow@@QAEXXZ
// DRAFT: x87 in the no-focus branch: retail loads scrollViewHeight onto the FPU for the compare (fcompp) and again between the maximum and border heights
void STStylizedWindow::EnsureFocusIsVisible( void ) {
	if ( scrollViewHeight < 0.0f || scrolling != 0 ) {
		return;
	}
	TGPane *subPane = GetSubPane();
	TGPane *interior = GetInteriorPane();
	TGUIObject *focus = interior->GetFocus();
	TGPane *last = NULL;
	if ( focus == NULL ) {
		if ( scrollViewHeight >= GetInteriorPane()->GetHeight() - GraphicsModeInfo::GetCurrentMode()->GetPixelHeight() * 2.0f ) {
			scrollViewHeight -= GetMaximumInteriorHeight() * 0.5f;
			if ( scrollViewHeight < 0.0f ) {
				scrollViewHeight = 0.0f;
			}
		}
		return;
	}
	while ( TGPane::Cast( focus ) != NULL ) {
		last = (TGPane *)focus;
		focus = TGPane::Cast( focus )->GetFocus();
	}
	if ( focus == NULL ) {
		focus = last;
	}
	if ( last == NULL ) {
		return;
	}
	STMenu *menu = STMenu::Cast( focus->GetConceptualParent() );
	if ( menu != NULL && focus == menu->GetFirstChild() ) {
		focus = menu;
	}
	NiPoint2 focusOffset;
	NiPoint2 subPaneOffset;
	NiPoint2 interiorOffset;
	focus->GetScreenOffset( &focusOffset );
	subPane->GetScreenOffset( &subPaneOffset );
	interior->GetScreenOffset( &interiorOffset );
	float top = focusOffset.y - interiorOffset.y;
	float scrolled = (float)fabs( interior->GetTop() );
	float visibleBottom = subPane->GetHeight() + scrolled;
	float bottom = focus->GetHeight() + top;
	STMenu *focusMenu = STMenu::Cast( focus );
	if ( visibleBottom < bottom && ( focusMenu == NULL || !focusMenu->IsOpened() ) ) {
		Scroll( -( bottom - visibleBottom ) );
		return;
	}
	if ( top < scrolled ) {
		Scroll( (float)fabs( scrolled - top ) );
	}
}

/*
================
STStylizedWindow::Unknown531730

Scrolls down, when the object reaches below the visible part, so that it
starts a third of the way down.
================
*/
// FUNCTION: STBC 0x00531730
void STStylizedWindow::Unknown531730( TGUIObject *object ) {
	if ( useScrolling ) {
		TGPane *subPane = GetSubPane();
		TGPane *interior = GetInteriorPane();
		NiPoint2 objectOffset;
		NiPoint2 subPaneOffset;
		NiPoint2 interiorOffset;
		object->GetScreenOffset( &objectOffset );
		subPane->GetScreenOffset( &subPaneOffset );
		interior->GetScreenOffset( &interiorOffset );
		float top = objectOffset.y - interiorOffset.y;
		float scrolled = (float)fabs( interior->GetTop() );
		float visibleBottom = subPane->GetHeight() + scrolled;
		if ( object->GetHeight() + top > visibleBottom ) {
			float scroll = top - ( ( visibleBottom - scrolled ) * 0.33f + scrolled );
			if ( scroll < 0.0f ) {
				scroll = 0.0f;
			}
			interior->SetPosition( interior->GetLeft(), interior->GetTop() - scroll, false );
			InteriorChangedSize( false );
		}
	}
}

/*
================
STStylizedWindow::SetTitleBarThickness

The thickness is rounded to whole pixels.
================
*/
// FUNCTION: STBC 0x00531810
// PARKED: x87 residue: retail copies the screen height into its local through fld/fstp (a double-to-float conversion does the same); the spelling that reads it so is not found
void STStylizedWindow::SetTitleBarThickness( float thickness, bool resize ) {
	float pixels = g_kIconManager.GetScreenHeight();
	if ( pixels != 0.0f ) {
		thickness = (float)(int)( pixels * thickness + 0.5f ) / pixels;
	}
	titleBarThickness = thickness;
	UpdateBorderWidth();
	if ( resize ) {
		InteriorChangedSize( true );
	}
}

/*
================
STStylizedWindow::SetFixedSize

A size of 0 by 0 frees the window; any other holds it to that size.
================
*/
// FUNCTION: STBC 0x00531880
void STStylizedWindow::SetFixedSize( float width, float height, bool resize ) {
	lastInteriorWidth = -1.0f;
	lastInteriorHeight = -1.0f;
	if ( width == 0.0f && height == 0.0f ) {
		fixedSize = false;
	} else {
		fixedSize = true;
		SetMaximumSize( width, height );
	}
	if ( resize ) {
		InteriorChangedSize( true );
	}
}

/*
================
STStylizedWindow::SetMinimized
================
*/
// FUNCTION: STBC 0x005318f0
void STStylizedWindow::SetMinimized( bool layout ) {
	if ( IsMinimized() != true && minimizable ) {
		flags |= STSW_MINIMIZED;
		GetSubPane()->SetNotVisible( false );
		InteriorChangedSize( false );
		if ( layout ) {
			Layout();
		}
	}
}

/*
================
STStylizedWindow::SetNotMinimized
================
*/
// FUNCTION: STBC 0x00531950
void STStylizedWindow::SetNotMinimized( bool layout ) {
	if ( IsMinimized() && minimizable ) {
		flags &= ~STSW_MINIMIZED;
		GetSubPane()->SetVisible( false );
		InteriorChangedSize( false );
		if ( layout ) {
			Layout();
		}
	}
}

/*
================
STStylizedWindow::IsMinimized
================
*/
// FUNCTION: STBC 0x005319b0
bool STStylizedWindow::IsMinimized( void ) {
	return ( flags >> 16 ) & 1;
}

/*
================
STStylizedWindow::SetUseScrolling
================
*/
// FUNCTION: STBC 0x005319c0
void STStylizedWindow::SetUseScrolling( bool useScrolling ) {
	this->useScrolling = useScrolling;
	Unknown5314F0();
	InteriorChangedSize( false );
}

/*
================
STStylizedWindow::UpdateScrollArrows
================
*/
// FUNCTION: STBC 0x005319e0
void STStylizedWindow::UpdateScrollArrows( void ) {
	CallPythonMethod( methodNames[STSW_METHOD_UPDATE_SCROLL_ARROWS] );
}

/*
================
STStylizedWindow::SetMinimizable
================
*/
// FUNCTION: STBC 0x005319f0
void STStylizedWindow::SetMinimizable( bool minimizable ) {
	this->minimizable = minimizable;
	CallPythonMethod( methodNames[STSW_METHOD_UPDATE_MINIMIZE] );
}

/*
================
STStylizedWindow::SetMaximumSize

The size is rounded to whole pixels; a negative size takes the maximum
away. A window bigger than the maximum shrinks to it.
================
*/
// FUNCTION: STBC 0x00531a10
// PARKED: x87 residue as SetTitleBarThickness: the screen height goes to its local through fld/fstp, and the rounded pixels reuse the size arguments' slots
void STStylizedWindow::SetMaximumSize( float width, float height ) {
	lastInteriorWidth = -1.0f;
	lastInteriorHeight = -1.0f;
	if ( width < 0.0f || height < 0.0f ) {
		hasMaximumSize = false;
		return;
	}
	float screenWidth = g_kIconManager.GetScreenWidth();
	float screenHeight = g_kIconManager.GetScreenHeight();
	float roundedWidth;
	int pixels = (int)( screenWidth * width + 0.5f );
	if ( pixels != 0 ) {
		roundedWidth = (float)pixels / screenWidth;
	} else {
		roundedWidth = 0.0f;
	}
	pixels = (int)( screenHeight * height + 0.5f );
	float roundedHeight;
	if ( pixels != 0 ) {
		roundedHeight = (float)pixels / screenHeight;
	} else {
		roundedHeight = 0.0f;
	}
	hasMaximumSize = true;
	maximumHeight = roundedHeight;
	maximumWidth = roundedWidth;

	bool resized = false;
	if ( GetWidth() > maximumWidth ) {
		Resize( maximumWidth, GetHeight(), false );
		resized = true;
	}
	if ( GetHeight() > maximumHeight ) {
		Resize( GetWidth(), maximumHeight, false );
		InteriorChangedSize( false );
		return;
	}
	if ( resized == true ) {
		InteriorChangedSize( false );
	}
}

/*
================
STStylizedWindow::UpdateBorderWidth
================
*/
// FUNCTION: STBC 0x00531b50
void STStylizedWindow::UpdateBorderWidth( void ) {
	PyObject *result;
	if ( CallPythonMethod( methodNames[STSW_METHOD_GET_BORDER_WIDTH], "O", &result ) ) {
		NiPoint2 *size = NULL;
		PyEmbed::Unknown6F98E0( result, (void **)&size, "NiPoint2" );
		if ( size != NULL ) {
			borderWidth = size->x;
			borderHeight = size->y;
		}
		Py_DECREF( result );
	}
}

/*
================
STStylizedWindow::GetNameParagraph

The paragraph the script object shows the name in; NULL without one.
================
*/
// FUNCTION: STBC 0x00531bd0
TGParagraph *STStylizedWindow::GetNameParagraph( void ) {
	PyObject *result;
	if ( CallPythonMethod( methodNames[STSW_METHOD_GET_NAME_PARAGRAPH], "O", &result ) ) {
		TGParagraph *paragraph = NULL;
		PyEmbed::Unknown6F98E0( result, (void **)&paragraph, "TGParagraph" );
		Py_DECREF( result );
		return paragraph;
	}
	return NULL;
}

/*
================
STStylizedWindow::CallPythonMethod
================
*/
// FUNCTION: STBC 0x00531c40 ?CallPythonMethod@STStylizedWindow@@IAEXPAD@Z
void STStylizedWindow::CallPythonMethod( char *method ) {
	CallPythonMethod( method, "", NULL );
}

/*
================
STStylizedWindow::CallPythonMethod
================
*/
// FUNCTION: STBC 0x00531c60 ?CallPythonMethod@STStylizedWindow@@IAE_NPAD0PAX@Z
bool STStylizedWindow::CallPythonMethod( char *method, char *resultFormat, void *result ) {
	if ( pythonObject == NULL ) {
		return false;
	}
	TGPane *exterior = GetExteriorPane();
	TGPane *subPane = GetSubPane();
	PyObject *pyWindow = VirtualSlot08();
	PyObject *pyExterior = exterior->VirtualSlot08();
	PyObject *pySubPane = subPane->VirtualSlot08();
	int status = PyEmbed::CallMethod( pythonObject, method, resultFormat, result, "(OOO)", pyWindow, pyExterior, pySubPane );
	Py_DECREF( pyWindow );
	Py_DECREF( pyExterior );
	Py_DECREF( pySubPane );
	if ( status == -1 ) {
		PyErr_Print();
	}
	return status != -1;
}

/*
================
STStylizedWindow::HandleMinimizeEvent
================
*/
// FUNCTION: STBC 0x00531d20
void STStylizedWindow::HandleMinimizeEvent( TGBoolEvent *event ) {
	if ( event->GetBool() == true ) {
		SetMinimized( true );
	} else {
		SetNotMinimized( true );
	}
	CallNextHandler( event );
}

/*
================
STStylizedWindow::HandleScrollEvent

An arrow pressed starts the periodic scroll its way; released, it stops.
================
*/
// FUNCTION: STBC 0x00531d60
void STStylizedWindow::HandleScrollEvent( TGIntEvent *event ) {
	switch ( event->GetInt() ) {
		case -1:
			scrolling = 1;
			PeriodicScrollUp( NULL );
			break;
		case 0:
			scrolling = 0;
			g_kSoundManager.StopSound( "UIScrolling" );
			break;
		case 1:
			scrolling = -1;
			PeriodicScrollDown( NULL );
			break;
	}
	CallNextHandler( event );
}

/*
================
STStylizedWindow::PeriodicScrollUp

Scrolls up by this frame's share of the scroll speed; the event is the
repeat ScrollAndRepeat sends, NULL for the first step.
================
*/
// FUNCTION: STBC 0x00531de0
void STStylizedWindow::PeriodicScrollUp( TGEvent *event ) {
	ScrollAndRepeat( ET_ST_PERIODIC_SCROLL_UP, NiApplication::application->GetFrameTime() * scrollSpeed );
	if ( event != NULL ) {
		CallNextHandler( event );
	}
}

/*
================
STStylizedWindow::PeriodicScrollDown
================
*/
// FUNCTION: STBC 0x00531e20
void STStylizedWindow::PeriodicScrollDown( TGEvent *event ) {
	ScrollAndRepeat( ET_ST_PERIODIC_SCROLL_DOWN, -( NiApplication::application->GetFrameTime() * scrollSpeed ) );
	if ( event != NULL ) {
		CallNextHandler( event );
	}
}

/*
================
STStylizedWindow::ScrollAndRepeat

Scrolls, with the scrolling sound while it moves. While an arrow is held
the event goes out again on the next frame.
================
*/
// FUNCTION: STBC 0x00531e60
void STStylizedWindow::ScrollAndRepeat( int eventType, float amount ) {
	if ( Scroll( amount ) ) {
		TGSound *sound = g_kSoundManager.GetSound( "UIScrolling" );
		if ( sound != NULL && g_kSoundManager.GetPlayingSound( "UIScrolling" ) == NULL ) {
			sound->SetLooping( true );
			sound->Play( true );
		}
	} else {
		g_kSoundManager.StopSound( "UIScrolling" );
	}
	UpdateScrollArrows();
	if ( scrolling != 0 ) {
		TGEvent *event = new TGEvent;
		event->SetEventType( eventType );
		event->SetDestination( this );
		TGTimer *timer = new TGTimer;
		timer->SetTimerStart( NiApplication::application->GetCurrentTime() + 0.001f );
		timer->SetEvent( event );
		g_kRealtimeTimerManager.AddTimer( timer );
	}
}

/*
================
STStylizedWindow::UISwitchIconGroups
================
*/
// FUNCTION: STBC 0x00531f60
void STStylizedWindow::UISwitchIconGroups( void ) {
	CallPythonMethod( "UISwitchIconGroups" );
}

/*
================
STStylizedWindow::UIResize
================
*/
// FUNCTION: STBC 0x00531f70
void STStylizedWindow::UIResize( void ) {
	CallPythonMethod( "UIResize" );
	UpdateBorderWidth();
}

/*
================
STStylizedWindow::UIReposition
================
*/
// FUNCTION: STBC 0x00531f90
void STStylizedWindow::UIReposition( void ) {
	CallPythonMethod( "UIReposition" );
}

/*
================
STStylizedWindow::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00531fa0
void STStylizedWindow::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STStylizedWindow::HandleMinimizeEvent, "STStylizedWindow::HandleMinimizeEvent" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STStylizedWindow::HandleScrollEvent, "STStylizedWindow::HandleScrollEvent" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STStylizedWindow::PeriodicScrollUp, "STStylizedWindow::PeriodicScrollUp" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STStylizedWindow::PeriodicScrollDown, "STStylizedWindow::PeriodicScrollDown" );
}

/*
================
STStylizedWindow::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00532000
void STStylizedWindow::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_ST_MINIMIZE, "STStylizedWindow::HandleMinimizeEvent" );
	AddMethodHandlerForClass( &classHandlers, ET_ST_SCROLL, "STStylizedWindow::HandleScrollEvent" );
	AddMethodHandlerForClass( &classHandlers, ET_ST_PERIODIC_SCROLL_UP, "STStylizedWindow::PeriodicScrollUp" );
	AddMethodHandlerForClass( &classHandlers, ET_ST_PERIODIC_SCROLL_DOWN, "STStylizedWindow::PeriodicScrollDown" );
}

/*
================
STStylizedWindow::ScrollToTop
================
*/
// FUNCTION: STBC 0x00532060
void STStylizedWindow::ScrollToTop( void ) {
	TGPane *interior = GetInteriorPane();
	TGPane *subPane = GetSubPane();		// not used: the sub pane's height only matters to ScrollToBottom
	interior->SetPosition( 0.0f, 0.0f, false );
	UpdateScrollArrows();
}

/*
================
STStylizedWindow::ScrollToBottom
================
*/
// FUNCTION: STBC 0x005320a0
void STStylizedWindow::ScrollToBottom( void ) {
	TGPane *interior = GetInteriorPane();
	TGPane *subPane = GetSubPane();
	interior->SetPosition( 0.0f, subPane->GetHeight() - interior->GetHeight(), false );
	UpdateScrollArrows();
}

/*
================
STStylizedWindow::Scroll
================
*/
// FUNCTION: STBC 0x005320f0
bool STStylizedWindow::Scroll( float amount ) {
	TGPane *interior = GetInteriorPane();
	TGPane *subPane = GetSubPane();
	bool canScroll = scrolling != 0;
	interior->SetPosition( 0.0f, interior->GetTop() + amount, true );
	if ( interior->GetTop() > 0.0f ) {
		interior->SetPosition( 0.0f, 0.0f, true );
		canScroll = false;
	} else if ( interior->GetBottom() < subPane->GetHeight() && amount < 0.0f ) {
		interior->SetPosition( 0.0f, subPane->GetHeight() - interior->GetBottom() + interior->GetTop(), true );
		canScroll = false;
	}
	UpdateScrollArrows();
	return canScroll;
}

/*
================
STStylizedWindow::WriteToStream
================
*/
// FUNCTION: STBC 0x005321c0
bool STStylizedWindow::WriteToStream( TGStream *stream ) {
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( pythonModule );
	stream->VirtualSlot11( pythonFunction );
	if ( !name.WriteToStream( stream ) ) {
		return false;
	}
	if ( pythonObject != NULL ) {
		stream->WriteBool( true );
		if ( !PyEmbed::SerializePython( pythonObject, stream, 0 ) ) {
			return false;
		}
	} else {
		stream->WriteBool( false );
	}
	stream->WriteBool( hasMaximumSize );
	stream->WriteLong( scrolling );
	stream->WriteBool( fixedSize );
	stream->WriteFloat( maximumWidth );
	stream->WriteFloat( maximumHeight );
	stream->WriteFloat( borderWidth );
	stream->WriteFloat( borderHeight );
	stream->WriteFloat( scrollViewHeight );
	stream->WriteFloat( scrollSpeed );
	stream->WriteFloat( titleBarThickness );
	stream->WriteFloat( lastInteriorWidth );
	stream->WriteFloat( lastInteriorHeight );
	stream->WriteFloat( color.r );
	stream->WriteFloat( color.g );
	stream->WriteFloat( color.b );
	stream->WriteFloat( color.a );
	stream->WriteBool( useScrolling );
	stream->WriteBool( minimizable );
	stream->WriteBool( useFocusGlass );
	return true;
}

/*
================
STStylizedWindow::ReadFromStream
================
*/
// FUNCTION: STBC 0x00532330
bool STStylizedWindow::ReadFromStream( TGStream *stream ) {
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	pythonModule = stream->VirtualSlot10( -1 );
	pythonFunction = stream->VirtualSlot10( -1 );
	if ( !name.ReadFromStream( stream ) ) {
		return false;
	}
	if ( stream->ReadBool() ) {
		pythonObject = PyEmbed::UnserializePython( stream, 0 );
		if ( pythonObject == NULL ) {
			return false;
		}
	} else {
		pythonObject = NULL;
	}
	hasMaximumSize = stream->ReadBool();
	scrolling = stream->ReadLong();
	fixedSize = stream->ReadBool();
	maximumWidth = stream->ReadFloat();
	maximumHeight = stream->ReadFloat();
	borderWidth = stream->ReadFloat();
	borderHeight = stream->ReadFloat();
	scrollViewHeight = stream->ReadFloat();
	scrollSpeed = stream->ReadFloat();
	titleBarThickness = stream->ReadFloat();
	lastInteriorWidth = stream->ReadFloat();
	lastInteriorHeight = stream->ReadFloat();
	color.r = stream->ReadFloat();
	color.g = stream->ReadFloat();
	color.b = stream->ReadFloat();
	color.a = stream->ReadFloat();
	useScrolling = stream->ReadBool();
	minimizable = stream->ReadBool();
	useFocusGlass = stream->ReadBool();
	return true;
}

/*
================
STStylizedWindow::GetExteriorPane

The linker folded this into TGPane::GetLastChild (0x72ED70), whose code is
the same; the address belongs to that function.
================
*/
TGPane *STStylizedWindow::GetExteriorPane( void ) {
	return (TGPane *)TGPane::GetLastChild();
}

// The script object's methods (invented name).
// DATA: STBC 0x008e34c0
char *STStylizedWindow::methodNames[STSW_NUM_METHODS] = {
	"Setup",
	"Resize",
	"GetBorderWidth",
	"UpdateScrollArrows",
	"GetNameParagraph",
	"UpdateMinimize",
	"UpdateGlass"
};

/*
================
STStylizedWindow_GetStyleInstance

The Python method STStylizedWindow.GetStyleInstance: the script object the
window's style function made, with a new reference; NULL when the argument
is no window.
================
*/
// FUNCTION: STBC 0x00532490
PyObject *STStylizedWindow_GetStyleInstance( PyObject *self, PyObject *args ) {
	PyObject *instance = NULL;
	PyArg_ParseTuple( args, "O", &instance );
	if ( instance != NULL ) {
		STStylizedWindow *window = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&window, "STStylizedWindow" );
		if ( window != NULL ) {
			Py_XINCREF( window->pythonObject );
			return window->pythonObject;
		}
	}
	return NULL;
}
