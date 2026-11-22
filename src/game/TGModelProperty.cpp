/*
===============================================================================

	TGModelProperty.cpp

===============================================================================
*/

#include <iostream>
#include "TGModelProperty.h"
#include "TGStream.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006eca10 _$E416
// FUNCTION: STBC 0x006eca30 _$E415

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006ec8b0 _$E32
// FUNCTION: STBC 0x006ec8c0 _$E29
// FUNCTION: STBC 0x006ec8d0 _$E31
// FUNCTION: STBC 0x006ec8e0 _$E30
// FUNCTION: STBC 0x006ec8f0 _$E38
// FUNCTION: STBC 0x006ec900 _$E35
// FUNCTION: STBC 0x006ec910 _$E37
// FUNCTION: STBC 0x006ec920 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006ec930 _$E53
// FUNCTION: STBC 0x006ec940 _$E52
// FUNCTION: STBC 0x006ec950 _$E57
// FUNCTION: STBC 0x006ec960 _$E56
// FUNCTION: STBC 0x006ec970 _$E68
// FUNCTION: STBC 0x006ec980 _$E67
// FUNCTION: STBC 0x006ec990 _$E72
// FUNCTION: STBC 0x006ec9a0 _$E71
// FUNCTION: STBC 0x006ec9b0 _$E76
// FUNCTION: STBC 0x006ec9c0 _$E75
// FUNCTION: STBC 0x006ec9d0 _$E80
// FUNCTION: STBC 0x006ec9e0 _$E79
// FUNCTION: STBC 0x006ec9f0 _$E204
// FUNCTION: STBC 0x006eca00 _$E203
// FUNCTION: STBC 0x006eca40 _$E146
// FUNCTION: STBC 0x006eca50 _$E145
// FUNCTION: STBC 0x006eca60 _$E159
// FUNCTION: STBC 0x006eca70 _$E158
// FUNCTION: STBC 0x006eca80 _$E209
// FUNCTION: STBC 0x006eca90 _$E208
// FUNCTION: STBC 0x006ecaa0 _$E214
// FUNCTION: STBC 0x006ecab0 _$E213
// FUNCTION: STBC 0x006ecac0 _$E218
// FUNCTION: STBC 0x006ecad0 _$E217
// FUNCTION: STBC 0x006ecae0 _$E222
// FUNCTION: STBC 0x006ecaf0 _$E221
// FUNCTION: STBC 0x006ecb00 _$E229
// FUNCTION: STBC 0x006ecb10 _$E228
// FUNCTION: STBC 0x006ecb20 _$E254
// FUNCTION: STBC 0x006ecb30 _$E253
// FUNCTION: STBC 0x006ecb40 _$E258
// FUNCTION: STBC 0x006ecb50 _$E257
// FUNCTION: STBC 0x006ecb60 _$E104
// FUNCTION: STBC 0x006ecb70 _$E103
// FUNCTION: STBC 0x006ecb80 _$E240
// FUNCTION: STBC 0x006ecb90 _$E239
// FUNCTION: STBC 0x006ecba0 _$E247
// FUNCTION: STBC 0x006ecbb0 _$E246
// FUNCTION: STBC 0x006ecbc0 _$E270
// FUNCTION: STBC 0x006ecbd0 _$E269
// FUNCTION: STBC 0x006ecbe0 _$E274
// FUNCTION: STBC 0x006ecbf0 _$E273
// FUNCTION: STBC 0x006ecc00 _$E314
// FUNCTION: STBC 0x006ecc10 _$E313
// FUNCTION: STBC 0x006ecc20 _$E320
// FUNCTION: STBC 0x006ecc30 _$E319
// FUNCTION: STBC 0x006ecc40 _$E262
// FUNCTION: STBC 0x006ecc50 _$E261
// FUNCTION: STBC 0x006ecc60 _$E266
// FUNCTION: STBC 0x006ecc70 _$E265
// FUNCTION: STBC 0x006ecc80 _$E278
// FUNCTION: STBC 0x006ecc90 _$E277
// FUNCTION: STBC 0x006ecca0 _$E282
// FUNCTION: STBC 0x006eccb0 _$E281
// FUNCTION: STBC 0x006eccc0 _$E286
// FUNCTION: STBC 0x006eccd0 _$E285
// FUNCTION: STBC 0x006ecce0 _$E290
// FUNCTION: STBC 0x006eccf0 _$E289
// FUNCTION: STBC 0x006ecd00 _$E294
// FUNCTION: STBC 0x006ecd10 _$E293
// FUNCTION: STBC 0x006ecd20 _$E298
// FUNCTION: STBC 0x006ecd30 _$E297
// FUNCTION: STBC 0x006ecd40 _$E302
// FUNCTION: STBC 0x006ecd50 _$E301
// FUNCTION: STBC 0x006ecd60 _$E306
// FUNCTION: STBC 0x006ecd70 _$E305
// FUNCTION: STBC 0x006ecd80 _$E109
// FUNCTION: STBC 0x006ecd90 _$E108
// FUNCTION: STBC 0x006ecda0 _$E310
// FUNCTION: STBC 0x006ecdb0 _$E309
// FUNCTION: STBC 0x006ecdc0 _$E114
// FUNCTION: STBC 0x006ecdd0 _$E113
// FUNCTION: STBC 0x006ecde0 _$E324
// FUNCTION: STBC 0x006ecdf0 _$E323
// FUNCTION: STBC 0x006ece00 _$E328
// FUNCTION: STBC 0x006ece10 _$E327
// FUNCTION: STBC 0x006ece20 _$E332
// FUNCTION: STBC 0x006ece30 _$E331
// FUNCTION: STBC 0x006ece40 _$E336
// FUNCTION: STBC 0x006ece50 _$E335
// FUNCTION: STBC 0x006ece60 _$E123
// FUNCTION: STBC 0x006ece70 _$E122
// FUNCTION: STBC 0x006ece80 _$E340
// FUNCTION: STBC 0x006ece90 _$E339
// FUNCTION: STBC 0x006ecea0 _$E344
// FUNCTION: STBC 0x006eceb0 _$E343
// FUNCTION: STBC 0x006ecec0 _$E348
// FUNCTION: STBC 0x006eced0 _$E347
// FUNCTION: STBC 0x006ecee0 _$E129
// FUNCTION: STBC 0x006ecef0 _$E128
// FUNCTION: STBC 0x006ecf00 _$E134
// FUNCTION: STBC 0x006ecf10 _$E133
// FUNCTION: STBC 0x006ecf20 _$E164
// FUNCTION: STBC 0x006ecf30 _$E163
// FUNCTION: STBC 0x006ecf40 _$E352
// FUNCTION: STBC 0x006ecf50 _$E351
// FUNCTION: STBC 0x006ecf60 _$E169
// FUNCTION: STBC 0x006ecf70 _$E168
// FUNCTION: STBC 0x006ecf80 _$E356
// FUNCTION: STBC 0x006ecf90 _$E355
// FUNCTION: STBC 0x006ecfa0 _$E360
// FUNCTION: STBC 0x006ecfb0 _$E359
// FUNCTION: STBC 0x006ecfc0 _$E174
// FUNCTION: STBC 0x006ecfd0 _$E173
// FUNCTION: STBC 0x006ecfe0 _$E364
// FUNCTION: STBC 0x006ecff0 _$E363
// FUNCTION: STBC 0x006ed000 _$E139
// FUNCTION: STBC 0x006ed010 _$E138
// FUNCTION: STBC 0x006ed020 _$E368
// FUNCTION: STBC 0x006ed030 _$E367
// FUNCTION: STBC 0x006ed040 _$E372
// FUNCTION: STBC 0x006ed050 _$E371
// FUNCTION: STBC 0x006ed060 _$E179
// FUNCTION: STBC 0x006ed070 _$E178
// FUNCTION: STBC 0x006ed080 _$E184
// FUNCTION: STBC 0x006ed090 _$E183
// FUNCTION: STBC 0x006ed0a0 _$E376
// FUNCTION: STBC 0x006ed0b0 _$E375
// FUNCTION: STBC 0x006ed0c0 _$E380
// FUNCTION: STBC 0x006ed0d0 _$E379
// FUNCTION: STBC 0x006ed0e0 _$E384
// FUNCTION: STBC 0x006ed0f0 _$E383
// FUNCTION: STBC 0x006ed100 _$E388
// FUNCTION: STBC 0x006ed110 _$E387
// FUNCTION: STBC 0x006ed120 _$E392
// FUNCTION: STBC 0x006ed130 _$E391
// FUNCTION: STBC 0x006ed140 _$E396
// FUNCTION: STBC 0x006ed150 _$E395
// FUNCTION: STBC 0x006ed160 _$E400
// FUNCTION: STBC 0x006ed170 _$E399
// FUNCTION: STBC 0x006ed180 _$E404
// FUNCTION: STBC 0x006ed190 _$E403
// FUNCTION: STBC 0x006ed1a0 _$E189
// FUNCTION: STBC 0x006ed1b0 _$E188
// FUNCTION: STBC 0x006ed1c0 _$E408
// FUNCTION: STBC 0x006ed1d0 _$E407
// FUNCTION: STBC 0x006ed1e0 _$E194
// FUNCTION: STBC 0x006ed1f0 _$E193
// FUNCTION: STBC 0x006ed200 _$E199
// FUNCTION: STBC 0x006ed210 _$E198

/*
================
TGModelProperty::Cast

The property as a TGModelProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x006ec5c0
TGModelProperty *TGModelProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( TGMODELPROPERTY_TYPE ) ) {
		return property;
	}
	return NULL;
}

/*
================
TGModelProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x006ec5f0
TGModelProperty *TGModelProperty::Create( char *name ) {
	TGModelProperty *property = new TGModelProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
TGModelProperty::TGModelProperty
================
*/
// FUNCTION: STBC 0x006ec640
TGModelProperty::TGModelProperty( long id ) : TGObject( id ) {
	refCount = 1;
	name.AssignC( "Uninitialized" );
}

/*
================
TGModelProperty::~TGModelProperty

Releases a shadow instance of the property.
================
*/
// FUNCTION: STBC 0x006ec6f0
TGModelProperty::~TGModelProperty( void ) {
	PyObject *self = VirtualSlot08();
	if ( self != Py_None ) {
		Py_DECREF( self );
	}
}

/*
================
TGModelProperty::VirtualSlot15
================
*/
// FUNCTION: STBC 0x006ec770
void TGModelProperty::VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent ) {
}

/*
================
TGModelProperty::IncRefCount
================
*/
// FUNCTION: STBC 0x006ec780
void TGModelProperty::IncRefCount( void ) {
	refCount++;
}

/*
================
TGModelProperty::DecRefCount

The property is deleted when nothing refers to it any more.
================
*/
// FUNCTION: STBC 0x006ec790
void TGModelProperty::DecRefCount( void ) {
	if ( refCount > 0 ) {
		refCount--;
		if ( refCount <= 0 ) {
			refCount = 0;
			delete this;
		}
	}
}

/*
================
TGModelProperty::GetRefCount
================
*/
// FUNCTION: STBC 0x006ec7c0
int TGModelProperty::GetRefCount( void ) {
	return refCount;
}

/*
================
TGModelProperty::Edit
================
*/
// FUNCTION: STBC 0x006ec7d0
bool TGModelProperty::Edit( HWND parent ) {
	return false;
}

/*
================
TGModelProperty::Duplicate

A new property of the same type with this one's settings.
================
*/
// FUNCTION: STBC 0x006ec7e0
TGModelProperty *TGModelProperty::Duplicate( void ) {
	TGModelProperty *property = (TGModelProperty *)CreateObject( GetObjType(), 0 );
	if ( property != NULL ) {
		property->Copy( this );
		property->refCount = 1;
	}
	return property;
}

/*
================
TGModelProperty::Copy
================
*/
// FUNCTION: STBC 0x006ec810
bool TGModelProperty::Copy( TGModelProperty *property ) {
	SetName( property->GetName() );
	return true;
}

/*
================
TGModelProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x006ec840
bool TGModelProperty::WriteToStream( TGStream *stream ) {
	if ( !TGObject::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteLong( refCount );
	name.WriteToStream( stream );
	return true;
}

/*
================
TGModelProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x006ec880
bool TGModelProperty::ReadFromStream( TGStream *stream ) {
	refCount = stream->ReadLong();
	name.ReadFromStream( stream );
	return true;
}

/*
================
TGModelProperty::WriteScript

Writes "<variable> = App.TGModelProperty_Create("<name>")", the variable
being the name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x006ed280
void TGModelProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.TGModelProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}
