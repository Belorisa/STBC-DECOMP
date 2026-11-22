// NiScreenPolygon.cpp -- polygons drawn over a camera's view

#include "NiRTLib.h"
#include <assert.h>
#include <string.h>

#include "NiScreenPolygon.h"
#include "NiColor.h"
#include "NiPoint2.h"
#include "NiPoint3.h"
#include "NiProperty.h"
#include "renderer/NiRender.h"
#include "NiCamera.h"
#include "NiClusterAccumulator.h"
#include "NiGeometry.h"
#include "NiGeometryData.h"
#include "NiTriBasedGeom.h"
#include "NiTriBasedGeomData.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007e7870 _$E30
// FUNCTION: STBC 0x007e7880 _$E170
// FUNCTION: STBC 0x007e7890 _$E100
// FUNCTION: STBC 0x007e78a0 _$E113
// FUNCTION: STBC 0x007e78b0 _$E41
// FUNCTION: STBC 0x007e78c0 _$E45
// FUNCTION: STBC 0x007e78d0 _$E49
// FUNCTION: STBC 0x007e78e0 _$E53
// FUNCTION: STBC 0x007e78f0 _$E188
// FUNCTION: STBC 0x007e7900 _$E193
// FUNCTION: STBC 0x007e7910 _$E198
// FUNCTION: STBC 0x007e7950 _$E158

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007e7850 _$E258
// FUNCTION: STBC 0x007e77f0 _$E17
// FUNCTION: STBC 0x007e7810 _$E15
// FUNCTION: STBC 0x007e7820 _$E23
// FUNCTION: STBC 0x007e7840 _$E21

// FUNCTION: STBC 0x007e68b0 _$E245
// FUNCTION: STBC 0x007e68d0 _$E242
// DATA: STBC 0x9A18D0 ?rtti@NiScreenPolygon@@2VNiRTTI@@B
const NiRTTI NiScreenPolygon::rtti( "NiScreenPolygon", &NiObject::rtti );

/*
================
NiScreenPolygon::Register
================
*/
// FUNCTION: STBC 0x7E68F0 ?Register@NiScreenPolygon@@SAHXZ
NiImplementRegister( NiScreenPolygon )

/*
================
NiScreenPolygon::NiScreenPolygon

Copies the vertices and whichever of the texture coordinates and colours
are given, and starts with a property state of defaults.
================
*/
// FUNCTION: STBC 0x7E6A40
NiScreenPolygon::NiScreenPolygon( unsigned short vertices_, const NiPoint3 *vertex_, const NiPoint2 *texture_, const NiColorA *color_ ) {
	vertices = vertices_;
	vertex = new NiPoint3[vertices_];
	memcpy( vertex, vertex_, vertices_ * sizeof( NiPoint3 ) );

	if ( texture_ ) {
		texture = new NiPoint2[vertices_];
		memcpy( texture, texture_, vertices_ * sizeof( NiPoint2 ) );
	} else {
		texture = 0;
	}

	if ( color_ ) {
		color = new NiColorA[vertices_];
		memcpy( color, color_, vertices_ * sizeof( NiColorA ) );
	} else {
		color = 0;
	}

	propertyState = new NiPropertyState;
}

/*
================
NiScreenPolygon::~NiScreenPolygon
================
*/
// FUNCTION: STBC 0x7E6BD0
NiScreenPolygon::~NiScreenPolygon() {
	delete [] vertex;
	delete [] texture;
	delete [] color;
}

/*
================
NiScreenPolygon::Draw
================
*/
// FUNCTION: STBC 0x7E6C60
void NiScreenPolygon::Draw( NiRender *renderer ) {
	renderer->SetPropertyState( propertyState );
	renderer->DrawScreenPolygon( vertices, vertex, texture, color );
}

/*
================
NiScreenPolygon::DrawBatched
================
*/
// FUNCTION: STBC 0x7E6CF0
void NiScreenPolygon::DrawBatched( NiRender *renderer ) {
	renderer->SetPropertyState( propertyState );
	renderer->VirtualSlot51( vertices, vertex, texture, color );
}

/*
================
NiScreenPolygon::CreateClone

The clone gets copies of the arrays, and CopyMembers its property state.
================
*/
// FUNCTION: STBC 0x7E6D60
NiObject *NiScreenPolygon::CreateClone() {
	NiScreenPolygon *clone = new NiScreenPolygon( vertices, vertex, texture, color );
	assert( clone );
	CopyMembers( clone );
	return clone;
}

/*
================
NiScreenPolygon::CopyMembers

The copy gets its own copy of the property state (Gamebryo lineage).
================
*/
// FUNCTION: STBC 0x7E6E30
void NiScreenPolygon::CopyMembers( NiScreenPolygon *dest ) {
	NiObject::CopyMembers( dest );
	dest->propertyState = new NiPropertyState( *propertyState );
}

/*
================
NiScreenPolygon::CreateObject
================
*/
// FUNCTION: STBC 0x7E6ED0
void NiScreenPolygon::CreateObject( NiStream &stream, NiObject *&object ) {
	NiScreenPolygon *polygon = new NiScreenPolygon;
	stream.SetLoadRecord( new NiScreenPolygonLoadRecord );
	polygon->LoadBinary( stream );
	object = polygon;
}

/*
================
NiScreenPolygon::LoadBinary

The vertices, the texture coordinates and the colours when saved (their
pointers save as the flags), then the link IDs of the properties.
================
*/
// FUNCTION: STBC 0x7E7000
void NiScreenPolygon::LoadBinary( NiStream &stream ) {
	NiObject::LoadBinary( stream );

	NiStreamLoadBinary( stream, vertices );
	assert( vertices );
	vertex = new NiPoint3[vertices];
	assert( vertex );
	for ( unsigned short i = 0; i < vertices; i++ ) {
		vertex[i].LoadBinary( stream );
	}

	NiStreamLoadBinary( stream, texture );
	if ( texture ) {
		texture = new NiPoint2[vertices];
		assert( texture );
		for ( i = 0; i < vertices; i++ ) {
			texture[i].LoadBinary( stream );
		}
	}

	NiStreamLoadBinary( stream, color );
	if ( color ) {
		color = new NiColorA[vertices];
		assert( color );
		for ( i = 0; i < vertices; i++ ) {
			color[i].LoadBinary( stream );
		}
	}

	NiScreenPolygonLoadRecord *record = (NiScreenPolygonLoadRecord *)stream.GetLoadRecord();
	unsigned int numProps;	// (name invented; fits the /Od frame order)
	NiStreamLoadBinary( stream, numProps );
	for ( unsigned int l = 0; l < numProps; l++ ) {
		unsigned int linkID;
		NiStreamLoadBinary( stream, linkID );
		record->propertyLinkIDs.Add( linkID );
	}
}

/*
================
NiScreenPolygon::LinkObject

Puts each property into the property state by its type, last first.
================
*/
// FUNCTION: STBC 0x7E7260
void NiScreenPolygon::LinkObject( NiStream &stream ) {
	NiObject::LinkObject( stream );

	NiScreenPolygonLoadRecord *record = (NiScreenPolygonLoadRecord *)stream.GetLoadRecord();
	unsigned int propertyCount = record->propertyLinkIDs.GetSize();	// (name invented; fits the /Od frame order)
	for ( int i = propertyCount - 1; i >= 0; i-- ) {
		NiProperty *property = (NiProperty *)stream.GetObjectFromLinkID( record->propertyLinkIDs.GetAt( i ) );
		SetProperty( property );
	}
}

/*
================
NiScreenPolygon::RegisterStreamables

Registers every property of the state; the state holds one of each.
================
*/
// FUNCTION: STBC 0x7E72E0
bool NiScreenPolygon::RegisterStreamables( NiStream &stream ) {
	if ( !NiObject::RegisterStreamables( stream ) ) {
		return false;
	}
	for ( unsigned int i = 0; i < NiProperty::MAX_TYPES; i++ ) {
		( *propertyState )[i]->RegisterStreamables( stream );
	}
	return true;
}

/*
================
NiScreenPolygon::SaveBinary

The order LoadBinary reads; the link IDs are the properties' addresses.
================
*/
// FUNCTION: STBC 0x7E7320
void NiScreenPolygon::SaveBinary( NiStream &stream ) {
	NiObject::SaveBinary( stream );
	assert( vertices );

	NiStreamSaveBinary( stream, vertices );
	for ( unsigned short i = 0; i < vertices; i++ ) {
		vertex[i].SaveBinary( stream );
	}

	NiStreamSaveBinary( stream, texture );
	if ( texture ) {
		for ( i = 0; i < vertices; i++ ) {
			texture[i].SaveBinary( stream );
		}
	}

	NiStreamSaveBinary( stream, color );
	if ( color ) {
		for ( i = 0; i < vertices; i++ ) {
			color[i].SaveBinary( stream );
		}
	}

	unsigned int count = NiProperty::MAX_TYPES;
	NiStreamSaveBinary( stream, count );
	if ( count > 0 ) {
		for ( i = 0; i < count; i++ ) {
			NiProperty *property = ( *propertyState )[i];
			NiStreamSaveBinary( stream, property );
		}
	}
}

/*
================
NiScreenPolygon::IsEqual

The vertices, whichever of the texture coordinates and colours both have,
and the same property in every slot of the state.
================
*/
// FUNCTION: STBC 0x7E7430
bool NiScreenPolygon::IsEqual( NiObject *object ) {
	if ( !NiObject::IsEqual( object ) ) {
		return false;
	}

	NiScreenPolygon *poly = (NiScreenPolygon *)object;	// (name invented; fits the /Od frame order)
	if ( vertices != poly->vertices ) {
		return false;
	}
	for ( unsigned short i = 0; i < vertices; i++ ) {
		if ( vertex[i] != poly->vertex[i] ) {
			return false;
		}
	}

	if ( ( texture && !poly->texture ) || ( !texture && poly->texture ) ) {
		return false;
	}
	if ( texture ) {
		for ( i = 0; i < vertices; i++ ) {
			if ( texture[i] != poly->texture[i] ) {
				return false;
			}
		}
	}

	if ( ( color && !poly->color ) || ( !color && poly->color ) ) {
		return false;
	}
	if ( color ) {
		for ( i = 0; i < vertices; i++ ) {
			if ( color[i] != poly->color[i] ) {
				return false;
			}
		}
	}

	for ( i = 0; i < NiProperty::MAX_TYPES; i++ ) {
		NiProperty *property0 = ( *propertyState )[i];	// (name invented; fits the /Od frame order)
		NiProperty *property1 = ( *poly->propertyState )[i];	// (name invented; fits the /Od frame order)
		if ( property0 != property1 ) {
			return false;
		}
	}
	return true;
}

/*
================
NiScreenPolygon::GetViewerStrings

The counts and arrays, then each property of the state by its class name.
================
*/
// FUNCTION: STBC 0x7E75E0
void NiScreenPolygon::GetViewerStrings( NiTArray<char *> *strings ) {
	NiObject::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_usVertices", vertices ) );
	strings->Add( NiGetViewerString( "m_pVertex", vertex ) );
	strings->Add( NiGetViewerString( "m_pTexture", texture ) );
	strings->Add( NiGetViewerString( "m_pColor", color ) );
	for ( unsigned int i = 0; i < NiProperty::MAX_TYPES; i++ ) {
		NiProperty *property = ( *propertyState )[i];
		strings->Add( NiGetViewerString( property->GetRTTI()->GetName(), property ) );
	}
}
