/*
===============================================================================

	STSubPaneStream.cpp

	The rest of STSubPane, in a unit of its own: retail places these
	functions after the static initializers of <iostream>, which the
	sub pane's main unit does not include. The bytes would be the same
	from one file that includes <iostream> before these functions; the
	split and the unit's name are invented.

===============================================================================
*/

#include <iostream>

#include "STSubPane.h"
#include "STButton.h"
#include "STMenu.h"
#include "TGStream.h"
#include "NiMain.h"

/*
================
STSubPane::ActivateSelection

Activates the focused option when it is enabled: a menu sends its
activation event and opens, a button sends its activation event. False
without a focused option.
================
*/
// FUNCTION: STBC 0x00534420
bool STSubPane::ActivateSelection( void ) {
	TGUIObject *focus = GetFocus();
	if ( focus == NULL ) {
		return false;
	}

	NiTListIterator pos = children.GetHeadPos();
	while ( pos ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child == focus && ( child->GetFlags() & TGUIO_ENABLED ) ) {
			if ( child->IsTypeOf( STMENU_TYPE ) ) {
				STMenu *menu = (STMenu *)child;
				menu->SendActivationEvent();
				if ( !menu->IsOpened() ) {
					menu->Open( true );
				}
				if ( menu->IsClickedOnce() ) {
					menu->ClearClickedOnce();
				}
			} else if ( child->IsTypeOf( STBUTTON_TYPE ) ) {
				( (STButton *)child )->SendActivationEvent();
			}
		}
	}
	return true;
}

/*
================
STSubPane::WriteToStream
================
*/
// FUNCTION: STBC 0x005344c0
bool STSubPane::WriteToStream( TGStream *stream ) {
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( vertSpacing );
	stream->WriteBool( expandToFillParent );
	return true;
}

/*
================
STSubPane::ReadFromStream
================
*/
// FUNCTION: STBC 0x00534500
bool STSubPane::ReadFromStream( TGStream *stream ) {
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	vertSpacing = stream->ReadFloat();
	expandToFillParent = stream->ReadBool();
	return true;
}
