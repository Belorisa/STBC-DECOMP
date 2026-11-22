/*
===============================================================================

	Nebula.cpp

===============================================================================
*/

#include <iostream>
#include "Nebula.h"
#include "SetClass.h"
#include "SetManager.h"
#include "NiAccumulator.h"
#include "DamageableObject.h"
#include "DistanceModifier.h"
#include "TGFuzzyClusterGeom.h"
#include "ShipClass.h"
#include "TGEvent.h"
#include "TGEventManager.h"
#include "TGPointerMap.h"
#include "TGStream.h"
#include "UtopiaModule.h"
#include "NiNode.h"
#include "NiCamera.h"
#include "NiMaterialProperty.h"
#include "NiMath.h"
#include "GlowPass.h"
#include "ImageManagerClass.h"
#include "NiAlphaProperty.h"
#include "NiFogProperty.h"
#include "NiTextureModeProperty.h"
#include "NiTextureProperty.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "TGModelUtils.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0059c680 _$E493
// FUNCTION: STBC 0x0059c6a0 _$E492

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00597330 _$E32
// FUNCTION: STBC 0x00597340 _$E29
// FUNCTION: STBC 0x00597350 _$E31
// FUNCTION: STBC 0x00597360 _$E30
// FUNCTION: STBC 0x00597370 _$E38
// FUNCTION: STBC 0x00597380 _$E35
// FUNCTION: STBC 0x00597390 _$E37
// FUNCTION: STBC 0x005973a0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00598290 _$E66
// FUNCTION: STBC 0x005982a0 _$E65
// FUNCTION: STBC 0x00598d30 _$E231
// FUNCTION: STBC 0x00598d40 _$E230
// FUNCTION: STBC 0x00599f90 _$E77
// FUNCTION: STBC 0x00599fa0 _$E76
// FUNCTION: STBC 0x0059a070 _$E81
// FUNCTION: STBC 0x0059a080 _$E80
// FUNCTION: STBC 0x0059aa60 _$E85
// FUNCTION: STBC 0x0059aa70 _$E84
// FUNCTION: STBC 0x0059ab60 _$E89
// FUNCTION: STBC 0x0059ab70 _$E88
// FUNCTION: STBC 0x0059b150 _$E206
// FUNCTION: STBC 0x0059b160 _$E205
// FUNCTION: STBC 0x0059bda0 _$E148
// FUNCTION: STBC 0x0059bdb0 _$E147
// FUNCTION: STBC 0x0059bdc0 _$E161
// FUNCTION: STBC 0x0059bdd0 _$E160
// FUNCTION: STBC 0x0059bde0 _$E211
// FUNCTION: STBC 0x0059bdf0 _$E210
// FUNCTION: STBC 0x0059be00 _$E288
// FUNCTION: STBC 0x0059be10 _$E287
// FUNCTION: STBC 0x0059be20 _$E215
// FUNCTION: STBC 0x0059be30 _$E214
// FUNCTION: STBC 0x0059be40 _$E292
// FUNCTION: STBC 0x0059be50 _$E291
// FUNCTION: STBC 0x0059be60 _$E299
// FUNCTION: STBC 0x0059be70 _$E298
// FUNCTION: STBC 0x0059be80 _$E318
// FUNCTION: STBC 0x0059be90 _$E317
// FUNCTION: STBC 0x0059bea0 _$E322
// FUNCTION: STBC 0x0059beb0 _$E321
// FUNCTION: STBC 0x0059bec0 _$E106
// FUNCTION: STBC 0x0059bed0 _$E105
// FUNCTION: STBC 0x0059bee0 _$E243
// FUNCTION: STBC 0x0059bef0 _$E242
// FUNCTION: STBC 0x0059bf00 _$E250
// FUNCTION: STBC 0x0059bf10 _$E249
// FUNCTION: STBC 0x0059bf20 _$E254
// FUNCTION: STBC 0x0059bf30 _$E253
// FUNCTION: STBC 0x0059bf40 _$E258
// FUNCTION: STBC 0x0059bf50 _$E257
// FUNCTION: STBC 0x0059bf60 _$E262
// FUNCTION: STBC 0x0059bf70 _$E261
// FUNCTION: STBC 0x0059bf80 _$E268
// FUNCTION: STBC 0x0059bf90 _$E267
// FUNCTION: STBC 0x0059bfa0 _$E326
// FUNCTION: STBC 0x0059bfb0 _$E325
// FUNCTION: STBC 0x0059bfc0 _$E330
// FUNCTION: STBC 0x0059bfd0 _$E329
// FUNCTION: STBC 0x0059bfe0 _$E334
// FUNCTION: STBC 0x0059bff0 _$E333
// FUNCTION: STBC 0x0059c000 _$E338
// FUNCTION: STBC 0x0059c010 _$E337
// FUNCTION: STBC 0x0059c020 _$E342
// FUNCTION: STBC 0x0059c030 _$E341
// FUNCTION: STBC 0x0059c040 _$E346
// FUNCTION: STBC 0x0059c050 _$E345
// FUNCTION: STBC 0x0059c060 _$E350
// FUNCTION: STBC 0x0059c070 _$E349
// FUNCTION: STBC 0x0059c080 _$E354
// FUNCTION: STBC 0x0059c090 _$E353
// FUNCTION: STBC 0x0059c0a0 _$E358
// FUNCTION: STBC 0x0059c0b0 _$E357
// FUNCTION: STBC 0x0059c0c0 _$E362
// FUNCTION: STBC 0x0059c0d0 _$E361
// FUNCTION: STBC 0x0059c0e0 _$E111
// FUNCTION: STBC 0x0059c0f0 _$E110
// FUNCTION: STBC 0x0059c100 _$E366
// FUNCTION: STBC 0x0059c110 _$E365
// FUNCTION: STBC 0x0059c120 _$E116
// FUNCTION: STBC 0x0059c130 _$E115
// FUNCTION: STBC 0x0059c140 _$E370
// FUNCTION: STBC 0x0059c150 _$E369
// FUNCTION: STBC 0x0059c160 _$E374
// FUNCTION: STBC 0x0059c170 _$E373
// FUNCTION: STBC 0x0059c180 _$E378
// FUNCTION: STBC 0x0059c190 _$E377
// FUNCTION: STBC 0x0059c1a0 _$E382
// FUNCTION: STBC 0x0059c1b0 _$E381
// FUNCTION: STBC 0x0059c1c0 _$E125
// FUNCTION: STBC 0x0059c1d0 _$E124
// FUNCTION: STBC 0x0059c1e0 _$E386
// FUNCTION: STBC 0x0059c1f0 _$E385
// FUNCTION: STBC 0x0059c200 _$E390
// FUNCTION: STBC 0x0059c210 _$E389
// FUNCTION: STBC 0x0059c220 _$E394
// FUNCTION: STBC 0x0059c230 _$E393
// FUNCTION: STBC 0x0059c240 _$E131
// FUNCTION: STBC 0x0059c250 _$E130
// FUNCTION: STBC 0x0059c260 _$E136
// FUNCTION: STBC 0x0059c270 _$E135
// FUNCTION: STBC 0x0059c280 _$E166
// FUNCTION: STBC 0x0059c290 _$E165
// FUNCTION: STBC 0x0059c2a0 _$E398
// FUNCTION: STBC 0x0059c2b0 _$E397
// FUNCTION: STBC 0x0059c2c0 _$E171
// FUNCTION: STBC 0x0059c2d0 _$E170
// FUNCTION: STBC 0x0059c2e0 _$E402
// FUNCTION: STBC 0x0059c2f0 _$E401
// FUNCTION: STBC 0x0059c300 _$E406
// FUNCTION: STBC 0x0059c310 _$E405
// FUNCTION: STBC 0x0059c320 _$E176
// FUNCTION: STBC 0x0059c330 _$E175
// FUNCTION: STBC 0x0059c340 _$E410
// FUNCTION: STBC 0x0059c350 _$E409
// FUNCTION: STBC 0x0059c360 _$E141
// FUNCTION: STBC 0x0059c370 _$E140
// FUNCTION: STBC 0x0059c3c0 _$E414
// FUNCTION: STBC 0x0059c3d0 _$E413
// FUNCTION: STBC 0x0059c3e0 _$E418
// FUNCTION: STBC 0x0059c3f0 _$E417
// FUNCTION: STBC 0x0059c400 _$E181
// FUNCTION: STBC 0x0059c410 _$E180
// FUNCTION: STBC 0x0059c420 _$E186
// FUNCTION: STBC 0x0059c430 _$E185
// FUNCTION: STBC 0x0059c440 _$E422
// FUNCTION: STBC 0x0059c450 _$E421
// FUNCTION: STBC 0x0059c460 _$E426
// FUNCTION: STBC 0x0059c470 _$E425
// FUNCTION: STBC 0x0059c480 _$E430
// FUNCTION: STBC 0x0059c490 _$E429
// FUNCTION: STBC 0x0059c4a0 _$E434
// FUNCTION: STBC 0x0059c4b0 _$E433
// FUNCTION: STBC 0x0059c4c0 _$E438
// FUNCTION: STBC 0x0059c4d0 _$E437
// FUNCTION: STBC 0x0059c4e0 _$E442
// FUNCTION: STBC 0x0059c4f0 _$E441
// FUNCTION: STBC 0x0059c500 _$E446
// FUNCTION: STBC 0x0059c510 _$E445
// FUNCTION: STBC 0x0059c520 _$E450
// FUNCTION: STBC 0x0059c530 _$E449
// FUNCTION: STBC 0x0059c540 _$E191
// FUNCTION: STBC 0x0059c550 _$E190
// FUNCTION: STBC 0x0059c560 _$E454
// FUNCTION: STBC 0x0059c570 _$E453
// FUNCTION: STBC 0x0059c580 _$E196
// FUNCTION: STBC 0x0059c590 _$E195
// FUNCTION: STBC 0x0059c5a0 _$E201
// FUNCTION: STBC 0x0059c5b0 _$E200
// FUNCTION: STBC 0x0059c700 _$E458
// FUNCTION: STBC 0x0059c710 _$E457
// FUNCTION: STBC 0x0059c720 _$E463
// FUNCTION: STBC 0x0059c730 _$E462
// FUNCTION: STBC 0x0059c740 _$E467
// FUNCTION: STBC 0x0059c750 _$E466

// DATA: STBC 0x98DE48 ?rtti@TGFuzzyClusterInnerGeom@@2VNiRTTI@@B
#define MIN_SPIN	0.1f		// the least of the overlay controller's random spin, before its scale (invented name)

// The radius the overlay's sphere drifts within, and its scale (Nebula::CreateOverlay; invented name;
// retail keeps it at 0x893F2C, among the unit's constants).
static const float	OVERLAY_RADIUS = 14.0f;

// The inner overlay's fade (TGFuzzyClusterInnerController::Update; invented names).
// DATA: STBC 0x008e5d60
static float	outerRadius = 7.0f;		// a target farther than this comes back
// DATA: STBC 0x008e5d64
static float	innerRadius = 5.0f;		// the alpha fades out from here to outerRadius
// DATA: STBC 0x008e5d68
static float	fadeTime = 1.0f;		// seconds of the fade in at the center
// DATA: STBC 0x008e5d6c
static float	maxAlpha = 1.0f;


/*
================
Nebula::Cast

The object as a Nebula; NULL when it is none.
================
*/
// FUNCTION: STBC 0x005973b0
Nebula *Nebula::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( NEBULA_TYPE ) ) {
		return (Nebula *)object;
	}
	return NULL;
}

// The class's NiRTTI: its dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00597410 _$E473
// FUNCTION: STBC 0x00597420 _$E469
// FUNCTION: STBC 0x00597440 _$E472
// FUNCTION: STBC 0x00597450 _$E470
const NiRTTI TGFuzzyClusterInnerGeom::rtti( "TGFuzzyClusterInnerGeom", &NiTriShape::rtti );

/*
	TGFuzzyClusterInnerController: moves and fades the overlay. Evidence: the
	vtable 0x893F30 is NiTimeController's 20 slots with the destructor
	(0x598150), Update (0x599BF0) and TargetIsRequiredType (0x598120, which
	asks for a TGFuzzyClusterInnerGeom) replaced; GetRTTI is NiTimeController's
	own, so the class has no NiRTTI and no string names it: the name is
	invented.

	The class is defined here, between the two NiRTTIs: retail gives their
	exit destructors separate guard bytes (0x98DE1C, 0x98DE1D) with no code
	between their initialisers, which a definition of inline functions between
	them gives (docs/levers/statics.md, "guard bytes"); only this unit uses the
	class.
*/
// FUNCTION: STBC 0x00598150 ??_GTGFuzzyClusterInnerController@@UAEPAXI@Z
// FUNCTION: STBC 0x00598170 ??1TGFuzzyClusterInnerController@@UAE@XZ
class TGFuzzyClusterInnerController : public NiTimeController {
public:
	virtual void			Update( float time );
	// FUNCTION: STBC 0x00598120 ?TargetIsRequiredType@TGFuzzyClusterInnerController@@UBE_NXZ
	virtual bool			TargetIsRequiredType() const {
								if ( !target ) {
									return false;
								}
								return target->IsKindOf( &TGFuzzyClusterInnerGeom::rtti );
							}

							TGFuzzyClusterInnerController() {
								fadeStart = -NI_INFINITY;
								material = NULL;
							}

	// the material whose alpha fades (invented name)
	void					SetMaterial( NiMaterialProperty *material_ ) { material = material_; }

	// Picks a new spin and puts the target at the position.
	void					Reset( const NiPoint3 &position );

protected:
	float					spinRate;			// 0x30 radians a second
	NiPoint3				spinAxis;			// 0x34
	float					fadeStart;			// 0x40 the time the fade in began
	NiPoint3				lastPosition;		// 0x44 where the target's parent was at the last update
	NiMaterialProperty *	material;			// 0x50 the target's; its alpha fades
};

// FUNCTION: STBC 0x00597470 _$E479
// FUNCTION: STBC 0x00597480 _$E475
// FUNCTION: STBC 0x005974a0 _$E478
// FUNCTION: STBC 0x005974b0 _$E476
// DATA: STBC 0x98DE98 ?rtti@TGFuzzyClusterGeom@@2VNiRTTI@@B
const NiRTTI TGFuzzyClusterGeom::rtti( "TGFuzzyClusterGeom", &TGFuzzyTriShape::rtti );

/*
================
Nebula::AddToSet
================
*/
// FUNCTION: STBC 0x005974d0
void Nebula::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, NEBULA_TYPE );
		ObjectClass::AddToSet( set );
	}
}

/*
================
Nebula::RemoveFromSet
================
*/
// FUNCTION: STBC 0x00597500
void Nebula::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, NEBULA_TYPE );
		ObjectClass::RemoveFromSet( set );
	}
}

/*
================
Nebula::GetClassObjects

The set's nebulas and their number.
================
*/
// FUNCTION: STBC 0x00597530
Nebula **Nebula::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (Nebula **)set->objectTable.GetObjects( NEBULA_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
Nebula::GetNumClassObjects

The number of the set's nebulas.
================
*/
// FUNCTION: STBC 0x00597560
int Nebula::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( NEBULA_TYPE );
	}
	return 0;
}

/*
================
Nebula::GetObjectByID

The nebula with the given ID in the set, or among every object when the
set is NULL; NULL when it is none or not a Nebula.
================
*/
// FUNCTION: STBC 0x00597580
Nebula *Nebula::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( NEBULA_TYPE ) ) {
			return (Nebula *)object;
		}
		return NULL;
	}
	return (Nebula *)set->GetClassObject( NEBULA_TYPE, id );
}

/*
================
Nebula::GetObject

The nebula of the name in the set, or in any set when the set is NULL,
the last set first.
================
*/
// FUNCTION: STBC 0x005975f0
Nebula *Nebula::GetObject( SetClass *set, char *name ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( NEBULA_TYPE, name );
		}
		return (Nebula *)object;
	}
	return (Nebula *)set->GetClassObject( NEBULA_TYPE, name );
}

/*
================
Nebula::Create

A nebula of the fog colour, the sight distance, the overlay texture and the
sensor scale.
================
*/
// FUNCTION: STBC 0x00597650
Nebula *Nebula::Create( float r, float g, float b, float visibility, char *textureName, float sensorDensity ) {
	return new Nebula( NiColor( r, g, b ), visibility, sensorDensity, textureName );
}

/*
================
Nebula::CreateOverlay

The nebula's object node, and the overlay drawn around the camera: under
its node, drawn without the depth buffer and fog and blended, a fuzzy
sphere of the fog colour (each component jittered by a tenth) left out of
the glow, which its controller keeps drifting within the overlay radius.
================
*/
// FUNCTION: STBC 0x005976c0
// DRAFT: inline set: retail keeps NiPoint3's constructor and operator+ (0x426C30) out of line in the texture loop (c2trace margins 58 and 846: not a budget, the loop's source differs) and expands the fog AttachProperty's NiPointer<NiProperty> constructor this build calls (0x42FD00); the property sequence, the jittered colour, the positions, the controller and the constants agree
void Nebula::CreateOverlay( void ) {
	SetNiObject( new NiNode );
	overlayNode = new NiNode;

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( false );
	zBuffer->SetZBufferWrite( false );
	overlayNode->AttachProperty( zBuffer );

	if ( textureName != NULL ) {
		NiPointer<NiTextureProperty> texture = new NiTextureProperty;
		texture->SetImage( g_kImageManager.GetImage( textureName, true, true ) );
		overlayNode->AttachProperty( texture );

		NiPointer<NiTextureModeProperty> textureMode = new NiTextureModeProperty;
		textureMode->SetFilterMode( g_kImageManager.GetTextureFilterMode() );
		textureMode->SetClampMode( NiTextureModeProperty::WRAP_S_WRAP_T );
		textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
		overlayNode->AttachProperty( textureMode );
	}

	NiPointer<NiVertexColorProperty> vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	overlayNode->AttachProperty( vertexColor );

	NiPointer<NiFogProperty> fog = new NiFogProperty;
	fog->SetFog( false );
	overlayNode->AttachProperty( fog );

	NiPointer<NiAlphaProperty> alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_INVSRCALPHA );
	overlayNode->AttachProperty( alpha );

	{
		NiColor sphereColor;
		sphereColor.r = color.r + rand() * ( 0.2f / RAND_MAX ) - 0.1f;
		sphereColor.g = color.g + rand() * ( 0.2f / RAND_MAX ) - 0.1f;
		sphereColor.b = color.b + rand() * ( 0.2f / RAND_MAX ) - 0.1f;
		if ( sphereColor.r > 1.0f ) {
			sphereColor.r = 1.0f;
		} else if ( sphereColor.r < 0.0f ) {
			sphereColor.r = 0.0f;
		}
		if ( sphereColor.g > 1.0f ) {
			sphereColor.g = 1.0f;
		} else if ( sphereColor.r < 0.0f ) {	// BUG: tests the red component, not the green
			sphereColor.g = 0.0f;
		}
		if ( sphereColor.b > 1.0f ) {
			sphereColor.b = 1.0f;
		} else if ( sphereColor.r < 0.0f ) {	// BUG: tests the red component, not the blue
			sphereColor.b = 0.0f;
		}
		NiPointer<NiTriShape> sphere = CreateSphere( 1.0f, 2, false, sphereColor );

		TGFuzzyClusterInnerGeom *geom = new TGFuzzyClusterInnerGeom( sphere->GetShapeData(), NULL );
		GlowPass *glow = new GlowPass;
		glow->unknown14 = true;
		geom->AddExtraData( glow );
		NiMaterialProperty *material = new NiMaterialProperty;
		geom->AttachProperty( material );

		TGFuzzyClusterInnerController *controller = new TGFuzzyClusterInnerController;
		controller->SetTarget( geom );
		controller->SetMaterial( material );
		NiPoint3 position;
		do {
			position.x = NiSymmetricRandom() * OVERLAY_RADIUS * 0.8f;
			position.y = NiSymmetricRandom() * OVERLAY_RADIUS * 0.8f;
			position.z = NiSymmetricRandom() * OVERLAY_RADIUS * 0.8f;
		} while ( position.Length() > OVERLAY_RADIUS * 0.8f );
		controller->Reset( position );

		// the texture coordinates: the vertices' x and y under a transform
		// left at identity
		float scale = 1.0f;
		NiMatrix3 rotation;
		rotation.MakeIdentity();
		NiPoint3 translation = NiPoint3::ZERO;
		NiGeometryData *data = geom->GetShapeData();
		NiPoint3 *vertices = data->GetVertices();
		NiPoint2 *textures = data->GetTextures();
		for ( unsigned int i = 0; i < geom->GetShapeData()->GetVertexCount(); i++ ) {
			NiPoint3 mapped = rotation * ( scale * vertices[i] ) + translation;
			textures[i].x = mapped.x;
			textures[i].y = mapped.y;
		}

		geom->SetScale( OVERLAY_RADIUS > 0.0f ? OVERLAY_RADIUS : -OVERLAY_RADIUS );
		overlayNode->AttachChild( geom, false );
		innerGeom = geom;
	}
	overlayNode->UpdateProperties();
}

/*
================
Nebula::Nebula
================
*/
// FUNCTION: STBC 0x00598180
// DRAFT: the colour copy: retail stores the three floats straight into the member, this build goes through a pointer to it (lea); the three-float constructor gives the direct stores but copies through the FPU
Nebula::Nebula( const NiColor &color_, float visibility_, float sensorDensity_, char *textureName_ ) :
	visibility( visibility_ ),
	sensorDensity( sensorDensity_ ),
	color( color_ ),
	objectIDs( 2 ),
	damage( 0.0f ) {
	SetCollisionFlags( CFB_NO_COLLISIONS );
	if ( textureName_ != NULL ) {
		textureName = new char[strlen( textureName_ ) + 1];
		strcpy( textureName, textureName_ );
	} else {
		textureName = NULL;
	}
	CreateOverlay();
	SetHailable( false );
	SetScannable( false );
}

/*
================
Nebula::~Nebula
================
*/
// FUNCTION: STBC 0x005982b0
Nebula::~Nebula( void ) {
	delete [] textureName;
}

/*
================
Nebula::SetupDamage

Sets the damage the nebula does; the second value is the first when it
is -1.
================
*/
// FUNCTION: STBC 0x00598380
void Nebula::SetupDamage( float damage_, float hullDamage_ ) {
	damage = damage_;
	if ( hullDamage_ == -1.0f ) {
		hullDamage = damage_;
	} else {
		hullDamage = hullDamage_;
	}
}

/*
================
Nebula::CreateSphere

A sphere of the radius around the origin: an icosahedron whose triangles are
split in four, subdivisions times, each new vertex pushed out to the unit
sphere, then scaled. The triangles face out, or in when facingOut is false.
The vertices are welded afterwards; each gets its direction from the centre
as its normal and the colour, opaque.
================
*/
// FUNCTION: STBC 0x005983c0
// DRAFT: complete; the order of the table stores and the frame slots differ (faces at 0x38, retail 0x34), and the subdivision loop keeps other values in registers
NiTriShape *Nebula::CreateSphere( float radius, unsigned int subdivisions, bool facingOut, const NiColor &color ) {
	float golden = ( (float)sqrt( 5.0 ) + 1.0f ) * 0.5f;
	NiPoint3 icosahedron[12] = {
		NiPoint3( golden, 1.0f, 0.0f ),
		NiPoint3( -golden, 1.0f, 0.0f ),
		NiPoint3( golden, -1.0f, 0.0f ),
		NiPoint3( -golden, -1.0f, 0.0f ),
		NiPoint3( 1.0f, 0.0f, golden ),
		NiPoint3( 1.0f, 0.0f, -golden ),
		NiPoint3( -1.0f, 0.0f, golden ),
		NiPoint3( -1.0f, 0.0f, -golden ),
		NiPoint3( 0.0f, golden, 1.0f ),
		NiPoint3( 0.0f, -golden, 1.0f ),
		NiPoint3( 0.0f, golden, -1.0f ),
		NiPoint3( 0.0f, -golden, -1.0f )
	};
	unsigned short faces[60] = {
		0, 8, 4,	0, 5, 10,	2, 4, 9,	2, 11, 5,
		1, 6, 8,	1, 10, 7,	3, 9, 6,	3, 7, 11,
		0, 10, 8,	1, 8, 10,	2, 9, 11,	3, 11, 9,
		4, 2, 0,	5, 0, 2,	6, 1, 3,	7, 3, 1,
		8, 6, 4,	9, 4, 6,	10, 5, 7,	11, 7, 5
	};
	unsigned int i;

	unsigned int numVertices = 12;
	unsigned int numTriangles = 20;
	unsigned int maxVertices = numVertices;
	unsigned int maxTriangles = numTriangles;
	for ( i = 0; i < subdivisions; i++ ) {
		maxVertices += maxTriangles * 3;
		maxTriangles *= 4;
	}

	NiPoint3 *vertices = new NiPoint3[maxVertices];
	unsigned short *triangles = new unsigned short[maxTriangles * 3];
	unsigned short *spare = new unsigned short[maxTriangles * 3];
	for ( i = 0; i < 12; i++ ) {
		icosahedron[i].Unitize();
		vertices[i] = icosahedron[i];
	}
	memcpy( triangles, faces, sizeof( faces ) );

	for ( unsigned int level = 0; level < subdivisions; level++ ) {
		unsigned short *in = triangles;
		unsigned short *out = spare;
		for ( i = 0; i < numTriangles; i++ ) {
			unsigned short i0 = *in++;
			unsigned short i1 = *in++;
			unsigned short i2 = *in++;
			vertices[numVertices] = vertices[i1] + vertices[i0];
			vertices[numVertices].Unitize();
			vertices[numVertices + 1] = vertices[i2] + vertices[i1];
			vertices[numVertices + 1].Unitize();
			vertices[numVertices + 2] = vertices[i0] + vertices[i2];
			vertices[numVertices + 2].Unitize();
			*out++ = i0;
			*out++ = numVertices;
			*out++ = numVertices + 2;
			*out++ = i1;
			*out++ = numVertices + 1;
			*out++ = numVertices;
			*out++ = i2;
			*out++ = numVertices + 2;
			*out++ = numVertices + 1;
			*out++ = numVertices;
			*out++ = numVertices + 1;
			*out++ = numVertices + 2;
			numVertices += 3;
		}
		unsigned short *swap = triangles;
		triangles = spare;
		spare = swap;
		numTriangles *= 4;
	}
	delete [] spare;

	if ( radius != 1.0f ) {
		for ( i = 0; i < numVertices; i++ ) {
			vertices[i] = radius * vertices[i];
		}
	}
	if ( !facingOut ) {
		unsigned short *triangle = triangles;
		for ( i = 0; i < numTriangles; i++ ) {
			unsigned short first = triangle[0];
			triangle[0] = triangle[2];
			triangle[2] = first;
			triangle += 3;
		}
	}

	NiTriShape *shape = new NiTriShape( numVertices, vertices, NULL, NULL, NULL, numTriangles, triangles );
	shape->CollapseVertices( 0 );

	NiGeometryData *data = shape->GetShapeData();
	unsigned short count = data->GetVertexCount();
	NiPoint3 *normals = new NiPoint3[count];
	memcpy( normals, data->GetVertices(), count * sizeof( NiPoint3 ) );
	if ( radius != 1.0f ) {
		float invRadius = 1.0f / radius;
		for ( i = 0; i < count; i++ ) {
			normals[i] = invRadius * normals[i];
		}
	}
	NiColorA *colors = new NiColorA[count];
	for ( i = 0; i < count; i++ ) {
		colors[i] = NiColorA( color.r, color.g, color.b, 1.0f );
	}
	NiPoint2 *textures = new NiPoint2[count];
	data->SetData( count, data->GetVertices(), normals, colors, textures );
	return shape;
}

/*
================
Nebula::SetContainingSet

Gives the new set the nebula's fog.
================
*/
// FUNCTION: STBC 0x00598d50
void Nebula::SetContainingSet( SetClass *set ) {
	ObjectClass::SetContainingSet( set );
	if ( containingSet != NULL ) {
		AttachToSet();
	}
}

/*
================
Nebula::Update
================
*/
// FUNCTION: STBC 0x00598d80
void Nebula::Update( float time ) {
	ObjectClass::Update( time );
	UpdateObjectsInside();
	DamageObjectsInside();
}

/*
================
Nebula::UpdateObjectsInside

Finds the set's damageable objects that are inside now, and tells the ones
that came in and the ones that went out.
================
*/
// FUNCTION: STBC 0x00598da0
void Nebula::UpdateObjectsInside( void ) {
	NiTList<long> previous( 2 );
	while ( !objectIDs.IsEmpty() ) {
		previous.AddTail( objectIDs.RemoveHead() );
	}

	if ( containingSet != NULL ) {
		int count = 0;
		DamageableObject **objects = DamageableObject::GetClassObjects( containingSet, &count );
		if ( count != 0 ) {
			bool *inside = new bool[count];
			memset( inside, 0, count );
			int i;
			for ( i = 0; i < count; i++ ) {
				inside[i] = IsObjectInside( objects[i] );
			}
			for ( i = 0; i < count; i++ ) {
				long id = objects[i]->GetObjID();
				if ( inside[i] ) {
					if ( !previous.FindPos( id ) ) {
						TGEvent *event = new TGEvent;
						event->SetEventType( ET_ENTERED_NEBULA );
						event->SetSource( this );
						event->SetDestination( objects[i] );
						g_kEventManager.AddEvent( event );
					}
					objectIDs.AddTail( id );
				}
			}
			delete [] inside;
		}
	}

	while ( !previous.IsEmpty() ) {
		long id = previous.RemoveHead();
		if ( !objectIDs.FindPos( id ) ) {
			DamageableObject *object = DamageableObject::GetObjectByID( NULL, id );
			if ( object != NULL ) {
				TGEvent *event = new TGEvent;
				event->SetEventType( ET_EXITED_NEBULA );
				event->SetSource( this );
				event->SetDestination( object );
				g_kEventManager.AddEvent( event );
			}
		}
	}
}

/*
================
Nebula::DamageObjectsInside

Sends ET_ENVIRONMENT_DAMAGE to each object inside, when the nebula does
damage; in a multiplayer game only the host does.
================
*/
// FUNCTION: STBC 0x00599120
void Nebula::DamageObjectsInside( void ) {
	if ( damage == 0.0f && hullDamage == 0.0f ) {
		return;
	}
	if ( g_kUtopiaModule.IsMultiplayer() && !g_kUtopiaModule.IsHost() ) {
		return;
	}
	NiTListIterator pos = objectIDs.GetHeadPos();
	while ( pos ) {
		long id = objectIDs.GetNext( pos );
		DamageableObject *object = DamageableObject::GetObjectByID( GetContainingSet(), id );
		if ( object != NULL ) {
			TGEvent *event = new TGEvent;
			event->SetEventType( ET_ENVIRONMENT_DAMAGE );
			event->SetSource( this );
			event->SetDestination( object );
			g_kEventManager.AddEvent( event );
		}
	}
}

/*
================
Nebula::AttachToSet

Gives the set the fog, the sensors' scale and the overlay.
================
*/
// FUNCTION: STBC 0x005991f0
void Nebula::AttachToSet( void ) {
	containingSet->SetFogColor( &color );
	containingSet->SetFog( true );
	ScaleDistanceModifier *modifier = new ScaleDistanceModifier;
	modifier->SetFactor( sensorDensity );
	containingSet->AddDistanceModifier( modifier );
	containingSet->GetForegroundRoot()->AttachChild( overlayNode, false );
	overlayNode->UpdateProperties();
}

/*
================
Nebula::VirtualSlot68
================
*/
// FUNCTION: STBC 0x00599260
float Nebula::VirtualSlot68( NiCamera *camera ) {
	return visibility;
}

/*
================
Nebula::IsObjectInside

A plain nebula fills its set.
================
*/
// FUNCTION: STBC 0x00599270
bool Nebula::IsObjectInside( ObjectClass *object ) {
	if ( object->GetContainingSet() == containingSet ) {
		return true;
	}
	return false;
}

/*
================
Nebula::IsObjectInNebula

Whether the object's ID is among those inside the nebula.
================
*/
// FUNCTION: STBC 0x00599290
bool Nebula::IsObjectInNebula( ObjectClass *object ) {
	if ( object == NULL ) {
		return false;
	}
	return objectIDs.FindPos( object->GetObjID() ) ? true : false;
}

/*
================
TGFuzzyClusterGeom::TGFuzzyClusterGeom

A sphere drawn with the shape's data.
================
*/
// FUNCTION: STBC 0x005992e0
TGFuzzyClusterGeom::TGFuzzyClusterGeom( NiTriShape *shape ) :
	TGFuzzyTriShape( shape->GetShapeData() ),
	colors( NULL ),
	fadeDistance( -1.0f ),
	opaque( true ) {
}

/*
================
TGFuzzyClusterGeom::~TGFuzzyClusterGeom
================
*/
// FUNCTION: STBC 0x00599350
TGFuzzyClusterGeom::~TGFuzzyClusterGeom() {
	delete [] colors;
}

/*
================
TGFuzzyClusterGeom::AllocateColors
================
*/
// FUNCTION: STBC 0x00599390
void TGFuzzyClusterGeom::AllocateColors() {
	delete [] colors;
	colors = new NiColor[GetVertexCount()];
}

/*
================
TGFuzzyClusterGeom::VirtualSlot35

While the camera is near the sphere's surface, the vertices near the
camera fade with their distance from it; the saved colours are put back
over the data's.
================
*/
// FUNCTION: STBC 0x00599400
void TGFuzzyClusterGeom::VirtualSlot35( NiCamera *camera ) {
	if ( fuzziness == NULL ) {
		AllocateFuzziness();
	}
	float *value = fuzziness;
	NiBound &bound = GetWorldBound();
	float depth = (float)fabs( ( GetWorldTranslate() - camera->GetWorldTranslate() ).Length() - bound.GetRadius() );
	float fade;
	if ( fadeDistance > 0.0f ) {
		fade = fadeDistance;
	} else {
		fade = GetWorldBound().GetRadius() / 10.0f;
	}
	int count = GetVertexCount();
	int i;
	if ( depth < fade ) {
		float invScale = 1.0f / GetWorldScale();
		NiPoint3 eye = invScale * ( ( camera->GetWorldTranslate() - GetWorldTranslate() ) * GetWorldRotate() );
		NiPoint3 *vertices = GetShapeData()->GetVertices();
		fade = invScale * fade;
		for ( i = 0; i < count; i++ ) {
			float distance = ( eye - vertices[i] ).Length();
			if ( distance < fade ) {
				value[i] = distance / fade;
				opaque = false;
			} else {
				value[i] = 1.0f;
			}
		}
	} else if ( !opaque ) {
		for ( i = 0; i < count; i++ ) {
			value[i] = 1.0f;
		}
		opaque = true;
	}
	if ( colors != NULL ) {
		NiColorA *vertexColors = GetShapeData()->GetColors();
		for ( i = 0; i < count; i++ ) {
			vertexColors[i].r = colors[i].r;
			vertexColors[i].g = colors[i].g;
			vertexColors[i].b = colors[i].b;
		}
	}
	TGFuzzyTriShape::VirtualSlot35( camera );
}

/*
================
TGFuzzyClusterInnerGeom::VirtualSlot35

Sets each vertex's alpha from the nebula's spheres before the base's pass:
a vertex inside a sphere is opaque, one near its surface fades with the
distance; a shape that lies wholly inside a sphere is opaque everywhere.
The material's alpha scales the result.
================
*/
// FUNCTION: STBC 0x00599630
// DRAFT: inline budget: retail destroys NiNode::GetAt's smart pointer temporaries out of line (~NiPointer, 0x401EA0) and expands both NiDynamicCast walks; this build calls NiObject::IsKindOf (0x450530) for one
void TGFuzzyClusterInnerGeom::VirtualSlot35( NiCamera *camera ) {
	NiGeometryData *data = modelData;
	NiColorA *colors = data->GetColors();
	NiPoint3 *vertices = data->GetVertices();
	unsigned int count = data->GetVertexCount();
	unsigned int i;
	if ( clusters != NULL ) {
		for ( i = 0; i < count; i++ ) {
			colors[i].a = 0.0f;
		}
		float invScale = 1.0f / GetWorldScale();
		float radius = GetWorldBound().GetRadius();
		bool inside = false;
		bool outside = true;
		for ( unsigned int j = 0; !inside && j < clusters->GetArrayCount(); j++ ) {
			TGFuzzyClusterGeom *sphere = NiDynamicCast( TGFuzzyClusterGeom, clusters->GetAt( j ) );
			if ( sphere != NULL ) {
				float sphereRadius = sphere->GetWorldBound().GetRadius();
				float distance = ( sphere->GetWorldTranslate() - GetWorldTranslate() ).Length();
				if ( distance < radius + radius + sphereRadius ) {
					outside = false;
					if ( distance >= sphereRadius - ( radius + radius ) ) {
						NiPoint3 center = TGModelUtils::WorldToLocalPoint( this, sphere->GetWorldTranslate() );
						float fade = radius * invScale;
						float solid = sphereRadius * invScale;
						float outer = solid + fade;
						for ( i = 0; i < count; i++ ) {
							if ( colors[i].a < 1.0f ) {
								float vertexDistance = ( center - vertices[i] ).Length();
								if ( vertexDistance < solid ) {
									colors[i].a = 1.0f;
								} else if ( vertexDistance < outer ) {
									colors[i].a += ( outer - vertexDistance ) / fade;
								}
							}
						}
					} else {
						for ( i = 0; i < count; i++ ) {
							colors[i].a = 1.0f;
						}
						inside = true;
					}
				}
			}
		}
		if ( !inside && !outside ) {
			for ( i = 0; i < count; i++ ) {
				if ( colors[i].a > 1.0f ) {
					colors[i].a = 1.0f;
				}
			}
		}
		modelData->IncRevisionID();
	}

	NiMaterialProperty *material = NiDynamicCast( NiMaterialProperty, GetProperty( NiMaterialProperty::MATERIAL ) );
	if ( material != NULL ) {
		float alpha = material->GetAlpha();
		for ( i = 0; i < count; i++ ) {
			colors[i].a = alpha * colors[i].a;
		}
		modelData->IncRevisionID();
	}
	NiTriShape::VirtualSlot35( camera );
}

/*
================
TGFuzzyClusterInnerController::Reset

Picks a new spin and puts the target at the position.
================
*/
// FUNCTION: STBC 0x00599ad0
void TGFuzzyClusterInnerController::Reset( const NiPoint3 &position ) {
	NiAVObject *object = (NiAVObject *)target;
	spinRate = ( rand() * ( 1.0f / RAND_MAX ) + MIN_SPIN ) * 0.0525f;
	spinAxis = NiPoint3( NiSymmetricRandom(), NiSymmetricRandom(), NiSymmetricRandom() );
	spinAxis.Unitize();
	object->SetTranslate( position );
	lastPosition = position;
}

/*
================
TGFuzzyClusterInnerController::Update

Keeps the target still in the world while its parent moves, and turns it
about its spin axis. A target that drifts past the outer radius comes back
at the outer radius on the far side of the motion with a new spin (or at
the center, fading in again, when the parent stopped). The alpha fades out
between the inner and the outer radius, and in over the fade time.
================
*/
// FUNCTION: STBC 0x00599bf0
// DRAFT: the call set agrees; retail copies outerRadius into a frame temporary before scaling the moved direction, this build multiplies by the static three times
void TGFuzzyClusterInnerController::Update( float time ) {
	NiAVObject *object = (NiAVObject *)target;
	NiNode *parent = object ? object->GetParent() : NULL;
	if ( object == NULL || parent == NULL ) {
		return;
	}

	NiPoint3 position = parent->GetWorldTranslate();
	NiPoint3 moved = position - lastPosition;
	object->SetTranslate( object->GetTranslate() - moved );
	float distance = object->GetTranslate().Length();
	if ( distance > outerRadius ) {
		spinRate = ( rand() * ( 1.0f / RAND_MAX ) + MIN_SPIN ) * 0.0525f;
		spinAxis = NiPoint3( NiSymmetricRandom(), NiSymmetricRandom(), NiSymmetricRandom() );
		spinAxis.Unitize();
		float length = moved.Length();
		if ( length > 0.01f ) {
			object->SetTranslate( moved / length * outerRadius );
			distance = outerRadius;
		} else {
			object->SetTranslate( NiPoint3::ZERO );
			distance = 0.0f;
			fadeStart = time;
		}
	}

	if ( fadeStart > 0.0f ) {
		float fade = ( time - fadeStart ) / fadeTime;
		if ( fade > 1.0f ) {
			fade = 1.0f;
			fadeStart = -NI_INFINITY;
		}
		if ( distance < innerRadius ) {
			material->SetAlpha( maxAlpha * fade );
		} else {
			material->SetAlpha( ( 1.0f - ( distance - innerRadius ) / ( outerRadius - innerRadius ) ) * maxAlpha * fade );
		}
	} else if ( distance < innerRadius ) {
		material->SetAlpha( maxAlpha );
	} else if ( distance < outerRadius ) {
		material->SetAlpha( ( 1.0f - ( distance - innerRadius ) / ( outerRadius - innerRadius ) ) * maxAlpha );
	} else {
		material->SetAlpha( 0.0f );
	}

	NiMatrix3 spin;
	spin.MakeRotation( time * spinRate, spinAxis );
	object->SetRotate( spin );
	lastPosition = position;
}

/*
================
Nebula::WriteToStream
================
*/
// FUNCTION: STBC 0x00599fb0
bool Nebula::WriteToStream( TGStream *stream ) {
	if ( !ObjectClass::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( visibility );
	stream->WriteFloat( sensorDensity );
	stream->VirtualSlot17( (long)(NiNode *)overlayNode );
	stream->WriteFloat( color.r );
	stream->WriteFloat( color.g );
	stream->WriteFloat( color.b );
	stream->VirtualSlot11( textureName );
	stream->WriteLong( objectIDs.GetSize() );
	NiTListIterator pos = objectIDs.GetHeadPos();
	while ( pos ) {
		stream->WriteID( objectIDs.GetNext( pos ) );
	}
	stream->WriteFloat( damage );
	stream->WriteFloat( hullDamage );
	return true;
}

/*
================
Nebula::ReadFromStream

The overlay is built again; the saved address of the old one maps to it.
================
*/
// FUNCTION: STBC 0x0059a090
bool Nebula::ReadFromStream( TGStream *stream ) {
	if ( !ObjectClass::ReadFromStream( stream ) ) {
		return false;
	}
	visibility = stream->ReadFloat();
	sensorDensity = stream->ReadFloat();
	long savedOverlayNode = stream->VirtualSlot16();
	color.r = stream->ReadFloat();
	color.g = stream->ReadFloat();
	color.b = stream->ReadFloat();
	textureName = stream->VirtualSlot10( -1 );
	int count = stream->ReadLong();
	for ( int i = 0; i < count; i++ ) {
		objectIDs.AddTail( stream->ReadID() );
	}
	damage = stream->ReadFloat();
	hullDamage = stream->ReadFloat();
	CreateOverlay();
	pointerMap.Add( savedOverlayNode, overlayNode );
	return true;
}
