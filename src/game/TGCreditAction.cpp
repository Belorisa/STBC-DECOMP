/*
===============================================================================

	TGCreditAction.cpp

===============================================================================
*/

#include <iostream>
#include "TGCreditAction.h"
#include "TGPane.h"
#include "TGParagraph.h"
#include "TGStream.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006b24f0 _$E435
// FUNCTION: STBC 0x006b2510 _$E434

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006b1160 _$E32
// FUNCTION: STBC 0x006b1170 _$E29
// FUNCTION: STBC 0x006b1180 _$E31
// FUNCTION: STBC 0x006b1190 _$E30
// FUNCTION: STBC 0x006b11a0 _$E38
// FUNCTION: STBC 0x006b11b0 _$E35
// FUNCTION: STBC 0x006b11c0 _$E37
// FUNCTION: STBC 0x006b11d0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006b11e0 _$E64
// FUNCTION: STBC 0x006b11f0 _$E63
// FUNCTION: STBC 0x006b1200 _$E153
// FUNCTION: STBC 0x006b1210 _$E152
// FUNCTION: STBC 0x006b1220 _$E166
// FUNCTION: STBC 0x006b1230 _$E165
// FUNCTION: STBC 0x006b1240 _$E79
// FUNCTION: STBC 0x006b1250 _$E78
// FUNCTION: STBC 0x006b1260 _$E83
// FUNCTION: STBC 0x006b1270 _$E82
// FUNCTION: STBC 0x006b1280 _$E87
// FUNCTION: STBC 0x006b1290 _$E86
// FUNCTION: STBC 0x006b12a0 _$E91
// FUNCTION: STBC 0x006b12b0 _$E90
// FUNCTION: STBC 0x006b12c0 _$E186
// FUNCTION: STBC 0x006b12d0 _$E185
// FUNCTION: STBC 0x006b12e0 _$E211
// FUNCTION: STBC 0x006b12f0 _$E210
// FUNCTION: STBC 0x006b1300 _$E68
// FUNCTION: STBC 0x006b1310 _$E67
// FUNCTION: STBC 0x006b1320 _$E216
// FUNCTION: STBC 0x006b1330 _$E215
// FUNCTION: STBC 0x006b1340 _$E221
// FUNCTION: STBC 0x006b1350 _$E220
// FUNCTION: STBC 0x006b1360 _$E225
// FUNCTION: STBC 0x006b1370 _$E224
// FUNCTION: STBC 0x006b1380 _$E229
// FUNCTION: STBC 0x006b1390 _$E228
// FUNCTION: STBC 0x006b13a0 _$E236
// FUNCTION: STBC 0x006b13b0 _$E235
// FUNCTION: STBC 0x006b13c0 _$E264
// FUNCTION: STBC 0x006b13d0 _$E263
// FUNCTION: STBC 0x006b13e0 _$E268
// FUNCTION: STBC 0x006b13f0 _$E267
// FUNCTION: STBC 0x006b1400 _$E115
// FUNCTION: STBC 0x006b1410 _$E114
// FUNCTION: STBC 0x006b1420 _$E250
// FUNCTION: STBC 0x006b1430 _$E249
// FUNCTION: STBC 0x006b1440 _$E257
// FUNCTION: STBC 0x006b1450 _$E256
// FUNCTION: STBC 0x006b1460 _$E280
// FUNCTION: STBC 0x006b1470 _$E279
// FUNCTION: STBC 0x006b1480 _$E284
// FUNCTION: STBC 0x006b1490 _$E283
// FUNCTION: STBC 0x006b14a0 _$E324
// FUNCTION: STBC 0x006b14b0 _$E323
// FUNCTION: STBC 0x006b14c0 _$E330
// FUNCTION: STBC 0x006b14d0 _$E329
// FUNCTION: STBC 0x006b14e0 _$E272
// FUNCTION: STBC 0x006b14f0 _$E271
// FUNCTION: STBC 0x006b1500 _$E276
// FUNCTION: STBC 0x006b1510 _$E275
// FUNCTION: STBC 0x006b1520 _$E288
// FUNCTION: STBC 0x006b1530 _$E287
// FUNCTION: STBC 0x006b1540 _$E292
// FUNCTION: STBC 0x006b1550 _$E291
// FUNCTION: STBC 0x006b1560 _$E296
// FUNCTION: STBC 0x006b1570 _$E295
// FUNCTION: STBC 0x006b1580 _$E300
// FUNCTION: STBC 0x006b1590 _$E299
// FUNCTION: STBC 0x006b15a0 _$E304
// FUNCTION: STBC 0x006b15b0 _$E303
// FUNCTION: STBC 0x006b15c0 _$E308
// FUNCTION: STBC 0x006b15d0 _$E307
// FUNCTION: STBC 0x006b15e0 _$E312
// FUNCTION: STBC 0x006b15f0 _$E311
// FUNCTION: STBC 0x006b1600 _$E316
// FUNCTION: STBC 0x006b1610 _$E315
// FUNCTION: STBC 0x006b1620 _$E120
// FUNCTION: STBC 0x006b1630 _$E119
// FUNCTION: STBC 0x006b1640 _$E320
// FUNCTION: STBC 0x006b1650 _$E319
// FUNCTION: STBC 0x006b1660 _$E125
// FUNCTION: STBC 0x006b1670 _$E124
// FUNCTION: STBC 0x006b1680 _$E334
// FUNCTION: STBC 0x006b1690 _$E333
// FUNCTION: STBC 0x006b16a0 _$E338
// FUNCTION: STBC 0x006b16b0 _$E337
// FUNCTION: STBC 0x006b16c0 _$E342
// FUNCTION: STBC 0x006b16d0 _$E341
// FUNCTION: STBC 0x006b16e0 _$E346
// FUNCTION: STBC 0x006b16f0 _$E345
// FUNCTION: STBC 0x006b1700 _$E130
// FUNCTION: STBC 0x006b1710 _$E129
// FUNCTION: STBC 0x006b1720 _$E350
// FUNCTION: STBC 0x006b1730 _$E349
// FUNCTION: STBC 0x006b1740 _$E354
// FUNCTION: STBC 0x006b1750 _$E353
// FUNCTION: STBC 0x006b1760 _$E358
// FUNCTION: STBC 0x006b1770 _$E357
// FUNCTION: STBC 0x006b1780 _$E136
// FUNCTION: STBC 0x006b1790 _$E135
// FUNCTION: STBC 0x006b17a0 _$E141
// FUNCTION: STBC 0x006b17b0 _$E140
// FUNCTION: STBC 0x006b17c0 _$E171
// FUNCTION: STBC 0x006b17d0 _$E170
// FUNCTION: STBC 0x006b17e0 _$E362
// FUNCTION: STBC 0x006b17f0 _$E361
// FUNCTION: STBC 0x006b1800 _$E176
// FUNCTION: STBC 0x006b1810 _$E175
// FUNCTION: STBC 0x006b1820 _$E366
// FUNCTION: STBC 0x006b1830 _$E365
// FUNCTION: STBC 0x006b1840 _$E370
// FUNCTION: STBC 0x006b1850 _$E369
// FUNCTION: STBC 0x006b1860 _$E181
// FUNCTION: STBC 0x006b1870 _$E180
// FUNCTION: STBC 0x006b1880 _$E374
// FUNCTION: STBC 0x006b1890 _$E373
// FUNCTION: STBC 0x006b18a0 _$E146
// FUNCTION: STBC 0x006b18b0 _$E145
// FUNCTION: STBC 0x006b18c0 _$E378
// FUNCTION: STBC 0x006b18d0 _$E377
// FUNCTION: STBC 0x006b18e0 _$E382
// FUNCTION: STBC 0x006b18f0 _$E381
// FUNCTION: STBC 0x006b1900 _$E191
// FUNCTION: STBC 0x006b1910 _$E190
// FUNCTION: STBC 0x006b1920 _$E386
// FUNCTION: STBC 0x006b1930 _$E385
// FUNCTION: STBC 0x006b1940 _$E390
// FUNCTION: STBC 0x006b1950 _$E389
// FUNCTION: STBC 0x006b1960 _$E394
// FUNCTION: STBC 0x006b1970 _$E393
// FUNCTION: STBC 0x006b1980 _$E398
// FUNCTION: STBC 0x006b1990 _$E397
// FUNCTION: STBC 0x006b19a0 _$E402
// FUNCTION: STBC 0x006b19b0 _$E401
// FUNCTION: STBC 0x006b19c0 _$E406
// FUNCTION: STBC 0x006b19d0 _$E405
// FUNCTION: STBC 0x006b19e0 _$E410
// FUNCTION: STBC 0x006b19f0 _$E409
// FUNCTION: STBC 0x006b1a00 _$E414
// FUNCTION: STBC 0x006b1a10 _$E413
// FUNCTION: STBC 0x006b1a20 _$E196
// FUNCTION: STBC 0x006b1a30 _$E195
// FUNCTION: STBC 0x006b1a40 _$E418
// FUNCTION: STBC 0x006b1a50 _$E417
// FUNCTION: STBC 0x006b1a60 _$E201
// FUNCTION: STBC 0x006b1a70 _$E200
// FUNCTION: STBC 0x006b1a80 _$E206
// FUNCTION: STBC 0x006b1a90 _$E205

// The dynamic initialisers of the two statics, and the list's exit-time
// destructor.
// FUNCTION: STBC 0x006b1aa0 _$E421
// FUNCTION: STBC 0x006b1ab0 _$E420
// FUNCTION: STBC 0x006b1ae0 _$E427
// FUNCTION: STBC 0x006b1af0 _$E423
// FUNCTION: STBC 0x006b1b20 _$E426
// FUNCTION: STBC 0x006b1b30 _$E424
// DATA: STBC 0x00995c40
NiColorA TGCreditAction::defaultColor( 1.0f, 1.0f, 1.0f, 0.0f );
// DATA: STBC 0x00995c80
NiTList<TGCreditAction *> TGCreditAction::credits( 2 );

/*
================
TGCreditAction::TGCreditAction
================
*/
// FUNCTION: STBC 0x006b1ba0 ??0TGCreditAction@@QAE@PAVTGString@@PAVTGPane@@MMMMMHHH@Z
TGCreditAction::TGCreditAction( TGString *string, TGPane *pane, float left_, float top_, float duration_,
		float fadeIn_, float fadeOut_, int fontSize_, int horizontalJustify_, int verticalJustify_ ) : text( "" ) {
	text = *string;
	if ( pane != NULL ) {
		paneID = pane->GetObjID();
	} else {
		paneID = 0;
	}
	left = left_;
	top = top_;
	duration = duration_;
	fontSize = fontSize_;
	fadeIn = fadeIn_;
	fadeOut = fadeOut_;
	horizontalJustify = horizontalJustify_;
	verticalJustify = verticalJustify_;
	paragraphID = 0;
	color = defaultColor;
	useRealTime = true;
	credits.AddTail( this );
}

/*
================
TGCreditAction::GetObjType
================
*/
// FUNCTION: STBC 0x006b1d50
int TGCreditAction::GetObjType( void ) {
	return TGCREDITACTION_TYPE;
}

/*
================
TGCreditAction::IsTypeOf
================
*/
// FUNCTION: STBC 0x006b1d60
bool TGCreditAction::IsTypeOf( int type ) {
	if ( type == TGCREDITACTION_TYPE ) {
		return true;
	}
	return TGTimedAction::IsTypeOf( type );
}

/*
================
TGCreditAction::GetClassName
================
*/
// FUNCTION: STBC 0x006b1da0
char *TGCreditAction::GetClassName( void ) {
	return "TGCreditAction";
}

/*
================
TGCreditAction::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x006b1db0
char *TGCreditAction::GetSwigTypeName( void ) {
	return "_p_TGCreditAction";
}

/*
================
TGCreditAction::GetPythonClassName
================
*/
// FUNCTION: STBC 0x006b1dc0
char *TGCreditAction::GetPythonClassName( void ) {
	return "TGCreditActionPtr";
}

/*
================
TGCreditAction::TGCreditAction

The load's constructor: the members come from ReadFromStream.
================
*/
// FUNCTION: STBC 0x006b1e00 ??0TGCreditAction@@QAE@J@Z
TGCreditAction::TGCreditAction( long id ) : TGTimedAction( id ), text( "" ) {
	paragraphID = 0;
	paneID = 0;
	credits.AddTail( this );
}

/*
================
TGCreditAction::~TGCreditAction

Takes the paragraph out of the pane and deletes it.
================
*/
// FUNCTION: STBC 0x006b1f30
TGCreditAction::~TGCreditAction( void ) {
	if ( paragraphID != 0 && paneID != 0 ) {
		TGPane *pane = (TGPane *)TGObject::GetTGObjectPtr( paneID );
		TGParagraph *paragraph = (TGParagraph *)TGObject::GetTGObjectPtr( paragraphID );
		if ( pane != NULL ) {
			if ( paragraph != NULL ) {
				pane->RemoveChild( paragraph );
				delete paragraph;
			}
		} else if ( paragraph != NULL ) {
			delete paragraph;
		}
	}
	credits.Remove( this );
}

/*
================
TGCreditAction::Play

Makes the paragraph, transparent, and puts it in the pane; the action
lasts its fades as well as its duration.
================
*/
// FUNCTION: STBC 0x006b20b0
void TGCreditAction::Play( void ) {
	NiColorA startColor = color;
	startColor.a = 0.0f;
	TGParagraph *paragraph = new TGParagraph( &text, 1.0f, &startColor, "Crillee", (float)fontSize, TGPF_READ_ONLY | TGPF_WORD_WRAP );
	paragraphID = paragraph->GetObjID();

	float x;
	switch ( horizontalJustify ) {
		case JUSTIFY_LEFT:
		case JUSTIFY_TOP:
			x = left;
			break;
		case JUSTIFY_RIGHT:
		case JUSTIFY_BOTTOM:
			x = left - paragraph->GetWidth();
			break;
		case JUSTIFY_CENTER:
			x = ( 1.0f - paragraph->GetWidth() ) * 0.5f;
			break;
	}

	float y;
	switch ( verticalJustify ) {
		case JUSTIFY_LEFT:
		case JUSTIFY_TOP:
			y = top;
			break;
		case JUSTIFY_RIGHT:
		case JUSTIFY_BOTTOM:
			y = top - paragraph->GetHeight();
			break;
		case JUSTIFY_CENTER:
			y = ( 1.0f - paragraph->GetHeight() ) * 0.5f;
			break;
	}

	TGPane *pane = (TGPane *)TGObject::GetTGObjectPtr( paneID );
	if ( pane != NULL ) {
		pane->InsertChild( 0x7FFFFFFF, paragraph, x, y );
	}

	duration = duration + ( fadeIn + fadeOut );
	TGTimedAction::Play();
	duration -= fadeIn + fadeOut;
}

/*
================
TGCreditAction::UpdateFades
================
*/
// FUNCTION: STBC 0x006b2210
void TGCreditAction::UpdateFades( float time ) {
	NiTListIterator pos = credits.GetHeadPos();
	while ( pos != NULL ) {
		TGCreditAction *credit = credits.GetNext( pos );
		if ( credit != NULL && credit->startTime > ACTION_NOT_PLAYING ) {
			float alpha = 1.0f;
			float elapsed = time - credit->startTime;
			if ( time - credit->startTime < credit->fadeIn ) {
				alpha = ( 1.0f / credit->fadeIn ) * ( time - credit->startTime );
			} else if ( credit->fadeIn + credit->duration < elapsed ) {
				alpha = 1.0f - ( elapsed - credit->fadeIn - credit->duration ) * ( 1.0f / credit->fadeOut );
			}
			NiColorA fadeColor = credit->color;
			fadeColor.a = alpha;
			if ( credit->paragraphID != 0 ) {
				TGParagraph *paragraph = (TGParagraph *)TGObject::GetTGObjectPtr( credit->paragraphID );
				if ( paragraph != NULL ) {
					paragraph->SetColor( &fadeColor );
				} else {
					credits.Remove( credit );
				}
			}
		}
	}
}

/*
================
TGCreditAction::WriteToStream
================
*/
// FUNCTION: STBC 0x006b23c0
bool TGCreditAction::WriteToStream( TGStream *stream ) {
	if ( !TGTimedAction::WriteToStream( stream ) ) {
		return false;
	}
	if ( !text.WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteInt( fontSize );
	stream->WriteID( paragraphID );
	stream->WriteID( paneID );
	stream->WriteFloat( left );
	stream->WriteFloat( top );
	stream->WriteFloat( fadeIn );
	stream->WriteFloat( fadeOut );
	stream->WriteInt( horizontalJustify );
	stream->WriteInt( verticalJustify );
	return true;
}

/*
================
TGCreditAction::ReadFromStream
================
*/
// FUNCTION: STBC 0x006b2460
bool TGCreditAction::ReadFromStream( TGStream *stream ) {
	if ( !TGTimedAction::ReadFromStream( stream ) ) {
		return false;
	}
	if ( !text.ReadFromStream( stream ) ) {
		return false;
	}
	fontSize = stream->ReadInt();
	paragraphID = stream->ReadID();
	paneID = stream->ReadID();
	left = stream->ReadFloat();
	top = stream->ReadFloat();
	fadeIn = stream->ReadFloat();
	fadeOut = stream->ReadFloat();
	horizontalJustify = stream->ReadInt();
	verticalJustify = stream->ReadInt();
	return true;
}
