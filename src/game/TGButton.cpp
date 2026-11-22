/*
===============================================================================

	TGButton.cpp

===============================================================================
*/

#include <iostream>
#include "TGButton.h"
#include "TGIcon.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include "NiColor.h"

#include <string.h>
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00743d80 _$E428
// FUNCTION: STBC 0x00743da0 _$E427

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00742f50 _$E32
// FUNCTION: STBC 0x00742f60 _$E29
// FUNCTION: STBC 0x00742f70 _$E31
// FUNCTION: STBC 0x00742f80 _$E30
// FUNCTION: STBC 0x00742f90 _$E38
// FUNCTION: STBC 0x00742fa0 _$E35
// FUNCTION: STBC 0x00742fb0 _$E37
// FUNCTION: STBC 0x00742fc0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00742fd0 _$E57
// FUNCTION: STBC 0x00742fe0 _$E56
// FUNCTION: STBC 0x00742ff0 _$E61
// FUNCTION: STBC 0x00743000 _$E60
// FUNCTION: STBC 0x00743010 _$E72
// FUNCTION: STBC 0x00743020 _$E71
// FUNCTION: STBC 0x00743030 _$E76
// FUNCTION: STBC 0x00743040 _$E75
// FUNCTION: STBC 0x00743050 _$E80
// FUNCTION: STBC 0x00743060 _$E79
// FUNCTION: STBC 0x00743070 _$E84
// FUNCTION: STBC 0x00743080 _$E83
// FUNCTION: STBC 0x00743090 _$E208
// FUNCTION: STBC 0x007430a0 _$E207
// FUNCTION: STBC 0x007430b0 _$E150
// FUNCTION: STBC 0x007430c0 _$E149
// FUNCTION: STBC 0x007430d0 _$E163
// FUNCTION: STBC 0x007430e0 _$E162
// FUNCTION: STBC 0x007430f0 _$E213
// FUNCTION: STBC 0x00743100 _$E212
// FUNCTION: STBC 0x00743110 _$E218
// FUNCTION: STBC 0x00743120 _$E217
// FUNCTION: STBC 0x00743130 _$E222
// FUNCTION: STBC 0x00743140 _$E221
// FUNCTION: STBC 0x00743150 _$E226
// FUNCTION: STBC 0x00743160 _$E225
// FUNCTION: STBC 0x00743170 _$E233
// FUNCTION: STBC 0x00743180 _$E232
// FUNCTION: STBC 0x00743190 _$E260
// FUNCTION: STBC 0x007431a0 _$E259
// FUNCTION: STBC 0x007431b0 _$E264
// FUNCTION: STBC 0x007431c0 _$E263
// FUNCTION: STBC 0x007431d0 _$E108
// FUNCTION: STBC 0x007431e0 _$E107
// FUNCTION: STBC 0x007431f0 _$E246
// FUNCTION: STBC 0x00743200 _$E245
// FUNCTION: STBC 0x00743210 _$E253
// FUNCTION: STBC 0x00743220 _$E252
// FUNCTION: STBC 0x00743230 _$E276
// FUNCTION: STBC 0x00743240 _$E275
// FUNCTION: STBC 0x00743250 _$E280
// FUNCTION: STBC 0x00743260 _$E279
// FUNCTION: STBC 0x00743270 _$E320
// FUNCTION: STBC 0x00743280 _$E319
// FUNCTION: STBC 0x00743290 _$E326
// FUNCTION: STBC 0x007432a0 _$E325
// FUNCTION: STBC 0x007432b0 _$E268
// FUNCTION: STBC 0x007432c0 _$E267
// FUNCTION: STBC 0x007432d0 _$E272
// FUNCTION: STBC 0x007432e0 _$E271
// FUNCTION: STBC 0x007432f0 _$E284
// FUNCTION: STBC 0x00743300 _$E283
// FUNCTION: STBC 0x00743310 _$E288
// FUNCTION: STBC 0x00743320 _$E287
// FUNCTION: STBC 0x00743330 _$E292
// FUNCTION: STBC 0x00743340 _$E291
// FUNCTION: STBC 0x00743350 _$E296
// FUNCTION: STBC 0x00743360 _$E295
// FUNCTION: STBC 0x00743370 _$E300
// FUNCTION: STBC 0x00743380 _$E299
// FUNCTION: STBC 0x00743390 _$E304
// FUNCTION: STBC 0x007433a0 _$E303
// FUNCTION: STBC 0x007433b0 _$E308
// FUNCTION: STBC 0x007433c0 _$E307
// FUNCTION: STBC 0x007433d0 _$E312
// FUNCTION: STBC 0x007433e0 _$E311
// FUNCTION: STBC 0x007433f0 _$E113
// FUNCTION: STBC 0x00743400 _$E112
// FUNCTION: STBC 0x00743410 _$E316
// FUNCTION: STBC 0x00743420 _$E315
// FUNCTION: STBC 0x00743430 _$E118
// FUNCTION: STBC 0x00743440 _$E117
// FUNCTION: STBC 0x00743450 _$E330
// FUNCTION: STBC 0x00743460 _$E329
// FUNCTION: STBC 0x00743470 _$E334
// FUNCTION: STBC 0x00743480 _$E333
// FUNCTION: STBC 0x00743490 _$E338
// FUNCTION: STBC 0x007434a0 _$E337
// FUNCTION: STBC 0x007434b0 _$E342
// FUNCTION: STBC 0x007434c0 _$E341
// FUNCTION: STBC 0x007434d0 _$E127
// FUNCTION: STBC 0x007434e0 _$E126
// FUNCTION: STBC 0x007434f0 _$E346
// FUNCTION: STBC 0x00743500 _$E345
// FUNCTION: STBC 0x00743510 _$E350
// FUNCTION: STBC 0x00743520 _$E349
// FUNCTION: STBC 0x00743530 _$E354
// FUNCTION: STBC 0x00743540 _$E353
// FUNCTION: STBC 0x00743550 _$E133
// FUNCTION: STBC 0x00743560 _$E132
// FUNCTION: STBC 0x00743570 _$E138
// FUNCTION: STBC 0x00743580 _$E137
// FUNCTION: STBC 0x00743590 _$E168
// FUNCTION: STBC 0x007435a0 _$E167
// FUNCTION: STBC 0x007435b0 _$E358
// FUNCTION: STBC 0x007435c0 _$E357
// FUNCTION: STBC 0x007435d0 _$E173
// FUNCTION: STBC 0x007435e0 _$E172
// FUNCTION: STBC 0x007435f0 _$E362
// FUNCTION: STBC 0x00743600 _$E361
// FUNCTION: STBC 0x00743610 _$E366
// FUNCTION: STBC 0x00743620 _$E365
// FUNCTION: STBC 0x00743630 _$E178
// FUNCTION: STBC 0x00743640 _$E177
// FUNCTION: STBC 0x00743650 _$E370
// FUNCTION: STBC 0x00743660 _$E369
// FUNCTION: STBC 0x00743670 _$E143
// FUNCTION: STBC 0x00743680 _$E142
// FUNCTION: STBC 0x00743690 _$E374
// FUNCTION: STBC 0x007436a0 _$E373
// FUNCTION: STBC 0x007436b0 _$E378
// FUNCTION: STBC 0x007436c0 _$E377
// FUNCTION: STBC 0x007436d0 _$E183
// FUNCTION: STBC 0x007436e0 _$E182
// FUNCTION: STBC 0x007436f0 _$E188
// FUNCTION: STBC 0x00743700 _$E187
// FUNCTION: STBC 0x00743710 _$E382
// FUNCTION: STBC 0x00743720 _$E381
// FUNCTION: STBC 0x00743730 _$E386
// FUNCTION: STBC 0x00743740 _$E385
// FUNCTION: STBC 0x00743750 _$E390
// FUNCTION: STBC 0x00743760 _$E389
// FUNCTION: STBC 0x00743770 _$E394
// FUNCTION: STBC 0x00743780 _$E393
// FUNCTION: STBC 0x00743790 _$E398
// FUNCTION: STBC 0x007437a0 _$E397
// FUNCTION: STBC 0x007437b0 _$E402
// FUNCTION: STBC 0x007437c0 _$E401
// FUNCTION: STBC 0x007437d0 _$E406
// FUNCTION: STBC 0x007437e0 _$E405
// FUNCTION: STBC 0x007437f0 _$E410
// FUNCTION: STBC 0x00743800 _$E409
// FUNCTION: STBC 0x00743810 _$E193
// FUNCTION: STBC 0x00743820 _$E192
// FUNCTION: STBC 0x00743830 _$E414
// FUNCTION: STBC 0x00743840 _$E413
// FUNCTION: STBC 0x00743850 _$E198
// FUNCTION: STBC 0x00743860 _$E197
// FUNCTION: STBC 0x00743870 _$E203
// FUNCTION: STBC 0x00743880 _$E202

/*
================
TGButton::TGButton

A disabled, unselected button showing the group's icons, tinted with a
copy of the colour when one is given.
================
*/
// FUNCTION: STBC 0x007438e0
TGButton::TGButton( char *groupName, int firstIconNum, NiColorA *color ) {
	TGUIObject::SetEnabled( false );
	TGUIObject::SetNotSelected( false );
	iconGroupName = NULL;
	if ( color != NULL ) {
		iconColor = new NiColorA;
		*iconColor = *color;
	} else {
		iconColor = NULL;
	}
	SetIcons( groupName, firstIconNum );
}

/*
================
TGButton::~TGButton
================
*/
// FUNCTION: STBC 0x00743990
TGButton::~TGButton( void ) {
	if ( iconGroupName != NULL ) {
		delete[] iconGroupName;
		iconGroupName = NULL;
	}
	if ( iconColor != NULL ) {
		delete iconColor;
		iconColor = NULL;
	}
}

/*
================
TGButton::Cast

The object as an icon button; NULL when it is none.
================
*/
// FUNCTION: STBC 0x007439d0
TGButton *TGButton::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGBUTTON_TYPE ) ) {
		return (TGButton *)object;
	}
	return NULL;
}

/*
================
TGButton::SetIcons

Replaces the button's icon with one of the named group.
================
*/
// FUNCTION: STBC 0x00743a00
void TGButton::SetIcons( char *groupName, int firstIconNum ) {
	KillChildren();
	if ( iconGroupName != NULL ) {
		// BUG: the old name is not cleared, so a button given no group keeps
		// a pointer to freed memory.
		delete[] iconGroupName;
	}
	if ( groupName == NULL ) {
		return;
	}

	iconGroupName = new char[strlen( groupName ) + 1];
	strcpy( iconGroupName, groupName );
	this->firstIconNum = firstIconNum;

	TGIcon *icon = new TGIcon( groupName, firstIconNum );
	if ( icon == NULL ) {
		return;
	}
	AddChild( icon, 0.0f, 0.0f, false );
	UpdateIcon( true );
	if ( iconColor != NULL ) {
		icon->SetColor( iconColor );
	}
}

/*
================
TGButton::SetIconColor

Tints the icon with a copy of the colour; NULL leaves it white.
================
*/
// FUNCTION: STBC 0x00743ad0
void TGButton::SetIconColor( NiColorA *color ) {
	if ( iconColor != NULL ) {
		delete iconColor;
		iconColor = NULL;
	}
	if ( color != NULL ) {
		iconColor = new NiColorA;
		*iconColor = *color;
	}
	UpdateIcon( true );
}

/*
================
TGButton::UpdateIcon

Shows the icon for the button's state, resizing the button to it, and
tints it.
================
*/
// FUNCTION: STBC 0x00743b40
void TGButton::UpdateIcon( bool layout ) {
	if ( iconGroupName == NULL ) {
		return;
	}
	TGIcon *icon = (TGIcon *)GetFirstChild();
	if ( icon == NULL ) {
		return;
	}

	int iconNum = firstIconNum + 2;		// disabled
	if ( flags & TGUIO_ENABLED ) {
		if ( flags & TGUIO_SELECTED ) {
			iconNum = firstIconNum;
		} else {
			iconNum = firstIconNum + 1;
		}
	}
	if ( iconNum != icon->GetIconNum() ) {
		icon->SetIconNum( iconNum, false );
		icon->SizeToArtwork( false );
		Resize( icon->GetWidth(), icon->GetHeight(), layout );
	}
	icon->SetColor( iconColor );
}

/*
================
TGButton::SetSelected
================
*/
// FUNCTION: STBC 0x00743bc0
void TGButton::SetSelected( bool layout ) {
	TGUIObject::SetSelected( false );
	UpdateIcon( layout );
}

/*
================
TGButton::SetNotSelected
================
*/
// FUNCTION: STBC 0x00743be0
void TGButton::SetNotSelected( bool layout ) {
	TGUIObject::SetNotSelected( false );
	UpdateIcon( layout );
}

/*
================
TGButton::SetEnabled
================
*/
// FUNCTION: STBC 0x00743c00
void TGButton::SetEnabled( bool layout ) {
	TGUIObject::SetEnabled( false );
	UpdateIcon( layout );
}

/*
================
TGButton::SetDisabled
================
*/
// FUNCTION: STBC 0x00743c20
void TGButton::SetDisabled( bool layout ) {
	TGButtonBase::SetDisabled( false );
	UpdateIcon( layout );
}

/*
================
TGButton::WriteToStream

The colour follows a flag that says whether there is one.
================
*/
// FUNCTION: STBC 0x00743c40
bool TGButton::WriteToStream( TGStream *stream ) {
	if ( !TGButtonBase::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteLong( firstIconNum );
	stream->VirtualSlot11( iconGroupName );
	if ( iconColor != NULL ) {
		stream->WriteBool( true );
		stream->WriteFloat( iconColor->r );
		stream->WriteFloat( iconColor->g );
		stream->WriteFloat( iconColor->b );
		stream->WriteFloat( iconColor->a );
	} else {
		stream->WriteBool( false );
	}
	return true;
}

/*
================
TGButton::ReadFromStream

The icon itself is a child, saved with the pane.
================
*/
// FUNCTION: STBC 0x00743cd0
bool TGButton::ReadFromStream( TGStream *stream ) {
	if ( !TGButtonBase::ReadFromStream( stream ) ) {
		return false;
	}
	firstIconNum = stream->ReadLong();
	iconGroupName = stream->VirtualSlot10( -1 );
	if ( stream->ReadBool() == true ) {
		iconColor = new NiColorA;
		iconColor->r = stream->ReadFloat();
		iconColor->g = stream->ReadFloat();
		iconColor->b = stream->ReadFloat();
		iconColor->a = stream->ReadFloat();
	} else {
		iconColor = NULL;
	}
	return true;
}

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x00743890 _$E420
// FUNCTION: STBC 0x007438a0 _$E416
// FUNCTION: STBC 0x007438b0 _$E419
// FUNCTION: STBC 0x007438c0 _$E417
// DATA: STBC 0x0099e618
TGEventHandlerTable TGButton::classHandlers;
