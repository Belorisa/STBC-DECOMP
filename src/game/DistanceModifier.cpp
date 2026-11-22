/*
===============================================================================

	DistanceModifier.cpp

===============================================================================
*/

// <iostream>'s statics: the ios_base::Init and _Winit quads and the locale
// id pair.
// FUNCTION: STBC 0x0046d910 _$E32
// FUNCTION: STBC 0x0046d920 _$E29
// FUNCTION: STBC 0x0046d930 _$E31
// FUNCTION: STBC 0x0046d940 _$E30
// FUNCTION: STBC 0x0046d950 _$E38
// FUNCTION: STBC 0x0046d960 _$E35
// FUNCTION: STBC 0x0046d970 _$E37
// FUNCTION: STBC 0x0046d980 _$E36
// FUNCTION: STBC 0x0046d8e0 _$E54
// FUNCTION: STBC 0x0046d900 _$E53
#include <iostream>

#include "DistanceModifier.h"
#include "TGGeomUtils.h"
#include "TGStream.h"

/*
================
DistanceModifier::WriteToStream
================
*/
// FUNCTION: STBC 0x0046ca70
bool DistanceModifier::WriteToStream( TGStream *stream ) {
	stream->WriteLong( type );
	return true;
}

/*
================
DistanceModifier::ReadFromStream
================
*/
// FUNCTION: STBC 0x0046ca90
bool DistanceModifier::ReadFromStream( TGStream *stream ) {
	return true;
}

/*
================
ScaleDistanceModifier::WriteToStream
================
*/
// FUNCTION: STBC 0x0046caa0
bool ScaleDistanceModifier::WriteToStream( TGStream *stream ) {
	if ( !DistanceModifier::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( factor );
	return true;
}

/*
================
ScaleDistanceModifier::ReadFromStream
================
*/
// FUNCTION: STBC 0x0046cad0
bool ScaleDistanceModifier::ReadFromStream( TGStream *stream ) {
	if ( !DistanceModifier::ReadFromStream( stream ) ) {
		return false;
	}
	factor = stream->ReadFloat();
	return true;
}

/*
================
PlaneDistanceModifier::PlaneDistanceModifier
================
*/
// FUNCTION: STBC 0x0046cb00
PlaneDistanceModifier::PlaneDistanceModifier( void ) : normal( NiPoint3::ZERO ) {
	type = DMT_PLANE;
	frontFactor = 1.0f;
	backFactor = 1.0f;
	constant = 0.0f;
}

/*
================
PlaneDistanceModifier::~PlaneDistanceModifier
================
*/
// FUNCTION: STBC 0x0046cb70
PlaneDistanceModifier::~PlaneDistanceModifier( void ) {
}

/*
================
PlaneDistanceModifier::ModifyDistance

A segment that crosses the plane is split where it crosses.
================
*/
// FUNCTION: STBC 0x0046cb80
float PlaneDistanceModifier::ModifyDistance( float distance, NiPoint3 *from, NiPoint3 *to ) {
	float fromDistance = normal.x * from->x + from->y * normal.y + from->z * normal.z + constant;
	float toDistance = normal.x * to->x + to->y * normal.y + to->z * normal.z + constant;
	if ( fromDistance >= 0.0f && toDistance >= 0.0f ) {
		return distance * frontFactor;
	}
	if ( fromDistance < 0.0f && toDistance < 0.0f ) {
		return distance * backFactor;
	}
	if ( fromDistance >= 0.0f ) {
		float total = fromDistance - toDistance;
		return fromDistance / total * frontFactor * distance - toDistance / total * backFactor * distance;
	}
	float total = toDistance - fromDistance;
	return toDistance / total * frontFactor * distance - fromDistance / total * backFactor * distance;
}

/*
================
PlaneDistanceModifier::SetPlane
================
*/
// FUNCTION: STBC 0x0046cc70
void PlaneDistanceModifier::SetPlane( const NiPoint3 &point, const NiPoint3 &normal_ ) {
	normal = normal_;
	constant = -( point.x * normal.x + point.y * normal.y + point.z * normal.z );
}

/*
================
PlaneDistanceModifier::WriteToStream

BUG: the factors are not written.
================
*/
// FUNCTION: STBC 0x0046ccb0
bool PlaneDistanceModifier::WriteToStream( TGStream *stream ) {
	if ( !DistanceModifier::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( normal.x );
	stream->WriteFloat( normal.y );
	stream->WriteFloat( normal.z );
	stream->WriteFloat( constant );
	return true;
}

/*
================
PlaneDistanceModifier::ReadFromStream
================
*/
// FUNCTION: STBC 0x0046cd00
bool PlaneDistanceModifier::ReadFromStream( TGStream *stream ) {
	if ( !DistanceModifier::ReadFromStream( stream ) ) {
		return false;
	}
	normal.x = stream->ReadFloat();
	normal.y = stream->ReadFloat();
	normal.z = stream->ReadFloat();
	constant = stream->ReadFloat();
	return true;
}

/*
================
SphereDistanceModifier::SphereDistanceModifier
================
*/
// FUNCTION: STBC 0x0046cd50
SphereDistanceModifier::SphereDistanceModifier( float factor_ ) : spheres( 2 ) {
	type = DMT_SPHERES;
	factor = factor_;
}

/*
================
SphereDistanceModifier::~SphereDistanceModifier
================
*/
// FUNCTION: STBC 0x0046cdb0
SphereDistanceModifier::~SphereDistanceModifier( void ) {
	NiTListIterator pos = spheres.GetHeadPos();
	while ( pos ) {
		delete spheres.GetNext( pos );
	}
}

/*
================
SphereDistanceModifier::ModifyDistance

Finds the part of the segment inside each sphere, joins the parts that
overlap and counts what is left factor times.
================
*/
// FUNCTION: STBC 0x0046ce20
float SphereDistanceModifier::ModifyDistance( float distance, NiPoint3 *from, NiPoint3 *to ) {
	float length = ( *to - *from ).Length();
	float modified = distance;
	NiTListIterator pos = spheres.GetHeadPos();
	NiTList<DistanceRange *> ranges( 2 );

	while ( pos ) {
		DistanceSphere *sphere = spheres.GetNext( pos );
		NiPoint3 hit1;
		NiPoint3 hit2;
		int hits = Unknown4570D0( *from, *to - *from, sphere->center, sphere->radius, &hit1, &hit2 );
		if ( hits != 0 ) {
			DistanceRange *range = new DistanceRange;
			if ( hits == 2 ) {
				range->start = ( hit1 - *from ).Length() / length;
				range->end = ( *to - hit2 ).Length() / length;
			} else if ( ( *from - sphere->center ).SqrLength() < ( *to - sphere->center ).SqrLength() ) {
				// the segment starts inside the sphere
				range->start = 0.0f;
				range->end = ( hit1 - *from ).Length() / length;
			} else {
				range->start = ( hit1 - *from ).Length() / length;
				range->end = 1.0f;
			}
			ranges.AddTail( range );
		} else {
			NiPoint3 fromOffset = sphere->center - *from;
			NiPoint3 toOffset = sphere->center - *to;
			float fromSqr = fromOffset * fromOffset;
			float toSqr = toOffset * toOffset;
			float radiusSqr = sphere->radius * sphere->radius;
			if ( fromSqr < radiusSqr && toSqr < radiusSqr ) {
				// the whole segment is inside
				DistanceRange *range = new DistanceRange;
				range->start = 0.0f;
				range->end = 1.0f;
				ranges.AddTail( range );
				break;
			}
		}
	}

	pos = ranges.GetHeadPos();
	while ( pos ) {
		NiTListIterator rangePos = pos;
		DistanceRange *range = ranges.GetNext( pos );
		NiTListIterator otherPos = pos;
		while ( otherPos ) {
			NiTListIterator removePos = otherPos;
			DistanceRange *other = ranges.GetNext( otherPos );
			if ( other->start <= range->start && other->end >= range->end ) {
				range->start = other->start;
				range->end = other->end;
			} else if ( range->start <= other->start && range->end >= other->end ) {
				// the other is inside this one
			} else if ( other->start <= range->end ) {
				range->end = other->end;
			} else if ( other->end >= range->start ) {
				range->start = other->start;
			} else {
				continue;
			}

			bool wasNext = false;
			if ( pos == removePos ) {
				wasNext = true;
			}
			delete other;
			ranges.RemovePos( removePos );
			if ( wasNext ) {
				pos = ranges.GetNextPos( rangePos );
			}
		}
	}

	while ( !ranges.IsEmpty() ) {
		DistanceRange *range = ranges.RemoveHead();
		float inside = ( range->end - range->start ) * length;
		modified += ( factor - 1.0f ) * inside;
		delete range;
	}
	return modified;
}

/*
================
SphereDistanceModifier::WriteToStream

Each sphere follows a true; a false ends the list.
================
*/
// FUNCTION: STBC 0x0046d3f0
bool SphereDistanceModifier::WriteToStream( TGStream *stream ) {
	if ( !DistanceModifier::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( factor );
	NiTListIterator pos = spheres.GetHeadPos();
	while ( pos ) {
		DistanceSphere *sphere = spheres.GetNext( pos );
		if ( sphere != NULL ) {
			stream->WriteBool( true );
			stream->WriteFloat( sphere->center.x );
			stream->WriteFloat( sphere->center.y );
			stream->WriteFloat( sphere->center.z );
			stream->WriteFloat( sphere->radius );
		}
	}
	stream->WriteBool( false );
	return true;
}

/*
================
SphereDistanceModifier::ReadFromStream

The factor was read by DistanceModifier::Read, for the constructor.
================
*/
// FUNCTION: STBC 0x0046d470
bool SphereDistanceModifier::ReadFromStream( TGStream *stream ) {
	if ( !DistanceModifier::ReadFromStream( stream ) ) {
		return false;
	}
	while ( stream->ReadBool() == true ) {
		DistanceSphere *sphere = new DistanceSphere;
		sphere->center.x = stream->ReadFloat();
		sphere->center.y = stream->ReadFloat();
		sphere->center.z = stream->ReadFloat();
		sphere->radius = stream->ReadFloat();
		spheres.AddTail( sphere );
	}
	return true;
}

/*
================
DistanceModifier::Write
================
*/
// FUNCTION: STBC 0x0046d5e0
bool DistanceModifier::Write( DistanceModifier *modifier, TGStream *stream ) {
	return modifier->WriteToStream( stream );
}

/*
================
DistanceModifier::Read

Makes the modifier whose type the stream holds and has it read the rest;
NULL when it can't.
================
*/
// FUNCTION: STBC 0x0046d5f0
DistanceModifier *DistanceModifier::Read( TGStream *stream ) {
	// BUG: modifier is not initialised: an unknown type goes on with whatever the variable holds
	DistanceModifier *modifier;
	switch ( stream->ReadLong() ) {
	case DMT_SPHERES: {
		float factor = stream->ReadFloat();
		modifier = new SphereDistanceModifier( factor );
		break;
	}
	case DMT_PLANE:
		modifier = new PlaneDistanceModifier;
		break;
	case DMT_SCALE:
		modifier = new ScaleDistanceModifier;
		break;
	}
	if ( modifier == NULL ) {
		return NULL;
	}
	if ( !modifier->ReadFromStream( stream ) ) {
		delete modifier;
		return NULL;
	}
	return modifier;
}
