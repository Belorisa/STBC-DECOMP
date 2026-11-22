/*
===============================================================================

	Waypoint.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>

#include "Waypoint.h"
#include "WaypointPathDisplay.h"
#include "SetClass.h"
#include "SetManager.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGFloatEvent.h"
#include "TGStream.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004a5330 _$E459
// FUNCTION: STBC 0x004a5350 _$E458

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004a4cd0 _$E32
// FUNCTION: STBC 0x004a4ce0 _$E29
// FUNCTION: STBC 0x004a4cf0 _$E31
// FUNCTION: STBC 0x004a4d00 _$E30
// FUNCTION: STBC 0x004a4d10 _$E38
// FUNCTION: STBC 0x004a4d20 _$E35
// FUNCTION: STBC 0x004a4d30 _$E37
// FUNCTION: STBC 0x004a4d40 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004a4d50 _$E66
// FUNCTION: STBC 0x004a4d60 _$E65
// FUNCTION: STBC 0x004a4d70 _$E253
// FUNCTION: STBC 0x004a4d80 _$E252
// FUNCTION: STBC 0x004a4d90 _$E77
// FUNCTION: STBC 0x004a4da0 _$E76
// FUNCTION: STBC 0x004a4db0 _$E81
// FUNCTION: STBC 0x004a4dc0 _$E80
// FUNCTION: STBC 0x004a4dd0 _$E85
// FUNCTION: STBC 0x004a4de0 _$E84
// FUNCTION: STBC 0x004a4df0 _$E89
// FUNCTION: STBC 0x004a4e00 _$E88
// FUNCTION: STBC 0x004a4e10 _$E206
// FUNCTION: STBC 0x004a4e20 _$E205
// FUNCTION: STBC 0x004a4e30 _$E148
// FUNCTION: STBC 0x004a4e40 _$E147
// FUNCTION: STBC 0x004a4e50 _$E161
// FUNCTION: STBC 0x004a4e60 _$E160
// FUNCTION: STBC 0x004a4e70 _$E211
// FUNCTION: STBC 0x004a4e80 _$E210
// FUNCTION: STBC 0x004a4e90 _$E258
// FUNCTION: STBC 0x004a4ea0 _$E257
// FUNCTION: STBC 0x004a4eb0 _$E215
// FUNCTION: STBC 0x004a4ec0 _$E214
// FUNCTION: STBC 0x004a4ed0 _$E262
// FUNCTION: STBC 0x004a4ee0 _$E261
// FUNCTION: STBC 0x004a4ef0 _$E269
// FUNCTION: STBC 0x004a4f00 _$E268
// FUNCTION: STBC 0x004a4f10 _$E291
// FUNCTION: STBC 0x004a4f20 _$E290
// FUNCTION: STBC 0x004a4f30 _$E295
// FUNCTION: STBC 0x004a4f40 _$E294
// FUNCTION: STBC 0x004a4f50 _$E106
// FUNCTION: STBC 0x004a4f60 _$E105
// FUNCTION: STBC 0x004a4f70 _$E277
// FUNCTION: STBC 0x004a4f80 _$E276
// FUNCTION: STBC 0x004a4f90 _$E284
// FUNCTION: STBC 0x004a4fa0 _$E283
// FUNCTION: STBC 0x004a4fb0 _$E307
// FUNCTION: STBC 0x004a4fc0 _$E306
// FUNCTION: STBC 0x004a4fd0 _$E311
// FUNCTION: STBC 0x004a4fe0 _$E310
// FUNCTION: STBC 0x004a4ff0 _$E351
// FUNCTION: STBC 0x004a5000 _$E350
// FUNCTION: STBC 0x004a5010 _$E357
// FUNCTION: STBC 0x004a5020 _$E356
// FUNCTION: STBC 0x004a5030 _$E299
// FUNCTION: STBC 0x004a5040 _$E298
// FUNCTION: STBC 0x004a5050 _$E303
// FUNCTION: STBC 0x004a5060 _$E302
// FUNCTION: STBC 0x004a5070 _$E315
// FUNCTION: STBC 0x004a5080 _$E314
// FUNCTION: STBC 0x004a5090 _$E319
// FUNCTION: STBC 0x004a50a0 _$E318
// FUNCTION: STBC 0x004a50b0 _$E323
// FUNCTION: STBC 0x004a50c0 _$E322
// FUNCTION: STBC 0x004a50d0 _$E327
// FUNCTION: STBC 0x004a50e0 _$E326
// FUNCTION: STBC 0x004a50f0 _$E331
// FUNCTION: STBC 0x004a5100 _$E330
// FUNCTION: STBC 0x004a5110 _$E335
// FUNCTION: STBC 0x004a5120 _$E334
// FUNCTION: STBC 0x004a5130 _$E339
// FUNCTION: STBC 0x004a5140 _$E338
// FUNCTION: STBC 0x004a5150 _$E343
// FUNCTION: STBC 0x004a5160 _$E342
// FUNCTION: STBC 0x004a5170 _$E111
// FUNCTION: STBC 0x004a5180 _$E110
// FUNCTION: STBC 0x004a5190 _$E347
// FUNCTION: STBC 0x004a51a0 _$E346
// FUNCTION: STBC 0x004a51b0 _$E116
// FUNCTION: STBC 0x004a51c0 _$E115
// FUNCTION: STBC 0x004a51d0 _$E361
// FUNCTION: STBC 0x004a51e0 _$E360
// FUNCTION: STBC 0x004a51f0 _$E365
// FUNCTION: STBC 0x004a5200 _$E364
// FUNCTION: STBC 0x004a5210 _$E369
// FUNCTION: STBC 0x004a5220 _$E368
// FUNCTION: STBC 0x004a5230 _$E373
// FUNCTION: STBC 0x004a5240 _$E372
// FUNCTION: STBC 0x004a5250 _$E125
// FUNCTION: STBC 0x004a5260 _$E124
// FUNCTION: STBC 0x004a5270 _$E377
// FUNCTION: STBC 0x004a5280 _$E376
// FUNCTION: STBC 0x004a5290 _$E381
// FUNCTION: STBC 0x004a52a0 _$E380
// FUNCTION: STBC 0x004a52b0 _$E385
// FUNCTION: STBC 0x004a52c0 _$E384
// FUNCTION: STBC 0x004a52d0 _$E131
// FUNCTION: STBC 0x004a52e0 _$E130
// FUNCTION: STBC 0x004a52f0 _$E136
// FUNCTION: STBC 0x004a5300 _$E135
// FUNCTION: STBC 0x004a5310 _$E166
// FUNCTION: STBC 0x004a5320 _$E165
// FUNCTION: STBC 0x004a5360 _$E389
// FUNCTION: STBC 0x004a5370 _$E388
// FUNCTION: STBC 0x004a5380 _$E171
// FUNCTION: STBC 0x004a5390 _$E170
// FUNCTION: STBC 0x004a53a0 _$E393
// FUNCTION: STBC 0x004a53b0 _$E392
// FUNCTION: STBC 0x004a53c0 _$E397
// FUNCTION: STBC 0x004a53d0 _$E396
// FUNCTION: STBC 0x004a53e0 _$E176
// FUNCTION: STBC 0x004a53f0 _$E175
// FUNCTION: STBC 0x004a5400 _$E401
// FUNCTION: STBC 0x004a5410 _$E400
// FUNCTION: STBC 0x004a5420 _$E141
// FUNCTION: STBC 0x004a5430 _$E140
// FUNCTION: STBC 0x004a5440 _$E405
// FUNCTION: STBC 0x004a5450 _$E404
// FUNCTION: STBC 0x004a5460 _$E409
// FUNCTION: STBC 0x004a5470 _$E408
// FUNCTION: STBC 0x004a5480 _$E181
// FUNCTION: STBC 0x004a5490 _$E180
// FUNCTION: STBC 0x004a54a0 _$E186
// FUNCTION: STBC 0x004a54b0 _$E185
// FUNCTION: STBC 0x004a54c0 _$E413
// FUNCTION: STBC 0x004a54d0 _$E412
// FUNCTION: STBC 0x004a54e0 _$E417
// FUNCTION: STBC 0x004a54f0 _$E416
// FUNCTION: STBC 0x004a5500 _$E421
// FUNCTION: STBC 0x004a5510 _$E420
// FUNCTION: STBC 0x004a5520 _$E425
// FUNCTION: STBC 0x004a5530 _$E424
// FUNCTION: STBC 0x004a5540 _$E429
// FUNCTION: STBC 0x004a5550 _$E428
// FUNCTION: STBC 0x004a5560 _$E433
// FUNCTION: STBC 0x004a5570 _$E432
// FUNCTION: STBC 0x004a5580 _$E437
// FUNCTION: STBC 0x004a5590 _$E436
// FUNCTION: STBC 0x004a55a0 _$E441
// FUNCTION: STBC 0x004a55b0 _$E440
// FUNCTION: STBC 0x004a55c0 _$E191
// FUNCTION: STBC 0x004a55d0 _$E190
// FUNCTION: STBC 0x004a55e0 _$E445
// FUNCTION: STBC 0x004a55f0 _$E444
// FUNCTION: STBC 0x004a5600 _$E196
// FUNCTION: STBC 0x004a5610 _$E195
// FUNCTION: STBC 0x004a5620 _$E201
// FUNCTION: STBC 0x004a5630 _$E200
// FUNCTION: STBC 0x004a5640 _$E240
// FUNCTION: STBC 0x004a5650 _$E239
// FUNCTION: STBC 0x004a5660 _$E245
// FUNCTION: STBC 0x004a5670 _$E244
// FUNCTION: STBC 0x004a5680 _$E249
// FUNCTION: STBC 0x004a5690 _$E248

// The handler table's dynamic initialiser, the registration of its exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x004a41f0 _$E451
// FUNCTION: STBC 0x004a4200 _$E447
// FUNCTION: STBC 0x004a4210 _$E450
// FUNCTION: STBC 0x004a4220 _$E448
// DATA: STBC 0x009837f8
TGEventHandlerTable Waypoint::classHandlers;

/*
================
Waypoint::Cast

The object as a Waypoint; NULL when it is none.
================
*/
// FUNCTION: STBC 0x004a4240
Waypoint *Waypoint::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( WAYPOINT_TYPE ) ) {
		return (Waypoint *)object;
	}
	return NULL;
}

/*
================
Waypoint::Waypoint
================
*/
// FUNCTION: STBC 0x004a4270
Waypoint::Waypoint( char *name, char *setName, char *modelName )
	: PlacementObject( name, setName, modelName ), speed( 25.0f ), next( NULL ), prev( NULL ), pathDisplay( NULL ), curve( NULL ) {
}

/*
================
Waypoint::~Waypoint
================
*/
// FUNCTION: STBC 0x004a42b0
Waypoint::~Waypoint( void ) {
	Unlink();
	if ( curve != NULL ) {
		delete curve;
		curve = NULL;
	}
}

/*
================
Waypoint::Create
================
*/
// FUNCTION: STBC 0x004a4320
Waypoint *Waypoint::Create( char *name, char *setName, char *modelName ) {
	return new Waypoint( name, setName, modelName );
}

/*
================
Waypoint::Update
================
*/
// FUNCTION: STBC 0x004a4360
void Waypoint::Update( float time ) {
	PlacementObject::Update( time );
	UpdateCurve();
}

/*
================
Waypoint::SetContainingSet
================
*/
// FUNCTION: STBC 0x004a4380
void Waypoint::SetContainingSet( SetClass *set ) {
	// the display is drawn along the path while the waypoint's set is shown
	if ( pathDisplay != NULL ) {
		delete pathDisplay;
		// the waypoint has no display of its own until its curve is rebuilt
		pathDisplay = NULL;
	}
	PlacementObject::SetContainingSet( set );
}

/*
================
Waypoint::InsertAfterObj

Links waypoint after this one: it takes over this waypoint's next one,
and the waypoint that follows it now points back at it.
================
*/
// FUNCTION: STBC 0x004a43c0
void Waypoint::InsertAfterObj( Waypoint *waypoint ) {
	if ( next == NULL ) {
		waypoint->SetLinks( waypoint->next, this );
	} else {
		waypoint->SetLinks( next, this );
	}
	SetLinks( waypoint, prev );
	if ( waypoint->next != NULL ) {
		waypoint->next->SetLinks( waypoint->next->next, waypoint );
	}
	waypoint->UpdateCurve();
}

/*
================
Waypoint::InsertBefore

Links the named object of the named set before this waypoint. Nothing in
the retail game calls it.
================
*/
// FUNCTION: STBC 0x004a4410 ?InsertBefore@Waypoint@@QAEXPAD0@Z
void Waypoint::InsertBefore( char *setName, char *name ) {
	SetClass *set = g_kSetManager.GetSet( setName );
	if ( set != NULL ) {
		InsertBeforeObj( Waypoint::Cast( PlacementObject::GetObject( set, name ) ) );
	}
}

/*
================
Waypoint::InsertBefore

Links the placement with the ID in the named set before this waypoint.
Nothing in the retail game calls it.
================
*/
// FUNCTION: STBC 0x004a44a0 ?InsertBefore@Waypoint@@QAEXPADJ@Z
void Waypoint::InsertBefore( char *setName, long id ) {
	SetClass *set = g_kSetManager.GetSet( setName );
	if ( set != NULL ) {
		InsertBeforeObj( Waypoint::Cast( PlacementObject::GetObjectByID( set, id ) ) );
	}
}

/*
================
Waypoint::InsertBeforeObj

Links waypoint before this one: it takes over this waypoint's previous
one, and the waypoint before it now points on to it.
================
*/
// FUNCTION: STBC 0x004a4530
void Waypoint::InsertBeforeObj( Waypoint *waypoint ) {
	if ( prev == NULL ) {
		waypoint->SetLinks( this, waypoint->prev );
	} else {
		waypoint->SetLinks( this, prev );
	}
	SetLinks( next, waypoint );
	if ( waypoint->prev != NULL ) {
		waypoint->prev->SetLinks( waypoint, waypoint->prev->prev );
	}
	UpdateCurve();
}

/*
================
Waypoint::GetPathPoint
================
*/
// FUNCTION: STBC 0x004a4580
NiPoint3 Waypoint::GetPathPoint( float t ) {
	if ( curve != NULL ) {
		return curve->Evaluate( t );
	}
	return GetWorldLocation();
}

/*
================
Waypoint::Unlink

Joins the neighbours to each other and clears both links.
================
*/
// FUNCTION: STBC 0x004a45d0
void Waypoint::Unlink( void ) {
	if ( prev != NULL ) {
		prev->SetLinks( next, prev->prev );
	}
	if ( next != NULL ) {
		next->SetLinks( next->next, prev );
	}
	SetLinks( NULL, NULL );
}

/*
================
Waypoint::VirtualSlot68

Writes the line of the saved script that creates the waypoint.
================
*/
// FUNCTION: STBC 0x004a4610
bool Waypoint::VirtualSlot68( FILE *file, char *setName ) {
	fprintf( file, "\t# Position \"%s\"\n", GetName() );
	if ( modelName != NULL ) {
		fprintf( file, "\tkThis = App.Waypoint_Create(\"%s\", sSetName, \"%s\")\n", GetName(), modelName );
	} else {
		fprintf( file, "\tkThis = App.Waypoint_Create(\"%s\", sSetName, None)\n", GetName() );
	}
	return true;
}

/*
================
Waypoint::VirtualSlot69

Writes PlacementObject's lines, then the speed.
================
*/
// FUNCTION: STBC 0x004a4670
bool Waypoint::VirtualSlot69( FILE *file ) {
	bool result = PlacementObject::VirtualSlot69( file );
	if ( result ) {
		fprintf( file, "\tkThis.SetSpeed(%f)\n", speed );
	}
	return result;
}

/*
================
Waypoint::SaveObjectSecondPass

Writes the lines of the saved script that link the next waypoint after
this one, once every waypoint of the set exists.
================
*/
// FUNCTION: STBC 0x004a46b0
bool Waypoint::SaveObjectSecondPass( FILE *file, char *setName ) {
	if ( next != NULL ) {
		fprintf( file, "\t# Attaching object \"%s\" after \"%s\"...\n", next->GetName(), GetName() );
		fprintf( file, "\tkThis = App.Waypoint_Cast( App.PlacementObject_GetObjectBySetName(sSetName, \"%s\") )\n", GetName() );
		fprintf( file, "\tkPrevious = App.Waypoint_Cast( App.PlacementObject_GetObjectBySetName(sSetName, \"%s\") )\n", next->GetName() );
		fprintf( file, "\tkThis.InsertAfterObj( kPrevious )\n" );
		fprintf( file, "\tkThis = kPrevious = None\n" );
	}
	return true;
}

/*
================
Waypoint::WriteToStream

The links are written as the waypoints' IDs.
================
*/
// FUNCTION: STBC 0x004a4730
bool Waypoint::WriteToStream( TGStream *stream ) {
	if ( !PlacementObject::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( speed );
	stream->WriteID( next != NULL ? next->GetObjID() : 0 );
	stream->WriteID( prev != NULL ? prev->GetObjID() : 0 );
	return true;
}

/*
================
Waypoint::ReadFromStream

The links hold the waypoints' IDs until PostFixUp.
================
*/
// FUNCTION: STBC 0x004a47a0
bool Waypoint::ReadFromStream( TGStream *stream ) {
	if ( !PlacementObject::ReadFromStream( stream ) ) {
		return false;
	}
	speed = stream->ReadFloat();
	next = (Waypoint *)stream->ReadID();
	prev = (Waypoint *)stream->ReadID();
	pathDisplay = NULL;
	curve = NULL;
	return true;
}

/*
================
Waypoint::FixUpPointers

Fails while a linked waypoint does not exist yet.
================
*/
// FUNCTION: STBC 0x004a47f0
bool Waypoint::FixUpPointers( void ) {
	if ( !PlacementObject::FixUpPointers() ) {
		return false;
	}
	if ( next != NULL && GetTGObjectPtr( (long)next ) == NULL ) {
		return false;
	}
	if ( prev != NULL && GetTGObjectPtr( (long)prev ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
Waypoint::PostFixUp

Turns the saved IDs back into the waypoints.
================
*/
// FUNCTION: STBC 0x004a4840
void Waypoint::PostFixUp( void ) {
	PlacementObject::PostFixUp();
	next = (Waypoint *)GetTGObjectPtr( (long)next );
	prev = (Waypoint *)GetTGObjectPtr( (long)prev );
	UpdateCurve();
}

/*
================
Waypoint::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x004a4870
void Waypoint::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&Waypoint::HandleSetSpeed, "Waypoint::HandleSetSpeed" );
}

/*
================
Waypoint::RegisterHandlers
================
*/
// FUNCTION: STBC 0x004a4890
void Waypoint::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_SET_WAYPOINT_SPEED, "Waypoint::HandleSetSpeed" );
}

/*
================
Waypoint::HandleSetSpeed

A negative speed stops the waypoint's traffic.
================
*/
// FUNCTION: STBC 0x004a48b0
void Waypoint::HandleSetSpeed( TGEvent *event ) {
	float newSpeed = ( (TGFloatEvent *)event )->GetFloat();
	if ( newSpeed < 0.0f ) {
		speed = 0.0f;
	} else {
		speed = newSpeed;
	}
}

/*
================
Waypoint::SetLinks
================
*/
// FUNCTION: STBC 0x004a48e0
void Waypoint::SetLinks( Waypoint *next_, Waypoint *prev_ ) {
	if ( next_ != next ) {
		next = next_;
		UpdateCurve();
	}
	prev = prev_;
}

/*
================
Waypoint::UpdateCurve

The curve runs from this waypoint to the next one, its inner control
points four speed-scaled steps along each waypoint's forward direction.
While the set is shown and the node isn't culled, the path display follows
the curve; it is rebuilt only when a control point moved.
================
*/
// FUNCTION: STBC 0x004a4910
void Waypoint::UpdateCurve( void ) {
	NiPoint3 points[4];
	if ( next != NULL ) {
		points[0] = GetWorldLocation();
		NiPoint3 offset = speed * GetWorldForward() * 4.0f;
		points[1] = offset + GetWorldLocation();
		NiPoint3 nextOffset = next->speed * next->GetWorldForward() * 4.0f;
		points[2] = next->GetWorldLocation() - nextOffset;
		points[3] = next->GetWorldLocation();

		if ( curve == NULL ) {
			curve = new BezierCurve( 4, points );
		} else {
			NiPoint3 *curvePoints = curve->GetPoints();
			for ( int i = 0; i < 4; i++ ) {
				curvePoints[i] = points[i];
			}
		}
	}

	if ( ( node == NULL || !node->GetAppCulled() ) && GetContainingSet() != NULL ) {
		if ( next != NULL ) {
			if ( pathDisplay != NULL ) {
				NiPoint3 *displayPoints = pathDisplay->GetCurve().GetPoints();
				bool changed = false;
				for ( int i = 0; i < 4; i++ ) {
					if ( displayPoints[i] != points[i] ) {
						changed = true;
						displayPoints[i] = points[i];
					}
				}
				if ( changed ) {
					pathDisplay->Build();
				}
			} else {
				BezierCurve displayCurve( 4, points );
				pathDisplay = new WaypointPathDisplay( GetContainingSet()->GetSceneRoot(), displayCurve );
			}
		} else if ( pathDisplay != NULL ) {
			delete pathDisplay;
			pathDisplay = NULL;
		}
	} else if ( pathDisplay != NULL ) {
		delete pathDisplay;
		pathDisplay = NULL;
	}
}

/*
================
Waypoint::AddToSet
================
*/
// FUNCTION: STBC 0x004a4c70
void Waypoint::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, WAYPOINT_TYPE );
		PlacementObject::AddToSet( set );
	}
}

/*
================
Waypoint::RemoveFromSet
================
*/
// FUNCTION: STBC 0x004a4ca0
void Waypoint::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, WAYPOINT_TYPE );
		PlacementObject::RemoveFromSet( set );
	}
}
