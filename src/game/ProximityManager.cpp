/*
===============================================================================

	ProximityManager.cpp

===============================================================================
*/

#include <iostream>
#include "ProximityManager.h"
#include "TGTrace.h"
#include "ProximityCheck.h"
#include <string.h>
#include "ObjectClass.h"
#include "Torpedo.h"
#include "DamageableObject.h"
#include "CollisionRecorder.h"
#include "ShipClass.h"
#include "MultiplayerGame.h"
#include "TGBufferStream.h"
#include "TGMessage.h"
#include "TGNetwork.h"
#include "UtopiaModule.h"
#include "Planet.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005a7380 _$E69
// FUNCTION: STBC 0x005a7390 _$E68
// FUNCTION: STBC 0x005a73a0 _$E290
// FUNCTION: STBC 0x005a73b0 _$E289
// FUNCTION: STBC 0x005a73c0 _$E80
// FUNCTION: STBC 0x005a73d0 _$E79
// FUNCTION: STBC 0x005a73e0 _$E84
// FUNCTION: STBC 0x005a73f0 _$E83
// FUNCTION: STBC 0x005a7400 _$E88
// FUNCTION: STBC 0x005a7410 _$E87
// FUNCTION: STBC 0x005a7530 _$E92
// FUNCTION: STBC 0x005a7540 _$E91
// FUNCTION: STBC 0x005a78c0 _$E209
// FUNCTION: STBC 0x005a78d0 _$E208
// FUNCTION: STBC 0x005a94b0 _$E151
// FUNCTION: STBC 0x005a94c0 _$E150
// FUNCTION: STBC 0x005a9700 _$E164
// FUNCTION: STBC 0x005a9710 _$E163
// FUNCTION: STBC 0x005a99f0 _$E214
// FUNCTION: STBC 0x005a9a00 _$E213
// FUNCTION: STBC 0x005a9ac0 _$E295
// FUNCTION: STBC 0x005a9ad0 _$E294
// FUNCTION: STBC 0x005a9ae0 _$E218
// FUNCTION: STBC 0x005a9af0 _$E217
// FUNCTION: STBC 0x005a9b00 _$E299
// FUNCTION: STBC 0x005a9b10 _$E298
// FUNCTION: STBC 0x005a9b20 _$E306
// FUNCTION: STBC 0x005a9b30 _$E305
// FUNCTION: STBC 0x005a9bb0 _$E325
// FUNCTION: STBC 0x005a9bc0 _$E324
// FUNCTION: STBC 0x005a9bd0 _$E329
// FUNCTION: STBC 0x005a9be0 _$E328
// FUNCTION: STBC 0x005a9bf0 _$E109
// FUNCTION: STBC 0x005a9c00 _$E108
// FUNCTION: STBC 0x005a9c10 _$E242
// FUNCTION: STBC 0x005a9c20 _$E241
// FUNCTION: STBC 0x005a9c30 _$E249
// FUNCTION: STBC 0x005a9c40 _$E248
// FUNCTION: STBC 0x005a9c50 _$E253
// FUNCTION: STBC 0x005a9c60 _$E252
// FUNCTION: STBC 0x005a9c70 _$E257
// FUNCTION: STBC 0x005a9c80 _$E256
// FUNCTION: STBC 0x005a9ce0 _$E261
// FUNCTION: STBC 0x005a9cf0 _$E260
// FUNCTION: STBC 0x005a9d30 _$E267
// FUNCTION: STBC 0x005a9d40 _$E266
// FUNCTION: STBC 0x005a9d50 _$E333
// FUNCTION: STBC 0x005a9d60 _$E332
// FUNCTION: STBC 0x005a9d70 _$E337
// FUNCTION: STBC 0x005a9d80 _$E336
// FUNCTION: STBC 0x005a9d90 _$E341
// FUNCTION: STBC 0x005a9da0 _$E340
// FUNCTION: STBC 0x005a9db0 _$E345
// FUNCTION: STBC 0x005a9dc0 _$E344
// FUNCTION: STBC 0x005a9dd0 _$E349
// FUNCTION: STBC 0x005a9de0 _$E348
// FUNCTION: STBC 0x005a9df0 _$E353
// FUNCTION: STBC 0x005a9e00 _$E352
// FUNCTION: STBC 0x005a9e10 _$E357
// FUNCTION: STBC 0x005a9e20 _$E356
// FUNCTION: STBC 0x005a9e30 _$E361
// FUNCTION: STBC 0x005a9e40 _$E360
// FUNCTION: STBC 0x005a9eb0 _$E365
// FUNCTION: STBC 0x005a9ec0 _$E364
// FUNCTION: STBC 0x005a9ed0 _$E369
// FUNCTION: STBC 0x005a9ee0 _$E368
// FUNCTION: STBC 0x005a9ef0 _$E114
// FUNCTION: STBC 0x005a9f00 _$E113
// FUNCTION: STBC 0x005a9f10 _$E373
// FUNCTION: STBC 0x005a9f20 _$E372
// FUNCTION: STBC 0x005a9f30 _$E119
// FUNCTION: STBC 0x005a9f40 _$E118
// FUNCTION: STBC 0x005a9f50 _$E377
// FUNCTION: STBC 0x005a9f60 _$E376
// FUNCTION: STBC 0x005a9f70 _$E381
// FUNCTION: STBC 0x005a9f80 _$E380
// FUNCTION: STBC 0x005a9f90 _$E385
// FUNCTION: STBC 0x005a9fa0 _$E384
// FUNCTION: STBC 0x005a9fb0 _$E389
// FUNCTION: STBC 0x005a9fc0 _$E388
// FUNCTION: STBC 0x005a9fd0 _$E128
// FUNCTION: STBC 0x005a9fe0 _$E127
// FUNCTION: STBC 0x005a9ff0 _$E393
// FUNCTION: STBC 0x005aa000 _$E392
// FUNCTION: STBC 0x005aa010 _$E397
// FUNCTION: STBC 0x005aa020 _$E396
// FUNCTION: STBC 0x005aa030 _$E401
// FUNCTION: STBC 0x005aa040 _$E400
// FUNCTION: STBC 0x005aa050 _$E134
// FUNCTION: STBC 0x005aa060 _$E133
// FUNCTION: STBC 0x005aa070 _$E139
// FUNCTION: STBC 0x005aa080 _$E138
// FUNCTION: STBC 0x005aa090 _$E169
// FUNCTION: STBC 0x005aa0a0 _$E168
// FUNCTION: STBC 0x005aa0b0 _$E405
// FUNCTION: STBC 0x005aa0c0 _$E404
// FUNCTION: STBC 0x005aa0d0 _$E174
// FUNCTION: STBC 0x005aa0e0 _$E173
// FUNCTION: STBC 0x005aa0f0 _$E409
// FUNCTION: STBC 0x005aa100 _$E408
// FUNCTION: STBC 0x005aa110 _$E413
// FUNCTION: STBC 0x005aa120 _$E412
// FUNCTION: STBC 0x005aa130 _$E179
// FUNCTION: STBC 0x005aa140 _$E178
// FUNCTION: STBC 0x005aa150 _$E417
// FUNCTION: STBC 0x005aa160 _$E416
// FUNCTION: STBC 0x005aa170 _$E144
// FUNCTION: STBC 0x005aa180 _$E143
// FUNCTION: STBC 0x005aa190 _$E421
// FUNCTION: STBC 0x005aa1a0 _$E420
// FUNCTION: STBC 0x005aa1b0 _$E425
// FUNCTION: STBC 0x005aa1c0 _$E424
// FUNCTION: STBC 0x005aa1d0 _$E184
// FUNCTION: STBC 0x005aa1e0 _$E183
// FUNCTION: STBC 0x005aa1f0 _$E189
// FUNCTION: STBC 0x005aa200 _$E188
// FUNCTION: STBC 0x005aa210 _$E429
// FUNCTION: STBC 0x005aa220 _$E428
// FUNCTION: STBC 0x005aa230 _$E433
// FUNCTION: STBC 0x005aa240 _$E432
// FUNCTION: STBC 0x005aa250 _$E437
// FUNCTION: STBC 0x005aa260 _$E436
// FUNCTION: STBC 0x005aa270 _$E441
// FUNCTION: STBC 0x005aa280 _$E440
// FUNCTION: STBC 0x005aa290 _$E445
// FUNCTION: STBC 0x005aa2a0 _$E444
// FUNCTION: STBC 0x005aa2f0 _$E449
// FUNCTION: STBC 0x005aa300 _$E448
// FUNCTION: STBC 0x005aa310 _$E453
// FUNCTION: STBC 0x005aa320 _$E452
// FUNCTION: STBC 0x005aa330 _$E457
// FUNCTION: STBC 0x005aa340 _$E456
// FUNCTION: STBC 0x005aa350 _$E194
// FUNCTION: STBC 0x005aa360 _$E193
// FUNCTION: STBC 0x005aa370 _$E461
// FUNCTION: STBC 0x005aa380 _$E460
// FUNCTION: STBC 0x005aa390 _$E199
// FUNCTION: STBC 0x005aa3a0 _$E198
// FUNCTION: STBC 0x005aa3b0 _$E204
// FUNCTION: STBC 0x005aa3c0 _$E203
// FUNCTION: STBC 0x005aa500 _$E465
// FUNCTION: STBC 0x005aa510 _$E464
// FUNCTION: STBC 0x005aa520 _$E470
// FUNCTION: STBC 0x005aa530 _$E469
// FUNCTION: STBC 0x005aa540 _$E474
// FUNCTION: STBC 0x005aa550 _$E473

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x005a7300 _$E32
// FUNCTION: STBC 0x005a7310 _$E29
// FUNCTION: STBC 0x005a7320 _$E31
// FUNCTION: STBC 0x005a7330 _$E30
// FUNCTION: STBC 0x005a7340 _$E38
// FUNCTION: STBC 0x005a7350 _$E35
// FUNCTION: STBC 0x005a7360 _$E37
// FUNCTION: STBC 0x005a7370 _$E36

// The <vector> and <list> instances retail kept from this unit (ObjectIterator's
// push_back, the touching list's destructor and erase (SortAxis calls it), and the node and element copies of
// its push_back).
// FUNCTION: STBC 0x005a9b40 ?size@?$vector@PAVObjectClass@@V?$allocator@PAVObjectClass@@@std@@@std@@QBEIXZ
// FUNCTION: STBC 0x005a9b60 ??1?$list@UProximityPair@@V?$allocator@UProximityPair@@@std@@@std@@QAE@XZ
// FUNCTION: STBC 0x005a9c90 ?_Destroy@?$vector@PAVObjectClass@@V?$allocator@PAVObjectClass@@@std@@@std@@IAEXPAPAVObjectClass@@0@Z
// FUNCTION: STBC 0x005a9ca0 ?erase@?$list@UProximityPair@@V?$allocator@UProximityPair@@@std@@@std@@QAE?AViterator@12@V312@@Z
// FUNCTION: STBC 0x005a9d00 ?_Buynode@?$list@UProximityPair@@V?$allocator@UProximityPair@@@std@@@std@@IAEPAU_Node@12@PAU312@0@Z
// FUNCTION: STBC 0x005a9e50 ?_Ucopy@?$vector@PAVObjectClass@@V?$allocator@PAVObjectClass@@@std@@@std@@IAEPAPAVObjectClass@@PBQAV3@0PAPAV3@@Z
// FUNCTION: STBC 0x005a9e80 ?_Ufill@?$vector@PAVObjectClass@@V?$allocator@PAVObjectClass@@@std@@@std@@IAEXPAPAVObjectClass@@IABQAV3@@Z
// FUNCTION: STBC 0x005aa3d0 ?_Construct@std@@YAXPAUProximityPair@@ABU2@@Z
// FUNCTION: STBC 0x005aa3f0 ?_Construct@std@@YAXPAPAVObjectClass@@ABQAV2@@Z

// A static of an included header: its initialiser with a guard byte, and the exit-time registration.
// FUNCTION: STBC 0x005aa4d0 _$E486
// FUNCTION: STBC 0x005aa4f0 _$E485

// DATA: STBC 0x008e5f58
bool ProximityManager::playerCollisionsEnabled = true;

// DATA: STBC 0x008e5f59
bool ProximityManager::multiplayerPlayerCollisionsEnabled = true;

/*
================
ProximityManager::ProximityManager
================
*/
// FUNCTION: STBC 0x005a7420
// PARKED: schedule: stores of the axis numbers 1 and 2 are scheduled after the owner stores (97.5)
ProximityManager::ProximityManager( void ) : set( NULL ), pairCounts( 0x80 ) {
	axes[0].Reserve( 0x100 );
	axes[1].Reserve( 0x100 );
	axes[2].Reserve( 0x100 );
	axes[0].axis = 0;
	axes[1].axis = 1;
	axes[2].axis = 2;
	axes[0].owner = this;
	axes[1].owner = this;
	axes[2].owner = this;
}

/*
================
ProximityAxis::ProximityAxis
================
*/
// FUNCTION: STBC 0x005a7510
ProximityAxis::ProximityAxis( void ) {
	endpoints = NULL;
	numEndpoints = 0;
	capacity = 0;
}

/*
================
ProximityAxis::~ProximityAxis
================
*/
// FUNCTION: STBC 0x005a7520
ProximityAxis::~ProximityAxis( void ) {
	delete endpoints;
}

/*
================
ProximityAxis::operator[]

The endpoint at index; an index out of range aborts.
================
*/
inline ProximityEndpoint &ProximityAxis::operator[]( int index ) {
	if ( index < 0 || index >= numEndpoints ) {
		abort();
	}
	return endpoints[index];
}

/*
================
ProximityManager::~ProximityManager

Removes the objects the table holds.
================
*/
// FUNCTION: STBC 0x005a7550
ProximityManager::~ProximityManager( void ) {
	for ( int i = 0; i < table.numEntries; i++ ) {
		ProximityEntry *entry = &table.entries[i];
		if ( entry->object != NULL ) {
			RemoveObject( entry->object );
		}
	}
}

/*
================
ProximityManager::AddObject

An object that is in the manager (its collision flags say so) and not yet in
the table gets an entry and its two endpoints on each axis. A proximity check
learns its manager.
================
*/
// FUNCTION: STBC 0x005a7640
void ProximityManager::AddObject( ObjectClass *object ) {
	// the lower and the upper end, as a table entry keeps them per axis
	ProximityEndpoint endpoints[2];
	NiPoint3 minimum, maximum;
	if ( ( object->GetCollisionFlags() & ObjectClass::CFB_IN_PROXIMITY_MANAGER ) && table.Find( object ) == -1 ) {
		int index = table.Add( object );
		if ( index >= pairCounts.numEntries ) {
			pairCounts.Resize( pairCounts.numEntries + 0x20 );
		}
		object->GetExtents( &minimum, &maximum );
		for ( int i = 0; i < 3; i++ ) {
			endpoints[0].value = minimum[i];
			endpoints[0].isMin = true;
			endpoints[0].entry = index;
			endpoints[1].value = maximum[i];
			endpoints[1].isMin = false;
			endpoints[1].entry = index;
			axes[i].Unknown5A8CC0( endpoints[0], endpoints[1], &pairCounts, &table, &touching );
		}
		if ( object->IsTypeOf( PROXIMITYCHECK_TYPE ) ) {
			( (ProximityCheck *)object )->SetManager( this );
		}
	}
}

/*
================
ProximityManager::RemoveObject

Takes the object's endpoints out of the axes, forgets the pairs it was in, and
empties its entry.
================
*/
// FUNCTION: STBC 0x005a7720
// MATCH-PENDING: R17, exact only through the invented ProximityPairCounts::GetCount/SetCount (no evidence beyond codegen shape)
void ProximityManager::RemoveObject( ObjectClass *object ) {
	if ( !( object->GetCollisionFlags() & ObjectClass::CFB_IN_PROXIMITY_MANAGER ) ) {
		return;
	}
	int index = table.Find( object );
	if ( index == -1 ) {
		return;
	}
	ProximityEntry entry = table.entries[index];
	for ( int axisIndex = 0; axisIndex < 3; axisIndex++ ) {
		axes[axisIndex].Remove( entry.endpoints[axisIndex][0], entry.endpoints[axisIndex][1], &table );
	}
	for ( int i = 0; i < pairCounts.numEntries; i++ ) {
		if ( index != i ) {
			if ( pairCounts.GetCount( index, i ) == 3 ) {
				ObjectClass *other = table.entries[i].object;
				if ( ObjectsInteract( object, other ) ) {
					touching.remove( ProximityPair( object, other ) );
				}
			}
			pairCounts.SetCount( index, i, 0 );
		}
	}
	table.Remove( index, this );
}

/*
================
ProximityManager::ObjectsInteract

Whether either object's collision groups include the other's.
================
*/
// FUNCTION: STBC 0x005a7890
bool ProximityManager::ObjectsInteract( ObjectClass *a, ObjectClass *b ) {
	unsigned char flagsB = b->GetCollisionFlags();
	unsigned char flagsA = a->GetCollisionFlags();
	if ( ( flagsB >> 1 ) & flagsA & ObjectClass::CFB_MEMBER_MASK ) {
		return true;
	}
	if ( ( flagsA >> 1 ) & flagsB & ObjectClass::CFB_MEMBER_MASK ) {
		return true;
	}
	return false;
}

/*
================
ProximityManager::GetNearObjects

Each axis's lower ends counts the objects whose extent along it comes within
radius of the point (a planet's atmosphere widens it when includeAtmospheres
is set); those near on all three axes are then measured by their bounds.
ProximityChecks are left out.
================
*/
// FUNCTION: STBC 0x005a78e0
ProximityManager::ObjectIterator *ProximityManager::GetNearObjects( const NiPoint3 &center, float radius, bool includeAtmospheres ) {
	ObjectIterator *iterator = new ObjectIterator;
	ProximityCandidate *candidates = new ProximityCandidate[axes[0].numEndpoints];
	int numCandidates = 0;

	for ( int axisIndex = 0; axisIndex < 3; axisIndex++ ) {
		ProximityAxis &axis = axes[axisIndex];
		float coordinate = center[axisIndex];
		for ( int i = 0; i < axis.numEndpoints; i++ ) {
			ProximityEndpoint endpoint = axis.endpoints[i];
			if ( table.entries[endpoint.entry].object->IsTypeOf( PROXIMITYCHECK_TYPE ) ) {
				continue;
			}
			float atmosphere = 0.0f;
			if ( includeAtmospheres ) {
				Planet *planet = Planet::Cast( table.entries[endpoint.entry].object );
				if ( planet != NULL ) {
					atmosphere = planet->GetAtmosphereRadius();
				}
			}
			if ( !endpoint.isMin ) {
				continue;
			}
			float upper = axes[axisIndex].endpoints[table.entries[endpoint.entry].endpoints[axisIndex][1]].value;
			float distance = 0.0f;
			if ( coordinate < endpoint.value ) {
				distance = endpoint.value - coordinate;
			} else if ( coordinate > upper ) {
				distance = coordinate - upper;
			}
			if ( distance < atmosphere + radius ) {
				AddCandidate( candidates, &numCandidates, table.entries[endpoint.entry].object );
			}
		}
	}

	for ( int i = 0; i < numCandidates; i++ ) {
		if ( candidates[i].count != 3 ) {
			continue;
		}
		float distance = ( candidates[i].object->GetWorldLocation() - center ).Length();
		ObjectClass *object = candidates[i].object;
		float atmosphere = 0.0f;
		if ( includeAtmospheres ) {
			Planet *planet = Planet::Cast( object );
			if ( planet != NULL ) {
				atmosphere = planet->GetAtmosphereRadius();
			}
		}
		if ( distance - object->GetWorldBound().GetRadius() <= atmosphere + radius ) {
			iterator->objects.push_back( candidates[i].object );
		}
	}

	iterator->index = 0;
	delete candidates;
	return iterator;
}

// GetLineIntersectObjects's per-entry counts, kept between queries and grown
// to the table's size (invented names). Not static: retail reloads the
// counts after growing them instead of keeping the new block.
char *			lineCounts = NULL;
int				lineCountsSize = 0;

// Larger than any extent of the box a segment spans.
static const float	LARGE_EXTENT = 1e20f;

/*
================
ProximityManager::GetLineIntersectObjects

Along the axis on which the segment's box is thinnest, counts the objects
whose extent overlaps the box (lower ends below its top add one, upper ends
below its bottom take one off); those left are measured by their bounds
against the closest point of the segment. ProximityChecks are left out.
================
*/
// FUNCTION: STBC 0x005a7cf0
// DRAFT: x87 order of the direction (retail stores y and z, keeps x) and the inlined push_back: retail also expands two of insert's size() calls (about 54 more budget units at the insert)
ProximityManager::ObjectIterator *ProximityManager::GetLineIntersectObjects( const NiPoint3 &start, const NiPoint3 &end, float radius, bool includeAtmospheres ) {
	if ( lineCountsSize < table.numEntries ) {
		if ( lineCounts != NULL ) {
			delete lineCounts;
		}
		lineCounts = new char[table.numEntries];
		lineCountsSize = table.numEntries;
	}
	char *counts = lineCounts;
	memset( counts, 0, table.numEntries );

	ObjectIterator *iterator = new ObjectIterator;

	NiPoint3 minimum, maximum;
	GetSegmentBounds( start, end, &minimum, &maximum );
	NiPoint3 extent = maximum - minimum;
	int thinnest = 0;
	float least = LARGE_EXTENT;
	for ( int axisIndex = 0; axisIndex < 3; axisIndex++ ) {
		if ( extent[axisIndex] < least ) {
			least = extent[axisIndex];
			thinnest = axisIndex;
		}
	}

	for ( int i = 0; i < axes[thinnest].numEndpoints; i++ ) {
		ProximityEndpoint &endpoint = axes[thinnest].endpoints[i];
		ObjectClass *object = table.entries[endpoint.entry].object;
		float atmosphere = 0.0f;
		if ( includeAtmospheres ) {
			Planet *planet = Planet::Cast( object );
			if ( planet != NULL ) {
				atmosphere = planet->GetAtmosphereRadius();
			}
		}
		if ( endpoint.isMin ) {
			if ( endpoint.value - atmosphere <= maximum[thinnest] && !object->IsTypeOf( PROXIMITYCHECK_TYPE ) ) {
				counts[endpoint.entry]++;
			}
		} else {
			if ( atmosphere + endpoint.value < minimum[thinnest] && !object->IsTypeOf( PROXIMITYCHECK_TYPE ) ) {
				counts[endpoint.entry]--;
			}
		}
	}

	NiPoint3 direction = end - start;
	float length = direction.Unitize();

	for ( int entry = 0; entry < table.numEntries; entry++ ) {
		if ( counts[entry] == 0 ) {
			continue;
		}
		ObjectClass *object = table.entries[entry].object;
		float atmosphere = 0.0f;
		if ( includeAtmospheres ) {
			Planet *planet = Planet::Cast( object );
			if ( planet != NULL ) {
				atmosphere = planet->GetAtmosphereRadius();
			}
		}
		NiBound &bound = object->GetWorldBound();
		float along = ( bound.GetCenter() - start ) * direction;
		NiPoint3 closest;
		if ( along >= 0.0f ) {
			if ( along <= length ) {
				closest = start + direction * along;
			} else {
				closest = end;
			}
		} else {
			closest = start;
		}
		float reach = bound.GetRadius() + atmosphere + radius;
		if ( ( bound.GetCenter() - closest ).SqrLength() <= reach * reach ) {
			iterator->objects.push_back( object );
		}
	}

	iterator->index = 0;
	return iterator;
}

/*
================
ProximityManager::GetSegmentBounds
================
*/
// FUNCTION: STBC 0x005a8290
void ProximityManager::GetSegmentBounds( const NiPoint3 &start, const NiPoint3 &end, NiPoint3 *minimum, NiPoint3 *maximum ) {
	if ( start.x <= end.x ) {
		minimum->x = start.x;
		maximum->x = end.x;
	} else {
		minimum->x = end.x;
		maximum->x = start.x;
	}
	if ( start.y <= end.y ) {
		minimum->y = start.y;
		maximum->y = end.y;
	} else {
		minimum->y = end.y;
		maximum->y = start.y;
	}
	if ( start.z <= end.z ) {
		minimum->z = start.z;
		maximum->z = end.z;
	} else {
		minimum->z = end.z;
		maximum->z = start.z;
	}
}

/*
================
ProximityManager::GetNextObject

The next object of the query's result; NULL after the last.
================
*/
// FUNCTION: STBC 0x005a8320
ObjectClass *ProximityManager::GetNextObject( ObjectIterator *iterator ) {
	ObjectClass *object = NULL;
	if ( iterator->index < iterator->objects.size() ) {
		object = iterator->objects[iterator->index++];
	}
	return object;
}

/*
================
ProximityManager::EndObjectIteration
================
*/
// FUNCTION: STBC 0x005a8350
void ProximityManager::EndObjectIteration( ObjectIterator *iterator ) {
	delete iterator;
}

/*
================
ProximityManager::Update

Moves every object's endpoints, sorts the axes, and lets the pairs that touch collide.
================
*/
// FUNCTION: STBC 0x005a83a0
// DRAFT: retail keeps the index in ebp and the byte offset in ebx, ours the reverse (70.4)
void ProximityManager::Update( void ) {
	for ( int i = 0; i < table.numEntries; i++ ) {
		ObjectClass *object = table.GetObject( i );
		if ( object != NULL ) {
			if ( object->IsTypeOf( PROXIMITYCHECK_TYPE ) && object->GetContainingSet() == NULL ) {
				object->UpdateNodeOnly();
			}
			UpdateEntryBounds( i );
		}
	}
	SortAxis( 0 );
	SortAxis( 1 );
	SortAxis( 2 );
	CollideTouching();
}

/*
================
ProximityManager::UpdateObject

Moves one object's endpoints to where its extents now are, then sorts the axes.
================
*/
// FUNCTION: STBC 0x005a8420
void ProximityManager::UpdateObject( ObjectClass *object ) {
	for ( int i = 0; i < table.numEntries; i++ ) {
		if ( table.entries[i].object == object ) {
			UpdateEntryBounds( i );
			break;
		}
	}
	SortAxis( 0 );
	SortAxis( 1 );
	SortAxis( 2 );
}

/*
================
ProximityManager::UpdateEntryBounds

Moves the object's endpoints on each axis to where its extents now are.
================
*/
// FUNCTION: STBC 0x005a8470
// DRAFT: esi and edi swap roles: retail keeps this in esi and the entry pointer in edi (88.5)
void ProximityManager::UpdateEntryBounds( int index ) {
	NiPoint3 minimum, maximum;
	ProximityEntry *entry = &table.entries[index];
	entry->object->GetExtents( &minimum, &maximum );
	for ( int i = 0; i < 3; i++ ) {
		axes[i][entry->endpoints[i][0]].value = minimum[i];
		axes[i][entry->endpoints[i][1]].value = maximum[i];
	}
}

/*
================
ProximityManager::SortAxis

Bubbles the axis's endpoints into order. When an upper endpoint passes a lower
one the two objects begin to overlap on that axis; when a lower endpoint passes
an upper one they stop. A pair that overlaps on all three axes and can collide
goes on the list of touching pairs, and comes off it when the overlap ends.
================
*/
// FUNCTION: STBC 0x005a8500
// DRAFT: inline budget: retail calls list::erase out of line (0x5A9CA0) inside remove; c2trace --whatif: three more free sites after remove flip exactly that
void ProximityManager::SortAxis( int axisIndex ) {
	ProximityAxis &axis = axes[axisIndex];
	int i = 0;
	bool swapped;
	do {
		int restart = axis.numEndpoints;
		swapped = false;
		for ( ; i + 1 < axis.numEndpoints; i++ ) {
			if ( axis.endpoints[i].value > axis.endpoints[i + 1].value ) {
				axis.Swap( i, i + 1, &table );
				swapped = true;
				ProximityEndpoint &lower = axis.endpoints[i];
				ProximityEndpoint &upper = axis.endpoints[i + 1];
				if ( upper.isMin ) {
					if ( !lower.isMin && lower.entry != upper.entry && pairCounts.Decrement( upper.entry, lower.entry ) == 2 ) {
						ObjectClass *a = table.entries[upper.entry].object;
						ObjectClass *b = table.entries[lower.entry].object;
						if ( ObjectsInteract( a, b ) ) {
							touching.remove( ProximityPair( a, b ) );
						}
					}
				} else if ( lower.isMin && lower.entry != upper.entry && pairCounts.Increment( upper.entry, lower.entry ) == 3 ) {
					ObjectClass *a = table.entries[upper.entry].object;
					ObjectClass *b = table.entries[lower.entry].object;
					if ( ObjectsInteract( a, b ) ) {
						touching.push_back( ProximityPair( a, b ) );
					}
				}
				if ( i < restart ) {
					restart = i;
				}
			}
		}
		if ( restart > 0 ) {
			restart--;
		}
		i = restart;
	} while ( swapped );
}

/*
================
ProximityManager::CollideTouching
================
*/
// FUNCTION: STBC 0x005a8740
void ProximityManager::CollideTouching( void ) {
	for ( std::list<ProximityPair>::iterator it = touching.begin(); it != touching.end(); ++it ) {
		CollidePair( it->first, it->second );
	}
}

/*
================
ProximityManager::DumpCollisions

Traces each pair of objects in contact; the trace is compiled out of the
release build, leaving the walk.
================
*/
// FUNCTION: STBC 0x005a8770
void ProximityManager::DumpCollisions( void ) {
	std::list<ProximityPair>::iterator it;
	for ( it = touching.begin(); it != touching.end(); ++it ) {
		TGTRACE( ( TGTRACE_TYPE_DEBUG, "%s touching %s\n", ( *it ).first->GetName(), ( *it ).second->GetName() ) );
	}
}

/*
================
ProximityManager::AddCandidate

A binary search by object ID.
================
*/
// FUNCTION: STBC 0x005a8780
void ProximityManager::AddCandidate( ProximityCandidate *candidates, int *numCandidates, ObjectClass *object ) {
	int low = 0;
	int high = *numCandidates;
	while ( low != high ) {
		int middle = ( low + high ) / 2;
		long middleID = candidates[middle].object->GetObjID();
		if ( object->GetObjID() > middleID ) {
			low = middle + 1;
		} else if ( object->GetObjID() < middleID ) {
			high = middle;
		} else {
			candidates[middle].count++;
			break;
		}
	}
	if ( low == high ) {
		memmove( &candidates[low + 1], &candidates[low], ( *numCandidates - low ) * sizeof( ProximityCandidate ) );
		candidates[low].object = object;
		candidates[low].count = 1;
		( *numCandidates )++;
	}
}

/*
================
ProximityManager::CollidePair

The two objects' extents overlap on all three axes: a proximity check looks at
the other object, a torpedo checks whether it hit it, and two damageable objects
collide.
================
*/
// FUNCTION: STBC 0x005a8810
void ProximityManager::CollidePair( ObjectClass *a, ObjectClass *b ) {
	if ( a->IsTypeOf( PROXIMITYCHECK_TYPE ) ) {
		( (ProximityCheck *)a )->CheckProximity( b );
		return;
	}
	if ( b->IsTypeOf( PROXIMITYCHECK_TYPE ) ) {
		( (ProximityCheck *)b )->CheckProximity( a );
		return;
	}
	if ( a->IsTypeOf( TORPEDO_TYPE ) || b->IsTypeOf( TORPEDO_TYPE ) ) {
		Torpedo *torpedo;
		ObjectClass *target;
		if ( a->IsTypeOf( TORPEDO_TYPE ) ) {
			torpedo = (Torpedo *)a;
			target = b;
		} else {
			torpedo = (Torpedo *)b;
			target = a;
		}
		torpedo->DetectCollision( target );
		return;
	}
	if ( a->IsTypeOf( DAMAGEABLEOBJECT_TYPE ) && b->IsTypeOf( DAMAGEABLEOBJECT_TYPE ) ) {
		CollideDamageables( (DamageableObject *)a, (DamageableObject *)b );
	}
}

// The squared speed at or below which CollideDamageables counts an object as
// at rest (invented name). A literal, not a const object: retail reads the
// shared 1e-7f constant (0x8942DC) that NiTriShape::NonDegenerateTriangle's
// literal reads too.
#define RESTING_SPEED	1.0e-7f

/*
================
ProximityManager::CollideDamageables

Two damageable objects touch: unless player collisions are off and one is a
player's ship, or neither moves and they didn't touch before, each tests its
model against the other's, and when they hit each takes the collision with
both objects' motion recorded.
================
*/
// FUNCTION: STBC 0x005a88e0
void ProximityManager::CollideDamageables( DamageableObject *a, DamageableObject *b ) {
	if ( !a->CanCollide( b ) ) {
		return;
	}
	bool collisionsEnabled;
	if ( g_kUtopiaModule.IsMultiplayer() ) {
		collisionsEnabled = multiplayerPlayerCollisionsEnabled;
	} else {
		collisionsEnabled = playerCollisionsEnabled;
	}
	if ( !collisionsEnabled ) {
		ShipClass *ship = ShipClass::Cast( a );
		if ( ship != NULL && ship->IsPlayerShip() ) {
			return;
		}
		ship = ShipClass::Cast( b );
		if ( ship != NULL && ship->IsPlayerShip() ) {
			return;
		}
	}
	if ( a->GetVelocity().SqrLength() <= RESTING_SPEED && b->GetVelocity().SqrLength() <= RESTING_SPEED ) {
		if ( a->GetAngularVelocity().SqrLength() <= RESTING_SPEED ) {
			if ( b->GetAngularVelocity().SqrLength() <= RESTING_SPEED && !a->WasTouching( b ) ) {
				return;
			}
		}
	}

	CollisionRecorder recorder;
	if ( a->VirtualSlot82( b, &recorder ) ) {
		CollisionState state;
		state.mass = a->GetMass();
		state.velocity = a->GetVelocity();
		a->SetAngularDirectionType( 1 );
		state.angularVelocity = a->GetAngularVelocity();
		recorder.SetState( 0, state );

		state.mass = b->GetMass();
		state.velocity = b->GetVelocity();
		b->SetAngularDirectionType( 1 );
		state.angularVelocity = b->GetAngularVelocity();
		recorder.SetState( 1, state );

		a->VirtualSlot84( &recorder, b, 0 );
		b->VirtualSlot84( &recorder, a, 1 );
	}
}

/*
================
ProximityAxis::Unknown5A8CC0

Puts an object's two endpoints in their places on the axis. The objects
whose extents are open at the lower endpoint, and those whose lower
endpoints fall between the two, now overlap the object on this axis: each
pair's count goes up, and a pair that now overlaps on all three axes and
can collide goes on the touching list.
================
*/
// FUNCTION: STBC 0x005a8cc0
// DRAFT: inline budget: retail calls NiTList<int>'s NewItem (0x486880) out of line (GetNext, 0x55B7C0, is, with the single-statement bodies unbraced), and the first push_back's _Buynode (0x5A9D00) and _Construct (0x5AA3D0); this build expands them. RemovePos (0x5AA410) is out of line through NiTList::Remove, as in retail. The searches, the open list and the two counting loops agree
void ProximityAxis::Unknown5A8CC0( const ProximityEndpoint &lower, const ProximityEndpoint &upper, ProximityPairCounts *counts, ProximityObjectTable *table, std::list<ProximityPair> *touching ) {
	if ( numEndpoints + 2 > capacity )
		Reserve( capacity + 0x40 );

	int low = 0;
	int high = numEndpoints;
	while ( low != high ) {
		int middle = ( low + high ) / 2;
		if ( lower.value > endpoints[middle].value )
			low = middle + 1;
		else if ( lower.value < endpoints[middle].value )
			high = middle;
		else {
			low = middle;
			break;
		}
	}
	int lowerIndex = low;
	Insert( lowerIndex, lower, table );

	low = lowerIndex + 1;
	high = numEndpoints;
	while ( low != high ) {
		int middle = ( low + high ) / 2;
		if ( upper.value > endpoints[middle].value )
			low = middle + 1;
		else if ( upper.value < endpoints[middle].value )
			high = middle;
		else {
			low = middle;
			break;
		}
	}
	int upperIndex = low;
	Insert( upperIndex, upper, table );

	// the objects whose extents are open at the lower endpoint
	NiTList<int> open( 2 );
	int i;
	for ( i = 0; i < lowerIndex; i++ ) {
		ProximityEndpoint &endpoint = endpoints[i];
		if ( endpoint.isMin )
			open.AddTail( endpoint.entry );
		else
			open.Remove( endpoint.entry );
	}

	int entry = lower.entry;
	NiTListIterator pos = open.GetHeadPos();
	while ( pos != 0 ) {
		int other = open.GetNext( pos );
		if ( counts->Increment( other, entry ) == 3 ) {
			ObjectClass *a = table->entries[entry].object;
			ObjectClass *b = table->entries[other].object;
			if ( ProximityManager::ObjectsInteract( a, b ) )
				touching->push_back( ProximityPair( a, b ) );
		}
	}

	// the objects whose lower endpoints fall inside the extent
	if ( upperIndex - lowerIndex - 1 > 0 ) {
		for ( i = lowerIndex + 1; i < upperIndex; i++ ) {
			if ( endpoints[i].isMin ) {
				int other = endpoints[i].entry;
				if ( counts->Increment( entry, other ) == 3 ) {
					ObjectClass *a = table->entries[entry].object;
					ObjectClass *b = table->entries[other].object;
					if ( ProximityManager::ObjectsInteract( a, b ) )
						touching->push_back( ProximityPair( a, b ) );
				}
			}
		}
	}
}

/*
================
ProximityAxis::Insert

Puts the endpoint at index, moving the ones after it up.
================
*/
// FUNCTION: STBC 0x005a90a0
void ProximityAxis::Insert( int index, const ProximityEndpoint &endpoint, ProximityObjectTable *table ) {
	if ( index < numEndpoints ) {
		memmove( &endpoints[index + 1], &endpoints[index], ( numEndpoints - index ) * sizeof( ProximityEndpoint ) );
	}
	numEndpoints++;
	endpoints[index] = endpoint;
	for ( ; index < numEndpoints; index++ ) {
		UpdateEntry( index, table );
	}
}

/*
================
ProximityAxis::Remove

Takes out the endpoints at lower and upper (lower < upper), moving the ones after them down.
================
*/
// FUNCTION: STBC 0x005a9120
void ProximityAxis::Remove( int lower, int upper, ProximityObjectTable *table ) {
	if ( upper - lower - 1 > 0 ) {
		memmove( &endpoints[lower], &endpoints[lower + 1], ( upper - lower - 1 ) * sizeof( ProximityEndpoint ) );
	}
	memmove( &endpoints[upper - 1], &endpoints[upper + 1], ( numEndpoints - upper - 1 ) * sizeof( ProximityEndpoint ) );
	numEndpoints -= 2;
	for ( int i = lower; i < numEndpoints; i++ ) {
		UpdateEntry( i, table );
	}
}

/*
================
ProximityAxis::RemoveAt

Takes out the endpoint at index, moving the ones after it down (invented
name; nothing in retail calls it).
================
*/
// FUNCTION: STBC 0x005a91b0
void ProximityAxis::RemoveAt( int index ) {
	numEndpoints--;
	if ( index < numEndpoints ) {
		memmove( &endpoints[index], &endpoints[index + 1], ( numEndpoints - index ) * sizeof( ProximityEndpoint ) );
	}
}

/*
================
ProximityAxis::UpdateEntry

Tells the entry of the object whose endpoint is at index where the endpoint now is.
================
*/
// FUNCTION: STBC 0x005a91f0
void ProximityAxis::UpdateEntry( int index, ProximityObjectTable *table ) {
	if ( endpoints[index].isMin ) {
		table->entries[endpoints[index].entry].endpoints[axis][0] = index;
	} else {
		table->entries[endpoints[index].entry].endpoints[axis][1] = index;
	}
}

/*
================
ProximityAxis::Swap
================
*/
// FUNCTION: STBC 0x005a9250
void ProximityAxis::Swap( int a, int b, ProximityObjectTable *table ) {
	ProximityEndpoint temp = endpoints[a];
	endpoints[a] = endpoints[b];
	endpoints[b] = temp;
	UpdateEntry( a, table );
	UpdateEntry( b, table );
}

/*
================
ProximityAxis::Reserve

Makes room for capacity endpoints, keeping the ones it holds.
================
*/
// FUNCTION: STBC 0x005a92e0
void ProximityAxis::Reserve( int newCapacity ) {
	if ( capacity == 0 ) {
		endpoints = (ProximityEndpoint *)operator new( newCapacity * sizeof( ProximityEndpoint ) );
		capacity = newCapacity;
		return;
	}
	ProximityEndpoint *block = (ProximityEndpoint *)operator new( newCapacity * sizeof( ProximityEndpoint ) );
	memcpy( block, endpoints, numEndpoints * sizeof( ProximityEndpoint ) );
	delete endpoints;
	capacity = newCapacity;
	endpoints = block;
}

/*
================
ProximityPair::ProximityPair
================
*/
// FUNCTION: STBC 0x005a9360
ProximityPair::ProximityPair( ObjectClass *a, ObjectClass *b ) {
	if ( a->GetObjID() < b->GetObjID() ) {
		first = a;
		second = b;
	} else {
		first = b;
		second = a;
	}
}

/*
================
ProximityPair::operator==
================
*/
// FUNCTION: STBC 0x005a9390 ??8ProximityPair@@QBE_NABU0@@Z
bool ProximityPair::operator==( const ProximityPair &other ) const {
	return first->GetObjID() == other.first->GetObjID() && second->GetObjID() == other.second->GetObjID();
}

/*
================
ProximityPair::operator>

By the first objects' IDs, then the second ones' (nothing in retail calls
it, nor operator<).
================
*/
// FUNCTION: STBC 0x005a93d0 ??OProximityPair@@QBE_NABU0@@Z
bool ProximityPair::operator>( const ProximityPair &other ) const {
	if ( first->GetObjID() == other.first->GetObjID() && second->GetObjID() > other.second->GetObjID() ) {
		return true;
	}
	return first->GetObjID() > other.first->GetObjID();
}

/*
================
ProximityPair::operator<
================
*/
// FUNCTION: STBC 0x005a9410 ??MProximityPair@@QBE_NABU0@@Z
bool ProximityPair::operator<( const ProximityPair &other ) const {
	if ( first->GetObjID() == other.first->GetObjID() && second->GetObjID() < other.second->GetObjID() ) {
		return true;
	}
	return first->GetObjID() < other.first->GetObjID();
}

/*
================
ProximityObjectTable::ProximityObjectTable
================
*/
// FUNCTION: STBC 0x005a9450
ProximityObjectTable::ProximityObjectTable( void ) {
	numEntries = 0x80;
	entries = new ProximityEntry[numEntries];
	memset( entries, 0, numEntries * sizeof( ProximityEntry ) );
}

/*
================
ProximityObjectTable::~ProximityObjectTable

A proximity check in the table is deleted with it.
================
*/
// FUNCTION: STBC 0x005a94d0
ProximityObjectTable::~ProximityObjectTable( void ) {
	for ( int i = 0; i < numEntries; i++ ) {
		ProximityCheck *check = (ProximityCheck *)entries[i].object;
		if ( check != NULL && check->IsTypeOf( PROXIMITYCHECK_TYPE ) ) {
			check->SetManager( NULL );
			check->RemoveAndDelete();
		}
		entries[i].object = NULL;
	}
	delete entries;
}

/*
================
ProximityObjectTable::Add

Puts the object in the first free entry, growing the table by 0x20 entries
when there is none; returns the entry's index.
================
*/
// FUNCTION: STBC 0x005a9530
int ProximityObjectTable::Add( ObjectClass *object ) {
	int index;
	for ( index = 0; index < numEntries; index++ ) {
		if ( entries[index].object == NULL ) {
			break;
		}
	}
	if ( index >= numEntries ) {
		ProximityEntry *block = new ProximityEntry[numEntries + 0x20];
		memcpy( block, entries, numEntries * sizeof( ProximityEntry ) );
		memset( block + numEntries, 0, 0x20 * sizeof( ProximityEntry ) );
		delete entries;
		entries = block;
		numEntries += 0x20;
	}
	entries[index].object = object;
	return index;
}

/*
================
ProximityObjectTable::Find

The index of the object's entry, or -1.
================
*/
// FUNCTION: STBC 0x005a9610
int ProximityObjectTable::Find( ObjectClass *object ) {
	for ( int i = 0; i < numEntries; i++ ) {
		if ( entries[i].object == object ) {
			return i;
		}
	}
	return -1;
}

/*
================
ProximityObjectTable::Remove

Empties the entry. The proximity checks attached to its object go too, and a
proximity check itself is deleted.
================
*/
// FUNCTION: STBC 0x005a9640
void ProximityObjectTable::Remove( int index, ProximityManager *manager ) {
	ObjectClass *object = entries[index].object;
	for ( int i = 0; i < numEntries; i++ ) {
		ObjectClass *other = entries[i].object;
		if ( other != NULL && other->IsTypeOf( PROXIMITYCHECK_TYPE ) && object->IsObjectAttached( other ) ) {
			manager->RemoveObject( other );
		}
	}
	if ( object->IsTypeOf( PROXIMITYCHECK_TYPE ) ) {
		ProximityCheck *check = (ProximityCheck *)object;
		check->SetManager( NULL );
		check->RemoveAndDelete();
	}
	entries[index].object = NULL;
}

/*
================
ProximityPairCounts::ProximityPairCounts
================
*/
// FUNCTION: STBC 0x005a9720
ProximityPairCounts::ProximityPairCounts( int n ) {
	numEntries = n;
	numCounts = ( n - 1 ) * n / 2;
	counts = (char *)operator new( numCounts );
	memset( counts, 0, numCounts );
}

/*
================
ProximityPairCounts::~ProximityPairCounts
================
*/
// FUNCTION: STBC 0x005a9760
ProximityPairCounts::~ProximityPairCounts( void ) {
	delete counts;
}

/*
================
ProximityPairCounts::Resize

Makes room for the pairs of numEntries entries, keeping the counts.
================
*/
// FUNCTION: STBC 0x005a9770
void ProximityPairCounts::Resize( int newEntries ) {
	numEntries = newEntries;
	int newCounts = ( newEntries - 1 ) * newEntries / 2;
	char *block = counts;
	if ( numCounts < newCounts ) {
		block = (char *)operator new( newCounts );
		memcpy( block, counts, numCounts );
		memset( block + numCounts, 0, newCounts - numCounts );
	} else if ( newCounts < numCounts ) {
		block = (char *)operator new( newCounts );
		memcpy( block, counts, newCounts );
	}
	delete counts;
	numCounts = newCounts;
	counts = block;
}

/*
================
ProximityPairCounts::Decrement
================
*/
// FUNCTION: STBC 0x005a9820
int ProximityPairCounts::Decrement( int a, int b ) {
	int index = PairIndex( a, b );
	counts[index]--;
	return counts[index];
}

/*
================
ProximityPairCounts::Increment
================
*/
// FUNCTION: STBC 0x005a9850
int ProximityPairCounts::Increment( int a, int b ) {
	int index = PairIndex( a, b );
	counts[index]++;
	return counts[index];
}
/*
================
ProximityManager::SetMultiplayerPlayerCollisionsEnabled

The host sets whether the players' ships collide with each other and tells
the other players.
================
*/
// FUNCTION: STBC 0x005a9880
void ProximityManager::SetMultiplayerPlayerCollisionsEnabled( bool enabled ) {
	TGNetwork *network = g_kUtopiaModule.GetNetwork();
	if ( network != NULL && network->IsHost() ) {
		multiplayerPlayerCollisionsEnabled = enabled;

		TGMessage *message = new TGMessage;
		message->SetGuaranteed( true );

		TGBufferStream stream;
		char buffer[4];
		stream.VirtualSlot60( buffer, sizeof( buffer ) );
		stream.WriteChar( COLLISION_ENABLED_MESSAGE );
		stream.WriteBool( enabled );
		message->SetData( buffer, stream.GetPos() );
		network->SendTGMessageToGroup( "NoMe", message );
	}
}

/*
================
ProximityPairCounts::PairIndex

The place of the pair a, b in the triangle of pairs.
================
*/
// FUNCTION: STBC 0x005a9970
int ProximityPairCounts::PairIndex( int a, int b ) {
	if ( a > b ) {
		return ( a - 1 ) * a / 2 + b;
	}
	return ( b - 1 ) * b / 2 + a;
}

/*
================
ProximityPairCounts::GetPair

The pair at the place in the triangle of pairs, PairIndex the other way
round: b the larger entry, a the smaller (invented name; nothing in retail
calls it).
================
*/
// FUNCTION: STBC 0x005a99a0
void ProximityPairCounts::GetPair( int index, int *a, int *b ) {
	*a = -1;
	*b = 1;
	for ( ;; ) {
		int rowStart = ( *b - 1 ) * *b / 2;
		if ( index >= rowStart && index <= rowStart + *b - 1 ) {
			*a = index - rowStart;
			return;
		}
		( *b )++;
	}
}

/*
================
ProximityManager::VirtualSlot00

Writes the proximity checks that are in no set to the stream, between two
markers; false when one fails.
================
*/
// FUNCTION: STBC 0x005a9a20
bool ProximityManager::VirtualSlot00( TGStream *stream ) {
	for ( int i = 0; i < table.numEntries; i++ ) {
		ProximityEntry entry = table.entries[i];
		ObjectClass *object = entry.object;
		if ( object != NULL && object->IsTypeOf( PROXIMITYCHECK_TYPE ) && object->GetContainingSet() == NULL ) {
			stream->WriteBool( true );
			if ( !object->WriteToStream( stream ) ) {
				return false;
			}
		}
	}
	stream->WriteBool( false );
	return true;
}

