/*
===============================================================================

	TGTemplatedAttrObject.cpp

===============================================================================
*/

#include <iostream>
#include "TGTemplatedAttrObject.h"
#include "TGStream.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006f1c50 _$E32
// FUNCTION: STBC 0x006f1c60 _$E29
// FUNCTION: STBC 0x006f1c70 _$E31
// FUNCTION: STBC 0x006f1c80 _$E30
// FUNCTION: STBC 0x006f1c90 _$E38
// FUNCTION: STBC 0x006f1ca0 _$E35
// FUNCTION: STBC 0x006f1cb0 _$E37
// FUNCTION: STBC 0x006f1cc0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006f1cd0 _$E52
// FUNCTION: STBC 0x006f1ce0 _$E51
// FUNCTION: STBC 0x006f1cf0 _$E56
// FUNCTION: STBC 0x006f1d00 _$E55
// FUNCTION: STBC 0x006f1d10 _$E67
// FUNCTION: STBC 0x006f1d20 _$E66
// FUNCTION: STBC 0x006f1d30 _$E71
// FUNCTION: STBC 0x006f1d40 _$E70
// FUNCTION: STBC 0x006f1d50 _$E75
// FUNCTION: STBC 0x006f1d60 _$E74
// FUNCTION: STBC 0x006f1d70 _$E79
// FUNCTION: STBC 0x006f1d80 _$E78
// FUNCTION: STBC 0x006f1d90 _$E203
// FUNCTION: STBC 0x006f1da0 _$E202
// FUNCTION: STBC 0x006f1db0 _$E145
// FUNCTION: STBC 0x006f1dc0 _$E144
// FUNCTION: STBC 0x006f1dd0 _$E158
// FUNCTION: STBC 0x006f1de0 _$E157
// FUNCTION: STBC 0x006f1df0 _$E208
// FUNCTION: STBC 0x006f1e00 _$E207
// FUNCTION: STBC 0x006f1e10 _$E213
// FUNCTION: STBC 0x006f1e20 _$E212
// FUNCTION: STBC 0x006f1e30 _$E217
// FUNCTION: STBC 0x006f1e40 _$E216
// FUNCTION: STBC 0x006f1e50 _$E221
// FUNCTION: STBC 0x006f1e60 _$E220
// FUNCTION: STBC 0x006f1e70 _$E228
// FUNCTION: STBC 0x006f1e80 _$E227
// FUNCTION: STBC 0x006f1e90 _$E253
// FUNCTION: STBC 0x006f1ea0 _$E252
// FUNCTION: STBC 0x006f1eb0 _$E257
// FUNCTION: STBC 0x006f1ec0 _$E256
// FUNCTION: STBC 0x006f1ed0 _$E103
// FUNCTION: STBC 0x006f1ee0 _$E102
// FUNCTION: STBC 0x006f1ef0 _$E239
// FUNCTION: STBC 0x006f1f00 _$E238
// FUNCTION: STBC 0x006f1f10 _$E246
// FUNCTION: STBC 0x006f1f20 _$E245
// FUNCTION: STBC 0x006f1f30 _$E269
// FUNCTION: STBC 0x006f1f40 _$E268
// FUNCTION: STBC 0x006f1f50 _$E273
// FUNCTION: STBC 0x006f1f60 _$E272
// FUNCTION: STBC 0x006f1f70 _$E313
// FUNCTION: STBC 0x006f1f80 _$E312
// FUNCTION: STBC 0x006f1f90 _$E319
// FUNCTION: STBC 0x006f1fa0 _$E318
// FUNCTION: STBC 0x006f1fb0 _$E261
// FUNCTION: STBC 0x006f1fc0 _$E260
// FUNCTION: STBC 0x006f1fd0 _$E265
// FUNCTION: STBC 0x006f1fe0 _$E264
// FUNCTION: STBC 0x006f1ff0 _$E277
// FUNCTION: STBC 0x006f2000 _$E276
// FUNCTION: STBC 0x006f2010 _$E281
// FUNCTION: STBC 0x006f2020 _$E280
// FUNCTION: STBC 0x006f2030 _$E285
// FUNCTION: STBC 0x006f2040 _$E284
// FUNCTION: STBC 0x006f2050 _$E289
// FUNCTION: STBC 0x006f2060 _$E288
// FUNCTION: STBC 0x006f2070 _$E293
// FUNCTION: STBC 0x006f2080 _$E292
// FUNCTION: STBC 0x006f2090 _$E297
// FUNCTION: STBC 0x006f20a0 _$E296
// FUNCTION: STBC 0x006f20b0 _$E301
// FUNCTION: STBC 0x006f20c0 _$E300
// FUNCTION: STBC 0x006f20d0 _$E305
// FUNCTION: STBC 0x006f20e0 _$E304
// FUNCTION: STBC 0x006f20f0 _$E108
// FUNCTION: STBC 0x006f2100 _$E107
// FUNCTION: STBC 0x006f2110 _$E309
// FUNCTION: STBC 0x006f2120 _$E308
// FUNCTION: STBC 0x006f2130 _$E113
// FUNCTION: STBC 0x006f2140 _$E112
// FUNCTION: STBC 0x006f2150 _$E323
// FUNCTION: STBC 0x006f2160 _$E322
// FUNCTION: STBC 0x006f2170 _$E327
// FUNCTION: STBC 0x006f2180 _$E326
// FUNCTION: STBC 0x006f2190 _$E331
// FUNCTION: STBC 0x006f21a0 _$E330
// FUNCTION: STBC 0x006f21b0 _$E335
// FUNCTION: STBC 0x006f21c0 _$E334
// FUNCTION: STBC 0x006f21d0 _$E122
// FUNCTION: STBC 0x006f21e0 _$E121
// FUNCTION: STBC 0x006f21f0 _$E339
// FUNCTION: STBC 0x006f2200 _$E338
// FUNCTION: STBC 0x006f2210 _$E343
// FUNCTION: STBC 0x006f2220 _$E342
// FUNCTION: STBC 0x006f2230 _$E347
// FUNCTION: STBC 0x006f2240 _$E346
// FUNCTION: STBC 0x006f2250 _$E128
// FUNCTION: STBC 0x006f2260 _$E127
// FUNCTION: STBC 0x006f2270 _$E133
// FUNCTION: STBC 0x006f2280 _$E132
// FUNCTION: STBC 0x006f2290 _$E163
// FUNCTION: STBC 0x006f22a0 _$E162
// FUNCTION: STBC 0x006f22b0 _$E351
// FUNCTION: STBC 0x006f22c0 _$E350
// FUNCTION: STBC 0x006f22d0 _$E168
// FUNCTION: STBC 0x006f22e0 _$E167
// FUNCTION: STBC 0x006f22f0 _$E355
// FUNCTION: STBC 0x006f2300 _$E354
// FUNCTION: STBC 0x006f2310 _$E359
// FUNCTION: STBC 0x006f2320 _$E358
// FUNCTION: STBC 0x006f2330 _$E173
// FUNCTION: STBC 0x006f2340 _$E172
// FUNCTION: STBC 0x006f2350 _$E363
// FUNCTION: STBC 0x006f2360 _$E362
// FUNCTION: STBC 0x006f2370 _$E138
// FUNCTION: STBC 0x006f2380 _$E137
// FUNCTION: STBC 0x006f2390 _$E367
// FUNCTION: STBC 0x006f23a0 _$E366
// FUNCTION: STBC 0x006f23b0 _$E371
// FUNCTION: STBC 0x006f23c0 _$E370
// FUNCTION: STBC 0x006f23d0 _$E178
// FUNCTION: STBC 0x006f23e0 _$E177
// FUNCTION: STBC 0x006f23f0 _$E183
// FUNCTION: STBC 0x006f2400 _$E182
// FUNCTION: STBC 0x006f2410 _$E375
// FUNCTION: STBC 0x006f2420 _$E374
// FUNCTION: STBC 0x006f2430 _$E379
// FUNCTION: STBC 0x006f2440 _$E378
// FUNCTION: STBC 0x006f2450 _$E383
// FUNCTION: STBC 0x006f2460 _$E382
// FUNCTION: STBC 0x006f2470 _$E387
// FUNCTION: STBC 0x006f2480 _$E386
// FUNCTION: STBC 0x006f2490 _$E391
// FUNCTION: STBC 0x006f24a0 _$E390
// FUNCTION: STBC 0x006f24b0 _$E395
// FUNCTION: STBC 0x006f24c0 _$E394
// FUNCTION: STBC 0x006f24d0 _$E399
// FUNCTION: STBC 0x006f24e0 _$E398
// FUNCTION: STBC 0x006f24f0 _$E403
// FUNCTION: STBC 0x006f2500 _$E402
// FUNCTION: STBC 0x006f2510 _$E188
// FUNCTION: STBC 0x006f2520 _$E187
// FUNCTION: STBC 0x006f2530 _$E407
// FUNCTION: STBC 0x006f2540 _$E406
// FUNCTION: STBC 0x006f2550 _$E193
// FUNCTION: STBC 0x006f2560 _$E192
// FUNCTION: STBC 0x006f2570 _$E198
// FUNCTION: STBC 0x006f2580 _$E197

/*
================
TGTemplatedAttrObject::TGTemplatedAttrObject
================
*/
// FUNCTION: STBC 0x006f2590
TGTemplatedAttrObject::TGTemplatedAttrObject( long id ) : TGAttrObject( id ) {
	templateObject = NULL;
}

/*
================
TGTemplatedAttrObject::GetObjType
================
*/
// FUNCTION: STBC 0x006f25b0
int TGTemplatedAttrObject::GetObjType( void ) {
	return TGTEMPLATEDATTROBJECT_TYPE;
}

/*
================
TGTemplatedAttrObject::GetClassName
================
*/
// FUNCTION: STBC 0x006f25f0
char *TGTemplatedAttrObject::GetClassName( void ) {
	return "TGTemplatedAttrObject";
}

/*
================
TGTemplatedAttrObject::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x006f2600
char *TGTemplatedAttrObject::GetSwigTypeName( void ) {
	return "_p_TGTemplatedAttrObject";
}

/*
================
TGTemplatedAttrObject::GetPythonClassName
================
*/
// FUNCTION: STBC 0x006f2610
char *TGTemplatedAttrObject::GetPythonClassName( void ) {
	return "TGTemplatedAttrObjectPtr";
}

/*
================
TGTemplatedAttrObject::~TGTemplatedAttrObject
================
*/
// FUNCTION: STBC 0x006f2650
TGTemplatedAttrObject::~TGTemplatedAttrObject( void ) {
	templateObject = NULL;
}

/*
================
TGTemplatedAttrObject::WriteToStream

The attribute object, then the template's ID (0 for none).
================
*/
// FUNCTION: STBC 0x006f2670
bool TGTemplatedAttrObject::WriteToStream( TGStream *stream ) {
	if ( !TGAttrObject::WriteToStream( stream ) ) {
		return false;
	}

	long templateID = 0;
	if ( templateObject != NULL ) {
		templateID = templateObject->GetObjID();
	}
	stream->WriteID( templateID );
	return true;
}

/*
================
TGTemplatedAttrObject::ReadFromStream

Keeps the template's ID in the pointer until PostFixUp.
================
*/
// FUNCTION: STBC 0x006f26b0
bool TGTemplatedAttrObject::ReadFromStream( TGStream *stream ) {
	if ( !TGAttrObject::ReadFromStream( stream ) ) {
		return false;
	}

	templateObject = (TGAttrObject *)stream->ReadID();
	return true;
}

/*
================
TGTemplatedAttrObject::LookupAttrInt

The same bodies as TGAttrObject's, over the overridden float accessors.
================
*/
// FUNCTION: STBC 0x006f26e0
bool TGTemplatedAttrObject::LookupAttrInt( AttrType type, int *value ) {
	attrValue_t bits;
	if ( !LookupAttrValue( type, &bits.f ) ) {
		return false;
	}
	*value = bits.i;
	return true;
}

/*
================
TGTemplatedAttrObject::GetAttrInt
================
*/
// FUNCTION: STBC 0x006f2710
int TGTemplatedAttrObject::GetAttrInt( AttrType type ) {
	attrValue_t bits;
	bits.f = GetAttrValue( type );
	return bits.i;
}

/*
================
TGTemplatedAttrObject::SetAttrInt
================
*/
// FUNCTION: STBC 0x006f2730
bool TGTemplatedAttrObject::SetAttrInt( AttrType type, int value ) {
	attrValue_t bits;
	bits.i = value;
	return SetAttrValue( type, bits.f );
}

/*
================
TGTemplatedAttrObject::LookupAttrValue

This object's own value, else the template's.
================
*/
// FUNCTION: STBC 0x006f2750
bool TGTemplatedAttrObject::LookupAttrValue( AttrType type, float *value ) {
	if ( TGAttrObject::LookupAttrValue( type, value ) ) {
		return true;
	}
	if ( templateObject != NULL ) {
		return templateObject->LookupAttrValue( type, value );
	}
	return false;
}

/*
================
TGTemplatedAttrObject::GetAttrValue

0 when neither this object nor the template sets the attribute.
================
*/
// FUNCTION: STBC 0x006f2790
float TGTemplatedAttrObject::GetAttrValue( AttrType type ) {
	float value = 0.0f;
	if ( !TGAttrObject::LookupAttrValue( type, &value ) && templateObject != NULL ) {
		templateObject->LookupAttrValue( type, &value );
	}
	return value;
}

/*
================
TGTemplatedAttrObject::LookupLocalAttrValue

This object's own value, ignoring the template.
================
*/
// FUNCTION: STBC 0x006f27d0
bool TGTemplatedAttrObject::LookupLocalAttrValue( AttrType type, float *value ) {
	return TGAttrObject::LookupAttrValue( type, value );
}

/*
================
TGTemplatedAttrObject::FixUpPointers

False while the template's ID names no object yet.
================
*/
// FUNCTION: STBC 0x006f27f0
bool TGTemplatedAttrObject::FixUpPointers( void ) {
	if ( templateObject != NULL && GetTGObjectPtr( (long)templateObject ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TGTemplatedAttrObject::PostFixUp

Turns the template's ID back into the object.
================
*/
// FUNCTION: STBC 0x006f2810
void TGTemplatedAttrObject::PostFixUp( void ) {
	if ( templateObject != NULL ) {
		templateObject = (TGAttrObject *)GetTGObjectPtr( (long)templateObject );
	}
}
