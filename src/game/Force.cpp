/*
===============================================================================

	Force.cpp

===============================================================================
*/

#include <iostream>
#include "Force.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiPointLight.h"
#include "NiShadeProperty.h"
#include "NiSkinController.h"
#include "NiSortAdjustNode.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "NiSpecularProperty.h"
#include "NiSpotLight.h"
#include "NiStencilProperty.h"
#include "NiStringExtraData.h"
#include "NiTextureEffect.h"
#include "NiTextureProperty.h"
#include "NiTransparentProperty.h"
#include "NiTriShapeDynamicData.h"
#include "NiTriShapeSkinController.h"
#include "NiTriStrip.h"
#include "NiTriStripData.h"
#include "NiTriStrips.h"
#include "NiTriStripsData.h"
#include "NiTriangles.h"
#include "NiTrianglesData.h"
#include "NiVertWeightsExtraData.h"
#include "NiVertexColorProperty.h"
#include "NiWireframeProperty.h"
#include "NiZBufferProperty.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00596d20 _$E421
// FUNCTION: STBC 0x00596d40 _$E420

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00596960 _$E32
// FUNCTION: STBC 0x00596970 _$E29
// FUNCTION: STBC 0x00596980 _$E31
// FUNCTION: STBC 0x00596990 _$E30
// FUNCTION: STBC 0x005969a0 _$E38
// FUNCTION: STBC 0x005969b0 _$E35
// FUNCTION: STBC 0x005969c0 _$E37
// FUNCTION: STBC 0x005969d0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005969e0 _$E50
// FUNCTION: STBC 0x005969f0 _$E49
// FUNCTION: STBC 0x00596a00 _$E54
// FUNCTION: STBC 0x00596a10 _$E53
// FUNCTION: STBC 0x00596a20 _$E65
// FUNCTION: STBC 0x00596a30 _$E64
// FUNCTION: STBC 0x00596a40 _$E69
// FUNCTION: STBC 0x00596a50 _$E68
// FUNCTION: STBC 0x00596a60 _$E73
// FUNCTION: STBC 0x00596a70 _$E72
// FUNCTION: STBC 0x00596a80 _$E77
// FUNCTION: STBC 0x00596a90 _$E76
// FUNCTION: STBC 0x00596aa0 _$E196
// FUNCTION: STBC 0x00596ab0 _$E195
// FUNCTION: STBC 0x00596ac0 _$E138
// FUNCTION: STBC 0x00596ad0 _$E137
// FUNCTION: STBC 0x00596ae0 _$E151
// FUNCTION: STBC 0x00596af0 _$E150
// FUNCTION: STBC 0x00596b00 _$E201
// FUNCTION: STBC 0x00596b10 _$E200
// FUNCTION: STBC 0x00596b20 _$E206
// FUNCTION: STBC 0x00596b30 _$E205
// FUNCTION: STBC 0x00596b40 _$E210
// FUNCTION: STBC 0x00596b50 _$E209
// FUNCTION: STBC 0x00596b60 _$E214
// FUNCTION: STBC 0x00596b70 _$E213
// FUNCTION: STBC 0x00596b80 _$E221
// FUNCTION: STBC 0x00596b90 _$E220
// FUNCTION: STBC 0x00596ba0 _$E246
// FUNCTION: STBC 0x00596bb0 _$E245
// FUNCTION: STBC 0x00596bc0 _$E250
// FUNCTION: STBC 0x00596bd0 _$E249
// FUNCTION: STBC 0x00596be0 _$E96
// FUNCTION: STBC 0x00596bf0 _$E95
// FUNCTION: STBC 0x00596c00 _$E232
// FUNCTION: STBC 0x00596c10 _$E231
// FUNCTION: STBC 0x00596c20 _$E239
// FUNCTION: STBC 0x00596c30 _$E238
// FUNCTION: STBC 0x00596c40 _$E262
// FUNCTION: STBC 0x00596c50 _$E261
// FUNCTION: STBC 0x00596c60 _$E266
// FUNCTION: STBC 0x00596c70 _$E265
// FUNCTION: STBC 0x00596c80 _$E306
// FUNCTION: STBC 0x00596c90 _$E305
// FUNCTION: STBC 0x00596ca0 _$E312
// FUNCTION: STBC 0x00596cb0 _$E311
// FUNCTION: STBC 0x00596cc0 _$E254
// FUNCTION: STBC 0x00596cd0 _$E253
// FUNCTION: STBC 0x00596ce0 _$E258
// FUNCTION: STBC 0x00596cf0 _$E257
// FUNCTION: STBC 0x00596d00 _$E270
// FUNCTION: STBC 0x00596d10 _$E269
// FUNCTION: STBC 0x00596d50 _$E274
// FUNCTION: STBC 0x00596d60 _$E273
// FUNCTION: STBC 0x00596d70 _$E278
// FUNCTION: STBC 0x00596d80 _$E277
// FUNCTION: STBC 0x00596d90 _$E282
// FUNCTION: STBC 0x00596da0 _$E281
// FUNCTION: STBC 0x00596db0 _$E286
// FUNCTION: STBC 0x00596dc0 _$E285
// FUNCTION: STBC 0x00596dd0 _$E290
// FUNCTION: STBC 0x00596de0 _$E289
// FUNCTION: STBC 0x00596df0 _$E294
// FUNCTION: STBC 0x00596e00 _$E293
// FUNCTION: STBC 0x00596e10 _$E298
// FUNCTION: STBC 0x00596e20 _$E297
// FUNCTION: STBC 0x00596e30 _$E101
// FUNCTION: STBC 0x00596e40 _$E100
// FUNCTION: STBC 0x00596e50 _$E302
// FUNCTION: STBC 0x00596e60 _$E301
// FUNCTION: STBC 0x00596e70 _$E106
// FUNCTION: STBC 0x00596e80 _$E105
// FUNCTION: STBC 0x00596e90 _$E316
// FUNCTION: STBC 0x00596ea0 _$E315
// FUNCTION: STBC 0x00596eb0 _$E320
// FUNCTION: STBC 0x00596ec0 _$E319
// FUNCTION: STBC 0x00596ed0 _$E324
// FUNCTION: STBC 0x00596ee0 _$E323
// FUNCTION: STBC 0x00596ef0 _$E328
// FUNCTION: STBC 0x00596f00 _$E327
// FUNCTION: STBC 0x00596f10 _$E115
// FUNCTION: STBC 0x00596f20 _$E114
// FUNCTION: STBC 0x00596f30 _$E332
// FUNCTION: STBC 0x00596f40 _$E331
// FUNCTION: STBC 0x00596f50 _$E336
// FUNCTION: STBC 0x00596f60 _$E335
// FUNCTION: STBC 0x00596f70 _$E340
// FUNCTION: STBC 0x00596f80 _$E339
// FUNCTION: STBC 0x00596f90 _$E121
// FUNCTION: STBC 0x00596fa0 _$E120
// FUNCTION: STBC 0x00596fb0 _$E126
// FUNCTION: STBC 0x00596fc0 _$E125
// FUNCTION: STBC 0x00596fd0 _$E156
// FUNCTION: STBC 0x00596fe0 _$E155
// FUNCTION: STBC 0x00596ff0 _$E344
// FUNCTION: STBC 0x00597000 _$E343
// FUNCTION: STBC 0x00597010 _$E161
// FUNCTION: STBC 0x00597020 _$E160
// FUNCTION: STBC 0x00597030 _$E348
// FUNCTION: STBC 0x00597040 _$E347
// FUNCTION: STBC 0x00597050 _$E352
// FUNCTION: STBC 0x00597060 _$E351
// FUNCTION: STBC 0x00597070 _$E166
// FUNCTION: STBC 0x00597080 _$E165
// FUNCTION: STBC 0x00597090 _$E356
// FUNCTION: STBC 0x005970a0 _$E355
// FUNCTION: STBC 0x005970b0 _$E131
// FUNCTION: STBC 0x005970c0 _$E130
// FUNCTION: STBC 0x005970d0 _$E360
// FUNCTION: STBC 0x005970e0 _$E359
// FUNCTION: STBC 0x005970f0 _$E364
// FUNCTION: STBC 0x00597100 _$E363
// FUNCTION: STBC 0x00597110 _$E171
// FUNCTION: STBC 0x00597120 _$E170
// FUNCTION: STBC 0x00597130 _$E176
// FUNCTION: STBC 0x00597140 _$E175
// FUNCTION: STBC 0x00597150 _$E368
// FUNCTION: STBC 0x00597160 _$E367
// FUNCTION: STBC 0x00597170 _$E372
// FUNCTION: STBC 0x00597180 _$E371
// FUNCTION: STBC 0x00597190 _$E376
// FUNCTION: STBC 0x005971a0 _$E375
// FUNCTION: STBC 0x005971b0 _$E380
// FUNCTION: STBC 0x005971c0 _$E379
// FUNCTION: STBC 0x005971d0 _$E384
// FUNCTION: STBC 0x005971e0 _$E383
// FUNCTION: STBC 0x005971f0 _$E388
// FUNCTION: STBC 0x00597200 _$E387
// FUNCTION: STBC 0x00597210 _$E392
// FUNCTION: STBC 0x00597220 _$E391
// FUNCTION: STBC 0x00597230 _$E396
// FUNCTION: STBC 0x00597240 _$E395
// FUNCTION: STBC 0x00597250 _$E181
// FUNCTION: STBC 0x00597260 _$E180
// FUNCTION: STBC 0x00597270 _$E400
// FUNCTION: STBC 0x00597280 _$E399
// FUNCTION: STBC 0x00597290 _$E186
// FUNCTION: STBC 0x005972a0 _$E185
// FUNCTION: STBC 0x005972b0 _$E191
// FUNCTION: STBC 0x005972c0 _$E190
// FUNCTION: STBC 0x005972d0 _$E404
// FUNCTION: STBC 0x005972e0 _$E403
// FUNCTION: STBC 0x005972f0 _$E409
// FUNCTION: STBC 0x00597300 _$E408
// FUNCTION: STBC 0x00597310 _$E413
// FUNCTION: STBC 0x00597320 _$E412

/*
================
Force::Force

No force, at the origin.
================
*/
// FUNCTION: STBC 0x00596590 ??0Force@@QAE@XZ
Force::Force( void ) : position( NiPoint3::ZERO ), force( NiPoint3::ZERO ), torque( NiPoint3::ZERO ) {
}

/*
================
Force::Force
================
*/
// FUNCTION: STBC 0x005965f0 ??0Force@@QAE@ABVNiPoint3@@00@Z
Force::Force( const NiPoint3 &position_, const NiPoint3 &force_, const NiPoint3 &torque_ )
	: position( position_ ), force( force_ ), torque( torque_ ) {
}

/*
================
Force::Force
================
*/
// FUNCTION: STBC 0x00596640 ??0Force@@QAE@ABV0@@Z
Force::Force( const Force &other )
	: position( other.position ), force( other.force ), torque( other.torque ) {
}

/*
================
Force::~Force
================
*/
// FUNCTION: STBC 0x00596680 ??1Force@@QAE@XZ
Force::~Force( void ) {
}

/*
================
Force::MoveTo
================
*/
// FUNCTION: STBC 0x00596690
void Force::MoveTo( const NiPoint3 &position_ ) {
	NiPoint3 arm = position - position_;
	NiPoint3 added = arm.Cross( force );
	torque += added;
	position = position_;
}

/*
================
Force::operator+=

Adds the other force, and the torque it has about this force's point; the
other force's own torque is not added.
================
*/
// FUNCTION: STBC 0x00596710 ??YForce@@QAEXABV0@@Z
void Force::operator+=( const Force &other ) {
	force += other.force;
	NiPoint3 arm = other.position - position;
	NiPoint3 added = arm.Cross( other.force );
	torque += added;
}

/*
================
Force::operator-=

Takes the other force away, and the torque it has about this force's point.
================
*/
// FUNCTION: STBC 0x00596790 ??ZForce@@QAEXABV0@@Z
void Force::operator-=( const Force &other ) {
	force -= other.force;
	NiPoint3 arm = other.position - position;
	NiPoint3 added = arm.Cross( -other.force );
	torque += added;
}

/*
================
Force::operator*=

Scales the force and its torque.
================
*/
// FUNCTION: STBC 0x00596830 ??XForce@@QAEXM@Z
void Force::operator*=( float scale ) {
	force *= scale;
	torque *= scale;
}

/*
================
Force::operator+
================
*/
// FUNCTION: STBC 0x00596870 ??HForce@@QBE?AV0@ABV0@@Z
Force Force::operator+( const Force &other ) const {
	Force sum( *this );
	sum += other;
	return sum;
}

/*
================
Force::operator-
================
*/
// FUNCTION: STBC 0x005968c0 ??GForce@@QBE?AV0@ABV0@@Z
Force Force::operator-( const Force &other ) const {
	Force difference( *this );
	difference -= other;
	return difference;
}

/*
================
Force::operator*
================
*/
// FUNCTION: STBC 0x00596910 ??DForce@@QBE?AV0@M@Z
Force Force::operator*( float scale ) const {
	Force product( *this );
	product *= scale;
	return product;
}
