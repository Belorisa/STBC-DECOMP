/*
===============================================================================

	PulsingLight.cpp

	The pulsing light's own unit: the class's handler table (0x986460),
	its methods and the static initialisers of the headers it includes
	(<iostream>, NiMain.h's class registrations and the sound classes'),
	.bss 0x986440-0x986580.

===============================================================================
*/

#include <iostream>

#include "PulsingLight.h"
#include "TGEventHandlerTable.h"
#include "SetClass.h"
#include "SetManager.h"
#include "NiLight.h"
#include "NiNode.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004e8b00 _$E66
// FUNCTION: STBC 0x004e8b10 _$E65
// FUNCTION: STBC 0x004e8b20 _$E235
// FUNCTION: STBC 0x004e8b30 _$E234
// FUNCTION: STBC 0x004e8b40 _$E77
// FUNCTION: STBC 0x004e8b50 _$E76
// FUNCTION: STBC 0x004e8b60 _$E81
// FUNCTION: STBC 0x004e8b70 _$E80
// FUNCTION: STBC 0x004e8b80 _$E85
// FUNCTION: STBC 0x004e8b90 _$E84
// FUNCTION: STBC 0x004e8ba0 _$E89
// FUNCTION: STBC 0x004e8bb0 _$E88
// FUNCTION: STBC 0x004e8bc0 _$E206
// FUNCTION: STBC 0x004e8bd0 _$E205
// FUNCTION: STBC 0x004e8be0 _$E148
// FUNCTION: STBC 0x004e8bf0 _$E147
// FUNCTION: STBC 0x004e8c00 _$E161
// FUNCTION: STBC 0x004e8c10 _$E160
// FUNCTION: STBC 0x004e8c20 _$E211
// FUNCTION: STBC 0x004e8c30 _$E210
// FUNCTION: STBC 0x004e8c40 _$E240
// FUNCTION: STBC 0x004e8c50 _$E239
// FUNCTION: STBC 0x004e8c60 _$E215
// FUNCTION: STBC 0x004e8c70 _$E214
// FUNCTION: STBC 0x004e8c80 _$E244
// FUNCTION: STBC 0x004e8c90 _$E243
// FUNCTION: STBC 0x004e8ca0 _$E251
// FUNCTION: STBC 0x004e8cb0 _$E250
// FUNCTION: STBC 0x004e8e90 _$E273
// FUNCTION: STBC 0x004e8ea0 _$E272
// FUNCTION: STBC 0x004e8fc0 _$E277
// FUNCTION: STBC 0x004e8fd0 _$E276
// FUNCTION: STBC 0x004e9070 _$E106
// FUNCTION: STBC 0x004e9080 _$E105
// FUNCTION: STBC 0x004e9090 _$E259
// FUNCTION: STBC 0x004e90a0 _$E258
// FUNCTION: STBC 0x004e90b0 _$E266
// FUNCTION: STBC 0x004e90c0 _$E265
// FUNCTION: STBC 0x004e90d0 _$E285
// FUNCTION: STBC 0x004e90e0 _$E284
// FUNCTION: STBC 0x004e90f0 _$E289
// FUNCTION: STBC 0x004e9100 _$E288
// FUNCTION: STBC 0x004e9110 _$E329
// FUNCTION: STBC 0x004e9120 _$E328
// FUNCTION: STBC 0x004e9130 _$E335
// FUNCTION: STBC 0x004e9140 _$E334
// FUNCTION: STBC 0x004e9150 _$E231
// FUNCTION: STBC 0x004e9160 _$E230
// FUNCTION: STBC 0x004e91f0 _$E281
// FUNCTION: STBC 0x004e9200 _$E280
// FUNCTION: STBC 0x004e9210 _$E293
// FUNCTION: STBC 0x004e9220 _$E292
// FUNCTION: STBC 0x004e9230 _$E297
// FUNCTION: STBC 0x004e9240 _$E296
// FUNCTION: STBC 0x004e9250 _$E301
// FUNCTION: STBC 0x004e9260 _$E300
// FUNCTION: STBC 0x004e9270 _$E305
// FUNCTION: STBC 0x004e9280 _$E304
// FUNCTION: STBC 0x004e9290 _$E309
// FUNCTION: STBC 0x004e92a0 _$E308
// FUNCTION: STBC 0x004e92b0 _$E313
// FUNCTION: STBC 0x004e92c0 _$E312
// FUNCTION: STBC 0x004e92d0 _$E317
// FUNCTION: STBC 0x004e92e0 _$E316
// FUNCTION: STBC 0x004e92f0 _$E321
// FUNCTION: STBC 0x004e9300 _$E320
// FUNCTION: STBC 0x004e9310 _$E111
// FUNCTION: STBC 0x004e9320 _$E110
// FUNCTION: STBC 0x004e9330 _$E325
// FUNCTION: STBC 0x004e9340 _$E324
// FUNCTION: STBC 0x004e9350 _$E116
// FUNCTION: STBC 0x004e9360 _$E115
// FUNCTION: STBC 0x004e9370 _$E339
// FUNCTION: STBC 0x004e9380 _$E338
// FUNCTION: STBC 0x004e9390 _$E343
// FUNCTION: STBC 0x004e93a0 _$E342
// FUNCTION: STBC 0x004e93b0 _$E347
// FUNCTION: STBC 0x004e93c0 _$E346
// FUNCTION: STBC 0x004e93d0 _$E351
// FUNCTION: STBC 0x004e93e0 _$E350
// FUNCTION: STBC 0x004e93f0 _$E125
// FUNCTION: STBC 0x004e9400 _$E124
// FUNCTION: STBC 0x004e9410 _$E355
// FUNCTION: STBC 0x004e9420 _$E354
// FUNCTION: STBC 0x004e9430 _$E359
// FUNCTION: STBC 0x004e9440 _$E358
// FUNCTION: STBC 0x004e9450 _$E363
// FUNCTION: STBC 0x004e9460 _$E362
// FUNCTION: STBC 0x004e95a0 _$E131
// FUNCTION: STBC 0x004e95b0 _$E130
// FUNCTION: STBC 0x004e96a0 _$E136
// FUNCTION: STBC 0x004e96b0 _$E135
// FUNCTION: STBC 0x004e96f0 _$E166
// FUNCTION: STBC 0x004e9700 _$E165
// FUNCTION: STBC 0x004e98c0 _$E367
// FUNCTION: STBC 0x004e98d0 _$E366
// FUNCTION: STBC 0x004e98e0 _$E171
// FUNCTION: STBC 0x004e98f0 _$E170
// FUNCTION: STBC 0x004e9900 _$E371
// FUNCTION: STBC 0x004e9910 _$E370
// FUNCTION: STBC 0x004e9920 _$E375
// FUNCTION: STBC 0x004e9930 _$E374
// FUNCTION: STBC 0x004e9940 _$E176
// FUNCTION: STBC 0x004e9950 _$E175
// FUNCTION: STBC 0x004e9960 _$E379
// FUNCTION: STBC 0x004e9970 _$E378
// FUNCTION: STBC 0x004e9980 _$E141
// FUNCTION: STBC 0x004e9990 _$E140
// FUNCTION: STBC 0x004e99a0 _$E383
// FUNCTION: STBC 0x004e99b0 _$E382
// FUNCTION: STBC 0x004e99c0 _$E387
// FUNCTION: STBC 0x004e99d0 _$E386
// FUNCTION: STBC 0x004e99e0 _$E181
// FUNCTION: STBC 0x004e99f0 _$E180
// FUNCTION: STBC 0x004e9a00 _$E186
// FUNCTION: STBC 0x004e9a10 _$E185
// FUNCTION: STBC 0x004e9a20 _$E391
// FUNCTION: STBC 0x004e9a30 _$E390
// FUNCTION: STBC 0x004e9a40 _$E395
// FUNCTION: STBC 0x004e9a50 _$E394
// FUNCTION: STBC 0x004e9a60 _$E399
// FUNCTION: STBC 0x004e9a70 _$E398
// FUNCTION: STBC 0x004e9a80 _$E403
// FUNCTION: STBC 0x004e9a90 _$E402
// FUNCTION: STBC 0x004e9aa0 _$E407
// FUNCTION: STBC 0x004e9ab0 _$E406
// FUNCTION: STBC 0x004e9ac0 _$E411
// FUNCTION: STBC 0x004e9ad0 _$E410
// FUNCTION: STBC 0x004e9ae0 _$E415
// FUNCTION: STBC 0x004e9af0 _$E414
// FUNCTION: STBC 0x004e9b00 _$E419
// FUNCTION: STBC 0x004e9b10 _$E418
// FUNCTION: STBC 0x004e9b20 _$E191
// FUNCTION: STBC 0x004e9b30 _$E190
// FUNCTION: STBC 0x004e9b40 _$E423
// FUNCTION: STBC 0x004e9b50 _$E422
// FUNCTION: STBC 0x004e9b60 _$E196
// FUNCTION: STBC 0x004e9b70 _$E195
// FUNCTION: STBC 0x004e9b80 _$E201
// FUNCTION: STBC 0x004e9b90 _$E200
// FUNCTION: STBC 0x004e9ba0 _$E427
// FUNCTION: STBC 0x004e9bb0 _$E426
// FUNCTION: STBC 0x004e9bc0 _$E432
// FUNCTION: STBC 0x004e9bd0 _$E431
// FUNCTION: STBC 0x004e9be0 _$E436
// FUNCTION: STBC 0x004e9bf0 _$E435

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004e8a80 _$E32
// FUNCTION: STBC 0x004e8a90 _$E29
// FUNCTION: STBC 0x004e8aa0 _$E31
// FUNCTION: STBC 0x004e8ab0 _$E30
// FUNCTION: STBC 0x004e8ac0 _$E38
// FUNCTION: STBC 0x004e8ad0 _$E35
// FUNCTION: STBC 0x004e8ae0 _$E37
// FUNCTION: STBC 0x004e8af0 _$E36

// The guard of std::ctype<unsigned short>::id and its exit-time thunk (<iostream>).
// FUNCTION: STBC 0x004e96c0 _$E450
// FUNCTION: STBC 0x004e96e0 _$E449

// The handler table's dynamic initialiser, its construction, the registration
// of its destructor and the guarded destructor.
// FUNCTION: STBC 0x004e83c0 _$E442
// FUNCTION: STBC 0x004e83d0 _$E438
// FUNCTION: STBC 0x004e83e0 _$E441
// FUNCTION: STBC 0x004e83f0 _$E439
// DATA: STBC 0x00986460
TGEventHandlerTable PulsingLight::classHandlers;

// The list of the affected nodes, kept from this unit: its destructor (the
// destructor's unwind), AddTail, Remove, GetNext, RemovePos, FindPos,
// RemoveHead and RemoveTail, its items' constructor and destructor and the
// smart pointer's copy constructor (NiTList.h, NiPointer.h).

/*
================
PulsingLight::AddToSet
================
*/
// FUNCTION: STBC 0x004e8410
void PulsingLight::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, PULSINGLIGHT_TYPE );
		LightObjectClass::AddToSet( set );
	}
}

/*
================
PulsingLight::RemoveFromSet
================
*/
// FUNCTION: STBC 0x004e8440
void PulsingLight::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, PULSINGLIGHT_TYPE );
		LightObjectClass::RemoveFromSet( set );
	}
}

/*
================
PulsingLight::GetClassObjects

The set's pulsing lights and their number; NULL and 0 when there are none.
================
*/
// FUNCTION: STBC 0x004e8470
PulsingLight **PulsingLight::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (PulsingLight **)set->objectTable.GetObjects( PULSINGLIGHT_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
PulsingLight::GetNumClassObjects
================
*/
// FUNCTION: STBC 0x004e84a0
int PulsingLight::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( PULSINGLIGHT_TYPE );
	}
	return 0;
}

/*
================
PulsingLight::GetObjectByID

The pulsing light with the given ID in the set, or among every object when
the set is NULL; NULL when it is none or not a pulsing light.
================
*/
// FUNCTION: STBC 0x004e84c0
PulsingLight *PulsingLight::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( PULSINGLIGHT_TYPE ) ) {
			return (PulsingLight *)object;
		}
		return NULL;
	}
	return (PulsingLight *)set->GetClassObject( PULSINGLIGHT_TYPE, id );
}

/*
================
PulsingLight::GetObject

The pulsing light with the name in the set, or in the last set that has one
when the set is NULL; NULL when there is none.
================
*/
// FUNCTION: STBC 0x004e8530
PulsingLight *PulsingLight::GetObject( SetClass *set, char *name_ ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( PULSINGLIGHT_TYPE, name_ );
		}
		return (PulsingLight *)object;
	}
	return (PulsingLight *)set->GetClassObject( PULSINGLIGHT_TYPE, name_ );
}

/*
================
PulsingLight::PulsingLight
================
*/
// FUNCTION: STBC 0x004e8590 ??0PulsingLight@@QAE@XZ
PulsingLight::PulsingLight( void ) : LightObjectClass( 0 ), period( 1.0f ), numLights( 0 ), lights( NULL ), affectedNodes( 2 ), entireSet( false ) {
	fading = NULL;
	rising = NULL;
}

/*
================
PulsingLight::~PulsingLight

Deletes the lights and their array. The dimmers are not freed.
================
*/
// FUNCTION: STBC 0x004e85e0
PulsingLight::~PulsingLight( void ) {
	DetachLights();
	if ( lights != NULL ) {
		for ( int i = 0; i < numLights; i++ ) {
			delete lights[i];
		}
		delete [] lights;
	}
}

/*
================
PulsingLight::Create
================
*/
// FUNCTION: STBC 0x004e86d0
PulsingLight *PulsingLight::Create( int numLights_, NiLight **lights_, float period_, char *name ) {
	PulsingLight *light = new PulsingLight;
	light->SetName( name );
	light->period = period_;
	light->numLights = numLights_;
	light->lights = lights_;
	light->dimmers = new float[light->numLights];
	for ( int i = 0; i < light->numLights; i++ ) {
		light->dimmers[i] = light->lights[i]->GetDimmer();
	}
	return light;
}

/*
================
PulsingLight::Update

Fades the light of the period time falls in out, and the next one in, both
from their full dimmers, and lights the objects' nodes with the two.
================
*/
// FUNCTION: STBC 0x004e8760
void PulsingLight::Update( float time ) {
	int cycle = (int)( time / period );
	float fraction = ( time - cycle * period ) / period;
	int current = cycle % numLights;
	DetachLights();
	NiLight *fadingLight = lights[current];
	NiLight *risingLight = lights[( current + 1 ) % numLights];
	fadingLight->SetDimmer( ( 1.0f - fraction ) * dimmers[current] );
	risingLight->SetDimmer( fraction * dimmers[( current + 1 ) % numLights] );
	AttachLights( fadingLight, risingLight );
	fadingLight->Update( time );
	risingLight->Update( time );
	LightObjectClass::Update( time );
}

/*
================
PulsingLight::GetLight
================
*/
// FUNCTION: STBC 0x004e8820
NiLight *PulsingLight::GetLight( int index ) {
	return lights[index];
}

/*
================
PulsingLight::AddIlluminatedObject

The light shines on the object's node, and no longer on the whole set.
================
*/
// FUNCTION: STBC 0x004e8830
void PulsingLight::AddIlluminatedObject( BaseObjectClass *object ) {
	entireSet = false;
	if ( object->GetNode() != NULL ) {
		affectedNodes.AddTail( object->GetNode() );
	}
}

/*
================
PulsingLight::RemoveIlluminatedObject
================
*/
// FUNCTION: STBC 0x004e8880
void PulsingLight::RemoveIlluminatedObject( BaseObjectClass *object ) {
	if ( object->GetNode() != NULL ) {
		affectedNodes.Remove( object->GetNode() );
	}
}

/*
================
PulsingLight::IlluminateEntireSet
================
*/
// FUNCTION: STBC 0x004e88e0
void PulsingLight::IlluminateEntireSet( void ) {
	affectedNodes.RemoveAll();
	entireSet = true;
}

/*
================
PulsingLight::UnilluminateEntireSet
================
*/
// FUNCTION: STBC 0x004e8950
void PulsingLight::UnilluminateEntireSet( void ) {
	affectedNodes.RemoveAll();
	entireSet = false;
}

/*
================
PulsingLight::DetachLights
================
*/
// FUNCTION: STBC 0x004e89c0
void PulsingLight::DetachLights( void ) {
	if ( fading != NULL ) {
		fading->DetachAllAffectedNodes();
		fading = NULL;
	}
	if ( rising != NULL ) {
		rising->DetachAllAffectedNodes();
		rising = NULL;
	}
}

/*
================
PulsingLight::AttachLights
================
*/
// FUNCTION: STBC 0x004e89f0
void PulsingLight::AttachLights( NiLight *fading_, NiLight *rising_ ) {
	fading = fading_;
	rising = rising_;
	// BUG: the list is walked only when it is empty, so the lights never
	// shine on the illuminated objects' nodes
	if ( !entireSet && affectedNodes.IsEmpty() ) {
		NiTListIterator pos = affectedNodes.GetHeadPos();
		while ( pos ) {
			NiNode *node = affectedNodes.GetNext( pos );
			fading_->AttachAffectedNode( node );
			rising_->AttachAffectedNode( node );
		}
	}
}
