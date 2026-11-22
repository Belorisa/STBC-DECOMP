// NiBezierMesh.cpp -- nodes of Bezier patches sharing control vertices

#include "NiRTLib.h"
#include "NiStream.h"
#include "NiBezierMesh.h"
#include "NiBezierPatch.h"
#include "NiBezierSkinController.h"
#include "NiBezierTriangle2.h"
#include "NiBezierTriangle3.h"
#include "NiBezierTriangle4.h"
#include "NiBezierRectangle2.h"
#include "NiBezierRectangle3.h"
#include "NiBezierCylinder.h"
#include "NiColor.h"
#include "NiPick.h"
#include "NiPoint2.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00832000 _$E285
// FUNCTION: STBC 0x00831e70 _$E17
// FUNCTION: STBC 0x00831e90 _$E15
// FUNCTION: STBC 0x00831ea0 _$E23
// FUNCTION: STBC 0x00831ec0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0082e080 _$E268
// FUNCTION: STBC 0x0082e0a0 _$E265
// FUNCTION: STBC 0x00831f10 _$E37
// FUNCTION: STBC 0x00831f20 _$E41
// FUNCTION: STBC 0x00831f50 _$E45
// FUNCTION: STBC 0x00831fa0 _$E49
// FUNCTION: STBC 0x00831fb0 _$E53
// FUNCTION: STBC 0x00831ff0 _$E177
// FUNCTION: STBC 0x00832020 _$E182
// FUNCTION: STBC 0x00832030 _$E186
// FUNCTION: STBC 0x00832040 _$E194
// FUNCTION: STBC 0x00832050 _$E198
// FUNCTION: STBC 0x00832060 _$E202
// FUNCTION: STBC 0x00832070 _$E206
// FUNCTION: STBC 0x00832080 _$E214
// FUNCTION: STBC 0x008320c0 _$E218
// FUNCTION: STBC 0x008320d0 _$E222
// FUNCTION: STBC 0x008320e0 _$E226
// FUNCTION: STBC 0x008320f0 _$E232
// FUNCTION: STBC 0x00832100 _$E236
// FUNCTION: STBC 0x00832110 _$E240
// FUNCTION: STBC 0x00832120 _$E244
// FUNCTION: STBC 0x00832130 _$E250
// FUNCTION: STBC 0x00832140 _$E254
// FUNCTION: STBC 0x00832150 _$E258
// FUNCTION: STBC 0x00832160 _$E262

// DATA: STBC 0x9B2F28 ?rtti@NiBezierMesh@@2VNiRTTI@@B
const NiRTTI NiBezierMesh::rtti( "NiBezierMesh", &NiNode::rtti );

/*
================
NiBezierMesh::Register
================
*/
// FUNCTION: STBC 0x82E0C0 ?Register@NiBezierMesh@@SAHXZ
NiImplementRegister( NiBezierMesh )

/*
================
NiBezierMesh::SetUnknown20
================
*/
inline void NiBezierMesh::SetUnknown20( bool unknown ) {
	if ( unknown ) {
		flags |= UNKNOWN_20_MASK;
	} else {
		flags &= ~UNKNOWN_20_MASK;
	}
}

/*
================
NiBezierMesh::SetUnknown40

Hands the setting to every patch child as the patch's flag 0x10 when it
changes.
================
*/
inline void NiBezierMesh::SetUnknown40( bool unknown ) {
	if ( unknown != ( ( flags & UNKNOWN_40_MASK ) != 0 ) ) {
		for ( unsigned int i = 0; i < GetArrayCount(); i++ ) {
			NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, GetAt( i ) );
			if ( patch ) {
				patch->SetUnknown10( unknown );
			}
		}
		SetField( unknown, UNKNOWN_40_BITS, UNKNOWN_40_POS );
	}
}

/*
================
NiBezierMesh::NiBezierMesh

A mesh on the given control vertex arrays, which it takes.
================
*/
// FUNCTION: STBC 0x82E210 ??0NiBezierMesh@@QAE@GPAVNiPoint3@@PAVNiPoint2@@PAVNiColorA@@_N@Z
NiBezierMesh::NiBezierMesh( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown ) {
	vertexCount = vertexCount_;
	edgeCount = 0;
	edges = 0;
	vertices = vertices_;
	textures = textures_;
	if ( textures_ ) {
		textureSets = 1;
	} else {
		textureSets = 0;
	}
	colors = colors_;
	SetUnknown20( unknown );
	SetUnknown40( false );
	unknownFE = 0;
	unknownFF = 0xFF;
}

/*
================
NiBezierMesh::NiBezierMesh
================
*/
// FUNCTION: STBC 0x82E3B0 ??0NiBezierMesh@@QAE@XZ
NiBezierMesh::NiBezierMesh() {
	vertexCount = 0;
	edgeCount = 0;
	edges = 0;
	vertices = 0;
	textures = 0;
	textureSets = 0;
	colors = 0;
	SetUnknown20( false );
	SetUnknown40( false );
	unknownFE = 0;
	unknownFF = 0;
}

/*
================
NiBezierMesh::~NiBezierMesh

The mesh owns the edge array and the control vertex arrays.
================
*/
// FUNCTION: STBC 0x82E530
NiBezierMesh::~NiBezierMesh() {
	delete edges;
	delete [] vertices;
	delete [] textures;
	delete [] colors;
}

/*
================
NiBezierMesh::Unknown82E5C0
================
*/
// FUNCTION: STBC 0x82E5C0
void NiBezierMesh::Unknown82E5C0( unsigned short degree, unsigned short *indices ) {
	NiBezierTriangle *patch;
	switch ( degree ) {
	case 2:
		patch = new NiBezierTriangle2( indices, vertices, textures, colors, GetUnknown20(), vertexCount );
		if ( unknownFF > patch->VirtualSlot40() ) {
			unknownFF = patch->VirtualSlot40();
		}
		AttachChild( patch, false );
		break;
	case 3:
		patch = new NiBezierTriangle3( indices, vertices, textures, colors, GetUnknown20(), vertexCount );
		if ( unknownFF > patch->VirtualSlot40() ) {
			unknownFF = patch->VirtualSlot40();
		}
		AttachChild( patch, false );
		break;
	case 4:
		patch = new NiBezierTriangle4( indices, vertices, textures, colors, GetUnknown20(), vertexCount );
		if ( unknownFF > patch->VirtualSlot40() ) {
			unknownFF = patch->VirtualSlot40();
		}
		AttachChild( patch, false );
		break;
	}
}

/*
================
NiBezierMesh::Unknown82E770
================
*/
// FUNCTION: STBC 0x82E770
// DRAFT: schedule: retail stores maxSize before saving the old block in the inlined NiTSet::Realloc (and loads the edge base into EDX); the frame, calls, branches and stores otherwise agree
void NiBezierMesh::Unknown82E770( unsigned short degree, unsigned short *points ) {
	if ( edgeCount == 0 ) {
		edges = new NiTSet<unsigned short>( 12, 2 );
	}

	unsigned short *indices = new unsigned short[15];
	bool found[3] = { false, false, false };
	indices[0] = points[0];
	indices[4] = points[3];
	indices[6] = points[5];
	indices[7] = points[6];
	indices[10] = points[9];
	indices[14] = points[11];
	for ( unsigned short i = 0; i < edgeCount * 4; i += 4 ) {
		if ( found[0] && found[1] && found[2] ) {
			break;
		}
		unsigned short edge0 = edges->GetBase()[i];
		unsigned short edge1 = edges->GetBase()[i + 1];
		unsigned short edge2 = edges->GetBase()[i + 2];
		unsigned short edge3 = edges->GetBase()[i + 3];
		if ( !found[0] ) {
			if ( points[0] == edge0 && points[1] == edge1 && points[2] == edge2 && points[3] == edge3 ) {
				unsigned short first = i * 3 / 4 + vertexCount;
				indices[1] = first;
				indices[2] = first + 1;
				indices[3] = first + 2;
				found[0] = true;
			} else if ( points[3] == edge0 && points[2] == edge1 && points[1] == edge2 && points[0] == edge3 ) {
				unsigned short first = i * 3 / 4 + vertexCount;
				indices[3] = first;
				indices[2] = first + 1;
				indices[1] = first + 2;
				found[0] = true;
			}
		}
		if ( !found[1] ) {
			if ( points[0] == edge0 && points[4] == edge1 && points[8] == edge2 && points[11] == edge3 ) {
				unsigned short first = i * 3 / 4 + vertexCount;
				indices[5] = first;
				indices[9] = first + 1;
				indices[12] = first + 2;
				found[1] = true;
			} else if ( points[11] == edge0 && points[8] == edge1 && points[4] == edge2 && points[0] == edge3 ) {
				unsigned short first = i * 3 / 4 + vertexCount;
				indices[12] = first;
				indices[9] = first + 1;
				indices[5] = first + 2;
				found[1] = true;
			}
		}
		if ( !found[2] ) {
			if ( points[3] == edge0 && points[7] == edge1 && points[10] == edge2 && points[11] == edge3 ) {
				unsigned short first = i * 3 / 4 + vertexCount;
				indices[8] = first;
				indices[11] = first + 1;
				indices[13] = first + 2;
				found[2] = true;
			} else if ( points[11] == edge0 && points[10] == edge1 && points[7] == edge2 && points[3] == edge3 ) {
				unsigned short first = i * 3 / 4 + vertexCount;
				indices[13] = first;
				indices[11] = first + 1;
				indices[8] = first + 2;
				found[2] = true;
			}
		}
	}

	if ( !found[0] ) {
		unsigned short first = edges->GetSize() * 3 / 4 + vertexCount;
		indices[1] = first;
		indices[2] = first + 1;
		indices[3] = first + 2;
		edges->Add( points[0] );
		edges->Add( points[1] );
		edges->Add( points[2] );
		edges->Add( points[3] );
		edgeCount++;
	}
	if ( !found[1] ) {
		unsigned short first = edges->GetSize() * 3 / 4 + vertexCount;
		indices[5] = first;
		indices[9] = first + 1;
		indices[12] = first + 2;
		edges->Add( points[0] );
		edges->Add( points[4] );
		edges->Add( points[8] );
		edges->Add( points[11] );
		edgeCount++;
	}
	if ( !found[2] ) {
		unsigned short first = edges->GetSize() * 3 / 4 + vertexCount;
		indices[8] = first;
		indices[11] = first + 1;
		indices[13] = first + 2;
		edges->Add( points[3] );
		edges->Add( points[7] );
		edges->Add( points[10] );
		edges->Add( points[11] );
		edgeCount++;
	}
	delete [] points;

	NiBezierTriangle4 *patch = new NiBezierTriangle4( indices, vertices, textures, colors, GetUnknown20(), vertexCount );
	if ( unknownFF > patch->VirtualSlot40() ) {
		unknownFF = patch->VirtualSlot40();
	}
	AttachChild( patch, false );
}

/*
================
NiBezierMesh::Unknown82F2B0
================
*/
// FUNCTION: STBC 0x82F2B0
void NiBezierMesh::Unknown82F2B0( unsigned short degree, unsigned short *indices ) {
	NiBezierRectangle *patch;
	switch ( degree ) {
	case 2:
		patch = new NiBezierRectangle2( indices, vertices, textures, colors, GetUnknown20(), vertexCount );
		if ( unknownFF > patch->VirtualSlot40() ) {
			unknownFF = patch->VirtualSlot40();
		}
		AttachChild( patch, false );
		break;
	case 3:
		patch = new NiBezierRectangle3( indices, vertices, textures, colors, GetUnknown20(), vertexCount );
		if ( unknownFF > patch->VirtualSlot40() ) {
			unknownFF = patch->VirtualSlot40();
		}
		AttachChild( patch, false );
		break;
	}
}

/*
================
NiBezierMesh::Unknown82F400
================
*/
// FUNCTION: STBC 0x82F400
void NiBezierMesh::Unknown82F400( unsigned short textureSets_ ) {
	if ( textureSets_ != textureSets ) {
		for ( unsigned int i = 0; i < GetArrayCount(); i++ ) {
			NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, GetAt( i ) );
			if ( patch ) {
				patch->Unknown83A650( textureSets_ );
			}
		}
		textureSets = textureSets_;
	}
}

/*
================
NiBezierMesh::VirtualSlot44

Retessellates every patch child at the level, when it is a new one within
the limit.
================
*/
// FUNCTION: STBC 0x82F5F0
void NiBezierMesh::VirtualSlot44( unsigned char level ) {
	if ( unknownFE == level || level > unknownFF ) {
		return;
	}

	for ( unsigned int i = 0; i < GetArrayCount(); i++ ) {
		NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, GetAt( i ) );
		if ( patch ) {
			patch->VirtualSlot39( level );
		}
	}
	unknownFE = level;
}

/*
================
NiBezierMesh::Unknown82F730

SetUnknown40's work as a flag field: hands the setting to every patch
child as the patch's flag 0x10 when it changes. Nothing in retail calls it.
TODO identity: the name, and why the constructors expand SetUnknown40
instead.
================
*/
// FUNCTION: STBC 0x82F730
void NiBezierMesh::Unknown82F730( int unknown ) {
	if ( ( ( flags & UNKNOWN_40_MASK ) >> UNKNOWN_40_POS ) != unknown ) {
		for ( unsigned int i = 0; i < GetArrayCount(); i++ ) {
			NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, GetAt( i ) );
			if ( patch ) {
				patch->SetUnknown10( unknown );
			}
		}
		flags = ( flags & ~UNKNOWN_40_MASK ) | ( unknown << UNKNOWN_40_POS );
	}
}

/*
================
NiBezierMesh::Unknown82F880
================
*/
// FUNCTION: STBC 0x82F880
void NiBezierMesh::Unknown82F880( float unknownF8 ) {
	for ( unsigned int i = 0; i < GetArrayCount(); i++ ) {
		NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, GetAt( i ) );
		if ( patch ) {
			patch->SetUnknownF8( unknownF8 );
		}
	}
}

/*
================
NiBezierMesh::Unknown82F960
================
*/
// FUNCTION: STBC 0x82F960
void NiBezierMesh::Unknown82F960( float unknownFC ) {
	for ( unsigned int i = 0; i < GetArrayCount(); i++ ) {
		NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, GetAt( i ) );
		if ( patch ) {
			patch->SetUnknownFC( unknownFC );
		}
	}
}

/*
================
NiBezierMesh::Unknown82FA40

Tessellates every patch child at its own level.
================
*/
// FUNCTION: STBC 0x82FA40
void NiBezierMesh::Unknown82FA40() {
	for ( unsigned int i = 0; i < GetArrayCount(); i++ ) {
		NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, GetAt( i ) );
		if ( patch ) {
			patch->Retessellate();
		}
	}
}

/*
================
NiBezierMesh::Unknown82FB60
================
*/
// FUNCTION: STBC 0x82FB60
void NiBezierMesh::Unknown82FB60( NiCamera *camera ) {
	for ( unsigned int i = 0; i < GetArrayCount(); i++ ) {
		NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, GetAt( i ) );
		if ( patch ) {
			patch->SetUnknown128( camera );
		}
	}
}

/*
================
NiBezierMesh::FindIntersections

Searches every patch child; true when one is hit.
================
*/
// FUNCTION: STBC 0x82FC40
bool NiBezierMesh::FindIntersections( const NiPoint3 &origin, const NiPoint3 &dir, NiPick &pick ) {
	if ( pick.GetObserveAppCullFlag() && GetAppCulled() ) {
		return false;
	}

	bool found = false;
	for ( unsigned int i = 0; i < GetArrayCount(); i++ ) {
		NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, GetAt( i ) );
		if ( patch ) {
			if ( patch->FindIntersections( origin, dir, pick ) ) {
				found = true;
			}
		}
	}
	return found;
}

/*
================
NiBezierMesh::Unknown82FDA0
================
*/
// FUNCTION: STBC 0x82FDA0
void NiBezierMesh::Unknown82FDA0() {
	// AnimOnly.exe 0x4D7281 (NiBezierMesh.cpp)
	children.Compact();

	for ( unsigned int i = 0; i < GetArrayCount(); i++ ) {
		NiPointer<NiBezierPatch> patch0 = NiDynamicCast( NiBezierPatch, GetAt( i ) );
		assert( patch0 );
		for ( unsigned int j = i + 1; j < GetArrayCount(); j++ ) {
			NiPointer<NiBezierPatch> patch1 = NiDynamicCast( NiBezierPatch, GetAt( j ) );
			assert( patch1 );

			// the corner indices of each side: a rectangle's first row,
			// first column, last column and last row; a triangle's first
			// row, first column and diagonal end at its apex
			unsigned short degree0 = patch0->GetDegree();
			unsigned short degree1 = patch1->GetDegree();
			unsigned short start0[4] = { 0, 0, degree0, degree0 * ( degree0 + 1 ) };
			unsigned short end0[4] = { degree0, degree0 * ( degree0 + 1 ), degree0 * ( degree0 + 2 ), degree0 * ( degree0 + 2 ) };
			unsigned short start1[4] = { 0, 0, degree1, degree1 * ( degree1 + 1 ) };
			unsigned short end1[4] = { degree1, degree1 * ( degree1 + 1 ), degree1 * ( degree1 + 2 ), degree1 * ( degree1 + 2 ) };
			unsigned short sides0;
			unsigned short sides1;
			if ( NiIsKindOf( NiBezierRectangle, patch0 ) ) {
				sides0 = 4;
			} else if ( NiIsKindOf( NiBezierTriangle, patch0 ) ) {
				sides0 = 3;
				end0[1] = end0[2] = ( degree0 + 1 ) * ( degree0 + 2 ) / 2 - 1;
			}
			if ( NiIsKindOf( NiBezierRectangle, patch1 ) ) {
				sides1 = 4;
			} else if ( NiIsKindOf( NiBezierTriangle, patch1 ) ) {
				sides1 = 3;
				end1[1] = end1[2] = ( degree1 + 1 ) * ( degree1 + 2 ) / 2 - 1;
			}

			for ( unsigned short side0 = 0; side0 < sides0; side0++ ) {
				NiPoint3 corner0;
				NiPoint3 corner1;
				if ( patch0->GetCtrlIndex() ) {
					corner0 = vertices[patch0->GetCtrlIndex()[start0[side0]]];
					corner1 = vertices[patch0->GetCtrlIndex()[end0[side0]]];
				} else {
					corner0 = patch0->GetCtrlPoint()[start0[side0]];
					corner1 = patch0->GetCtrlPoint()[end0[side0]];
				}
				for ( unsigned short side1 = 0; side1 < sides1; side1++ ) {
					NiPoint3 corner2;
					NiPoint3 corner3;
					if ( patch1->GetCtrlIndex() ) {
						corner2 = vertices[patch1->GetCtrlIndex()[start1[side1]]];
						corner3 = vertices[patch1->GetCtrlIndex()[end1[side1]]];
					} else {
						corner2 = patch1->GetCtrlPoint()[start1[side1]];
						corner3 = patch1->GetCtrlPoint()[end1[side1]];
					}
					if ( corner0.x == corner2.x && corner0.y == corner2.y && corner0.z == corner2.z &&
						 corner1.x == corner3.x && corner1.y == corner3.y && corner1.z == corner3.z ) {
						NiPointer<NiBezierRectangle> rectangle0 = NiDynamicCast( NiBezierRectangle, patch0 );
						NiPointer<NiBezierTriangle> triangle0 = NiDynamicCast( NiBezierTriangle, patch0 );
						NiPointer<NiBezierRectangle> rectangle1 = NiDynamicCast( NiBezierRectangle, patch1 );
						NiPointer<NiBezierTriangle> triangle1 = NiDynamicCast( NiBezierTriangle, patch1 );
						if ( rectangle0 ) {
							rectangle0->SetNeighbor( side0, j, side1 + 1 );
						} else if ( triangle0 ) {
							triangle0->SetNeighbor( side0, j, side1 + 1 );
						} else {
							assert( 0 );
						}
						if ( rectangle1 ) {
							rectangle1->SetNeighbor( side1, i, side0 + 1 );
						} else if ( triangle1 ) {
							triangle1->SetNeighbor( side1, i, side0 + 1 );
						} else {
							assert( 0 );
						}
					} else if ( corner0.x == corner3.x && corner0.y == corner3.y && corner0.z == corner3.z &&
								corner1.x == corner2.x && corner1.y == corner2.y && corner1.z == corner2.z ) {
						NiPointer<NiBezierRectangle> rectangle0 = NiDynamicCast( NiBezierRectangle, patch0 );
						NiPointer<NiBezierTriangle> triangle0 = NiDynamicCast( NiBezierTriangle, patch0 );
						NiPointer<NiBezierRectangle> rectangle1 = NiDynamicCast( NiBezierRectangle, patch1 );
						NiPointer<NiBezierTriangle> triangle1 = NiDynamicCast( NiBezierTriangle, patch1 );
						if ( rectangle0 ) {
							rectangle0->SetNeighbor( side0, j, -( side1 + 1 ) );
						} else if ( triangle0 ) {
							triangle0->SetNeighbor( side0, j, -( side1 + 1 ) );
						} else {
							assert( 0 );
						}
						if ( rectangle1 ) {
							rectangle1->SetNeighbor( side1, i, -( side0 + 1 ) );
						} else if ( triangle1 ) {
							triangle1->SetNeighbor( side1, i, -( side0 + 1 ) );
						} else {
							assert( 0 );
						}
					}
				}
			}
		}
	}
}

/*
================
NiBezierMesh::Unknown8308D0
================
*/
// FUNCTION: STBC 0x8308D0
void NiBezierMesh::Unknown8308D0() {
	if ( edgeCount == 0 ) {
		return;
	}

	NiPoint2 *newTextures = 0;
	unsigned short newCount = vertexCount + 3 * edgeCount;
	NiPoint3 *newVertices = new NiPoint3[newCount];
	memcpy( newVertices, vertices, vertexCount * sizeof( NiPoint3 ) );
	delete [] vertices;

	if ( textures ) {
		newTextures = new NiPoint2[newCount];
		memcpy( newTextures, textures, vertexCount * sizeof( NiPoint2 ) );
		delete [] textures;
	}

	NiColorA *newColors = 0;
	if ( colors ) {
		// BUG: the grown colors are copied into a block that is never
		// stored: it leaks, and the mesh is left without colors
		NiColorA *grownColors = new NiColorA[newCount];
		memcpy( grownColors, colors, vertexCount * sizeof( NiColorA ) );
		delete [] colors;
	}

	vertexCount = newCount;
	vertices = newVertices;
	textures = newTextures;
	colors = newColors;
	Unknown830B80();

	for ( unsigned int i = 0; i < GetArrayCount(); i++ ) {
		NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, GetAt( i ) );
		if ( patch ) {
			patch->SetArrays( vertices, textures, colors );
			patch->Unknown8330E0( unknownFE );
		}
	}
}

/*
================
NiBezierMesh::Unknown830B80

Places each edge's three interior control points, at the quarter steps
between consecutive points of the edge's index list, at the end of the
vertex arrays.
================
*/
// FUNCTION: STBC 0x830B80
// The statements are the debug copy's (AnimOnly.exe 0x4D82C7).
void NiBezierMesh::Unknown830B80() {
	unsigned int i;
	unsigned int j;

	for ( i = 0; i < edgeCount; i++ ) {
		for ( j = 0; j < 3; j++ ) {
			unsigned short index = vertexCount + j - 3 * ( edgeCount - i );
			unsigned short first = edges->GetAt( 4 * i + j );
			unsigned short second = edges->GetAt( 4 * i + j + 1 );
			float firstWeight = ( j + 1 ) / 4.0f;
			float secondWeight = ( 3 - j ) / 4.0f;
			vertices[index] = firstWeight * vertices[first] + secondWeight * vertices[second];
			if ( textures ) {
				textures[index] = firstWeight * textures[first] + secondWeight * textures[second];
			}
			if ( colors ) {
				colors[index] = firstWeight * colors[first] + secondWeight * colors[second];
			}
		}
	}
}

/*
================
NiBezierMesh::CreateClone
================
*/
// FUNCTION: STBC 0x830F50
NiObject *NiBezierMesh::CreateClone() {
	NiBezierMesh *mesh = new NiBezierMesh;
	assert( mesh );
	CopyMembers( mesh );
	return mesh;
}

/*
================
NiBezierMesh::CopyMembers

The clone gets its own copies of the control vertex arrays and of the edge
indices.
================
*/
// FUNCTION: STBC 0x8310E0
void NiBezierMesh::CopyMembers( NiBezierMesh *dest ) {
	NiNode::CopyMembers( dest );

	dest->vertexCount = vertexCount;
	dest->unknownFF = unknownFF;
	dest->VirtualSlot44( GetUnknownFE() );

	dest->vertices = new NiPoint3[vertexCount];
	memcpy( dest->vertices, vertices, vertexCount * sizeof( NiPoint3 ) );
	if ( textures ) {
		dest->textures = new NiPoint2[vertexCount];
		memcpy( dest->textures, textures, vertexCount * sizeof( NiPoint2 ) );
	}
	if ( colors ) {
		dest->colors = new NiColorA[vertexCount];
		memcpy( dest->colors, colors, vertexCount * sizeof( NiColorA ) );
	}

	if ( edgeCount ) {
		dest->edges = new NiTSet<unsigned short>( edgeCount, 2 );
		for ( unsigned short i = 0; i < edgeCount * 4; i++ ) {
			dest->edges->Add( edges->GetAt( i ) );
		}
	}
	dest->edgeCount = edgeCount;
}

/*
================
NiBezierMesh::ProcessClone

The clone's patches are handed the clone's own arrays and tessellate at the
clone's level.
================
*/
// FUNCTION: STBC 0x8313D0
void NiBezierMesh::ProcessClone( NiObject *clone ) {
	NiNode::ProcessClone( clone );

	NiBezierMesh *mesh = (NiBezierMesh *)clone;
	for ( unsigned int i = 0; i < mesh->GetArrayCount(); i++ ) {
		NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, mesh->GetAt( i ) );
		if ( patch ) {
			patch->SetArrays( mesh->vertices, mesh->textures, mesh->colors );
			patch->Unknown8330E0( mesh->unknownFE );
		}
	}
}

/*
================
NiBezierMesh::CreateObject

Creates the mesh and loads it.
================
*/
// FUNCTION: STBC 0x831510
void NiBezierMesh::CreateObject( NiStream &stream, NiObject *&object ) {
	NiBezierMesh *mesh = new NiBezierMesh;
	stream.SetLoadRecord( new NiBezierMeshLoadRecord );
	mesh->LoadBinary( stream );
	object = mesh;
}

/*
================
NiBezierMesh::LoadBinary

The pointers are read as presence flags, each followed by its array; the
edge indices are read one at a time. The vertices are read as floats, three
per vertex, as the SDK's debug NetImmerse 3.1 reads them (AnimOnly.exe
0x4D8CB3, NiBezierMesh.cpp).
================
*/
// FUNCTION: STBC 0x831730
void NiBezierMesh::LoadBinary( NiStream &stream ) {
	NiNode::LoadBinary( stream );

	NiStreamLoadBinary( stream, vertexCount );
	NiStreamLoadBinary( stream, unknownFE );
	NiStreamLoadBinary( stream, unknownFF );

	vertices = new NiPoint3[vertexCount];
	NiStreamLoadBinary( stream, &vertices->x, vertexCount * 3 );

	NiStreamLoadBinary( stream, textures );
	if ( textures ) {
		textureSets = 1;
		textures = new NiPoint2[vertexCount];
		NiStreamLoadBinary( stream, &textures->x, vertexCount * 2 );
	}

	NiStreamLoadBinary( stream, colors );
	if ( colors ) {
		colors = new NiColorA[vertexCount];
		NiStreamLoadBinary( stream, &colors->r, vertexCount * 4 );
	}

	NiStreamLoadBinary( stream, edgeCount );
	if ( edgeCount > 0 ) {
		edges = new NiTSet<unsigned short>( edgeCount, 2 );
		assert( edges );
		for ( unsigned short i = 0; i < edgeCount * 4; i++ ) {
			unsigned short index;
			NiStreamLoadBinary( stream, index );
			edges->Add( index );
		}
	}
}

/*
================
NiBezierMesh::LinkObject

The patches take the mesh's arrays and tessellate at its level.
================
*/
// FUNCTION: STBC 0x8319F0
void NiBezierMesh::LinkObject( NiStream &stream ) {
	NiNode::LinkObject( stream );

	for ( unsigned int i = 0; i < GetArrayCount(); i++ ) {
		NiPointer<NiBezierPatch> patch = NiDynamicCast( NiBezierPatch, GetAt( i ) );
		if ( patch ) {
			patch->SetArrays( vertices, textures, colors );
			if ( textures ) {
				patch->textureSets = 1;
			}
			patch->Unknown8330E0( unknownFE );
		}
	}
}

/*
================
NiBezierMesh::RegisterStreamables
================
*/
// FUNCTION: STBC 0x831B40
bool NiBezierMesh::RegisterStreamables( NiStream &stream ) {
	return NiNode::RegisterStreamables( stream );
}

/*
================
NiBezierMesh::SaveBinary

The arrays follow their pointers, which stand as presence flags; the edge
indices are written one at a time, when there are edges.
================
*/
// FUNCTION: STBC 0x831B50
void NiBezierMesh::SaveBinary( NiStream &stream ) {
	NiNode::SaveBinary( stream );

	NiStreamSaveBinary( stream, vertexCount );
	NiStreamSaveBinary( stream, unknownFE );
	NiStreamSaveBinary( stream, unknownFF );
	NiStreamSaveBinary( stream, (float *)vertices, vertexCount * 3 );

	NiStreamSaveBinary( stream, textures );
	if ( textures ) {
		NiStreamSaveBinary( stream, (float *)textures, vertexCount * 2 );
	}

	NiStreamSaveBinary( stream, colors );
	if ( colors ) {
		NiStreamSaveBinary( stream, (float *)colors, vertexCount * 4 );
	}

	NiStreamSaveBinary( stream, edgeCount );
	if ( edgeCount > 0 ) {
		for ( unsigned short i = 0; i < edgeCount * 4; i++ ) {
			unsigned short index = edges->GetAt( i );
			NiStreamSaveBinary( stream, index );
		}
	}
}

/*
================
NiBezierMesh::IsEqual

The vertices and the optional texture coordinates and colors are compared
element by element.
================
*/
// FUNCTION: STBC 0x831C70
bool NiBezierMesh::IsEqual( NiObject *object ) {
	if ( !NiNode::IsEqual( object ) ) {
		return false;
	}

	NiBezierMesh *mesh = (NiBezierMesh *)object;
	if ( ( flags & UNKNOWN_40_MASK ) != ( mesh->flags & UNKNOWN_40_MASK )
		|| vertexCount != mesh->vertexCount
		|| GetUnknown20() != mesh->GetUnknown20()
		|| unknownFE != mesh->unknownFE
		|| unknownFF != mesh->unknownFF ) {
		return false;
	}

	if ( ( textures && !mesh->textures ) || ( !textures && mesh->textures ) ) {
		return false;
	}
	if ( ( colors && !mesh->colors ) || ( !colors && mesh->colors ) ) {
		return false;
	}

	unsigned short i;
	for ( i = 0; i < vertexCount; i++ ) {
		if ( vertices[i] != mesh->vertices[i] ) {
			return false;
		}
	}
	if ( textures ) {
		for ( i = 0; i < vertexCount; i++ ) {
			if ( textures[i] != mesh->textures[i] ) {
				return false;
			}
		}
	}
	if ( colors ) {
		for ( i = 0; i < vertexCount; i++ ) {
			if ( colors[i] != mesh->colors[i] ) {
				return false;
			}
		}
	}
	return true;
}
