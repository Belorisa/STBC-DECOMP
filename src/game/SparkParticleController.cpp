/*
===============================================================================

	SparkParticleController.cpp

===============================================================================
*/

#include "SparkParticleController.h"
#include "NiNode.h"
#include "TriShapeParticles.h"
#include "NiAlphaProperty.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"
#include "NiMain.h"

// The NiRTTI's dynamic initialiser, the registration of its exit-time destructor and the guarded destructor.
// FUNCTION: STBC 0x004d2380 _$E413
// FUNCTION: STBC 0x004d2390 _$E409
// FUNCTION: STBC 0x004d23b0 _$E412
// FUNCTION: STBC 0x004d23c0 _$E410
// DATA: STBC 0x00985450 ?rtti@SparkParticleController@@2VNiRTTI@@B
const NiRTTI SparkParticleController::rtti( "SparkParticleController", &PointParticleController::rtti );

/*
================
SparkParticleController::FindController
================
*/
// FUNCTION: STBC 0x004d23e0
SparkParticleController *SparkParticleController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		SparkParticleController *found = NiDynamicCast( SparkParticleController, controller );
		if ( found ) {
			return found;
		}
	}
	return 0;
}

/*
================
SparkParticleController::Create
================
*/
// FUNCTION: STBC 0x004d2430
SparkParticleController *SparkParticleController::Create( float lifeTime, float emitLife, float emitFrequency, float emitVelocity, float angleVariance, NiAVObject *emitFrom ) {
	return new SparkParticleController( lifeTime, emitLife, emitFrequency, emitVelocity, angleVariance, emitFrom );
}

/*
================
SparkParticleController::SparkParticleController
================
*/
// FUNCTION: STBC 0x004d24b0
SparkParticleController::SparkParticleController( float lifeTime_, float emitLife_, float emitFrequency_, float emitVelocity_, float angleVariance_, NiAVObject *emitFrom_ ) : PointParticleController( lifeTime_, emitLife_, emitFrequency_, emitVelocity_, angleVariance_, emitFrom_ ), tails( NULL ) {
	tailLength = 1.0f;
	tailFade = 0.2f;
}

/*
================
SparkParticleController::CreateClone
================
*/
// FUNCTION: STBC 0x004d25b0
NiObject *SparkParticleController::CreateClone() {
	SparkParticleController *clone = new SparkParticleController( 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, NULL );
	CopyMembers( clone );
	return clone;
}

/*
================
SparkParticleController::CopyMembers

The tails are not copied.
================
*/
// FUNCTION: STBC 0x004d2630
void SparkParticleController::CopyMembers( SparkParticleController *dest ) {
	EmitterController::CopyMembers( dest );
	dest->tailLength = tailLength;
	dest->tailFade = tailFade;
}

/*
================
SparkParticleController::CreateTarget

Builds the point particles, then the tails: a line per pair of vertices,
from a white head, blended over what is behind them.
================
*/
// FUNCTION: STBC 0x004d2660
void SparkParticleController::CreateTarget( char *textureName ) {
	PointParticleController::CreateTarget( textureName );
	if ( target ) {
		int count = ( (TriShapeParticles *)target )->GetShapeData()->GetVertexCount() * 2;
		NiPoint3 *vertices = new NiPoint3[count];
		NiColorA *tailColors = new NiColorA[count];
		bool *flags = new bool[count];
		for ( int i = 0; i < count; i++ ) {
			vertices[i] = NiPoint3::ZERO;
			tailColors[i].r = 1.0f;
			tailColors[i].g = 0.0f;
			tailColors[i].b = 0.0f;
			tailColors[i].a = 0.0f;
			// each head joins its tail, and not the next head
			if ( i & 1 ) {
				flags[i] = false;
			} else {
				flags[i] = true;
			}
		}
		tails = new NiLines( count, vertices, tailColors, NULL, flags );

		NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
		vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
		vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
		tails->AttachProperty( vertexColor );

		NiAlphaProperty *alpha = new NiAlphaProperty;
		alpha->SetAlphaBlending( true );
		alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
		alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_INVSRCALPHA );
		tails->AttachProperty( alpha );

		NiZBufferProperty *zBuffer = new NiZBufferProperty;
		zBuffer->SetZBufferTest( true );
		zBuffer->SetZBufferWrite( false );
		tails->AttachProperty( zBuffer );

		tails->UpdateProperties();
		tails->Update( 0.0f );
	}
}

/*
================
SparkParticleController::VirtualSlot35

The tail runs from the particle back along the direction it was moving in
before the step, its far end faded.
================
*/
// FUNCTION: STBC 0x004d2980
void SparkParticleController::VirtualSlot35( int index, float time, float deltaTime, float lifeFraction ) {
	NiPoint3 direction = velocities[index];
	EmitterController::VirtualSlot35( index, time, deltaTime, lifeFraction );
	if ( tails ) {
		NiColorA *tailColors = tails->GetLinesData()->GetColors();
		tailColors[index * 2] = colors[index];
		tailColors[index * 2 + 1] = colors[index];
		tailColors[index * 2 + 1].a *= tailFade;
		NiPoint3 *vertices = tails->GetLinesData()->GetVertices();
		vertices[index * 2] = positions[index];
		direction.Unitize();
		vertices[index * 2 + 1] = positions[index] - direction * tailLength;
		tails->GetLinesData()->IncRevisionID();
		tails->GetLinesData()->GetBound().unknown10 = true;
	}
}

/*
================
SparkParticleController::VirtualSlot37

The last tail moves into the removed particle's place, and the last pair of
vertices is hidden.
================
*/
// FUNCTION: STBC 0x004d2b70
void SparkParticleController::VirtualSlot37( int index ) {
	if ( tails && index != numActive - 1 ) {
		NiLinesData *data = tails->GetLinesData();
		NiPoint3 *vertices = data->GetVertices();
		NiColorA *tailColors = data->GetColors();
		vertices[index * 2] = vertices[( numActive - 1 ) * 2];
		vertices[index * 2 + 1] = vertices[( numActive - 1 ) * 2 + 1];
		tailColors[index * 2] = tailColors[( numActive - 1 ) * 2];
		tailColors[index * 2 + 1] = tailColors[( numActive - 1 ) * 2 + 1];
		tailColors[( numActive - 1 ) * 2].a = 0.0f;
		tailColors[( numActive - 1 ) * 2 + 1].a = 0.0f;
		tails->GetLinesData()->IncRevisionID();
		tails->GetLinesData()->GetBound().unknown10 = true;
	}
	PointParticleController::VirtualSlot37( index );
}

/*
================
SparkParticleController::DetachEffect
================
*/
// FUNCTION: STBC 0x004d2c90
void SparkParticleController::DetachEffect() {
	if ( tails && tails->GetParent() ) {
		tails->GetParent()->DetachChild( tails );
	}
	tails = NULL;
	EmitterController::DetachEffect();
}

/*
================
SparkParticleController::AttachEffect
================
*/
// FUNCTION: STBC 0x004d2d10
void SparkParticleController::AttachEffect( NiNode *parent ) {
	EffectController::AttachEffect( parent );
	if ( tails ) {
		parent->AttachChild( tails, false );
	}
}
