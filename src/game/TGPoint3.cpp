/*
===============================================================================

	TGPoint3.cpp

	TGPoint3's out-of-line methods and its constant points.

===============================================================================
*/

#include <iostream>
#include <stdlib.h>
#include <math.h>

#include "TGPoint3.h"
#include "TGMatrix3.h"
#include "PyEmbed.h"
#include "NiAlphaAccumulator.h"
#include "NiAmbientLight.h"
#include "NiAlphaController.h"
#include "NiAnimBlender.h"
#include "NiAutoNormalParticles.h"
#include "NiAutoNormalParticlesData.h"
#include "NiBSPNode.h"
#include "NiBillboardNode.h"
#include "NiBezFloatKey.h"
#include "NiBezPosKey.h"
#include "NiBezRotKey.h"
#include "NiBinaryVoxelData.h"
#include "NiBinaryVoxelExtraData.h"
#include "NiBone.h"
#include "NiClusterAccumulator.h"
#include "NiCollisionSwitch.h"
#include "NiColorData.h"
#include "NiDirectionalLight.h"
#include "NiEnvMappedTriShape.h"
#include "NiEnvMappedTriShapeData.h"
#include "NiEulerRotKey.h"
#include "NiFlipController.h"
#include "NiFloatController.h"
#include "NiFloatData.h"
#include "NiFltAnimationNode.h"
#include "NiForce.h"
#include "NiGravity.h"
#include "NiKeyType4FloatKey.h"
#include "NiKeyType6FloatKey.h"
#include "NiKeyframeController.h"
#include "NiKeyframeManager.h"
#include "NiLODNode.h"
#include "NiLines.h"
#include "NiLightColorController.h"
#include "NiLinRotKey.h"
#include "NiListener.h"
#include "NiLookAtController.h"
#include "NiMaterialColorController.h"
#include "NiMorphController.h"
#include "NiMorphData.h"
#include "NiMorphKey.h"
#include "NiMorpherController.h"
#include "NiParticleBomb.h"
#include "NiParticleSystemController.h"
#include "NiParticles.h"
#include "NiParticlesData.h"
#include "NiPathController.h"
#include "NiPlanarCollider.h"
#include "NiPointLight.h"
#include "NiPosData.h"
#include "NiRollController.h"
#include "NiSequenceStreamHelper.h"
#include "NiSkinController.h"
#include "NiSortAdjustNode.h"
#include "NiSoundSystem.h"
#include "NiSphericalCollider.h"
#include "NiSpotLight.h"
#include "NiStringExtraData.h"
#include "NiSwitchNode.h"
#include "NiTCBFloatKey.h"
#include "NiTCBPosKey.h"
#include "NiTCBRotKey.h"
#include "NiTextKeyExtraData.h"
#include "NiTextureEffect.h"
#include "NiTriShapeDynamicData.h"
#include "NiTriShapeSkinController.h"
#include "NiTriStrip.h"
#include "NiTriStripData.h"
#include "NiTriStrips.h"
#include "NiTriStripsData.h"
#include "NiTriangles.h"
#include "NiTrianglesData.h"
#include "NiVertWeightsExtraData.h"
#include "NiVisController.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).

// The standard streams' initialisers and exit-time destructors (<iostream>), and the locale-id guard.
// FUNCTION: STBC 0x0045d920 _$E32
// FUNCTION: STBC 0x0045d930 _$E29
// FUNCTION: STBC 0x0045d940 _$E31
// FUNCTION: STBC 0x0045d950 _$E30
// FUNCTION: STBC 0x0045d960 _$E38
// FUNCTION: STBC 0x0045d970 _$E35
// FUNCTION: STBC 0x0045d980 _$E37
// FUNCTION: STBC 0x0045d990 _$E36
// FUNCTION: STBC 0x0045daa0 _$E687
// FUNCTION: STBC 0x0045dac0 _$E686

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0045d9e0 _$E76
// FUNCTION: STBC 0x0045d9f0 _$E75
// FUNCTION: STBC 0x0045da00 _$E91
// FUNCTION: STBC 0x0045da10 _$E90
// FUNCTION: STBC 0x0045da20 _$E95
// FUNCTION: STBC 0x0045da30 _$E94
// FUNCTION: STBC 0x0045da40 _$E99
// FUNCTION: STBC 0x0045da50 _$E98
// FUNCTION: STBC 0x0045da60 _$E103
// FUNCTION: STBC 0x0045da70 _$E102
// FUNCTION: STBC 0x0045da80 _$E220
// FUNCTION: STBC 0x0045da90 _$E219
// FUNCTION: STBC 0x0045dad0 _$E80
// FUNCTION: STBC 0x0045dae0 _$E79
// FUNCTION: STBC 0x0045daf0 _$E162
// FUNCTION: STBC 0x0045db00 _$E161
// FUNCTION: STBC 0x0045db10 _$E175
// FUNCTION: STBC 0x0045db20 _$E174
// FUNCTION: STBC 0x0045db30 _$E225
// FUNCTION: STBC 0x0045db40 _$E224
// FUNCTION: STBC 0x0045db50 _$E230
// FUNCTION: STBC 0x0045db60 _$E229
// FUNCTION: STBC 0x0045db70 _$E234
// FUNCTION: STBC 0x0045db80 _$E233
// FUNCTION: STBC 0x0045db90 _$E238
// FUNCTION: STBC 0x0045dba0 _$E237
// FUNCTION: STBC 0x0045dbb0 _$E245
// FUNCTION: STBC 0x0045dbc0 _$E244
// FUNCTION: STBC 0x0045dbd0 _$E270
// FUNCTION: STBC 0x0045dbe0 _$E269
// FUNCTION: STBC 0x0045dbf0 _$E274
// FUNCTION: STBC 0x0045dc00 _$E273
// FUNCTION: STBC 0x0045dc10 _$E120
// FUNCTION: STBC 0x0045dc20 _$E119
// FUNCTION: STBC 0x0045dc30 _$E256
// FUNCTION: STBC 0x0045dc40 _$E255
// FUNCTION: STBC 0x0045dc50 _$E263
// FUNCTION: STBC 0x0045dc60 _$E262
// FUNCTION: STBC 0x0045dc70 _$E342
// FUNCTION: STBC 0x0045dc80 _$E341
// FUNCTION: STBC 0x0045dc90 _$E346
// FUNCTION: STBC 0x0045dca0 _$E345
// FUNCTION: STBC 0x0045dcb0 _$E415
// FUNCTION: STBC 0x0045dcc0 _$E414
// FUNCTION: STBC 0x0045dcd0 _$E421
// FUNCTION: STBC 0x0045dce0 _$E420
// FUNCTION: STBC 0x0045dcf0 _$E278
// FUNCTION: STBC 0x0045dd00 _$E277
// FUNCTION: STBC 0x0045dd10 _$E282
// FUNCTION: STBC 0x0045dd20 _$E281
// FUNCTION: STBC 0x0045dd30 _$E350
// FUNCTION: STBC 0x0045dd40 _$E349
// FUNCTION: STBC 0x0045dd50 _$E354
// FUNCTION: STBC 0x0045dd60 _$E353
// FUNCTION: STBC 0x0045dd70 _$E358
// FUNCTION: STBC 0x0045dd80 _$E357
// FUNCTION: STBC 0x0045dd90 _$E362
// FUNCTION: STBC 0x0045dda0 _$E361
// FUNCTION: STBC 0x0045ddb0 _$E370
// FUNCTION: STBC 0x0045ddc0 _$E369
// FUNCTION: STBC 0x0045ddd0 _$E391
// FUNCTION: STBC 0x0045dde0 _$E390
// FUNCTION: STBC 0x0045ddf0 _$E366
// FUNCTION: STBC 0x0045de00 _$E365
// FUNCTION: STBC 0x0045de10 _$E395
// FUNCTION: STBC 0x0045de20 _$E394
// FUNCTION: STBC 0x0045de30 _$E125
// FUNCTION: STBC 0x0045de40 _$E124
// FUNCTION: STBC 0x0045de50 _$E411
// FUNCTION: STBC 0x0045de60 _$E410
// FUNCTION: STBC 0x0045de70 _$E130
// FUNCTION: STBC 0x0045de80 _$E129
// FUNCTION: STBC 0x0045de90 _$E425
// FUNCTION: STBC 0x0045dea0 _$E424
// FUNCTION: STBC 0x0045deb0 _$E429
// FUNCTION: STBC 0x0045dec0 _$E428
// FUNCTION: STBC 0x0045ded0 _$E441
// FUNCTION: STBC 0x0045dee0 _$E440
// FUNCTION: STBC 0x0045def0 _$E445
// FUNCTION: STBC 0x0045df00 _$E444
// FUNCTION: STBC 0x0045df10 _$E139
// FUNCTION: STBC 0x0045df20 _$E138
// FUNCTION: STBC 0x0045df30 _$E478
// FUNCTION: STBC 0x0045df40 _$E477
// FUNCTION: STBC 0x0045df50 _$E482
// FUNCTION: STBC 0x0045df60 _$E481
// FUNCTION: STBC 0x0045df70 _$E474
// FUNCTION: STBC 0x0045df80 _$E473
// FUNCTION: STBC 0x0045df90 _$E145
// FUNCTION: STBC 0x0045dfa0 _$E144
// FUNCTION: STBC 0x0045dfb0 _$E150
// FUNCTION: STBC 0x0045dfc0 _$E149
// FUNCTION: STBC 0x0045dfd0 _$E180
// FUNCTION: STBC 0x0045dfe0 _$E179
// FUNCTION: STBC 0x0045e080 _$E541
// FUNCTION: STBC 0x0045e090 _$E540
// FUNCTION: STBC 0x0045e0a0 _$E185
// FUNCTION: STBC 0x0045e0b0 _$E184
// FUNCTION: STBC 0x0045e0c0 _$E553
// FUNCTION: STBC 0x0045e0d0 _$E552
// FUNCTION: STBC 0x0045e0e0 _$E557
// FUNCTION: STBC 0x0045e0f0 _$E556
// FUNCTION: STBC 0x0045e100 _$E190
// FUNCTION: STBC 0x0045e110 _$E189
// FUNCTION: STBC 0x0045e120 _$E570
// FUNCTION: STBC 0x0045e130 _$E569
// FUNCTION: STBC 0x0045e140 _$E155
// FUNCTION: STBC 0x0045e150 _$E154
// FUNCTION: STBC 0x0045e160 _$E574
// FUNCTION: STBC 0x0045e170 _$E573
// FUNCTION: STBC 0x0045e180 _$E584
// FUNCTION: STBC 0x0045e190 _$E583
// FUNCTION: STBC 0x0045e1a0 _$E195
// FUNCTION: STBC 0x0045e1b0 _$E194
// FUNCTION: STBC 0x0045e1d0 _$E200
// FUNCTION: STBC 0x0045e1e0 _$E199
// FUNCTION: STBC 0x0045e1f0 _$E612
// FUNCTION: STBC 0x0045e200 _$E611
// FUNCTION: STBC 0x0045e210 _$E616
// FUNCTION: STBC 0x0045e220 _$E615
// FUNCTION: STBC 0x0045e230 _$E588
// FUNCTION: STBC 0x0045e240 _$E587
// FUNCTION: STBC 0x0045e250 _$E592
// FUNCTION: STBC 0x0045e260 _$E591
// FUNCTION: STBC 0x0045e270 _$E596
// FUNCTION: STBC 0x0045e280 _$E595
// FUNCTION: STBC 0x0045e290 _$E600
// FUNCTION: STBC 0x0045e2a0 _$E599
// FUNCTION: STBC 0x0045e2b0 _$E604
// FUNCTION: STBC 0x0045e2c0 _$E603
// FUNCTION: STBC 0x0045e2d0 _$E608
// FUNCTION: STBC 0x0045e2e0 _$E607
// FUNCTION: STBC 0x0045e2f0 _$E205
// FUNCTION: STBC 0x0045e300 _$E204
// FUNCTION: STBC 0x0045e310 _$E620
// FUNCTION: STBC 0x0045e320 _$E619
// FUNCTION: STBC 0x0045e330 _$E210
// FUNCTION: STBC 0x0045e340 _$E209
// FUNCTION: STBC 0x0045e350 _$E215
// FUNCTION: STBC 0x0045e360 _$E214
// FUNCTION: STBC 0x0045e370 _$E326
// FUNCTION: STBC 0x0045e380 _$E325
// FUNCTION: STBC 0x0045e390 _$E317
// FUNCTION: STBC 0x0045e3a0 _$E316
// FUNCTION: STBC 0x0045e3b0 _$E291
// FUNCTION: STBC 0x0045e3c0 _$E290
// FUNCTION: STBC 0x0045e3d0 _$E330
// FUNCTION: STBC 0x0045e3e0 _$E329
// FUNCTION: STBC 0x0045e3f0 _$E334
// FUNCTION: STBC 0x0045e400 _$E333
// FUNCTION: STBC 0x0045e410 _$E624
// FUNCTION: STBC 0x0045e420 _$E623
// FUNCTION: STBC 0x0045e430 _$E628
// FUNCTION: STBC 0x0045e440 _$E627

// FUNCTION: STBC 0x0045e450 _$E633
// FUNCTION: STBC 0x0045e460 _$E630
// FUNCTION: STBC 0x0045e480 _$E632
// FUNCTION: STBC 0x0045e490 _$E631
// DATA: STBC 0x00980df0
const TGPoint3 MODEL_FORWARD( 0.0f, 1.0f, 0.0f );
// FUNCTION: STBC 0x0045e4a0 _$E638
// FUNCTION: STBC 0x0045e4b0 _$E635
// FUNCTION: STBC 0x0045e4d0 _$E637
// FUNCTION: STBC 0x0045e4e0 _$E636
// DATA: STBC 0x00980f20
const TGPoint3 MODEL_BACKWARD( 0.0f, -1.0f, 0.0f );
// FUNCTION: STBC 0x0045e4f0 _$E643
// FUNCTION: STBC 0x0045e500 _$E640
// FUNCTION: STBC 0x0045e520 _$E642
// FUNCTION: STBC 0x0045e530 _$E641
// DATA: STBC 0x00980e30
const TGPoint3 MODEL_UP( 0.0f, 0.0f, 1.0f );
// FUNCTION: STBC 0x0045e540 _$E648
// FUNCTION: STBC 0x0045e550 _$E645
// FUNCTION: STBC 0x0045e570 _$E647
// FUNCTION: STBC 0x0045e580 _$E646
// DATA: STBC 0x00980ee8
const TGPoint3 MODEL_DOWN( 0.0f, 0.0f, -1.0f );
// FUNCTION: STBC 0x0045e590 _$E653
// FUNCTION: STBC 0x0045e5a0 _$E650
// FUNCTION: STBC 0x0045e5c0 _$E652
// FUNCTION: STBC 0x0045e5d0 _$E651
// DATA: STBC 0x00980e50
const TGPoint3 MODEL_RIGHT( 1.0f, 0.0f, 0.0f );
// FUNCTION: STBC 0x0045e5e0 _$E658
// FUNCTION: STBC 0x0045e5f0 _$E655
// FUNCTION: STBC 0x0045e610 _$E657
// FUNCTION: STBC 0x0045e620 _$E656
// DATA: STBC 0x00980e70
const TGPoint3 MODEL_LEFT( -1.0f, 0.0f, 0.0f );

// FUNCTION: STBC 0x0045e630 _$E664
// FUNCTION: STBC 0x0045e640 _$E660
// FUNCTION: STBC 0x0045e660 _$E663
// FUNCTION: STBC 0x0045e670 _$E661
// DATA: STBC 0x00980f48
const TGPoint3 TGPoint3::ZERO( 0.0f, 0.0f, 0.0f );
// FUNCTION: STBC 0x0045e690 _$E669
// FUNCTION: STBC 0x0045e6a0 _$E666
// FUNCTION: STBC 0x0045e6c0 _$E668
// FUNCTION: STBC 0x0045e6d0 _$E667
// DATA: STBC 0x00980ed8
const TGPoint3 TGPoint3::UNIT_X( 1.0f, 0.0f, 0.0f );
// FUNCTION: STBC 0x0045e6f0 _$E674
// FUNCTION: STBC 0x0045e700 _$E671
// FUNCTION: STBC 0x0045e720 _$E673
// FUNCTION: STBC 0x0045e730 _$E672
// DATA: STBC 0x00980f68
const TGPoint3 TGPoint3::UNIT_Y( 0.0f, 1.0f, 0.0f );
// FUNCTION: STBC 0x0045e750 _$E679
// FUNCTION: STBC 0x0045e760 _$E676
// FUNCTION: STBC 0x0045e780 _$E678
// FUNCTION: STBC 0x0045e790 _$E677
// DATA: STBC 0x00980ea8
const TGPoint3 TGPoint3::UNIT_Z( 0.0f, 0.0f, 1.0f );

/*
================
TGPoint3::GetRandomUnitVector

A unit vector in a random direction: random points in the cube [-1,1]^3,
unitized, until one is not degenerate.
================
*/
// FUNCTION: STBC 0x0045e7b0
TGPoint3 TGPoint3::GetRandomUnitVector( void ) {
	TGPoint3 v;
	do {
		v.x = NiSymmetricRandom();
		v.y = NiSymmetricRandom();
		v.z = NiSymmetricRandom();
		v.Unitize();
	} while ( v.SqrLength() < 0.5f );
	return v;
}

/*
================
TGPoint3::MultMatrix

This point, as a row vector, times the matrix.
================
*/
// FUNCTION: STBC 0x0045e8d0
void TGPoint3::MultMatrix( TGMatrix3 *matrix ) {
	TGPoint3 pt;
	pt.x = x;
	pt.y = y;
	pt.z = z;
	pt = pt * *matrix;
	SetXYZ( pt.x, pt.y, pt.z );
}

/*
================
TGPoint3::MultMatrixLeft

The matrix times this point, as a column vector.
================
*/
// FUNCTION: STBC 0x0045e950
void TGPoint3::MultMatrixLeft( TGMatrix3 *matrix ) {
	*this = *matrix * *this;
}

/*
================
TGPoint3::GetAlignedComponent

The part of this vector along dir: its projection onto dir. Zero when dir
is the zero vector.
================
*/
// FUNCTION: STBC 0x0045e980
TGPoint3 TGPoint3::GetAlignedComponent( const TGPoint3 &dir ) const {
	float dot = Dot( dir );
	float sqrLength = dir.SqrLength();
	if ( sqrLength != 0.0f ) {
		return TGPoint3( dir * ( 1.0f / sqrLength ) * dot );
	}
	return ZERO;
}

/*
================
TGPoint3::GetPerpendicularComponent

The part of this vector perpendicular to dir.
================
*/
// FUNCTION: STBC 0x0045ea50
TGPoint3 TGPoint3::GetPerpendicularComponent( const TGPoint3 &dir ) const {
	return TGPoint3( *this - GetAlignedComponent( dir ) );
}

/*
================
TGPoint3_SaveInstance

A point's state is the tuple of its coordinates and the instance's
thisown flag. False when the instance holds no point.
================
*/
// FUNCTION: STBC 0x0045eaa0
bool TGPoint3_SaveInstance( PyObject *instance, PyObject **state ) {
	TGPoint3 *point = NULL;
	PyEmbed::Unknown6F98E0( instance, (void **)&point, "TGPoint3" );
	if ( point != NULL ) {
		PyObject *tuple = PyTuple_New( 4 );
		*state = tuple;
		PyTuple_SetItem( tuple, 0, PyFloat_FromDouble( point->x ) );
		PyTuple_SetItem( tuple, 1, PyFloat_FromDouble( point->y ) );
		PyTuple_SetItem( tuple, 2, PyFloat_FromDouble( point->z ) );
		PyTuple_SetItem( tuple, 3, PyObject_GetAttrString( instance, "thisown" ) );
		return true;
	}
	return false;
}

/*
================
TGPoint3_LoadInstance

A saved point's state is the tuple of its coordinates and its thisown
flag: the instance gets a new point of them, as its SWIG pointer, and the
flag back. False when the state is anything else.
================
*/
// FUNCTION: STBC 0x0045eb50
bool TGPoint3_LoadInstance( PyObject *instance, PyObject *state ) {
	if ( PyTuple_Check( state ) && PyTuple_Size( state ) == 4 && PyFloat_Check( PyTuple_GetItem( state, 0 ) )
		 && PyFloat_Check( PyTuple_GetItem( state, 1 ) ) && PyFloat_Check( PyTuple_GetItem( state, 2 ) )
		 && PyInt_Check( PyTuple_GetItem( state, 3 ) ) ) {
		TGPoint3 *point = new TGPoint3( (float)PyFloat_AsDouble( PyTuple_GetItem( state, 0 ) ),
										(float)PyFloat_AsDouble( PyTuple_GetItem( state, 1 ) ),
										(float)PyFloat_AsDouble( PyTuple_GetItem( state, 2 ) ) );
		PyObject_SetAttrString( instance, "this", PyEmbed::NewPointerString( (unsigned int)point, "TGPoint3" ) );
		PyObject_SetAttrString( instance, "thisown", PyTuple_GetItem( state, 3 ) );
		return true;
	}
	return false;
}

/*
================
TGPoint3::Set

Takes the other point's coordinates.
================
*/
// FUNCTION: STBC 0x005ee360 ?Set@TGPoint3@@QAEXV1@@Z
void TGPoint3::Set( TGPoint3 pt ) {
	x = pt.x;
	SetY( pt.y );
	SetZ( pt.z );
}
