/*
===============================================================================

	STCheckbox.cpp

===============================================================================
*/

#include "STCheckbox.h"
#include "InterfaceModule.h"
#include "GraphicsModeInfo.h"
#include "TGIcon.h"
#include "TGStream.h"
#include "NiPoint2.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0051cb60 _$E438
// FUNCTION: STBC 0x0051cb80 _$E437

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0051c6e0 _$E33
// FUNCTION: STBC 0x0051c6f0 _$E32
// FUNCTION: STBC 0x0051c700 _$E37
// FUNCTION: STBC 0x0051c710 _$E36
// FUNCTION: STBC 0x0051c720 _$E48
// FUNCTION: STBC 0x0051c730 _$E47
// FUNCTION: STBC 0x0051c740 _$E52
// FUNCTION: STBC 0x0051c750 _$E51
// FUNCTION: STBC 0x0051c760 _$E56
// FUNCTION: STBC 0x0051c770 _$E55
// FUNCTION: STBC 0x0051c780 _$E60
// FUNCTION: STBC 0x0051c790 _$E59
// FUNCTION: STBC 0x0051c7a0 _$E184
// FUNCTION: STBC 0x0051c7b0 _$E183
// FUNCTION: STBC 0x0051c7c0 _$E126
// FUNCTION: STBC 0x0051c7d0 _$E125
// FUNCTION: STBC 0x0051c7e0 _$E139
// FUNCTION: STBC 0x0051c7f0 _$E138
// FUNCTION: STBC 0x0051c800 _$E189
// FUNCTION: STBC 0x0051c810 _$E188
// FUNCTION: STBC 0x0051c820 _$E194
// FUNCTION: STBC 0x0051c830 _$E193
// FUNCTION: STBC 0x0051c840 _$E198
// FUNCTION: STBC 0x0051c850 _$E197
// FUNCTION: STBC 0x0051c860 _$E202
// FUNCTION: STBC 0x0051c870 _$E201
// FUNCTION: STBC 0x0051c880 _$E209
// FUNCTION: STBC 0x0051c890 _$E208
// FUNCTION: STBC 0x0051c8a0 _$E263
// FUNCTION: STBC 0x0051c8b0 _$E262
// FUNCTION: STBC 0x0051c8c0 _$E267
// FUNCTION: STBC 0x0051c8d0 _$E266
// FUNCTION: STBC 0x0051c8e0 _$E84
// FUNCTION: STBC 0x0051c8f0 _$E83
// FUNCTION: STBC 0x0051c900 _$E249
// FUNCTION: STBC 0x0051c910 _$E248
// FUNCTION: STBC 0x0051c920 _$E256
// FUNCTION: STBC 0x0051c930 _$E255
// FUNCTION: STBC 0x0051c940 _$E279
// FUNCTION: STBC 0x0051c950 _$E278
// FUNCTION: STBC 0x0051c960 _$E283
// FUNCTION: STBC 0x0051c970 _$E282
// FUNCTION: STBC 0x0051c980 _$E323
// FUNCTION: STBC 0x0051c990 _$E322
// FUNCTION: STBC 0x0051c9a0 _$E329
// FUNCTION: STBC 0x0051c9b0 _$E328
// FUNCTION: STBC 0x0051c9c0 _$E271
// FUNCTION: STBC 0x0051c9d0 _$E270
// FUNCTION: STBC 0x0051c9e0 _$E275
// FUNCTION: STBC 0x0051c9f0 _$E274
// FUNCTION: STBC 0x0051ca00 _$E287
// FUNCTION: STBC 0x0051ca10 _$E286
// FUNCTION: STBC 0x0051ca20 _$E291
// FUNCTION: STBC 0x0051ca30 _$E290
// FUNCTION: STBC 0x0051ca40 _$E295
// FUNCTION: STBC 0x0051ca50 _$E294
// FUNCTION: STBC 0x0051ca60 _$E299
// FUNCTION: STBC 0x0051ca70 _$E298
// FUNCTION: STBC 0x0051ca80 _$E303
// FUNCTION: STBC 0x0051ca90 _$E302
// FUNCTION: STBC 0x0051caa0 _$E307
// FUNCTION: STBC 0x0051cab0 _$E306
// FUNCTION: STBC 0x0051cac0 _$E311
// FUNCTION: STBC 0x0051cad0 _$E310
// FUNCTION: STBC 0x0051cae0 _$E315
// FUNCTION: STBC 0x0051caf0 _$E314
// FUNCTION: STBC 0x0051cb00 _$E89
// FUNCTION: STBC 0x0051cb10 _$E88
// FUNCTION: STBC 0x0051cb20 _$E319
// FUNCTION: STBC 0x0051cb30 _$E318
// FUNCTION: STBC 0x0051cb40 _$E94
// FUNCTION: STBC 0x0051cb50 _$E93
// FUNCTION: STBC 0x0051cb90 _$E333
// FUNCTION: STBC 0x0051cba0 _$E332
// FUNCTION: STBC 0x0051cbb0 _$E337
// FUNCTION: STBC 0x0051cbc0 _$E336
// FUNCTION: STBC 0x0051cbd0 _$E341
// FUNCTION: STBC 0x0051cbe0 _$E340
// FUNCTION: STBC 0x0051cbf0 _$E345
// FUNCTION: STBC 0x0051cc00 _$E344
// FUNCTION: STBC 0x0051cc10 _$E103
// FUNCTION: STBC 0x0051cc20 _$E102
// FUNCTION: STBC 0x0051cc30 _$E349
// FUNCTION: STBC 0x0051cc40 _$E348
// FUNCTION: STBC 0x0051cc50 _$E353
// FUNCTION: STBC 0x0051cc60 _$E352
// FUNCTION: STBC 0x0051cc70 _$E357
// FUNCTION: STBC 0x0051cc80 _$E356
// FUNCTION: STBC 0x0051cc90 _$E109
// FUNCTION: STBC 0x0051cca0 _$E108
// FUNCTION: STBC 0x0051ccb0 _$E114
// FUNCTION: STBC 0x0051ccc0 _$E113
// FUNCTION: STBC 0x0051ccd0 _$E144
// FUNCTION: STBC 0x0051cce0 _$E143
// FUNCTION: STBC 0x0051ccf0 _$E361
// FUNCTION: STBC 0x0051cd00 _$E360
// FUNCTION: STBC 0x0051cd10 _$E149
// FUNCTION: STBC 0x0051cd20 _$E148
// FUNCTION: STBC 0x0051cd30 _$E365
// FUNCTION: STBC 0x0051cd40 _$E364
// FUNCTION: STBC 0x0051cd50 _$E369
// FUNCTION: STBC 0x0051cd60 _$E368
// FUNCTION: STBC 0x0051cd70 _$E154
// FUNCTION: STBC 0x0051cd80 _$E153
// FUNCTION: STBC 0x0051cd90 _$E373
// FUNCTION: STBC 0x0051cda0 _$E372
// FUNCTION: STBC 0x0051cdb0 _$E119
// FUNCTION: STBC 0x0051cdc0 _$E118
// FUNCTION: STBC 0x0051cdd0 _$E377
// FUNCTION: STBC 0x0051cde0 _$E376
// FUNCTION: STBC 0x0051cdf0 _$E381
// FUNCTION: STBC 0x0051ce00 _$E380
// FUNCTION: STBC 0x0051ce10 _$E159
// FUNCTION: STBC 0x0051ce20 _$E158
// FUNCTION: STBC 0x0051ce30 _$E164
// FUNCTION: STBC 0x0051ce40 _$E163
// FUNCTION: STBC 0x0051ce50 _$E385
// FUNCTION: STBC 0x0051ce60 _$E384
// FUNCTION: STBC 0x0051ce70 _$E389
// FUNCTION: STBC 0x0051ce80 _$E388
// FUNCTION: STBC 0x0051ce90 _$E393
// FUNCTION: STBC 0x0051cea0 _$E392
// FUNCTION: STBC 0x0051ceb0 _$E397
// FUNCTION: STBC 0x0051cec0 _$E396
// FUNCTION: STBC 0x0051ced0 _$E401
// FUNCTION: STBC 0x0051cee0 _$E400
// FUNCTION: STBC 0x0051cef0 _$E405
// FUNCTION: STBC 0x0051cf00 _$E404
// FUNCTION: STBC 0x0051cf10 _$E409
// FUNCTION: STBC 0x0051cf20 _$E408
// FUNCTION: STBC 0x0051cf30 _$E413
// FUNCTION: STBC 0x0051cf40 _$E412
// FUNCTION: STBC 0x0051cf50 _$E169
// FUNCTION: STBC 0x0051cf60 _$E168
// FUNCTION: STBC 0x0051cf70 _$E417
// FUNCTION: STBC 0x0051cf80 _$E416
// FUNCTION: STBC 0x0051cf90 _$E174
// FUNCTION: STBC 0x0051cfa0 _$E173
// FUNCTION: STBC 0x0051cfb0 _$E179
// FUNCTION: STBC 0x0051cfc0 _$E178
// FUNCTION: STBC 0x0051cfe0 _$E421
// FUNCTION: STBC 0x0051cff0 _$E420
// FUNCTION: STBC 0x0051d000 _$E426
// FUNCTION: STBC 0x0051d010 _$E425
// FUNCTION: STBC 0x0051d020 _$E430
// FUNCTION: STBC 0x0051d030 _$E429

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0051c660 _$E235
// FUNCTION: STBC 0x0051c670 _$E232
// FUNCTION: STBC 0x0051c680 _$E234
// FUNCTION: STBC 0x0051c690 _$E233
// FUNCTION: STBC 0x0051c6a0 _$E241
// FUNCTION: STBC 0x0051c6b0 _$E238
// FUNCTION: STBC 0x0051c6c0 _$E240
// FUNCTION: STBC 0x0051c6d0 _$E239

/*
================
STCheckbox::Cast

The object as a checkbox; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0051c060
STCheckbox *STCheckbox::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STCHECKBOX_TYPE ) ) {
		return (STCheckbox *)object;
	}
	return NULL;
}

/*
================
STCheckbox::STCheckbox

Builds the checkmark over the box, both from the LCARS group of the
current resolution, unchecked.
================
*/
// FUNCTION: STBC 0x0051c090 ??0STCheckbox@@QAE@MM@Z
STCheckbox::STCheckbox( float width, float height ) : TGPane( width, height ) {
	SetBatchChildPolys( false );
	checked = false;
	onColor = g_kSTButtonCheckmarkOn;
	offColor = g_kSTButtonCheckmarkOff;

	GraphicsModeInfo *mode = GraphicsModeInfo::GetCurrentMode();
	TGIcon *checkmark = new TGIcon( mode->GetLcarsString(), STCHECKBOX_ICON_CHECKMARK, &offColor );
	TGIcon *box = new TGIcon( mode->GetLcarsString(), STCHECKBOX_ICON_BOX, &g_kSTMenu1NormalBase );
	AddChild( checkmark, 0.0f, 0.0f, false );
	AddChild( box, 0.0f, 0.0f, false );
	Resize( width, height, false );
}

/*
================
STCheckbox::STCheckbox

The checkbox a stream loads; its children come from the stream.
================
*/
// FUNCTION: STBC 0x0051c2f0 ??0STCheckbox@@QAE@J@Z
STCheckbox::STCheckbox( long id ) : TGPane( id ) {
	checked = false;
}

/*
================
STCheckbox::~STCheckbox
================
*/
// FUNCTION: STBC 0x0051c330
STCheckbox::~STCheckbox( void ) {
}

/*
================
STCheckbox::Resize

Stretches the box to the size and the checkmark by the same scale,
centred on the box.
================
*/
// FUNCTION: STBC 0x0051c340
// PARKED: frame residue: retail keeps the two scales in swapped stack slots (the width's above the height's); separate float locals let the divisions sink past the box's resize
void STCheckbox::Resize( float width, float height, bool layout ) {
	TGIcon *box = TGIcon::Cast( GetNthChild( STCHECKBOX_BOX ) );
	TGIcon *checkmark = TGIcon::Cast( GetNthChild( STCHECKBOX_CHECKMARK ) );
	box->SizeToArtwork( false );
	checkmark->SizeToArtwork( false );
	NiPoint2 scale( width / box->GetWidth(), height / box->GetHeight() );
	box->Resize( width, height, false );
	checkmark->Resize( checkmark->GetWidth() * scale.x, checkmark->GetHeight() * scale.y, false );
	checkmark->SetPosition( ( box->GetWidth() - checkmark->GetWidth() ) * 0.5f + box->GetLeft(),
							( box->GetHeight() - checkmark->GetHeight() ) * 0.5f + box->GetTop(), false );
	TGUIObject::Resize( width, height, false );
	if ( layout == true ) {
		Layout();
	}
}

/*
================
STCheckbox::SetChecked
================
*/
// FUNCTION: STBC 0x0051c430
void STCheckbox::SetChecked( bool checked ) {
	if ( checked == this->checked ) {
		return;
	}
	TGIcon *checkmark = TGIcon::Cast( GetNthChild( STCHECKBOX_CHECKMARK ) );
	this->checked = checked;
	if ( checked ) {
		checkmark->SetColor( &onColor );
	} else {
		checkmark->SetColor( &offColor );
	}
}

/*
================
STCheckbox::SetBoxColor
================
*/
// FUNCTION: STBC 0x0051c480
void STCheckbox::SetBoxColor( NiColorA *color ) {
	TGIcon *box = TGIcon::Cast( GetNthChild( STCHECKBOX_BOX ) );
	box->SetColor( color );
}

/*
================
STCheckbox::SetOnColor
================
*/
// FUNCTION: STBC 0x0051c4b0
void STCheckbox::SetOnColor( NiColorA *color ) {
	onColor = *color;
	if ( checked ) {
		TGIcon *checkmark = TGIcon::Cast( GetNthChild( STCHECKBOX_CHECKMARK ) );
		checkmark->SetColor( &onColor );
	}
}

/*
================
STCheckbox::SetOffColor
================
*/
// FUNCTION: STBC 0x0051c500
void STCheckbox::SetOffColor( NiColorA *color ) {
	offColor = *color;
	if ( !checked ) {
		TGIcon *checkmark = TGIcon::Cast( GetNthChild( STCHECKBOX_CHECKMARK ) );
		checkmark->SetColor( &offColor );
	}
}

/*
================
STCheckbox::WriteToStream
================
*/
// FUNCTION: STBC 0x0051c550
bool STCheckbox::WriteToStream( TGStream *stream ) {
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteBool( checked );
	stream->WriteFloat( onColor.r );
	stream->WriteFloat( onColor.g );
	stream->WriteFloat( onColor.b );
	stream->WriteFloat( onColor.a );
	stream->WriteFloat( offColor.r );
	stream->WriteFloat( offColor.g );
	stream->WriteFloat( offColor.b );
	stream->WriteFloat( offColor.a );
	return true;
}

/*
================
STCheckbox::ReadFromStream
================
*/
// FUNCTION: STBC 0x0051c5e0
bool STCheckbox::ReadFromStream( TGStream *stream ) {
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	checked = stream->ReadBool();
	onColor.r = stream->ReadFloat();
	onColor.g = stream->ReadFloat();
	onColor.b = stream->ReadFloat();
	onColor.a = stream->ReadFloat();
	offColor.r = stream->ReadFloat();
	offColor.g = stream->ReadFloat();
	offColor.b = stream->ReadFloat();
	offColor.a = stream->ReadFloat();
	return true;
}
