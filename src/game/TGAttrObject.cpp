/*
===============================================================================

	TGAttrObject.cpp

===============================================================================
*/

#include <iostream>
#include "TGAttrObject.h"
#include "TGStream.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006f2860 _$E32
// FUNCTION: STBC 0x006f2870 _$E29
// FUNCTION: STBC 0x006f2880 _$E31
// FUNCTION: STBC 0x006f2890 _$E30
// FUNCTION: STBC 0x006f28a0 _$E38
// FUNCTION: STBC 0x006f28b0 _$E35
// FUNCTION: STBC 0x006f28c0 _$E37
// FUNCTION: STBC 0x006f28d0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006f28e0 _$E52
// FUNCTION: STBC 0x006f28f0 _$E51
// FUNCTION: STBC 0x006f2900 _$E56
// FUNCTION: STBC 0x006f2910 _$E55
// FUNCTION: STBC 0x006f2920 _$E67
// FUNCTION: STBC 0x006f2930 _$E66
// FUNCTION: STBC 0x006f2940 _$E71
// FUNCTION: STBC 0x006f2950 _$E70
// FUNCTION: STBC 0x006f2960 _$E75
// FUNCTION: STBC 0x006f2970 _$E74
// FUNCTION: STBC 0x006f2980 _$E79
// FUNCTION: STBC 0x006f2990 _$E78
// FUNCTION: STBC 0x006f29a0 _$E203
// FUNCTION: STBC 0x006f29b0 _$E202
// FUNCTION: STBC 0x006f29c0 _$E145
// FUNCTION: STBC 0x006f29d0 _$E144
// FUNCTION: STBC 0x006f29e0 _$E158
// FUNCTION: STBC 0x006f29f0 _$E157
// FUNCTION: STBC 0x006f2a00 _$E208
// FUNCTION: STBC 0x006f2a10 _$E207
// FUNCTION: STBC 0x006f2a20 _$E213
// FUNCTION: STBC 0x006f2a30 _$E212
// FUNCTION: STBC 0x006f2a40 _$E217
// FUNCTION: STBC 0x006f2a50 _$E216
// FUNCTION: STBC 0x006f2a60 _$E221
// FUNCTION: STBC 0x006f2a70 _$E220
// FUNCTION: STBC 0x006f2a80 _$E228
// FUNCTION: STBC 0x006f2a90 _$E227
// FUNCTION: STBC 0x006f2aa0 _$E253
// FUNCTION: STBC 0x006f2ab0 _$E252
// FUNCTION: STBC 0x006f2ac0 _$E257
// FUNCTION: STBC 0x006f2ad0 _$E256
// FUNCTION: STBC 0x006f2ae0 _$E103
// FUNCTION: STBC 0x006f2af0 _$E102
// FUNCTION: STBC 0x006f2b00 _$E239
// FUNCTION: STBC 0x006f2b10 _$E238
// FUNCTION: STBC 0x006f2b20 _$E246
// FUNCTION: STBC 0x006f2b30 _$E245
// FUNCTION: STBC 0x006f2b40 _$E269
// FUNCTION: STBC 0x006f2b50 _$E268
// FUNCTION: STBC 0x006f2b60 _$E273
// FUNCTION: STBC 0x006f2b70 _$E272
// FUNCTION: STBC 0x006f2b80 _$E313
// FUNCTION: STBC 0x006f2b90 _$E312
// FUNCTION: STBC 0x006f2ba0 _$E319
// FUNCTION: STBC 0x006f2bb0 _$E318
// FUNCTION: STBC 0x006f2bc0 _$E261
// FUNCTION: STBC 0x006f2bd0 _$E260
// FUNCTION: STBC 0x006f2be0 _$E265
// FUNCTION: STBC 0x006f2bf0 _$E264
// FUNCTION: STBC 0x006f2c00 _$E277
// FUNCTION: STBC 0x006f2c10 _$E276
// FUNCTION: STBC 0x006f2c20 _$E281
// FUNCTION: STBC 0x006f2c30 _$E280
// FUNCTION: STBC 0x006f2c40 _$E285
// FUNCTION: STBC 0x006f2c50 _$E284
// FUNCTION: STBC 0x006f2c60 _$E289
// FUNCTION: STBC 0x006f2c70 _$E288
// FUNCTION: STBC 0x006f2c80 _$E293
// FUNCTION: STBC 0x006f2c90 _$E292
// FUNCTION: STBC 0x006f2ca0 _$E297
// FUNCTION: STBC 0x006f2cb0 _$E296
// FUNCTION: STBC 0x006f2cc0 _$E301
// FUNCTION: STBC 0x006f2cd0 _$E300
// FUNCTION: STBC 0x006f2ce0 _$E305
// FUNCTION: STBC 0x006f2cf0 _$E304
// FUNCTION: STBC 0x006f2d00 _$E108
// FUNCTION: STBC 0x006f2d10 _$E107
// FUNCTION: STBC 0x006f2d20 _$E309
// FUNCTION: STBC 0x006f2d30 _$E308
// FUNCTION: STBC 0x006f2d40 _$E113
// FUNCTION: STBC 0x006f2d50 _$E112
// FUNCTION: STBC 0x006f2d60 _$E323
// FUNCTION: STBC 0x006f2d70 _$E322
// FUNCTION: STBC 0x006f2d80 _$E327
// FUNCTION: STBC 0x006f2d90 _$E326
// FUNCTION: STBC 0x006f2da0 _$E331
// FUNCTION: STBC 0x006f2db0 _$E330
// FUNCTION: STBC 0x006f2dc0 _$E335
// FUNCTION: STBC 0x006f2dd0 _$E334
// FUNCTION: STBC 0x006f2de0 _$E122
// FUNCTION: STBC 0x006f2df0 _$E121
// FUNCTION: STBC 0x006f2e00 _$E339
// FUNCTION: STBC 0x006f2e10 _$E338
// FUNCTION: STBC 0x006f2e20 _$E343
// FUNCTION: STBC 0x006f2e30 _$E342
// FUNCTION: STBC 0x006f2e40 _$E347
// FUNCTION: STBC 0x006f2e50 _$E346
// FUNCTION: STBC 0x006f2e60 _$E128
// FUNCTION: STBC 0x006f2e70 _$E127
// FUNCTION: STBC 0x006f2e80 _$E133
// FUNCTION: STBC 0x006f2e90 _$E132
// FUNCTION: STBC 0x006f2ea0 _$E163
// FUNCTION: STBC 0x006f2eb0 _$E162
// FUNCTION: STBC 0x006f2ec0 _$E351
// FUNCTION: STBC 0x006f2ed0 _$E350
// FUNCTION: STBC 0x006f2ee0 _$E168
// FUNCTION: STBC 0x006f2ef0 _$E167
// FUNCTION: STBC 0x006f2f00 _$E355
// FUNCTION: STBC 0x006f2f10 _$E354
// FUNCTION: STBC 0x006f2f20 _$E359
// FUNCTION: STBC 0x006f2f30 _$E358
// FUNCTION: STBC 0x006f2f40 _$E173
// FUNCTION: STBC 0x006f2f50 _$E172
// FUNCTION: STBC 0x006f2f60 _$E363
// FUNCTION: STBC 0x006f2f70 _$E362
// FUNCTION: STBC 0x006f2f80 _$E138
// FUNCTION: STBC 0x006f2f90 _$E137
// FUNCTION: STBC 0x006f2fa0 _$E367
// FUNCTION: STBC 0x006f2fb0 _$E366
// FUNCTION: STBC 0x006f2fc0 _$E371
// FUNCTION: STBC 0x006f2fd0 _$E370
// FUNCTION: STBC 0x006f2fe0 _$E178
// FUNCTION: STBC 0x006f2ff0 _$E177
// FUNCTION: STBC 0x006f3000 _$E183
// FUNCTION: STBC 0x006f3010 _$E182
// FUNCTION: STBC 0x006f3020 _$E375
// FUNCTION: STBC 0x006f3030 _$E374
// FUNCTION: STBC 0x006f3040 _$E379
// FUNCTION: STBC 0x006f3050 _$E378
// FUNCTION: STBC 0x006f3060 _$E383
// FUNCTION: STBC 0x006f3070 _$E382
// FUNCTION: STBC 0x006f3080 _$E387
// FUNCTION: STBC 0x006f3090 _$E386
// FUNCTION: STBC 0x006f30a0 _$E391
// FUNCTION: STBC 0x006f30b0 _$E390
// FUNCTION: STBC 0x006f30c0 _$E395
// FUNCTION: STBC 0x006f30d0 _$E394
// FUNCTION: STBC 0x006f30e0 _$E399
// FUNCTION: STBC 0x006f30f0 _$E398
// FUNCTION: STBC 0x006f3100 _$E403
// FUNCTION: STBC 0x006f3110 _$E402
// FUNCTION: STBC 0x006f3120 _$E188
// FUNCTION: STBC 0x006f3130 _$E187
// FUNCTION: STBC 0x006f3140 _$E407
// FUNCTION: STBC 0x006f3150 _$E406
// FUNCTION: STBC 0x006f3160 _$E193
// FUNCTION: STBC 0x006f3170 _$E192
// FUNCTION: STBC 0x006f3180 _$E198
// FUNCTION: STBC 0x006f3190 _$E197

/*
================
TGAttrObject::TGAttrObject
================
*/
// FUNCTION: STBC 0x006f31a0
TGAttrObject::TGAttrObject( long id ) : TGObject( id ) {
	attributes = NULL;
}

/*
================
TGAttrObject::GetObjType
================
*/
// FUNCTION: STBC 0x006f31c0
int TGAttrObject::GetObjType( void ) {
	return TGATTROBJECT_TYPE;
}

/*
================
TGAttrObject::GetClassName
================
*/
// FUNCTION: STBC 0x006f31f0
char *TGAttrObject::GetClassName( void ) {
	return "TGAttrObject";
}

/*
================
TGAttrObject::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x006f3200
char *TGAttrObject::GetSwigTypeName( void ) {
	return "_p_TGAttrObject";
}

/*
================
TGAttrObject::GetPythonClassName
================
*/
// FUNCTION: STBC 0x006f3210
char *TGAttrObject::GetPythonClassName( void ) {
	return "TGAttrObjectPtr";
}

/*
================
TGAttrObject::FixUpPointers

An attribute object holds no pointers to fix up.
================
*/
// FUNCTION: STBC 0x006f3220
bool TGAttrObject::FixUpPointers( void ) {
	return true;
}

/*
================
TGAttrObject::PostFixUp
================
*/
// FUNCTION: STBC 0x006f3230
void TGAttrObject::PostFixUp( void ) {
}

/*
================
TGAttrObject::~TGAttrObject
================
*/
// FUNCTION: STBC 0x006f3270
TGAttrObject::~TGAttrObject( void ) {
	if ( attributes != NULL ) {
		delete attributes;
		attributes = NULL;
	}
}

/*
================
TGAttrObject::WriteToStream

The object, then the attributes; an object without attributes writes an
empty count.
================
*/
// FUNCTION: STBC 0x006f32d0
bool TGAttrObject::WriteToStream( TGStream *stream ) {
	if ( !TGObject::WriteToStream( stream ) ) {
		return false;
	}

	if ( attributes == NULL ) {
		stream->WriteInt( 0 );
	} else if ( !attributes->WriteToStream( stream ) ) {
		return false;
	}
	return true;
}

/*
================
TGAttrObject::ReadFromStream
================
*/
// FUNCTION: STBC 0x006f3310
bool TGAttrObject::ReadFromStream( TGStream *stream ) {
	if ( attributes == NULL ) {
		attributes = new TGAttrTable;
		if ( attributes == NULL ) {
			return false;
		}
	}
	if ( !attributes->ReadFromStream( stream ) ) {
		return false;
	}
	return true;
}

/*
================
TGAttrObject::LookupAttrValue
================
*/
// FUNCTION: STBC 0x006f33a0
bool TGAttrObject::LookupAttrValue( AttrType type, float *value ) {
	if ( attributes == NULL ) {
		return false;
	}
	return attributes->Lookup( type, value );
}

/*
================
TGAttrObject::GetAttrValue

0 when the attribute is not set.
================
*/
// FUNCTION: STBC 0x006f33c0
float TGAttrObject::GetAttrValue( AttrType type ) {
	float value = 0.0f;
	if ( attributes == NULL ) {
		return 0.0f;
	}
	attributes->Lookup( type, &value );
	return value;
}

/*
================
TGAttrObject::SetAttrValue
================
*/
// FUNCTION: STBC 0x006f3400
bool TGAttrObject::SetAttrValue( AttrType type, float value ) {
	if ( attributes == NULL ) {
		attributes = new TGAttrTable;
		if ( attributes == NULL ) {
			return false;
		}
	}
	return attributes->Set( type, value );
}

/*
================
TGAttrObject::LookupAttrInt

The Int accessors keep an int's bits in the attribute's float.
================
*/
// FUNCTION: STBC 0x006f3490
bool TGAttrObject::LookupAttrInt( AttrType type, int *value ) {
	attrValue_t bits;
	if ( !LookupAttrValue( type, &bits.f ) ) {
		return false;
	}
	*value = bits.i;
	return true;
}

/*
================
TGAttrObject::GetAttrInt
================
*/
// FUNCTION: STBC 0x006f34c0
int TGAttrObject::GetAttrInt( AttrType type ) {
	attrValue_t bits;
	bits.f = GetAttrValue( type );
	return bits.i;
}

/*
================
TGAttrObject::SetAttrInt
================
*/
// FUNCTION: STBC 0x006f34e0
bool TGAttrObject::SetAttrInt( AttrType type, int value ) {
	attrValue_t bits;
	bits.i = value;
	return SetAttrValue( type, bits.f );
}

/*
================
TGAttrObject::RemoveAttr

False when the attribute was not set.
================
*/
// FUNCTION: STBC 0x006f3500
bool TGAttrObject::RemoveAttr( AttrType type ) {
	if ( attributes == NULL ) {
		return false;
	}
	return attributes->Remove( type );
}
