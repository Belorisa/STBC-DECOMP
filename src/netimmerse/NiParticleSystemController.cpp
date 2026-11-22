// NiParticleSystemController.cpp -- particle emission and motion

#include "NiRTLib.h"
#include <math.h>
#include <stdlib.h>

#include "NiParticleSystemController.h"
#include "NiNode.h"
#include "NiParticles.h"
#include "NiParticlesData.h"
#include "NiMath.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007b33c0 _$E30
// FUNCTION: STBC 0x007b33d0 _$E29
// FUNCTION: STBC 0x007b33e0 _$E61
// FUNCTION: STBC 0x007b33f0 _$E60
// FUNCTION: STBC 0x007b3400 _$E65
// FUNCTION: STBC 0x007b3410 _$E64
// FUNCTION: STBC 0x007b3420 _$E69
// FUNCTION: STBC 0x007b3430 _$E68
// FUNCTION: STBC 0x007b3440 _$E85
// FUNCTION: STBC 0x007b3450 _$E84
// FUNCTION: STBC 0x007b3460 _$E198
// FUNCTION: STBC 0x007b3470 _$E197
// FUNCTION: STBC 0x007b3800 _$E215
// FUNCTION: STBC 0x007b3810 _$E214
// FUNCTION: STBC 0x007b3940 _$E222
// FUNCTION: STBC 0x007b3950 _$E221
// FUNCTION: STBC 0x007b3960 _$E226
// FUNCTION: STBC 0x007b3970 _$E225
// FUNCTION: STBC 0x007b3980 _$E230
// FUNCTION: STBC 0x007b3990 _$E229
// FUNCTION: STBC 0x007b39a0 _$E234
// FUNCTION: STBC 0x007b39b0 _$E233
// FUNCTION: STBC 0x007b39c0 _$E238
// FUNCTION: STBC 0x007b39d0 _$E237
// FUNCTION: STBC 0x007b3a60 _$E203
// FUNCTION: STBC 0x007b3a70 _$E202
// FUNCTION: STBC 0x007b3a80 _$E207
// FUNCTION: STBC 0x007b3a90 _$E206
// FUNCTION: STBC 0x007b3aa0 _$E49
// FUNCTION: STBC 0x007b3ab0 _$E48
// FUNCTION: STBC 0x007b3ac0 _$E53
// FUNCTION: STBC 0x007b3ad0 _$E52
// FUNCTION: STBC 0x007b3b80 _$E81
// FUNCTION: STBC 0x007b3b90 _$E80
// FUNCTION: STBC 0x007b3bd0 _$E57
// FUNCTION: STBC 0x007b3be0 _$E56

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007b3820 _$E258
// FUNCTION: STBC 0x007b3840 _$E257
// FUNCTION: STBC 0x007b3240 _$E17
// FUNCTION: STBC 0x007b3250 _$E14
// FUNCTION: STBC 0x007b3260 _$E16
// FUNCTION: STBC 0x007b3270 _$E15
// FUNCTION: STBC 0x007b3280 _$E23
// FUNCTION: STBC 0x007b3290 _$E20
// FUNCTION: STBC 0x007b32a0 _$E22
// FUNCTION: STBC 0x007b32b0 _$E21

// FUNCTION: STBC 0x007ae970 _$E244
// FUNCTION: STBC 0x007ae980 _$E240
// FUNCTION: STBC 0x007ae9a0 _$E243
// FUNCTION: STBC 0x007ae9b0 _$E241
// DATA: STBC 0x9A0770 ?rtti@NiParticleSystemController@@2VNiRTTI@@B
const NiRTTI NiParticleSystemController::rtti( "NiParticleSystemController", &NiTimeController::rtti );

// DATA: STBC 0x89878C ?epsilon@NiParticleSystemController@@1MB
const float NiParticleSystemController::epsilon = 0.0001f;

/*
================
NiParticleSystemController::Register
================
*/
// FUNCTION: STBC 0x7AE9D0 ?Register@NiParticleSystemController@@SAHXZ
NiImplementRegister( NiParticleSystemController )

/*
================
NiParticleSystemController::FindController
================
*/
// FUNCTION: STBC 0x7AEB20
NiParticleSystemController *NiParticleSystemController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		NiParticleSystemController *particleController = NiDynamicCast( NiParticleSystemController, controller );
		if ( particleController ) {
			return particleController;
		}
	}
	return 0;
}

/*
================
NiParticleSystemController::NiParticleSystemController

No particles and no arrays yet; the particles are emitted about -z.
================
*/
// FUNCTION: STBC 0x7AEB70
NiParticleSystemController::NiParticleSystemController( NiColorData *colorData_ ) :
	initialNormal( -NiPoint3::UNIT_Z ),
	initialNormalU( NiPoint3::UNIT_X ),
	initialNormalV( NiPoint3::UNIT_Y ),
	unknown10C( NiPoint3::UNIT_X ),
	colorData( colorData_ ) {
	speed = 1.0f;
	speedRandom = 0.0f;
	unknown48 = 1.0f;
	unknown44 = 1.0f;
	unknown4C = 0.0f;
	unknown3C = 0.0f;
	unknown50 = 0.0f;
	unknown94 = 0.0f;
	unknown54 = 0;
	unknown40 = 1.0f;
	unknown30 = 1.0f;
	unknown58 = 0;
	lastTime = -1.0f;
	velocities = 0;
	forces = 0;
	ages = 0;
	unknown9C = 0.0f;
	SetUnknown10( false );
	SetUnknown20( true );
	colliders = 0;
	SetUnknownC0( 1 );
	numGenerations = 0;
	SetUnknown700( 0 );
	generations = 0;
	maxNumParticles = 0;
	numParticles = 0;
	positions = 0;
	particleNodes = 0;
	numParticleObjects = 0;
	particleObject = 0;
	particleObjects = 0;
	unknown104 = 0;
	unknown108 = 0.0f;
	unknownFC = 0.0f;
	unknown100 = 0.0f;
	rotations = 0;
	lifeSpans = 0;
	unknown118 = 0.0f;
	unknown11C = -1.0f;
	savedParticles = 0;
	numSavedParticles = 0;
	unknown120 = 0.0f;
	unknown124 = 0.0f;
	unknown128 = 0.0f;
	unknown12C = 0.0f;
	lastColorIndex = 0;
	growTime = 0.0f;
	fadeTime = 0.0f;
	srand( 13 );
	particles = 0;
	node = 0;
}

/*
================
NiParticleSystemController::~NiParticleSystemController
================
*/
// FUNCTION: STBC 0x7AED80
NiParticleSystemController::~NiParticleSystemController() {
	delete [] velocities;
	delete [] rotations;
	delete [] lifeSpans;
	delete [] ages;
	delete [] generations;
	delete [] positions;

	unsigned int i;
	if ( savedParticles ) {
		for ( i = 0; i < numSavedParticles; i++ ) {
			savedParticles[i].object = 0;
		}
		delete [] savedParticles;
	}

	if ( numParticleObjects ) {
		for ( i = 0; i < numParticleObjects; i++ ) {
			particleObjects[i] = 0;
		}
		delete [] particleObjects;
	}
	particleObject = 0;

	if ( particleNodes ) {
		for ( i = 0; i < numParticles; i++ ) {
			particleNodes[i] = 0;
		}
		delete [] particleNodes;
	}

	NiForce *force = forces;
	while ( force ) {
		NiForce *next = force->GetNext();
		delete force;
		force = next;
	}
	NiForce *collider = colliders;
	while ( collider ) {
		NiForce *next = collider->GetNext();
		delete collider;
		collider = next;
	}

	colorData = 0;
}

/*
================
NiParticleSystemController::SetInitialNormal

The first perpendicular drops the normal's smaller components; the second
is the cross product of the two.
================
*/
// FUNCTION: STBC 0x7AF0D0
void NiParticleSystemController::SetInitialNormal( const NiPoint3 &normal ) {
	initialNormal = normal;
	initialNormal.Unitize();

	if ( NiAbs( normal.x ) >= NiAbs( normal.y ) && NiAbs( normal.x ) >= NiAbs( normal.z ) ) {
		initialNormalU.x = normal.y;
		initialNormalU.y = -normal.x;
		initialNormalU.z = 0.0f;
	} else {
		initialNormalU.x = 0.0f;
		initialNormalU.y = -normal.z;
		initialNormalU.z = normal.y;
	}
	initialNormalU.Unitize();

	initialNormalV = initialNormal.Cross( initialNormalU );
}

/*
================
NiParticleSystemController::GetParticleObjectAt
================
*/
// FUNCTION: STBC 0x7AF270
NiAVObject *NiParticleSystemController::GetParticleObjectAt( unsigned int index ) {
	if ( index < numParticleObjects ) {
		return particleObjects[index];
	}
	return 0;
}

/*
================
NiParticleSystemController::SetParticleObjectAt
================
*/
// FUNCTION: STBC 0x7AF290
void NiParticleSystemController::SetParticleObjectAt( NiAVObject *object, unsigned int index ) {
	if ( index < numParticleObjects ) {
		if ( particleObjects[index] ) {
			particleObjects[index] = 0;
		}
		particleObjects[index] = object;
	}
}

/*
================
NiParticleSystemController::SetNumParticleObjects
================
*/
// FUNCTION: STBC 0x7AF310
void NiParticleSystemController::SetNumParticleObjects( unsigned int count ) {
	unsigned int i;
	if ( numParticleObjects ) {
		for ( i = 0; i < numParticleObjects; i++ ) {
			particleObjects[i] = 0;
		}
		delete [] particleObjects;
		particleObjects = 0;
	}

	numParticleObjects = count;
	if ( numParticleObjects > 0 ) {
		particleObjects = new NiPointer<NiAVObject>[numParticleObjects];
		for ( i = 0; i < numParticleObjects; i++ ) {
			particleObjects[i] = 0;
		}
	}
}

/*
================
NiParticleSystemController::VirtualSlot20
================
*/
// FUNCTION: STBC 0x7AF450
void NiParticleSystemController::VirtualSlot20() {
	particles = 0;
	node = 0;
	// AnimOnly.exe 0x49E523 (NiPSysController.cpp)
	if ( target ) {
		if ( NiIsKindOf( NiParticles, target ) ) {
			NiParticles *particles_ = (NiParticles *)target;
			SetMaxNumParticles( particles_->GetVertexCount() );
			particles_->SetActiveParticleCount( numParticles );
			particles_->GetModelBound().SetNeedUpdate( true );
			particles = particles_;
		} else if ( NiIsKindOf( NiNode, target ) ) {
			node = (NiNode *)target;
		} else {
			assert( false );
		}
	}
}

/*
================
NiParticleSystemController::VirtualSlot11
================
*/
// FUNCTION: STBC 0x7AF500
void NiParticleSystemController::VirtualSlot11() {
	VirtualSlot20();
}

/*
================
NiParticleSystemController::SetTarget
================
*/
// FUNCTION: STBC 0x7AF510
void NiParticleSystemController::SetTarget( NiObjectNET *target_ ) {
	NiTimeController::SetTarget( target_ );
	VirtualSlot20();
}

/*
================
NiParticleSystemController::SetMaxNumParticles
================
*/
// FUNCTION: STBC 0x7AF530
void NiParticleSystemController::SetMaxNumParticles( unsigned int count ) {
	delete [] velocities;
	delete [] rotations;
	delete [] lifeSpans;
	delete [] ages;
	delete [] generations;
	delete [] positions;

	unsigned int i;
	if ( particleNodes ) {
		for ( i = 0; i < numParticles; i++ ) {
			particleNodes[i] = 0;
		}
		delete [] particleNodes;
		particleNodes = 0;
	}
	numParticles = 0;

	maxNumParticles = count;
	velocities = new NiPoint3[count];
	rotations = new Rotation[count];
	lifeSpans = new float[count];
	ages = new float[count];
	generations = new int[count];
	positions = new NiPoint3[count];
	particleNodes = new NiPointer<NiNode>[count];
	for ( i = 0; i < count; i++ ) {
		particleNodes[i] = 0;
	}

	if ( savedParticles ) {
		for ( i = 0; i < numSavedParticles; i++ ) {
			savedParticles[i].object = 0;
		}
		delete [] savedParticles;
		savedParticles = 0;
	}
	numSavedParticles = 0;
}

/*
================
NiParticleSystemController::SetParticleObject

Drops the particles' nodes along with the old object.
================
*/
// FUNCTION: STBC 0x7AF7D0
void NiParticleSystemController::SetParticleObject( NiAVObject *object ) {
	particleObject = 0;
	if ( particleNodes ) {
		for ( unsigned int i = 0; i < numParticles; i++ ) {
			particleNodes[i] = 0;
		}
	}
	particleObject = object;
	numParticles = 0;
}

/*
================
NiParticleSystemController::AddSavedParticle
================
*/
// FUNCTION: STBC 0x7AF870
void NiParticleSystemController::AddSavedParticle( const Particle &particle ) {
	if ( !savedParticles ) {
		savedParticles = new Particle[maxNumParticles];
		numSavedParticles = 0;
	}
	savedParticles[numSavedParticles++] = particle;
}

/*
================
NiParticleSystemController::AddParticle

Emits a saved particle: its direction scaled by a speed drawn about the
emission speed.
================
*/
// FUNCTION: STBC 0x7AF9C0
void NiParticleSystemController::AddParticle( const Particle &particle ) {
	if ( numParticles >= maxNumParticles ) {
		return;
	}

	float speedDelta = rand() * speedRandom * ( 1.0f / RAND_MAX ) - speedRandom * 0.5f;
	NiPoint3 *vertices = particles ? particles->modelData->vertices : positions;
	vertices[numParticles] = particle.position;
	velocities[numParticles] = ( speedDelta + speed ) * particle.direction;
	rotations[numParticles] = particle.rotation;
	lifeSpans[numParticles] = particle.lifeSpan;
	ages[numParticles] = 0.0f;
	generations[numParticles] = 0;

	if ( particleObject || particle.object ) {
		NiNode *particleNode = new NiNode;
		if ( particle.object ) {
			particleNode->AttachChild( particle.object, true );
		} else {
			particleNode->AttachChild( (NiAVObject *)particleObject->Clone(), true );
		}
		particleNode->SetTranslate( vertices[numParticles] );
		particleNodes[numParticles] = particleNode;
		node->AttachChild( particleNode, true );
		particleNode->UpdateProperties();
		particleNode->UpdateEffects();
	}

	numParticles++;
	if ( particles ) {
		NiParticlesData *data = (NiParticlesData *)(NiGeometryData *)particles->modelData;
		if ( (unsigned short)numParticles <= data->vertexCount ) {
			data->activeVertexCount = (unsigned short)numParticles;
		} else {
			data->activeVertexCount = data->vertexCount;
		}
	}
	if ( particles ) {
		particles->modelData->bound.unknown10 = true;
	}
}

/*
================
NiParticleSystemController::AddForce
================
*/
// FUNCTION: STBC 0x7AFBF0
void NiParticleSystemController::AddForce( NiForce *force ) {
	force->SetNext( forces );
	forces = force;
}

/*
================
NiParticleSystemController::AddCollider
================
*/
// FUNCTION: STBC 0x7AFC10
void NiParticleSystemController::AddCollider( NiForce *collider ) {
	collider->SetNext( colliders );
	colliders = collider;
}

/*
================
NiParticleSystemController::RemoveParticle
================
*/
// FUNCTION: STBC 0x7AFC30
void NiParticleSystemController::RemoveParticle( unsigned int index ) {
	NiPoint3 *vertices = particles ? particles->modelData->vertices : positions;
	unsigned int last = numParticles - 1;
	ages[index] = ages[last];
	generations[index] = generations[last];
	vertices[index] = vertices[last];
	if ( particleNodes[index] ) {
		node->DetachChild( particleNodes[index] );
		particleNodes[index] = 0;
		particleNodes[index] = particleNodes[last];
		particleNodes[last] = 0;
	}
	velocities[index] = velocities[last];
	rotations[index] = rotations[last];
	lifeSpans[index] = lifeSpans[last];
	numParticles = last;
}

/*
================
NiParticleSystemController::SpawnParticle

A particle of the next generation, born where its parent is.
================
*/
// FUNCTION: STBC 0x7AFE10
void NiParticleSystemController::SpawnParticle( const Particle &parent ) {
	if ( parent.generation >= numGenerations ) {
		return;
	}

	NiPoint3 *vertices;
	NiColorA *colors;
	float *sizes;
	if ( particles ) {
		NiParticlesData *data = (NiParticlesData *)(NiGeometryData *)particles->modelData;
		colors = data->colors;
		sizes = data->sizes;
		vertices = data->vertices;
	} else {
		vertices = positions;
		colors = 0;
		sizes = 0;
	}

	ages[numParticles] = parent.time - unknown3C;
	generations[numParticles] = parent.generation + 1;
	vertices[numParticles] = parent.position;
	velocities[numParticles] = parent.direction;
	rotations[numParticles] = parent.rotation;
	lifeSpans[numParticles] = parent.lifeSpan;

	if ( particleObject ) {
		NiNode *particleNode = new NiNode;
		if ( numParticleObjects ) {
			unsigned int index = generations[numParticles] - 1;
			if ( index >= numParticleObjects ) {
				index = numParticleObjects - 1;
			}
			particleNode->AttachChild( (NiAVObject *)particleObjects[index]->Clone(), true );
		} else {
			particleNode->AttachChild( (NiAVObject *)particleObject->Clone(), true );
		}
		particleNode->SetTranslate( vertices[numParticles] );
		NiMatrix3 rotate;
		rotate.MakeRotation( rotations[numParticles].angle, rotations[numParticles].axis.x, rotations[numParticles].axis.y,
			rotations[numParticles].axis.z );
		particleNode->SetRotate( rotate );
		particleNodes[numParticles] = particleNode;
		node->AttachChild( particleNode, true );
		particleNode->UpdateProperties();
		particleNode->UpdateEffects();
	}

	if ( colors && GetNumKeys() ) {
		colors[numParticles] = GetColor( ages[numParticles] );
	}

	if ( growTime != 0.0f || fadeTime != 0.0f ) {
		float growFade = GetGrowFade( ages[numParticles], generations[numParticles] );
		if ( sizes ) {
			sizes[numParticles] = growFade;
		} else if ( particleNodes[numParticles] ) {
			if ( growFade > 0.0f ) {
				particleNodes[numParticles]->SetScale( growFade );
			} else {
				particleNodes[numParticles]->SetScale( -growFade );
			}
		}
	}

	numParticles++;
	if ( particles ) {
		NiParticlesData *data = (NiParticlesData *)(NiGeometryData *)particles->modelData;
		if ( (unsigned short)numParticles <= data->vertexCount ) {
			data->activeVertexCount = (unsigned short)numParticles;
		} else {
			data->activeVertexCount = data->vertexCount;
		}
	}
}

/*
================
NiParticleSystemController::GetGrowFade
================
*/
// FUNCTION: STBC 0x7B0140
float NiParticleSystemController::GetGrowFade( float age, int generation ) {
	if ( generation == 0 && growTime != 0.0f && age <= growTime ) {
		if ( age < 0.0f ) {
			age = 0.0f;
		}
		return age / growTime;
	}

	if ( generation >= numGenerations && fadeTime != 0.0f ) {
		float fadeStart = unknown3C - fadeTime;
		if ( age > fadeStart ) {
			if ( age > unknown3C ) {
				age = unknown3C;
			}
			return 1.0f - ( age - fadeStart ) / fadeTime;
		}
	}
	return 1.0f;
}

/*
================
NiParticleSystemController::GetColor
================
*/
// FUNCTION: STBC 0x7B0200
NiColorA NiParticleSystemController::GetColor( float age ) {
	unsigned int numKeys;
	NiColorKey::KeyType keyType;
	NiColorKey **keys = GetAnim( numKeys, keyType );
	NiColorA keyColor = NiColorKey::GenInterp( age / unknown3C, keys, keyType, numKeys, lastColorIndex );

	if ( keyColor.r < 0.0f ) {
		keyColor.r = 0.0f;
	} else if ( keyColor.r > 1.0f ) {
		keyColor.r = 1.0f;
	}
	if ( keyColor.g < 0.0f ) {
		keyColor.g = 0.0f;
	} else if ( keyColor.g > 1.0f ) {
		keyColor.g = 1.0f;
	}
	if ( keyColor.b < 0.0f ) {
		keyColor.b = 0.0f;
	} else if ( keyColor.b > 1.0f ) {
		keyColor.b = 1.0f;
	}
	if ( keyColor.a < 0.0f ) {
		keyColor.a = 0.0f;
	} else if ( keyColor.a > 1.0f ) {
		keyColor.a = 1.0f;
	}
	return keyColor;
}

/*
================
NiParticleSystemController::Unknown7B0330

One step of the system up to time. When the system moves with its target,
the forces' frames and the particles follow the target's motion since the
last step. Particles past the system's lifetime are retired (and may
spawn), the others are rotated, collided, moved by the forces and
recolored and resized. Then the particles born during the step are
emitted, each one aged by the part of the step it has lived.
================
*/
// FUNCTION: STBC 0x7B0330
// PARKED: call set and control flow as retail (with the single-statement bodies unbraced the dying particle's release calls DecRefCount out of line); the rest is frame layout (0x1C8 against 0x1C0) and x87 scheduling
void NiParticleSystemController::Unknown7B0330( float time ) {
	float scale;
	NiPoint3 translate;
	NiMatrix3 rotate;
	if ( GetBit( UNKNOWN_20_POS ) ) {
		if ( particles )
			GetWorldTransform( particles, scale, translate, rotate );
		else
			GetWorldTransform( node, scale, translate, rotate );
	}

	// the target's motion since the last step, in its old frame
	NiMatrix3 deltaRotate;
	NiPoint3 deltaTranslate;
	float deltaScale;
	if ( GetBit( UNKNOWN_20_POS ) ) {
		deltaRotate = rotate.TransposeTimes( worldRotate );
		deltaTranslate = ( worldTranslate - translate ) * rotate;
		if ( scale < 0.0001f ) {
			deltaScale = 0.0f;
			deltaTranslate.x = 0.0f;
			deltaTranslate.y = 0.0f;
			deltaTranslate.z = 0.0f;
		} else {
			deltaScale = worldScale / scale;
			deltaTranslate = ( 1.0f / scale ) * deltaTranslate;
		}
		worldRotate = rotate;
		worldTranslate = translate;
		worldScale = scale;
	}

	NiPoint3 *vertices;
	NiColorA *colors;
	float *sizes;
	if ( particles ) {
		NiParticlesData *data = (NiParticlesData *)(NiGeometryData *)particles->modelData;
		colors = data->colors;
		sizes = data->sizes;
		vertices = data->vertices;
	} else {
		vertices = positions;
		colors = 0;
		sizes = 0;
	}

	if ( GetBit( UNKNOWN_20_POS ) ) {
		NiForce *collider;
		for ( collider = colliders; collider; collider = collider->GetNext() )
			collider->UpdateWorld( deltaRotate, deltaTranslate, deltaScale );
		NiForce *force;
		for ( force = forces; force; force = force->GetNext() )
			force->UpdateWorld( deltaRotate, deltaTranslate, deltaScale );
	}

	float deltaTime = time - lastTime;

	// retire the particles that outlive the system's lifetime
	unsigned int i;
	for ( i = 0; i < numParticles; i++ ) {
		if ( deltaTime + ages[i] > unknown3C ) {
			Particle particle;
			particle.position = vertices[i];
			particle.direction = velocities[i];
			particle.rotation = rotations[i];
			particle.lifeSpan = lifeSpans[i];
			particle.time = ages[i];
			particle.generation = generations[i];
			RemoveParticle( i );
			i--;
			if ( GetField( UNKNOWN_700_MASK, UNKNOWN_700_POS ) == 3 )
				SpawnParticle( particle );
		}
	}

	// move the others
	for ( i = 0; i < numParticles; i++ ) {
		if ( GetBit( UNKNOWN_20_POS ) ) {
			vertices[i] = deltaTranslate + deltaRotate * deltaScale * vertices[i];
			velocities[i] = deltaRotate * deltaScale * velocities[i];
		}

		// the collider hit last along the step wins
		bool hit = false;
		float hitTime = -1.0f;
		NiPoint3 hitPosition;
		NiPoint3 hitVelocity;
		float hitStep;
		NiForce *collider;
		for ( collider = colliders; collider; collider = collider->GetNext() ) {
			NiPoint3 position = vertices[i];
			float stepTime = deltaTime;
			NiPoint3 velocity = velocities[i];
			if ( collider->Apply( position, velocity, stepTime, time ) ) {
				hit = true;
				if ( stepTime > hitTime ) {
					hitPosition = position;
					hitVelocity = velocity;
					hitStep = stepTime;
					hitTime = stepTime;
				}
			}
		}
		if ( hit ) {
			vertices[i] = hitPosition;
			velocities[i] = hitVelocity;
			vertices[i] += hitStep * velocities[i];
		}

		if ( particleNodes[i] ) {
			NiMatrix3 particleRotate;
			particleRotate.MakeRotation( rotations[i].angle, rotations[i].axis.x, rotations[i].axis.y, rotations[i].axis.z );
			particleNodes[i]->SetRotate( particleRotate );
		}
		rotations[i].angle += deltaTime * lifeSpans[i];
		if ( rotations[i].angle > NI_TWO_PI )
			rotations[i].angle -= NI_TWO_PI;

		// a collision that kills the particle, or splits it
		unsigned int onCollide = GetField( UNKNOWN_700_MASK, UNKNOWN_700_POS );
		if ( !hit )
			vertices[i] += deltaTime * velocities[i];
		else if ( onCollide == 1 || onCollide == 2 ) {
			Particle particle;
			particle.position = vertices[i];
			particle.direction = velocities[i];
			particle.rotation = rotations[i];
			particle.lifeSpan = lifeSpans[i];
			particle.time = ages[i];
			particle.generation = generations[i];
			RemoveParticle( i );
			i--;
			if ( onCollide == 2 )
				SpawnParticle( particle );
			continue;
		}

		NiForce *force;
		for ( force = forces; force; force = force->GetNext() )
			force->Apply( vertices[i], velocities[i], deltaTime, time );
		if ( particleNodes[i] )
			particleNodes[i]->SetTranslate( vertices[i] );
		ages[i] += deltaTime;

		if ( colors && GetNumKeys() )
			colors[i] = GetColor( ages[i] );
		if ( growTime != 0.0f || fadeTime != 0.0f ) {
			float growFade = GetGrowFade( ages[i], generations[i] );
			if ( sizes )
				sizes[i] = growFade;
			else if ( particleNodes[i] ) {
				if ( growFade > 0.0f )
					particleNodes[i]->SetScale( growFade );
				else
					particleNodes[i]->SetScale( -growFade );
			}
		}
	}

	// how many particles are born during the step; the fraction carries
	// over to the next step
	unsigned int numNew;
	if ( unknown11C == -1.0f || time <= unknown11C ) {
		float birth;
		if ( GetBit( UNKNOWN_10_POS ) )
			birth = deltaTime / unknown3C * maxNumParticles;
		else
			birth = deltaTime * unknown40;
		birth += unknown9C;
		unsigned int room = maxNumParticles - numParticles;
		numNew = (unsigned int)birth;
		unknown9C = birth - numNew;
		if ( numNew > room ) {
			numNew = room;
			unknown9C = 0.0f;
		}
	} else {
		numNew = 0;
		unknown9C = 0.0f;
	}

	for ( unsigned int j = 0; j < numNew; j++ ) {
		float ageOffset = rand() * deltaTime * ( 1.0f / RAND_MAX );
		float speedDelta = rand() * speedRandom * ( 1.0f / RAND_MAX ) - speedRandom * 0.5f;
		ages[numParticles + j] = deltaTime - ageOffset;
		float age = ages[numParticles + j];
		generations[numParticles + j] = 0;

		unsigned short emitType = GetField( UNKNOWN_C0_MASK, UNKNOWN_C0_POS );
		if ( emitType == 0 ) {
			velocities[numParticles + j].x = rand() * speedRandom * ( 1.0f / RAND_MAX ) - speedRandom * 0.5f;
			velocities[numParticles + j].y = rand() * speedRandom * ( 1.0f / RAND_MAX ) - speedRandom * 0.5f;
			velocities[numParticles + j].z = speedRandom * 0.5f - ( rand() * ( 1.0f / RAND_MAX ) * speedRandom + speed );
		} else if ( emitType == 1 && GetField( UNKNOWN_7800_MASK, UNKNOWN_7800_POS ) == 3 ) {
			float declination = NiSymmetricRandom() * unknown120 + unknown124;
			float planarAngle = NiSymmetricRandom() * unknown128 + unknown12C;
			float sinDeclination = (float)sin( declination );
			velocities[numParticles + j].x = (float)sin( planarAngle ) * sinDeclination;
			velocities[numParticles + j].y = (float)cos( planarAngle ) * sinDeclination;
			velocities[numParticles + j].z = (float)cos( declination );
			velocities[numParticles + j].x *= ( NiSymmetricRandom() * speedDelta + 1.0f ) * speed;
			velocities[numParticles + j].y *= ( NiSymmetricRandom() * speedDelta + 1.0f ) * speed;
			velocities[numParticles + j].z *= ( NiSymmetricRandom() * speedDelta + 1.0f ) * speed;
		} else {
			if ( emitType == 1 ) {
				float u, v;
				if ( unknown94 != 0.0f ) {
					u = 0.05f - rand() * ( 0.1f / RAND_MAX );
					v = 0.05f - rand() * ( 0.1f / RAND_MAX );
				} else {
					u = 0.0f;
					v = 0.0f;
				}
				velocities[numParticles + j] = u * initialNormalU + initialNormal + v * initialNormalV;
			} else {
				velocities[numParticles + j].x = 0.5f - rand() * ( 1.0f / RAND_MAX );
				velocities[numParticles + j].y = 0.5f - rand() * ( 1.0f / RAND_MAX );
				velocities[numParticles + j].z = 0.5f - rand() * ( 1.0f / RAND_MAX );
			}
			velocities[numParticles + j] *= ( speedDelta + speed ) / velocities[numParticles + j].Length();
		}

		if ( unknown104 )
			rotations[numParticles + j].axis = unknown10C;
		else {
			rotations[numParticles + j].axis.x = NiSymmetricRandom();
			rotations[numParticles + j].axis.y = NiSymmetricRandom();
			rotations[numParticles + j].axis.z = NiSymmetricRandom();
			rotations[numParticles + j].axis.Unitize();
		}
		if ( GetField( UNKNOWN_7800_MASK, UNKNOWN_7800_POS ) == 1 )
			rotations[numParticles + j].angle = rand() * NI_TWO_PI * ( 1.0f / RAND_MAX );
		else
			rotations[numParticles + j].angle = 0.0f;
		float period = ( rand() * unknown100 * ( 1.0f / RAND_MAX ) + 1.0f ) * unknownFC;
		if ( fabsf( period ) >= epsilon )
			lifeSpans[numParticles + j] = NI_TWO_PI / period;
		else
			lifeSpans[numParticles + j] = 0.0f;

		vertices[numParticles + j].x = rand() * unknown44 * ( 1.0f / RAND_MAX ) - unknown44 * 0.5f;
		vertices[numParticles + j].y = rand() * unknown48 * ( 1.0f / RAND_MAX ) - unknown48 * 0.5f;
		vertices[numParticles + j].z = 0.0f;

		bool hit = false;
		float hitTime = 0.0f;
		NiPoint3 hitPosition;
		NiPoint3 hitVelocity;
		float hitStep;
		NiForce *collider;
		for ( collider = colliders; collider; collider = collider->GetNext() ) {
			float stepTime = age;
			NiPoint3 position = vertices[numParticles + j];
			NiPoint3 velocity = velocities[numParticles + j];
			if ( collider->Apply( position, velocity, stepTime, time ) ) {
				hit = true;
				if ( stepTime > hitTime ) {
					hitPosition = position;
					hitVelocity = velocity;
					hitStep = stepTime;
					hitTime = stepTime;
				}
			}
		}
		if ( hit ) {
			vertices[numParticles + j] = hitPosition;
			velocities[numParticles + j] = hitVelocity;
			vertices[numParticles + j] += hitStep * velocities[numParticles + j];
		}

		// a collision that kills the new particle, or splits it, takes it
		// out of the count
		unsigned int onCollide = GetField( UNKNOWN_700_MASK, UNKNOWN_700_POS );
		if ( !hit )
			vertices[numParticles + j] += age * velocities[numParticles + j];
		else if ( onCollide == 1 || onCollide == 2 ) {
			Particle particle;
			particle.position = vertices[numParticles + j];
			particle.direction = velocities[numParticles + j];
			particle.rotation = rotations[numParticles + j];
			particle.lifeSpan = lifeSpans[numParticles + j];
			particle.time = ages[numParticles + j];
			particle.generation = generations[numParticles + j];
			j--;
			numNew--;
			if ( onCollide == 2 )
				SpawnParticle( particle );
			continue;
		}

		NiForce *force;
		for ( force = forces; force; force = force->GetNext() )
			force->Apply( vertices[numParticles + j], velocities[numParticles + j], age, time );

		if ( particleObject ) {
			NiNode *particleNode = new NiNode;
			particleNode->AttachChild( (NiAVObject *)particleObject->Clone(), true );
			particleNode->SetTranslate( vertices[numParticles + j] );
			particleNodes[numParticles + j] = particleNode;
			node->AttachChild( particleNode, true );
			particleNode->UpdateProperties();
			particleNode->UpdateEffects();
		}

		if ( colors && GetNumKeys() )
			colors[numParticles + j] = GetColor( ages[numParticles + j] );
		if ( growTime != 0.0f || fadeTime != 0.0f ) {
			float growFade = GetGrowFade( ages[numParticles + j], generations[numParticles + j] );
			if ( sizes )
				sizes[numParticles + j] = growFade;
			else if ( particleNodes[numParticles + j] ) {
				if ( growFade > 0.0f )
					particleNodes[numParticles + j]->SetScale( growFade );
				else
					particleNodes[numParticles + j]->SetScale( -growFade );
			}
		}
	}

	numParticles += numNew;
	if ( particles ) {
		NiParticlesData *data = (NiParticlesData *)(NiGeometryData *)particles->modelData;
		if ( (unsigned short)numParticles <= data->vertexCount )
			data->activeVertexCount = (unsigned short)numParticles;
		else
			data->activeVertexCount = data->vertexCount;
	}
	if ( particles ) {
		( (NiParticlesData *)(NiGeometryData *)particles->modelData )->radius = unknown30 * 0.5f;
		particles->modelData->bound.unknown10 = true;
	}
}

/*
================
NiParticleSystemController::ResetParticles

Emits the saved particles, resets the forces' frames, then runs the
emission over the time before the start, five steps a second.
================
*/
// FUNCTION: STBC 0x7B19A0
void NiParticleSystemController::ResetParticles() {
	unsigned int i;
	for ( i = 0; i < numSavedParticles; i++ ) {
		AddParticle( savedParticles[i] );
	}

	NiForce *collider;
	for ( collider = colliders; collider; collider = collider->GetNext() ) {
		collider->UpdateWorldIdentity();
	}
	NiForce *force;
	for ( force = forces; force; force = force->GetNext() ) {
		force->UpdateWorldIdentity();
	}

	if ( unknown118 > 0.0f ) {
		int steps = (int)( unknown118 * 5.0f );
		if ( steps < 1 ) {
			steps = 1;
		}
		float step = unknown118 / steps;
		float saveLastTime = lastTime;
		lastTime = loKeyTime - unknown118;
		for ( int j = 0; j < steps; j++ ) {
			Unknown7B0330( step + lastTime );
			lastTime = step + lastTime;
		}
		lastTime = saveLastTime;
	}
}

/*
================
NiParticleSystemController::GetWorldTransform
================
*/
// FUNCTION: STBC 0x7B1AB0
void NiParticleSystemController::GetWorldTransform( NiAVObject *object, float &scale, NiPoint3 &translate, NiMatrix3 &rotate ) {
	NiNode *parent = object->GetParent();
	if ( parent ) {
		scale = object->GetScale() * parent->GetWorldScale();
		rotate = parent->GetWorldRotate() * object->GetRotate();
		float parentScale = parent->GetWorldScale();
		translate = parentScale * ( parent->GetWorldRotate() * object->GetTranslate() ) + parent->GetWorldTranslate();
	} else {
		scale = object->GetScale();
		rotate = object->GetRotate();
		translate = object->GetTranslate();
	}
}

/*
================
NiParticleSystemController::Update

Running backwards restarts the system; the first update after a stop
restarts it without emitting.
================
*/
// FUNCTION: STBC 0x7B1B90
void NiParticleSystemController::Update( float time ) {
	if ( DontDoUpdate( time ) ) {
		return;
	}

	float scaledTime = ComputeScaledTime( time );
	if ( lastTime > 0.0f && time != 0.0f ) {
		if ( fabsf( scaledTime - lastTime ) < epsilon ) {
			return;
		}
		if ( scaledTime < lastTime ) {
			while ( numParticles ) {
				RemoveParticle( numParticles - 1 );
			}
			lastTime = loKeyTime;
			if ( particles ) {
				GetWorldTransform( particles, worldScale, worldTranslate, worldRotate );
			} else {
				GetWorldTransform( node, worldScale, worldTranslate, worldRotate );
			}
			ResetParticles();
		}
		Unknown7B0330( scaledTime );
		lastTime = scaledTime;
	} else {
		lastTime = scaledTime;
		if ( particles ) {
			GetWorldTransform( particles, worldScale, worldTranslate, worldRotate );
		} else {
			GetWorldTransform( node, worldScale, worldTranslate, worldRotate );
		}
		ResetParticles();
	}
}

/*
================
NiParticleSystemController::ReplaceAnim
================
*/
// FUNCTION: STBC 0x7B1D10
void NiParticleSystemController::ReplaceAnim( NiColorKey **keys, unsigned int numKeys, NiColorKey::KeyType keyType ) {
	if ( !colorData ) {
		colorData = new NiColorData;
	}
	colorData->SetAnim( keys, numKeys, keyType );
	lastColorIndex = 0;
}

/*
================
NiParticleSystemController::GetAnim
================
*/
// FUNCTION: STBC 0x7B1DF0
NiColorKey **NiParticleSystemController::GetAnim( unsigned int &numKeys, NiColorKey::KeyType &keyType ) const {
	if ( colorData ) {
		return colorData->GetAnim( numKeys, keyType );
	}
	numKeys = 0;
	keyType = NiColorKey::NOINTERP;
	return 0;
}

/*
================
NiParticleSystemController::GetNumKeys
================
*/
// FUNCTION: STBC 0x7B1E30
unsigned int NiParticleSystemController::GetNumKeys() const {
	return colorData ? colorData->GetNumKeys() : 0;
}

/*
================
NiParticleSystemController::CreateClone
================
*/
// FUNCTION: STBC 0x7B1E50
NiObject *NiParticleSystemController::CreateClone() {
	NiParticleSystemController *clone = new NiParticleSystemController;
	CopyMembers( clone );
	return clone;
}

/*
================
NiParticleSystemController::CopyMembers

Clones the color data, the force and collider chains, the particle
objects and the saved particles' objects.
================
*/
// FUNCTION: STBC 0x7B1EC0
void NiParticleSystemController::CopyMembers( NiParticleSystemController *dest ) {
	NiTimeController::CopyMembers( dest );

	if ( colorData ) {
		dest->SetColorData( (NiColorData *)colorData->CreateClone() );
	}
	dest->SetUnknown10( GetUnknown10() );
	dest->SetUnknown30( GetUnknown30() );
	dest->SetSpeed( GetSpeed() );
	dest->SetSpeedRandom( GetSpeedRandom() );
	dest->SetUnknown3C( GetUnknown3C() );
	dest->SetUnknown50( GetUnknown50() );
	dest->SetUnknown40( GetUnknown40() );
	dest->SetUnknown48( GetUnknown48() );
	dest->SetUnknown44( GetUnknown44() );
	dest->SetUnknown4C( GetUnknown4C() );
	dest->SetUnknown94( GetUnknown94() );
	dest->SetUnknown120( GetUnknown120() );
	dest->SetUnknown124( GetUnknown124() );
	dest->SetUnknown128( GetUnknown128() );
	dest->SetUnknown12C( GetUnknown12C() );
	dest->SetUnknown108( GetUnknown108() );
	dest->SetUnknownFC( GetUnknownFC() );
	dest->SetUnknown100( GetUnknown100() );
	dest->SetUnknown118( GetUnknown118() );
	dest->SetUnknown11C( GetUnknown11C() );
	dest->SetUnknown54( GetUnknown54() );
	dest->SetUnknown58( GetUnknown58() );
	dest->SetNumGenerations( GetNumGenerations() );
	dest->SetUnknown104( GetUnknown104() );
	dest->SetNumParticleObjects( GetNumParticleObjects() );
	dest->SetInitialNormal( GetInitialNormal() );
	dest->SetUnknown10C( GetUnknown10C() );
	dest->SetUnknownC0( GetUnknownC0() );
	dest->SetUnknown700( GetUnknown700() );
	dest->SetUnknown7800( GetUnknown7800() );
	dest->SetGrowTime( GetGrowTime() );
	dest->SetFadeTime( GetFadeTime() );
	dest->SetUnknown20( GetUnknown20() );
	dest->SetMaxNumParticles( GetMaxNumParticles() );

	if ( forces ) {
		dest->forces = (NiForce *)forces->Clone();
	}
	if ( colliders ) {
		dest->colliders = (NiForce *)colliders->Clone();
	}
	if ( GetParticleObject() ) {
		dest->SetParticleObject( (NiAVObject *)GetParticleObject()->Clone() );
	}
	unsigned int i;
	for ( i = 0; i < GetNumParticleObjects(); i++ ) {
		dest->SetParticleObjectAt( (NiAVObject *)GetParticleObjectAt( i )->Clone(), i );
	}

	if ( numSavedParticles > 0 ) {
		dest->savedParticles = new Particle[maxNumParticles];
		assert( dest->savedParticles );
		dest->numSavedParticles = numSavedParticles;
		for ( i = 0; i < numSavedParticles; i++ ) {
			dest->savedParticles[i] = savedParticles[i];
			dest->savedParticles[i].object = (NiAVObject *)savedParticles[i].object->Clone();
		}
	}

	if ( dest->target ) {
		if ( NiIsKindOf( NiParticles, dest->target ) ) {
			dest->particles = (NiParticles *)dest->target;
		} else if ( NiIsKindOf( NiNode, dest->target ) ) {
			dest->node = (NiNode *)dest->target;
		} else {
			assert( 0 );
		}
	}
}

/*
================
NiParticleSystemController::CreateObject
================
*/
// FUNCTION: STBC 0x7B2320
void NiParticleSystemController::CreateObject( NiStream &stream, NiObject *&object ) {
	NiParticleSystemController *controller = new NiParticleSystemController;
	stream.SetLoadRecord( new NiParticleSystemControllerLoadRecord );
	controller->LoadBinary( stream );
	object = controller;
}

/*
================
NiParticleSystemController::LoadBinary

The saved particles are read for every slot of the array, which
SaveBinary writes the same way.
================
*/
// FUNCTION: STBC 0x7B2460
// PARKED: inline budget: the /Od body equals AnimOnly 0x4A20AC; retail calls the fourteenth read (unknown94) out of line, where this build has a margin of 0 and expands it, and expands the saved particle's NiPointer constructor (4 units short here)
void NiParticleSystemController::LoadBinary( NiStream &stream ) {
	NiTimeController::LoadBinary( stream );

	unsigned int count;
	NiStreamLoadBinary( stream, count );
	SetMaxNumParticles( count );
	NiStreamLoadBinary( stream, unknown30 );
	NiStreamLoadBinary( stream, speed );
	NiStreamLoadBinary( stream, speedRandom );
	NiStreamLoadBinary( stream, unknown3C );
	NiStreamLoadBinary( stream, unknown50 );
	NiStreamLoadBinary( stream, unknown40 );
	NiStreamLoadBinary( stream, unknown44 );
	NiStreamLoadBinary( stream, unknown48 );
	NiStreamLoadBinary( stream, unknown4C );
	NiStreamLoadBinary( stream, unknown54 );
	NiStreamLoadBinary( stream, unknown58 );
	NiStreamLoadBinary( stream, unknown94 );

	NiPoint3 normal;
	normal.LoadBinary( stream );
	SetInitialNormal( normal );

	NiStreamLoadBinary( stream, numGenerations );
	NiStreamLoadBinary( stream, unknownFC );
	NiStreamLoadBinary( stream, unknown100 );
	NiStreamLoadBinary( stream, unknown104 );
	NiStreamLoadBinary( stream, unknown108 );
	unknown10C.LoadBinary( stream );
	NiStreamLoadBinary( stream, unknown118 );
	NiStreamLoadBinary( stream, unknown11C );
	NiStreamLoadBinary( stream, unknown120 );
	NiStreamLoadBinary( stream, unknown124 );
	NiStreamLoadBinary( stream, unknown128 );
	NiStreamLoadBinary( stream, unknown12C );

	NiParticleSystemControllerLoadRecord *linkRecord = (NiParticleSystemControllerLoadRecord *)stream.GetLoadRecord();
	NiStreamLoadBinary( stream, linkRecord->particleObjectLinkID );
	unsigned int i;
	if ( linkRecord->particleObjectLinkID ) {
		// the count is read into the loop counter (AnimOnly.exe 0x4A2320)
		NiStreamLoadBinary( stream, i );
		SetNumParticleObjects( i );
		for ( i = 0; i < numParticleObjects; i++ ) {
			unsigned int linkID;
			NiStreamLoadBinary( stream, linkID );
			linkRecord->particleObjectLinkIDs.Add( linkID );
		}
	}

	unsigned int hasSavedParticles;
	NiStreamLoadBinary( stream, hasSavedParticles );
	if ( hasSavedParticles ) {
		for ( i = 0; i < maxNumParticles; i++ ) {
			Particle particle;
			NiStreamLoadBinary( stream, particle.lifeSpan );
			NiStreamLoadBinary( stream, particle.time );
			NiStreamLoadBinary( stream, particle.generation );
			particle.position.LoadBinary( stream );
			particle.direction.LoadBinary( stream );
			unsigned char *linkID;
			NiStreamLoadBinary( stream, linkID );
			linkRecord->savedParticleLinkIDs.Add( linkID );
			NiStreamLoadBinary( stream, particle.rotation.angle );
			particle.rotation.axis.LoadBinary( stream );
			AddSavedParticle( particle );
		}
	}

	unsigned int numLinks;
	NiForce *linkID;
	NiStreamLoadBinary( stream, numLinks );
	for ( i = 0; i < numLinks; i++ ) {
		NiStreamLoadBinary( stream, linkID );
		linkRecord->forceLinkIDs.Add( linkID );
	}
	NiStreamLoadBinary( stream, numLinks );
	for ( i = 0; i < numLinks; i++ ) {
		NiStreamLoadBinary( stream, linkID );
		linkRecord->colliderLinkIDs.Add( linkID );
	}

	NiStreamLoadBinary( stream, linkRecord->colorDataLinkID );
	NiStreamLoadBinary( stream, growTime );
	NiStreamLoadBinary( stream, fadeTime );
}

/*
================
NiParticleSystemController::LinkObject
================
*/
// FUNCTION: STBC 0x7B2910
// PARKED: address-mode operand order: retail scales the link index once and reuses it ([esi+ecx]) and forms the saved particle's object address as offset + base
void NiParticleSystemController::LinkObject( NiStream &stream ) {
	NiTimeController::LinkObject( stream );

	NiParticleSystemControllerLoadRecord *linkRecord = (NiParticleSystemControllerLoadRecord *)stream.GetLoadRecord();
	assert( linkRecord );
	particleObject = (NiAVObject *)stream.GetObjectFromLinkID( linkRecord->particleObjectLinkID );
	unsigned int i;
	NiForce *forceOrCollider;	// (name invented; fits the /Od frame order)
	if ( particleObject ) {
		for ( i = 0; i < numParticleObjects; i++ ) {
			particleObjects[i] = (NiAVObject *)stream.GetObjectFromLinkID( linkRecord->particleObjectLinkIDs.GetAt( i ) );
		}
	}

	if ( savedParticles ) {
		for ( i = 0; i < maxNumParticles; i++ ) {
			savedParticles[i].object = (NiAVObject *)stream.GetObjectFromLinkID( (unsigned int)linkRecord->savedParticleLinkIDs.GetAt( i ) );
		}
	}

	for ( i = 0; i < linkRecord->forceLinkIDs.GetSize(); i++ ) {
		forceOrCollider = (NiForce *)stream.GetObjectFromLinkID( (unsigned int)linkRecord->forceLinkIDs.GetAt( i ) );
		AddForce( forceOrCollider );
	}
	for ( i = 0; i < linkRecord->colliderLinkIDs.GetSize(); i++ ) {
		forceOrCollider = (NiForce *)stream.GetObjectFromLinkID( (unsigned int)linkRecord->colliderLinkIDs.GetAt( i ) );
		if ( forceOrCollider ) {
			AddCollider( forceOrCollider );
		}
	}

	colorData = (NiColorData *)stream.GetObjectFromLinkID( linkRecord->colorDataLinkID );
}

/*
================
NiParticleSystemController::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7B2AF0
bool NiParticleSystemController::RegisterStreamables( NiStream &stream ) {
	if ( !NiTimeController::RegisterStreamables( stream ) ) {
		return false;
	}

	if ( particleObject ) {
		particleObject->RegisterStreamables( stream );
	}
	unsigned int i;
	if ( particleObjects ) {
		for ( i = 0; i < numParticleObjects; i++ ) {
			particleObjects[i]->RegisterStreamables( stream );
		}
	}
	if ( savedParticles ) {
		for ( i = 0; i < maxNumParticles; i++ ) {
			savedParticles[i].object->RegisterStreamables( stream );
		}
	}
	NiForce *pForce;	// NiParticleBomb.cpp's assert names a force pForce
	for ( pForce = forces; pForce; pForce = pForce->GetNext() ) {
		pForce->RegisterStreamables( stream );
	}
	for ( pForce = colliders; pForce; pForce = pForce->GetNext() ) {
		pForce->RegisterStreamables( stream );
	}
	if ( colorData ) {
		colorData->RegisterStreamables( stream );
	}
	return true;
}

/*
================
NiParticleSystemController::SaveBinary

The link IDs are the objects' addresses; the saved particles' array
pointer stands as the flag that they follow.
================
*/
// FUNCTION: STBC 0x7B2BC0
void NiParticleSystemController::SaveBinary( NiStream &stream ) {
	NiTimeController::SaveBinary( stream );

	NiStreamSaveBinary( stream, maxNumParticles );
	NiStreamSaveBinary( stream, unknown30 );
	NiStreamSaveBinary( stream, speed );
	NiStreamSaveBinary( stream, speedRandom );
	NiStreamSaveBinary( stream, unknown3C );
	NiStreamSaveBinary( stream, unknown50 );
	NiStreamSaveBinary( stream, unknown40 );
	NiStreamSaveBinary( stream, unknown44 );
	NiStreamSaveBinary( stream, unknown48 );
	NiStreamSaveBinary( stream, unknown4C );
	NiStreamSaveBinary( stream, unknown54 );
	NiStreamSaveBinary( stream, unknown58 );
	NiStreamSaveBinary( stream, unknown94 );
	initialNormal.SaveBinary( stream );
	NiStreamSaveBinary( stream, numGenerations );
	NiStreamSaveBinary( stream, unknownFC );
	NiStreamSaveBinary( stream, unknown100 );
	NiStreamSaveBinary( stream, unknown104 );
	NiStreamSaveBinary( stream, unknown108 );
	unknown10C.SaveBinary( stream );
	NiStreamSaveBinary( stream, unknown118 );
	NiStreamSaveBinary( stream, unknown11C );
	NiStreamSaveBinary( stream, unknown120 );
	NiStreamSaveBinary( stream, unknown124 );
	NiStreamSaveBinary( stream, unknown128 );
	NiStreamSaveBinary( stream, unknown12C );

	unsigned int idx;	// (name invented; fits the /Od frame order)
	NiStreamSaveBinary( stream, particleObject );
	if ( particleObject ) {
		NiStreamSaveBinary( stream, numParticleObjects );
		for ( idx = 0; idx < numParticleObjects; idx++ ) {
			NiStreamSaveBinary( stream, particleObjects[idx] );
		}
	}

	NiStreamSaveBinary( stream, savedParticles );
	if ( savedParticles ) {
		for ( idx = 0; idx < maxNumParticles; idx++ ) {
			Particle &particle = savedParticles[idx];
			NiStreamSaveBinary( stream, particle.lifeSpan );
			NiStreamSaveBinary( stream, particle.time );
			NiStreamSaveBinary( stream, particle.generation );
			particle.position.SaveBinary( stream );
			particle.direction.SaveBinary( stream );
			NiStreamSaveBinary( stream, particle.object );
			NiStreamSaveBinary( stream, particle.rotation.angle );
			particle.rotation.axis.SaveBinary( stream );
		}
	}

	unsigned int count = forces ? forces->GetChainLength() : 0;
	NiStreamSaveBinary( stream, count );
	NiForce *pForce;	// NiParticleBomb.cpp's assert names a force pForce
	for ( pForce = forces; pForce; pForce = pForce->GetNext() ) {
		NiStreamSaveBinary( stream, pForce );
	}
	count = colliders ? colliders->GetChainLength() : 0;
	NiStreamSaveBinary( stream, count );
	for ( pForce = colliders; pForce; pForce = pForce->GetNext() ) {
		NiStreamSaveBinary( stream, pForce );
	}

	NiStreamSaveBinary( stream, colorData );
	NiStreamSaveBinary( stream, growTime );
	NiStreamSaveBinary( stream, fadeTime );
}

/*
================
NiParticleSystemController::IsEqual

The particle objects, forces and saved particles are not compared, only
their counts.
================
*/
// FUNCTION: STBC 0x7B2F40
bool NiParticleSystemController::IsEqual( NiObject *object ) {
	if ( !NiTimeController::IsEqual( object ) ) {
		return false;
	}

	NiParticleSystemController *controller = (NiParticleSystemController *)object;
	if ( maxNumParticles != controller->maxNumParticles
		|| numParticleObjects != controller->numParticleObjects
		|| GetUnknown10() != controller->GetUnknown10()
		|| GetUnknown20() != controller->GetUnknown20()
		|| unknown30 != controller->unknown30
		|| speed != controller->speed
		|| speedRandom != controller->speedRandom
		|| unknown3C != controller->unknown3C
		|| unknown50 != controller->unknown50
		|| unknown40 != controller->unknown40
		|| unknown44 != controller->unknown44
		|| unknown48 != controller->unknown48
		|| unknown4C != controller->unknown4C
		|| unknown54 != controller->unknown54
		|| unknown58 != controller->unknown58
		|| GetUnknownC0() != controller->GetUnknownC0()
		|| unknown94 != controller->unknown94
		|| initialNormal != controller->initialNormal
		|| numGenerations != controller->numGenerations
		|| GetUnknown700() != controller->GetUnknown700()
		|| unknownFC != controller->unknownFC
		|| unknown100 != controller->unknown100
		|| unknown104 != controller->unknown104
		|| unknown108 != controller->unknown108
		|| unknown10C != controller->unknown10C
		|| unknown118 != controller->unknown118
		|| unknown11C != controller->unknown11C
		|| unknown120 != controller->unknown120
		|| unknown124 != controller->unknown124
		|| unknown128 != controller->unknown128
		|| unknown12C != controller->unknown12C
		|| GetUnknown7800() != controller->GetUnknown7800()
		|| growTime != controller->growTime
		|| fadeTime != controller->fadeTime ) {
		return false;
	}
	if ( !colorData->IsEqual( controller->colorData ) ) {
		return false;
	}
	return true;
}

/*
================
NiParticleSystemController::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7B32C0
void NiParticleSystemController::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTimeController::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
