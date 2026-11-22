/*
===============================================================================

	STSubPane.cpp

===============================================================================
*/

#include <iostream>
#include "STSubPane.h"
#include "STButton.h"
#include "STMenu.h"
#include "STStylizedWindow.h"
#include "STToggle.h"
#include "STTopLevelMenu.h"
#include "InterfaceModule.h"
#include "KeyboardBinding.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGIntEvent.h"
#include "TGKeyboardEvent.h"
#include "TGRootPane.h"
#include "NiPoint2.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x005343a0 _$E32
// FUNCTION: STBC 0x005343b0 _$E29
// FUNCTION: STBC 0x005343c0 _$E31
// FUNCTION: STBC 0x005343d0 _$E30
// FUNCTION: STBC 0x005343e0 _$E38
// FUNCTION: STBC 0x005343f0 _$E35
// FUNCTION: STBC 0x00534400 _$E37
// FUNCTION: STBC 0x00534410 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00534540 _$E57
// FUNCTION: STBC 0x00534550 _$E56
// FUNCTION: STBC 0x00534560 _$E61
// FUNCTION: STBC 0x00534570 _$E60
// FUNCTION: STBC 0x00534580 _$E72
// FUNCTION: STBC 0x00534590 _$E71
// FUNCTION: STBC 0x005345a0 _$E76
// FUNCTION: STBC 0x005345b0 _$E75
// FUNCTION: STBC 0x005345c0 _$E80
// FUNCTION: STBC 0x005345d0 _$E79
// FUNCTION: STBC 0x005345e0 _$E84
// FUNCTION: STBC 0x005345f0 _$E83
// FUNCTION: STBC 0x00534600 _$E208
// FUNCTION: STBC 0x00534610 _$E207
// FUNCTION: STBC 0x00534620 _$E150
// FUNCTION: STBC 0x00534630 _$E149
// FUNCTION: STBC 0x00534640 _$E163
// FUNCTION: STBC 0x00534650 _$E162
// FUNCTION: STBC 0x00534660 _$E213
// FUNCTION: STBC 0x00534670 _$E212
// FUNCTION: STBC 0x00534680 _$E218
// FUNCTION: STBC 0x00534690 _$E217
// FUNCTION: STBC 0x005346a0 _$E222
// FUNCTION: STBC 0x005346b0 _$E221
// FUNCTION: STBC 0x005346c0 _$E226
// FUNCTION: STBC 0x005346d0 _$E225
// FUNCTION: STBC 0x005346e0 _$E233
// FUNCTION: STBC 0x005346f0 _$E232
// FUNCTION: STBC 0x00534700 _$E282
// FUNCTION: STBC 0x00534710 _$E281
// FUNCTION: STBC 0x00534720 _$E286
// FUNCTION: STBC 0x00534730 _$E285
// FUNCTION: STBC 0x00534740 _$E108
// FUNCTION: STBC 0x00534750 _$E107
// FUNCTION: STBC 0x00534760 _$E268
// FUNCTION: STBC 0x00534770 _$E267
// FUNCTION: STBC 0x00534780 _$E275
// FUNCTION: STBC 0x00534790 _$E274
// FUNCTION: STBC 0x005347a0 _$E298
// FUNCTION: STBC 0x005347b0 _$E297
// FUNCTION: STBC 0x005347c0 _$E302
// FUNCTION: STBC 0x005347d0 _$E301
// FUNCTION: STBC 0x005347e0 _$E342
// FUNCTION: STBC 0x005347f0 _$E341
// FUNCTION: STBC 0x00534800 _$E348
// FUNCTION: STBC 0x00534810 _$E347
// FUNCTION: STBC 0x00534820 _$E290
// FUNCTION: STBC 0x00534830 _$E289
// FUNCTION: STBC 0x00534840 _$E294
// FUNCTION: STBC 0x00534850 _$E293
// FUNCTION: STBC 0x00534860 _$E306
// FUNCTION: STBC 0x00534870 _$E305
// FUNCTION: STBC 0x00534880 _$E310
// FUNCTION: STBC 0x00534890 _$E309
// FUNCTION: STBC 0x005348a0 _$E314
// FUNCTION: STBC 0x005348b0 _$E313
// FUNCTION: STBC 0x005348c0 _$E318
// FUNCTION: STBC 0x005348d0 _$E317
// FUNCTION: STBC 0x005348e0 _$E322
// FUNCTION: STBC 0x005348f0 _$E321
// FUNCTION: STBC 0x00534900 _$E326
// FUNCTION: STBC 0x00534910 _$E325
// FUNCTION: STBC 0x00534920 _$E330
// FUNCTION: STBC 0x00534930 _$E329
// FUNCTION: STBC 0x00534940 _$E334
// FUNCTION: STBC 0x00534950 _$E333
// FUNCTION: STBC 0x00534960 _$E113
// FUNCTION: STBC 0x00534970 _$E112
// FUNCTION: STBC 0x00534980 _$E338
// FUNCTION: STBC 0x00534990 _$E337
// FUNCTION: STBC 0x005349a0 _$E118
// FUNCTION: STBC 0x005349b0 _$E117
// FUNCTION: STBC 0x005349c0 _$E352
// FUNCTION: STBC 0x005349d0 _$E351
// FUNCTION: STBC 0x005349e0 _$E356
// FUNCTION: STBC 0x005349f0 _$E355
// FUNCTION: STBC 0x00534a00 _$E360
// FUNCTION: STBC 0x00534a10 _$E359
// FUNCTION: STBC 0x00534a20 _$E364
// FUNCTION: STBC 0x00534a30 _$E363
// FUNCTION: STBC 0x00534a40 _$E127
// FUNCTION: STBC 0x00534a50 _$E126
// FUNCTION: STBC 0x00534a60 _$E368
// FUNCTION: STBC 0x00534a70 _$E367
// FUNCTION: STBC 0x00534a80 _$E372
// FUNCTION: STBC 0x00534a90 _$E371
// FUNCTION: STBC 0x00534aa0 _$E376
// FUNCTION: STBC 0x00534ab0 _$E375
// FUNCTION: STBC 0x00534ac0 _$E133
// FUNCTION: STBC 0x00534ad0 _$E132
// FUNCTION: STBC 0x00534ae0 _$E138
// FUNCTION: STBC 0x00534af0 _$E137
// FUNCTION: STBC 0x00534b00 _$E168
// FUNCTION: STBC 0x00534b10 _$E167
// FUNCTION: STBC 0x00534b20 _$E380
// FUNCTION: STBC 0x00534b30 _$E379
// FUNCTION: STBC 0x00534b40 _$E173
// FUNCTION: STBC 0x00534b50 _$E172
// FUNCTION: STBC 0x00534b90 _$E384
// FUNCTION: STBC 0x00534ba0 _$E383
// FUNCTION: STBC 0x00534bb0 _$E388
// FUNCTION: STBC 0x00534bc0 _$E387
// FUNCTION: STBC 0x00534bd0 _$E178
// FUNCTION: STBC 0x00534be0 _$E177
// FUNCTION: STBC 0x00534bf0 _$E392
// FUNCTION: STBC 0x00534c00 _$E391
// FUNCTION: STBC 0x00534c10 _$E143
// FUNCTION: STBC 0x00534c20 _$E142
// FUNCTION: STBC 0x00534c30 _$E396
// FUNCTION: STBC 0x00534c40 _$E395
// FUNCTION: STBC 0x00534c50 _$E400
// FUNCTION: STBC 0x00534c60 _$E399
// FUNCTION: STBC 0x00534c70 _$E183
// FUNCTION: STBC 0x00534c80 _$E182
// FUNCTION: STBC 0x00534c90 _$E188
// FUNCTION: STBC 0x00534ca0 _$E187
// FUNCTION: STBC 0x00534cb0 _$E404
// FUNCTION: STBC 0x00534cc0 _$E403
// FUNCTION: STBC 0x00534cd0 _$E408
// FUNCTION: STBC 0x00534ce0 _$E407
// FUNCTION: STBC 0x00534cf0 _$E412
// FUNCTION: STBC 0x00534d00 _$E411
// FUNCTION: STBC 0x00534d10 _$E416
// FUNCTION: STBC 0x00534d20 _$E415
// FUNCTION: STBC 0x00534d30 _$E420
// FUNCTION: STBC 0x00534d40 _$E419
// FUNCTION: STBC 0x00534d50 _$E424
// FUNCTION: STBC 0x00534d60 _$E423
// FUNCTION: STBC 0x00534d70 _$E428
// FUNCTION: STBC 0x00534d80 _$E427
// FUNCTION: STBC 0x00534d90 _$E432
// FUNCTION: STBC 0x00534da0 _$E431
// FUNCTION: STBC 0x00534db0 _$E193
// FUNCTION: STBC 0x00534dc0 _$E192
// FUNCTION: STBC 0x00534dd0 _$E436
// FUNCTION: STBC 0x00534de0 _$E435
// FUNCTION: STBC 0x00534df0 _$E198
// FUNCTION: STBC 0x00534e00 _$E197
// FUNCTION: STBC 0x00534e10 _$E203
// FUNCTION: STBC 0x00534e20 _$E202
// FUNCTION: STBC 0x00534e30 _$E440
// FUNCTION: STBC 0x00534e40 _$E439
// FUNCTION: STBC 0x00534e50 _$E445
// FUNCTION: STBC 0x00534e60 _$E444
// FUNCTION: STBC 0x00534e70 _$E449
// FUNCTION: STBC 0x00534e80 _$E448

// Further static initialisers of the unit.
// FUNCTION: STBC 0x00532b60 _$E463
// FUNCTION: STBC 0x00532b80 _$E462
// FUNCTION: STBC 0x00532ed0 _$E455
// FUNCTION: STBC 0x00532ee0 _$E451
// FUNCTION: STBC 0x00532ef0 _$E454
// FUNCTION: STBC 0x00532f00 _$E452

// DATA: STBC 0x00989aa8
TGEventHandlerTable STSubPane::classHandlers;

/*
================
STSubPane::Cast

The object as a sub pane; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00532f20
STSubPane *STSubPane::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STSUBPANE_TYPE ) ) {
		return (STSubPane *)object;
	}
	return NULL;
}

/*
================
STSubPane::STSubPane

An empty pane; its children are stacked vertSpacing apart.
================
*/
// FUNCTION: STBC 0x00532f50 ??0STSubPane@@QAE@M_N@Z
STSubPane::STSubPane( float vertSpacing, bool expandToFillParent ) : TGPane( 0.0f, 0.0f ) {
	this->vertSpacing = vertSpacing;
	this->expandToFillParent = expandToFillParent;
}

/*
================
STSubPane::STSubPane

A pane with the ID, its children stacked twice DEFAULT_ST_INDENT_VERT
apart.
================
*/
// FUNCTION: STBC 0x005331a0 ??0STSubPane@@QAE@J_N@Z
STSubPane::STSubPane( long id, bool expandToFillParent ) : TGPane( id ) {
	vertSpacing = DEFAULT_ST_INDENT_VERT * 2.0f;
	this->expandToFillParent = expandToFillParent;
}

/*
================
STSubPane::~STSubPane
================
*/
// FUNCTION: STBC 0x005331d0
STSubPane::~STSubPane( void ) {
}

/*
================
STSubPane::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x005331e0
void STSubPane::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STSubPane::KeyboardHandler, "STSubPane::KeyboardHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STSubPane::HandleSelectOption, "STSubPane::HandleSelectOption" );
}

/*
================
STSubPane::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00533210
void STSubPane::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "STSubPane::KeyboardHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_SELECT_X, "STSubPane::HandleSelectOption" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_SELECT_OPTION, "STSubPane::HandleSelectOption" );
}

/*
================
STSubPane::InsertChild

Inserts the child and resizes the pane to its contents; a child inserted
at or above the focus scrolls the window up by its height, keeping the
focus in place.
================
*/
// FUNCTION: STBC 0x00533250
void STSubPane::InsertChild( int index, TGUIObject *child, float x, float y, bool layout ) {
	if ( child == NULL ) {
		return;
	}

	STStylizedWindow *window = GetContainingWindow();
	int focusPos = FindPos( GetFocus() );
	TGPane::InsertChild( index, child, x, y, false );
	ResizeToContents( layout );
	if ( window != NULL && focusPos != -1 && focusPos >= index ) {
		window->Scroll( -child->GetHeight() );
	}
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STSubPane::RemoveChild

Removes the child and resizes the pane to its contents; a child removed
at or above the focus scrolls the window down by its height.
================
*/
// FUNCTION: STBC 0x005332e0
void STSubPane::RemoveChild( TGUIObject *child, bool layout ) {
	if ( child == NULL ) {
		return;
	}

	STStylizedWindow *window = GetContainingWindow();
	int focusPos = FindPos( GetFocus() );
	int childPos = FindPos( child );
	TGPane::RemoveChild( child, false );
	ResizeToContents( layout );
	if ( window != NULL && focusPos != -1 && focusPos >= childPos ) {
		window->Scroll( child->GetHeight() );
	}
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STSubPane::DeleteChild

Destroys the child and resizes the pane to its contents; a child above
the focus scrolls the window down by its height.
================
*/
// FUNCTION: STBC 0x00533370
void STSubPane::DeleteChild( TGUIObject *child, bool layout ) {
	if ( child == NULL ) {
		return;
	}

	STStylizedWindow *window = GetContainingWindow();
	int focusPos = FindPos( GetFocus() );
	int childPos = FindPos( child );
	float height = child->GetHeight();
	TGPane::DeleteChild( child, false );
	ResizeToContents( layout );
	if ( window != NULL && focusPos != -1 && focusPos > childPos ) {
		window->Scroll( height );
	}
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STSubPane::GotFocus

A pane without a focused child focuses and highlights its first child.
================
*/
// FUNCTION: STBC 0x00533400
void STSubPane::GotFocus( bool layout ) {
	if ( GetFocus() == NULL ) {
		TGUIObject *child = GetFirstChild();
		SetFocus( child, layout );
		if ( child != NULL ) {
			GetFirstChild()->SetHighlighted( true );
		}
	}
	TGPane::GotFocus( layout );
}

/*
================
STSubPane::SetFocus
================
*/
// FUNCTION: STBC 0x00533460
void STSubPane::SetFocus( TGUIObject *child, bool layout ) {
	TGPane::SetFocus( child, layout );
}

/*
================
STSubPane::SetRadioGroup
================
*/
// FUNCTION: STBC 0x00533480
void STSubPane::SetRadioGroup( bool radioGroup ) {
	if ( radioGroup == true ) {
		flags |= STSPF_RADIO_GROUP;
	} else {
		flags &= ~STSPF_RADIO_GROUP;
	}
}

/*
================
STSubPane::VirtualSlot59
================
*/
// FUNCTION: STBC 0x005334b0
void STSubPane::VirtualSlot59( void ) {
	TGPane::VirtualSlot59();
	ResizeToContents( true );
}

/*
================
STSubPane::Layout

Stacks the children, then lays out the visible children within the clip
rectangle, back to front; the others leave their batches.
================
*/
// FUNCTION: STBC 0x005334d0
// PARKED: complete; retail subtracts the position as fld member, fsubr clip and loads each child edge afresh per test, where every spelling tried keeps the child's edges on the FPU stack
void STSubPane::Layout( void ) {
	if ( GetNumChildren() == 0 || !IsCompletelyVisible() ) {
		return;
	}

	TGRect clipRect;
	GetClipRect( &clipRect );
	clipRect.top -= bounds.top;
	clipRect.left -= bounds.left;
	StackChildren();

	NiTListIterator pos = children.GetTailPos();
	while ( pos ) {
		TGUIObject *child = children.GetPrev( pos );
		if ( child->IsVisible() ) {
			if ( clipRect.top > child->GetHeight() + child->GetTop()
				|| clipRect.height + clipRect.top < child->GetTop()
				|| clipRect.width + clipRect.left < child->GetLeft()
				|| child->GetWidth() + child->GetLeft() < clipRect.left ) {
				child->VirtualSlot60();
			} else {
				child->Layout();
			}
		}
	}
}

/*
================
STSubPane::StackChildren
================
*/
// FUNCTION: STBC 0x005335f0
void STSubPane::StackChildren( void ) {
	float y = 0.0f;
	NiTListIterator pos = children.GetHeadPos();
	while ( pos ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child->IsVisible() ) {
			child->SetPosition( 0.0f, y, false );
			y = child->GetHeight() + child->GetTop() + vertSpacing;
		}
	}
}

/*
================
STSubPane::GetTotalHeightOfChildren
================
*/
// FUNCTION: STBC 0x00533640
float STSubPane::GetTotalHeightOfChildren( void ) {
	float height = DEFAULT_ST_INDENT_VERT * 2.0f;
	NiTListIterator pos = children.GetHeadPos();
	while ( pos ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child->IsVisible() ) {
			height += child->GetHeight() + vertSpacing;
		}
	}
	return height;
}

/*
================
STSubPane::GetDesiredSize

As tall as the visible children and their spacing. In a window, as wide
as the window allows when expanding to fill it, else as the widest child
or the pane, whichever is wider; outside one, as the widest child. The
width is narrowed by the indent of each menu the pane sits in.
================
*/
// FUNCTION: STBC 0x00533670
void STSubPane::GetDesiredSize( NiPoint2 *size ) {
	size->x = NiPoint2::ZERO.x;
	size->y = NiPoint2::ZERO.y;
	float width = 0.0f;
	float height = 0.0f;
	bool anyVisible = false;
	for ( int i = 0; i < GetNumChildren(); i++ ) {
		TGUIObject *child = GetNthChild( i );
		if ( child != NULL && child->IsVisible() ) {
			NiPoint2 childSize;
			child->GetDesiredSize( &childSize );
			if ( childSize.x > width ) {
				width = childSize.x;
			}
			if ( childSize.y > 0.0f ) {
				height += childSize.y + vertSpacing;
			}
			anyVisible = true;
		}
	}

	STStylizedWindow *window = GetContainingWindow();
	int depth = GetMenuDepth();
	if ( window != NULL ) {
		if ( expandToFillParent == true ) {
			size->x = window->GetMaximumInteriorWidth() - GetMenuIndent() * depth;
		} else {
			if ( width <= GetWidth() ) {
				width = GetWidth();
			}
			size->x = width;
		}
	} else {
		size->x = width - GetMenuIndent() * depth;
	}
	// The last child has no spacing below it.
	size->y = height - ( anyVisible == true ? vertSpacing : 0.0f );
}

/*
================
STSubPane::GetSubmenuW
================
*/
// FUNCTION: STBC 0x005337c0
STMenu *STSubPane::GetSubmenuW( TGString *name ) {
	TGString menuName( "" );
	int numChildren = GetNumChildren();
	for ( int i = 0; i < numChildren; i++ ) {
		STMenu *menu = STMenu::Cast( GetNthChild( i ) );
		if ( menu != NULL ) {
			menu->GetName( &menuName );
			if ( menuName.Compare( name, true ) == 0 ) {
				return menu;
			}
		}
	}
	return NULL;
}

/*
================
STSubPane::GetButtonW

The button with the name, or the toggle whose label is the name and a
space.
================
*/
// FUNCTION: STBC 0x00533890
STButton *STSubPane::GetButtonW( TGString *name ) {
	TGString buttonName( "" );
	int numChildren = GetNumChildren();
	for ( int i = 0; i < numChildren; i++ ) {
		TGUIObject *child = GetNthChild( i );
		if ( child->IsTypeOf( STBUTTON_TYPE ) ) {
			( (STButton *)child )->GetName( &buttonName );
			if ( buttonName.Compare( name, true ) == 0 ) {
				return (STButton *)child;
			}
		}
		if ( child->IsTypeOf( STTOGGLE_TYPE ) ) {
			buttonName = ( (STToggle *)child )->GetPrimaryCaption();
			TGString label = *name + TGString( " " );
			if ( buttonName.Compare( &label, true ) == 0 ) {
				return (STButton *)child;
			}
		}
	}
	return NULL;
}

/*
================
STSubPane::RemoveItemW

Destroys the first menu or button with the name.
================
*/
// FUNCTION: STBC 0x00533b30
void STSubPane::RemoveItemW( TGString *name ) {
	TGString itemName( "" );
	int numChildren = GetNumChildren();
	for ( int i = 0; i < numChildren; i++ ) {
		TGUIObject *child = GetNthChild( i );
		if ( child->IsTypeOf( STMENU_TYPE ) ) {
			( (STMenu *)child )->GetName( &itemName );
			if ( itemName.Compare( name, true ) == 0 ) {
				DeleteChild( child, true );
				return;
			}
		}
		if ( child->IsTypeOf( STBUTTON_TYPE ) ) {
			( (STButton *)child )->GetName( &itemName );
			if ( itemName.Compare( name, true ) == 0 ) {
				DeleteChild( child, true );
				return;
			}
		}
	}
}

/*
================
STSubPane::GetContainingWindow

The nearest stylized window the pane sits in; NULL when there is none.
================
*/
// FUNCTION: STBC 0x00533c40
STStylizedWindow *STSubPane::GetContainingWindow( void ) {
	STStylizedWindow *window = NULL;
	for ( TGPane *pane = parent; pane != NULL && pane != UI_NO_PARENT; pane = pane->GetParent() ) {
		window = STStylizedWindow::Cast( pane );
		if ( window != NULL ) {
			break;
		}
	}
	return window;
}

/*
================
STSubPane::GetMenuDepth
================
*/
// FUNCTION: STBC 0x00533c70
int STSubPane::GetMenuDepth( void ) {
	int depth = 0;
	TGPane *pane = parent;
	while ( pane != NULL && pane != UI_NO_PARENT ) {
		STMenu *menu = STMenu::Cast( pane );
		STTopLevelMenu *topLevelMenu = STTopLevelMenu::Cast( menu );
		if ( menu != NULL && topLevelMenu == NULL ) {
			depth++;
		}
		pane = pane->GetParent();
	}
	return depth;
}

/*
================
STSubPane::GetMenuIndent
================
*/
// FUNCTION: STBC 0x00533cb0
float STSubPane::GetMenuIndent( void ) {
	TGPane *pane = parent;
	while ( pane != NULL && pane != UI_NO_PARENT ) {
		STMenu *menu = STMenu::Cast( pane );
		STTopLevelMenu *topLevelMenu = STTopLevelMenu::Cast( menu );
		if ( menu != NULL && topLevelMenu == NULL ) {
			return menu->Unknown5260B0();
		}
		pane = pane->GetParent();
	}
	return 0.0f;
}

/*
================
STSubPane::ResizeToContents

Stacks the children and takes the desired height. In a window the pane
is as wide as the window allows when expanding to fill it, else keeps
its width; in a menu it is as wide as the menu less the menu's indent.
With layout, the menus and sub-panes it sits in, up to its window,
resize to their contents and the window follows a change of size.
================
*/
// FUNCTION: STBC 0x00533d00
void STSubPane::ResizeToContents( bool layout ) {
	float oldWidth = GetWidth();
	TGPane *parentPane = parent;
	float oldHeight = GetHeight();
	STStylizedWindow *window = GetContainingWindow();
	float width = GetWidth();
	StackChildren();

	NiPoint2 size;
	GetDesiredSize( &size );
	if ( window != NULL ) {
		if ( expandToFillParent == true ) {
			size.x = window->GetMaximumInteriorWidth();
		} else {
			size.x = oldWidth;
		}
	} else if ( STMenu::Cast( GetConceptualParent() ) != NULL && STTopLevelMenu::Cast( GetConceptualParent() ) == NULL ) {
		float parentWidth = parentPane->GetWidth();
		size.x = parentWidth - GetMenuIndent();
	} else {
		size.x = width;
	}
	Resize( size.x, size.y, false );

	if ( !layout ) {
		return;
	}

	TGPane *pane = parent;
	while ( pane != NULL && pane != window ) {
		if ( STMenu::Cast( pane ) == NULL && STSubPane::Cast( pane ) == NULL ) {
			break;
		}
		STMenu *menu = STMenu::Cast( pane );
		STSubPane *subPane = STSubPane::Cast( pane );
		if ( menu != NULL ) {
			menu->ResizeToContents( false );
		}
		if ( subPane != NULL ) {
			subPane->ResizeToContents( false );
		}
		pane = pane->GetParent();
	}
	if ( window != NULL ) {
		if ( bounds.width != oldWidth || bounds.height != oldHeight ) {
			window->Unknown5314F0();
		}
		window->InteriorChangedSize( false );
		window->Layout();
	}
}

/*
================
STSubPane::KeyboardHandler

Runs the key's binding for the pane when nothing holds the mouse and the
pane is at most one level above the focused leaf.
================
*/
// FUNCTION: STBC 0x00533e90
void STSubPane::KeyboardHandler( TGKeyboardEvent *event ) {
	if ( g_kRootWindow.GetMouseGrabOwner() == NULL && FocusHeight() < 2 ) {
		if ( g_kKeyboardBinding.LaunchEvent( event->GetUnicode(), event->GetKeyState(), this ) ) {
			event->SetHandled();
		}
	}
	if ( !event->EventHandled() ) {
		CallNextHandler( event );
	}
}

/*
================
STSubPane::HandleSelectOption

Selects an option by number, moves the selection or activates it, when
nothing holds the mouse and the pane is at most one level above the
focused leaf; a used event is taken from its destination.
================
*/
// FUNCTION: STBC 0x00533ef0
void STSubPane::HandleSelectOption( TGIntEvent *event ) {
	if ( g_kRootWindow.GetMouseGrabOwner() != NULL ) {
		if ( event->GetDestination() != NULL ) {
			CallNextHandler( event );
		}
		return;
	}

	if ( event->GetEventType() == ET_INPUT_SELECT_X && FocusHeight() < 2 ) {
		if ( SelectOption( event->GetInt() - 1 ) == true ) {
			event->SetDestination( NULL );
		}
	}
	if ( event->GetEventType() == ET_INPUT_SELECT_OPTION && FocusHeight() < 2 ) {
		int offset = event->GetInt();
		if ( offset != 0 ) {
			if ( MoveSelection( offset ) == true ) {
				event->SetDestination( NULL );
			}
		} else if ( ActivateSelection() == true ) {
			event->SetDestination( NULL );
		}
	}
	if ( event->GetDestination() != NULL ) {
		CallNextHandler( event );
	}
}

/*
================
STSubPane::SelectOption
================
*/
// FUNCTION: STBC 0x00533fb0
bool STSubPane::SelectOption( int index ) {
	TGUIObject *child = GetFirstChild();
	int option = 0;
	bool selected = false;
	while ( child != NULL ) {
		if ( InterfaceModule::IsUIElement( child, 0 ) && child->IsVisible() ) {
			if ( option++ == index ) {
				break;
			}
		}
		child = GetNextChild( child );
	}

	if ( child != NULL && ( child->GetFlags() & TGUIO_ENABLED ) ) {
		child->SetHighlighted( true );
		SetFocus( child, true );
		selected = true;
		if ( child->IsTypeOf( STBUTTON_TYPE ) && ( child->GetFlags() & TGUIO_ENABLED ) ) {
			( (STButton *)child )->SendActivationEvent();
		}
		if ( child->IsTypeOf( STMENU_TYPE ) && ( child->GetFlags() & TGUIO_ENABLED ) ) {
			( (STMenu *)child )->Unknown527480();
		}
	}
	Layout();
	return selected;
}

/*
================
STSubPane::MoveSelection
================
*/
// FUNCTION: STBC 0x00534080
// PARKED: register allocation: control flow matches; retail loads offset into EBX before the pos != -1 test and keeps focus, then the child, in EBP (this build: focus in EBX, offset reloaded per branch)
bool STSubPane::MoveSelection( int offset ) {
	TGUIObject *focus = GetFocus();
	int moved = 0;
	int pos = FindPos( focus );
	if ( pos != -1 ) {
		TGUIObject *child;
		pos += offset;
		for ( ;; ) {
			if ( pos < 0 || pos >= GetNumChildren() ) {
				// Past an end: on into the neighbouring menus.
				if ( pos < 0 ) {
					return MoveToNeighbourMenu( pos );
				}
				return MoveToNeighbourMenu( pos - GetNumChildren() );
			}
			child = GetNthChild( pos );
			moved += offset;
			if ( child == NULL || ( child->IsVisible() && InterfaceModule::IsUIElement( child, 0 ) ) ) {
				break;
			}
			pos += offset;
		}
		if ( child == NULL ) {
			return false;
		}
		MoveFocus( moved, false, true );
	} else {
		SetFocus( GetNthChild( offset > 0 ? offset - 1 : GetNumChildren() + offset ), true );
	}

	if ( focus != NULL ) {
		focus->SetNotHighlighted( false );
	}
	if ( GetFocus() != NULL ) {
		GetFocus()->SetHighlighted( false );
	}
	// The original asks for the conceptual parent here and never uses it.
	GetConceptualParent();
	STStylizedWindow *window = GetContainingWindow();
	if ( window != NULL ) {
		window->EnsureFocusIsVisible();
		window->UpdateScrollArrows();
		window->Layout();
	}
	return true;
}

/*
================
STSubPane::MoveToNeighbourMenu
================
*/
// FUNCTION: STBC 0x00534210
bool STSubPane::MoveToNeighbourMenu( int offset ) {
	TGPane *menu = GetConceptualParent();
	if ( menu == NULL ) {
		return true;
	}
	TGPane *container = menu->GetConceptualParent();
	if ( container == NULL ) {
		return true;
	}
	if ( menu->IsTypeOf( STSTYLIZEDWINDOW_TYPE ) || container->IsTypeOf( STSTYLIZEDWINDOW_TYPE ) ) {
		return true;
	}

	int moved = 0;
	if ( offset < 0 ) {
		do {
			// BUG: redundant: menu is never NULL here (tested above and by the loop condition)
			if ( menu != NULL ) {
				// Skip the children that are not panes.
				TGUIObject *child = container->GetPrevChild( menu );
				while ( TGPane::Cast( child ) == NULL && child != NULL ) {
					child = container->GetPrevChild( child );
				}
				menu = TGPane::Cast( child );
				if ( menu != NULL ) {
					moved--;
				}
			}
		} while ( moved > offset && menu != NULL );
		if ( menu != NULL ) {
			container->SetFocus( menu, true );
			menu->SetFocus( InterfaceModule::GetLastUIElement( menu, 0 ), true );
		}
	} else {
		do {
			// BUG: redundant: menu is never NULL here (tested above and by the loop condition)
			if ( menu != NULL ) {
				// Skip the children that are not panes.
				TGUIObject *child = container->GetNextChild( menu );
				while ( TGPane::Cast( child ) == NULL && child != NULL ) {
					child = container->GetNextChild( child );
				}
				menu = TGPane::Cast( child );
				if ( menu != NULL ) {
					moved++;
				}
			}
		} while ( moved < offset && menu != NULL );
		if ( menu != NULL ) {
			container->SetFocus( menu, true );
			menu->SetFocus( InterfaceModule::GetFirstUIElement( menu, 0 ), true );
		}
	}

	STStylizedWindow *window = GetContainingWindow();
	if ( window != NULL ) {
		window->EnsureFocusIsVisible();
		window->UpdateScrollArrows();
		window->Layout();
	}
	return true;
}
