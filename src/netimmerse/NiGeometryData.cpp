// NiGeometryData.cpp -- the shareable vertex data of geometry objects

#include "NiRTLib.h"
#include <string.h>

#include "NiGeometryData.h"
#include "NiStream.h"
#include "renderer/NiRender.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007eda20 _$E41
// FUNCTION: STBC 0x007eda60 _$E49
// FUNCTION: STBC 0x007eda90 _$E60
// FUNCTION: STBC 0x007edaa0 _$E67
// FUNCTION: STBC 0x007edab0 _$E80
// FUNCTION: STBC 0x007edac0 _$E84
// FUNCTION: STBC 0x007edad0 _$E88
// FUNCTION: STBC 0x007edae0 _$E92
// FUNCTION: STBC 0x007edaf0 _$E96
// FUNCTION: STBC 0x007edb00 _$E189
// FUNCTION: STBC 0x007edb10 _$E194
// FUNCTION: STBC 0x007edb20 _$E199

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007eda00 _$E222
// FUNCTION: STBC 0x007ed9a0 _$E17
// FUNCTION: STBC 0x007ed9c0 _$E15
// FUNCTION: STBC 0x007ed9d0 _$E23
// FUNCTION: STBC 0x007ed9f0 _$E21

// FUNCTION: STBC 0x007ec9b0 _$E205
// FUNCTION: STBC 0x007ec9d0 _$E202
// DATA: STBC 0x9A1A60 ?rtti@NiGeometryData@@2VNiRTTI@@B
const NiRTTI NiGeometryData::rtti( "NiGeometryData", &NiObject::rtti );

// DATA: STBC 0x978738
unsigned short NiGeometryData::nextID = 1;

/*
================
NiGeometryData::Register
================
*/
// FUNCTION: STBC 0x7EC9F0 ?Register@NiGeometryData@@SAHXZ
NiImplementRegister( NiGeometryData )

/*
================
NiGeometryData::NiGeometryData

Takes ownership of the arrays.
================
*/
// FUNCTION: STBC 0x7ECB40 ??0NiGeometryData@@QAE@GPAVNiPoint3@@0PAVNiColorA@@PAVNiPoint2@@@Z
NiGeometryData::NiGeometryData( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_ ) {
	vertexCount = vertexCount_;
	textureSets = textures_ ? 1 : 0;
	vertices = vertices_;
	normals = normals_;
	colors = colors_;
	textures = textures_;
	if ( vertexCount > 0 && vertices ) {
		bound.ComputeFromData( vertexCount, vertices );
	}
	id = nextID;
	nextID++;
	revisionID = 0;
}

/*
================
NiGeometryData::NiGeometryData
================
*/
// FUNCTION: STBC 0x7ECC00 ??0NiGeometryData@@QAE@XZ
NiGeometryData::NiGeometryData() {
	vertexCount = 0;
	textureSets = 0;
	vertices = 0;
	normals = 0;
	colors = 0;
	textures = 0;
	id = nextID;
	nextID++;
	revisionID = 0;
}

/*
================
NiGeometryData::~NiGeometryData

Tells the renderers to drop their copies first.
================
*/
// FUNCTION: STBC 0x7ECC80 ??1NiGeometryData@@UAE@XZ
NiGeometryData::~NiGeometryData() {
	NiRender::RemoveGeometryData( this );
	delete [] vertices;
	delete [] normals;
	delete [] colors;
	delete [] textures;
}

/*
================
NiGeometryData::CreateVertices
================
*/
// FUNCTION: STBC 0x7ECD10
void NiGeometryData::CreateVertices( bool initialize ) {
	if ( !vertices ) {
		vertices = new NiPoint3[vertexCount];
	}
	if ( initialize ) {
		memset( vertices, 0, vertexCount * sizeof( NiPoint3 ) );
	}
}

/*
================
NiGeometryData::CreateNormals
================
*/
// FUNCTION: STBC 0x7ECD70
void NiGeometryData::CreateNormals( bool initialize ) {
	if ( !normals ) {
		normals = new NiPoint3[vertexCount];
	}
	if ( initialize ) {
		memset( normals, 0, vertexCount * sizeof( NiPoint3 ) );
	}
}

/*
================
NiGeometryData::CreateColors

initialize sets every colour to opaque black.
================
*/
// FUNCTION: STBC 0x7ECDD0
void NiGeometryData::CreateColors( bool initialize ) {
	if ( !colors ) {
		colors = new NiColorA[vertexCount];
	}
	if ( initialize ) {
		for ( unsigned short i = 0; i < vertexCount; i++ ) {
			colors[i] = NiColorA::BLACK;
		}
	}
}

/*
================
NiGeometryData::CreateTextures

A different set count frees the old coordinates first.
================
*/
// FUNCTION: STBC 0x7ECE70
void NiGeometryData::CreateTextures( bool initialize, unsigned short sets ) {
	if ( textures && sets != textureSets ) {
		delete [] textures;
		textures = 0;
	}
	unsigned short count = vertexCount * sets;
	if ( !textures ) {
		textures = new NiPoint2[count];
		textureSets = sets;
	}
	if ( initialize ) {
		memset( textures, 0, count * sizeof( NiPoint2 ) );
	}
}

/*
================
NiGeometryData::GetTextureSet

The coordinates of one set; NULL when there is no such set.
================
*/
// FUNCTION: STBC 0x7ECF00 ?GetTextureSet@NiGeometryData@@QAEPAVNiPoint2@@G@Z
NiPoint2 *NiGeometryData::GetTextureSet( unsigned short set ) {
	if ( !textures || set >= textureSets ) {
		return 0;
	}
	return &textures[vertexCount * set];
}

/*
================
NiGeometryData::GetTextureSet
================
*/
// FUNCTION: STBC 0x7ECF30 ?GetTextureSet@NiGeometryData@@QBEPBVNiPoint2@@G@Z
const NiPoint2 *NiGeometryData::GetTextureSet( unsigned short set ) const {
	if ( !textures || set >= textureSets ) {
		return 0;
	}
	return &textures[vertexCount * set];
}

/*
================
NiGeometryData::AppendTextureSet

Adds a copy of one more set of coordinates after the sets there are; the
first set when there are none. The name is invented.
================
*/
// FUNCTION: STBC 0x7ECF60
void NiGeometryData::AppendTextureSet( NiPoint2 *set ) {
	if ( textures == NULL ) {
		textures = new NiPoint2[vertexCount];
		textureSets = 1;
		for ( unsigned short i = 0; i < vertexCount; i++ ) {
			textures[i] = set[i];
		}
		return;
	}

	unsigned short count = textureSets * vertexCount;
	NiPoint2 *newTextures = new NiPoint2[vertexCount + count];
	for ( unsigned short i = 0; i < count; i++ ) {
		newTextures[i] = textures[i];
	}
	delete [] textures;
	textures = newTextures;
	for ( i = 0; i < vertexCount; i++ ) {
		textures[count + i] = set[i];
	}
	textureSets++;
}

/*
================
NiGeometryData::Replace
================
*/
// FUNCTION: STBC 0x7ED080
void NiGeometryData::Replace( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_ ) {
	delete [] vertices;
	delete [] normals;
	delete [] colors;
	delete [] textures;
	SetData( vertexCount_, vertices_, normals_, colors_, textures_ );
}

/*
================
NiGeometryData::SetData

Recomputes the bound from the new positions.
================
*/
// FUNCTION: STBC 0x7ED110
void NiGeometryData::SetData( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_ ) {
	vertexCount = vertexCount_;
	textureSets = textures_ ? 1 : 0;
	vertices = vertices_;
	normals = normals_;
	if ( vertexCount > 0 ) {
		bound.ComputeFromData( vertexCount, vertices );
	}
	colors = colors_;
	textures = textures_;
}

/*
================
NiGeometryData::CreateClone

NiGeometryData itself is never cloned; the derived classes create the
clone.
================
*/
// FUNCTION: STBC 0x7ED170
NiObject *NiGeometryData::CreateClone() {
	assert( false );
	return 0;
}

/*
================
NiGeometryData::CopyMembers

Never called: the derived classes copy the data (AnimOnly.exe 0x42D477
(NiGeometryData.cpp) asserts false and calls no base).
================
*/
// FUNCTION: STBC 0x7ED180
void NiGeometryData::CopyMembers( NiGeometryData *dest ) {
	assert( false );
}

/*
================
NiGeometryData::CreateObject

The class is abstract: the stream creates nothing for it.
================
*/
// FUNCTION: STBC 0x7ED190
void NiGeometryData::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiGeometryData::LoadBinary

NiObject's members, the vertex count, then each array behind its saved
pointer, which only tells whether the array exists; the bound comes
between the normals and the colours, the texture set count before the
coordinates.
================
*/
// FUNCTION: STBC 0x7ED1A0
void NiGeometryData::LoadBinary( NiStream &stream ) {
	unsigned short i;
	NiObject::LoadBinary( stream );
	NiStreamLoadBinary( stream, vertexCount );

	NiStreamLoadBinary( stream, vertices );
	if ( vertices ) {
		vertices = new NiPoint3[vertexCount];
		assert( vertices );
		for ( i = 0; i < vertexCount; i++ ) {
			vertices[i].LoadBinary( stream );
		}
	}

	NiStreamLoadBinary( stream, normals );
	if ( normals ) {
		normals = new NiPoint3[vertexCount];
		assert( normals );
		for ( i = 0; i < vertexCount; i++ ) {
			normals[i].LoadBinary( stream );
		}
	}

	bound.LoadBinary( stream );

	NiStreamLoadBinary( stream, colors );
	if ( colors ) {
		colors = new NiColorA[vertexCount];
		assert( colors );
		for ( i = 0; i < vertexCount; i++ ) {
			colors[i].LoadBinary( stream );
		}
	}

	NiStreamLoadBinary( stream, textureSets );
	NiStreamLoadBinary( stream, textures );
	if ( textures ) {
		// quantity: (name invented; fits the /Od frame order)
		unsigned short quantity = vertexCount * textureSets;
		textures = new NiPoint2[quantity];
		assert( textures );
		for ( i = 0; i < quantity; i++ ) {
			textures[i].LoadBinary( stream );
		}
	}
}

/*
================
NiGeometryData::LinkObject
================
*/
// FUNCTION: STBC 0x7ED370
void NiGeometryData::LinkObject( NiStream &stream ) {
	NiObject::LinkObject( stream );
}

/*
================
NiGeometryData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7ED380
bool NiGeometryData::RegisterStreamables( NiStream &stream ) {
	return NiObject::RegisterStreamables( stream );
}

/*
================
NiGeometryData::SaveBinary

The order LoadBinary reads; each array's pointer saves as its flag.
================
*/
// FUNCTION: STBC 0x7ED390
void NiGeometryData::SaveBinary( NiStream &stream ) {
	unsigned short i;
	NiObject::SaveBinary( stream );
	NiStreamSaveBinary( stream, vertexCount );

	NiStreamSaveBinary( stream, vertices );
	if ( vertices ) {
		for ( i = 0; i < vertexCount; i++ ) {
			vertices[i].SaveBinary( stream );
		}
	}

	NiStreamSaveBinary( stream, normals );
	if ( normals ) {
		for ( i = 0; i < vertexCount; i++ ) {
			normals[i].SaveBinary( stream );
		}
	}

	bound.SaveBinary( stream );

	NiStreamSaveBinary( stream, colors );
	if ( colors ) {
		for ( i = 0; i < vertexCount; i++ ) {
			colors[i].SaveBinary( stream );
		}
	}

	NiStreamSaveBinary( stream, textureSets );
	NiStreamSaveBinary( stream, textures );
	if ( textures ) {
		unsigned short count = vertexCount * textureSets;
		for ( i = 0; i < count; i++ ) {
			textures[i].SaveBinary( stream );
		}
	}
}

/*
================
NiGeometryData::IsEqual

Compares the vertex count, the arrays element by element and the bound.
================
*/
// FUNCTION: STBC 0x7ED4C0
bool NiGeometryData::IsEqual( NiObject *object ) {
	if ( !NiObject::IsEqual( object ) ) {
		return false;
	}

	NiGeometryData *data = (NiGeometryData *)object;
	unsigned short i;
	if ( vertexCount != data->vertexCount ) {
		return false;
	}

	if ( ( vertices && !data->vertices ) || ( !vertices && data->vertices ) ) {
		return false;
	}
	if ( vertices ) {
		for ( i = 0; i < vertexCount; i++ ) {
			if ( vertices[i] != data->vertices[i] ) {
				return false;
			}
		}
	}

	if ( ( normals && !data->normals ) || ( !normals && data->normals ) ) {
		return false;
	}
	if ( normals ) {
		for ( i = 0; i < vertexCount; i++ ) {
			if ( normals[i] != data->normals[i] ) {
				return false;
			}
		}
	}

	if ( bound != data->bound ) {
		return false;
	}

	// NOTE: the second half tests the texture coordinates where the colours
	// were meant: data with colours on one side only can compare equal
	if ( ( colors && !data->colors ) || ( !textures && data->textures ) ) {
		return false;
	}
	if ( colors ) {
		for ( i = 0; i < vertexCount; i++ ) {
			if ( colors[i] != data->colors[i] ) {
				return false;
			}
		}
	}

	if ( ( textures && !data->textures ) || ( !textures && data->textures ) ) {
		return false;
	}
	if ( textures ) {
		if ( textureSets != data->textureSets ) {
			return false;
		}
		unsigned short count = vertexCount * textureSets;
		for ( i = 0; i < count; i++ ) {
			if ( textures[i] != data->textures[i] ) {
				return false;
			}
		}
	}
	return true;
}

/*
================
NiGeometryData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7ED730
// PARKED: inline budget: retail expands NiTArray<char *>::SetSize at the texture-set count's Add (share 224, cost 225: 1 unit short) and so calls SetAt there; the register swap follows. Only NiTArray.h's SetAtGrow cost moves it
void NiGeometryData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiObject::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_usVertices", vertexCount ) );
	strings->Add( NiGetViewerString( "m_pVertex", vertices ) );
	strings->Add( NiGetViewerString( "m_pNormal", normals ) );
	strings->Add( bound.GetViewerString( "m_bound" ) );
	strings->Add( NiGetViewerString( "m_pColor", colors ) );
	strings->Add( NiGetViewerString( "m_usTextureSets", textureSets ) );
	strings->Add( NiGetViewerString( "m_pTexture", textures ) );
}
