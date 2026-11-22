// NiTriShape.cpp -- triangle list geometry objects

#include "NiRTLib.h"
#include <assert.h>
#include "NiCamera.h"
#include "NiSkinController.h"
#include "NiOBBRoot.h"
#include "NiStream.h"
#include "NiTriShape.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007f3480 _$E30
// FUNCTION: STBC 0x007f3490 _$E45
// FUNCTION: STBC 0x007f34a0 _$E49
// FUNCTION: STBC 0x007f34d0 _$E53
// FUNCTION: STBC 0x007f34e0 _$E57
// FUNCTION: STBC 0x007f34f0 _$E181
// FUNCTION: STBC 0x007f3500 _$E34
// FUNCTION: STBC 0x007f3510 _$E123
// FUNCTION: STBC 0x007f3520 _$E136
// FUNCTION: STBC 0x007f3530 _$E186
// FUNCTION: STBC 0x007f3540 _$E191
// FUNCTION: STBC 0x007f3550 _$E195
// FUNCTION: STBC 0x007f3560 _$E199
// FUNCTION: STBC 0x007f3570 _$E206
// FUNCTION: STBC 0x007f3580 _$E210
// FUNCTION: STBC 0x007f3590 _$E218
// FUNCTION: STBC 0x007f35a0 _$E225
// FUNCTION: STBC 0x007f35b0 _$E229
// FUNCTION: STBC 0x007f35c0 _$E233
// FUNCTION: STBC 0x007f35d0 _$E237
// FUNCTION: STBC 0x007f3600 _$E243

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007f34b0 _$E261
// FUNCTION: STBC 0x007f3420 _$E17
// FUNCTION: STBC 0x007f3440 _$E15
// FUNCTION: STBC 0x007f3450 _$E23
// FUNCTION: STBC 0x007f3470 _$E21

// FUNCTION: STBC 0x007f1eb0 _$E249
// FUNCTION: STBC 0x007f1ed0 _$E246
// DATA: STBC 0x9A1BB8 ?rtti@NiTriShape@@2VNiRTTI@@B
const NiRTTI NiTriShape::rtti( "NiTriShape", &NiTriBasedGeom::rtti );

// DATA: STBC 0x9A1BC8 ?textureTolerance@NiTriShape@@2MA
float NiTriShape::textureTolerance;
// DATA: STBC 0x9A1BCC ?textureToleranceSquared@NiTriShape@@2MA
float NiTriShape::textureToleranceSquared;

/*
================
NiTriShape::Register
================
*/
// FUNCTION: STBC 0x7F1EF0 ?Register@NiTriShape@@SAHXZ
NiImplementRegister( NiTriShape )

/*
================
NiTriShape::NiTriShape

Creates the data, which takes ownership of the arrays.
================
*/
// FUNCTION: STBC 0x7F2040 ??0NiTriShape@@QAE@GPAVNiPoint3@@0PAVNiColorA@@PAVNiPoint2@@GPAG@Z
NiTriShape::NiTriShape( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_, NiPoint2 *textures_, unsigned short triangleCount_, unsigned short *triList_ )
	: NiTriBasedGeom( new NiTriShapeData( vertexCount_, vertices_, normals_, colors_, textures_, triangleCount_, triList_ ) ) {
}

/*
================
NiTriShape::NiTriShape
================
*/
// FUNCTION: STBC 0x7F20D0 ??0NiTriShape@@QAE@PAVNiTriShapeData@@@Z
NiTriShape::NiTriShape( NiTriShapeData *data ) : NiTriBasedGeom( data ) {
}

/*
================
NiTriShape::NiTriShape
================
*/
// FUNCTION: STBC 0x7F20F0 ??0NiTriShape@@QAE@XZ
NiTriShape::NiTriShape() {
}

/*
================
NiTriShape::GetWorldTriangle

NULL vertices when there is no world copy.
================
*/
// FUNCTION: STBC 0x7F2110
void NiTriShape::GetWorldTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 ) {
	if ( worldVertices ) {
		unsigned short i = 3 * index;
		unsigned short *triList = GetTriList();
		vertex0 = &worldVertices[triList[i++]];
		vertex1 = &worldVertices[triList[i++]];
		vertex2 = &worldVertices[triList[i]];
	} else {
		vertex0 = 0;
		vertex1 = 0;
		vertex2 = 0;
	}
}

/*
================
NiTriShape::GetModelTriangle
================
*/
// FUNCTION: STBC 0x7F21C0
void NiTriShape::GetModelTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 ) {
	NiTriShapeData *data = GetShapeData();
	NiPoint3 *vertices = data->GetVertices();
	unsigned short *triList = data->GetTriList();
	unsigned short i = 3 * index;
	vertex0 = &vertices[triList[i++]];
	vertex1 = &vertices[triList[i++]];
	vertex2 = &vertices[triList[i]];
}

/*
================
NiTriShape::GetTriangleIndices
================
*/
// FUNCTION: STBC 0x7F2230
void NiTriShape::GetTriangleIndices( unsigned short index, unsigned short &index0, unsigned short &index1, unsigned short &index2 ) const {
	unsigned short i0 = 3 * index;
	unsigned short i1 = i0 + 1;
	unsigned short i2 = i1 + 1;
	unsigned short *triList = GetTriList();
	index0 = triList[i0];
	index1 = triList[i1];
	index2 = triList[i2];
}

/*
================
NiTriShape::Display

Draws the shape unless the camera's sorter takes it, and counts it.
================
*/
// FUNCTION: STBC 0x7F2280
void NiTriShape::Display( NiCamera *camera ) {
	NiRender *renderer = camera->GetRenderer();
	if ( !renderer->RegisterObject( this ) ) {
		camera->UpdateActivePlanes();
		renderer->VirtualSlot42( worldTranslate, worldRotate, worldScale );
		renderer->VirtualSlot45( GetShapeData(), GetWorldBound() );
		if ( controllers ) {
			controllers->VirtualSlot17();
		}
		shapesDrawn++;
		trianglesDrawn += ( (NiTriShapeData *)GetModelData() )->GetActiveTriangleCount();
		verticesDrawn += GetShapeData()->GetActiveVertexCount();
	}
}

/*
================
NiTriShape::NonDegenerateTriangle

False when the triangle at the triangle list position repeats a vertex, has
an edge shorter than 1e-15, a ratio of two edges' squared lengths outside
1e-7 to 1e7, or an area too small for its edges.
Gamebryo 1.2.2: NiOptimize::NonDegenerateTriangle, there given the
indices and the points; the statements are AnimOnly.exe 0x433D14's
(NiTriShape.cpp): list positions in locals, the list read at each use.
================
*/
// FUNCTION: STBC 0x7F2340
bool NiTriShape::NonDegenerateTriangle( unsigned short first ) {
	unsigned int i0 = first++;
	unsigned int i1 = first++;
	unsigned int i2 = first;
	unsigned short *triList = GetTriList();

	// check for degenerate connectivity
	if ( triList[i0] == triList[i1] || triList[i0] == triList[i2] || triList[i1] == triList[i2] ) {
		return false;
	}

	// check for short edges
	const float minLengthSquared = 1e-30f;

	NiPoint3 *vertices = GetVertices();
	const NiPoint3 &point0 = vertices[triList[i0]];
	const NiPoint3 &point1 = vertices[triList[i1]];
	const NiPoint3 &point2 = vertices[triList[i2]];

	NiPoint3 edge10 = point1 - point0;
	float lengthSquared10 = edge10.SqrLength();
	if ( lengthSquared10 < minLengthSquared ) {
		return false;
	}

	NiPoint3 edge20 = point2 - point0;
	float lengthSquared20 = edge20.SqrLength();
	if ( lengthSquared20 < minLengthSquared ) {
		return false;
	}

	NiPoint3 edge21 = point2 - point1;
	float lengthSquared21 = edge21.SqrLength();
	if ( lengthSquared21 < minLengthSquared ) {
		return false;
	}

	// check for slivers
	const float minEdgeRatio = 1e-7f;
	const float maxEdgeRatio = 1e+7f;

	float edgeRatio = lengthSquared10 / lengthSquared20;
	if ( edgeRatio < minEdgeRatio || edgeRatio > maxEdgeRatio ) {
		return false;
	}

	// check for small area triangles
	const float minSineSquared = 1e-7f;

	NiPoint3 cross = edge10.Cross( edge20 );
	float lengthSquaredCross = cross.SqrLength();
	if ( lengthSquaredCross < lengthSquared10 * lengthSquared20 * minSineSquared ) {
		return false;
	}

	return true;
}

/*
================
NiTriShape::RemoveDegenerateTriangles

Drops the triangles NonDegenerateTriangle rejects, giving the data a list
of the right size when any went; true when none is left.
Gamebryo 1.2.2: NiOptimize::RemoveDegenerateTriangles.
================
*/
// FUNCTION: STBC 0x7F2540
bool NiTriShape::RemoveDegenerateTriangles() {
	NiTriShapeData *data = GetShapeData();
	unsigned short newTriangles = 0;
	unsigned short newPosition = 0;
	unsigned int triListLength = data->GetTriListLength();
	unsigned short *triList = data->GetTriList();

	for ( unsigned short i = 0; i < triListLength; i += 3 ) {
		// if the triangle doesn't have repeated vertices, keep it
		if ( NonDegenerateTriangle( i ) ) {
			if ( newPosition != i ) {
				triList[newPosition] = triList[i];
				triList[newPosition + 1] = triList[i + 1];
				triList[newPosition + 2] = triList[i + 2];
			}
			newTriangles++;
			newPosition += 3;
		}
	}

	// reallocate the triangle list if necessary
	if ( newTriangles != GetShapeData()->GetTriangleCount() ) {
		unsigned int newListLength = 3 * newTriangles;
		unsigned short *newTriList = new unsigned short[newListLength];
		memcpy( newTriList, triList, newListLength * sizeof( newTriList[0] ) );

		GetShapeData()->SetTriangleCount( newTriangles );
		GetShapeData()->Replace( newTriList );
	}

	return newTriangles == 0;
}

/*
================
NiTriShape::VertexCompare

Compares two vertices member by member; the mask selects the members.
Gamebryo 1.2.2: NiGeometryData::VertexCompare.
================
*/
// FUNCTION: STBC 0x7F2630
int NiTriShape::VertexCompare( unsigned short index0, unsigned short index1, unsigned char mask ) {
	NiTriShapeData *data = GetShapeData();

	NiPoint3 *vertices = data->GetVertices();
	if ( mask & VERTEX_MASK ) {
		int result = memcmp( &vertices[index0], &vertices[index1], sizeof( NiPoint3 ) );
		if ( result ) {
			return result;
		}
	}

	NiPoint3 *normals = data->GetNormals();
	if ( normals && ( mask & NORMAL_MASK ) ) {
		int result = memcmp( &normals[index0], &normals[index1], sizeof( NiPoint3 ) );
		if ( result ) {
			return result;
		}
	}

	NiColorA *colors = data->GetColors();
	if ( colors && ( mask & COLOR_MASK ) ) {
		int result = memcmp( &colors[index0], &colors[index1], sizeof( NiColorA ) );
		if ( result ) {
			return result;
		}
	}

	NiPoint2 *textures = data->GetTextures();
	if ( textures && ( mask & TEXTURE_MASK ) ) {
		unsigned short vertexCount = data->GetVertexCount();
		unsigned short textureSets = data->GetTextureSets();
		unsigned short offset = 0;
		for ( unsigned short i = 0; i < textureSets; i++ ) {
			int result = memcmp( &textures[offset + index0], &textures[offset + index1], sizeof( NiPoint2 ) );
			if ( result ) {
				return result;
			}
			offset += vertexCount;
		}
	}
	return 0;
}

/*
================
NiTriShape::CollapseVertices

Merges the vertices that compare equal, renumbering the triangles, then
drops the vertices no triangle uses. With a texture tolerance the texture
coordinates of two vertices need only lie within it, and every earlier
vertex equal in the other members is tried.
Gamebryo 1.2.2: NiOptimize::CollapseVertices.
================
*/
// FUNCTION: STBC 0x7F27B0
// The debug NetImmerse 3.1's statements (AnimOnly 0x434330).
void NiTriShape::CollapseVertices( unsigned short **newToOld ) {
	bool exactTextures = false;
	unsigned char mask;
	if ( textureTolerance == 0.0f ) {
		exactTextures = true;
		mask = ~0;
	} else {
		mask = ~TEXTURE_MASK;
	}

	unsigned short *index0;
	unsigned short *index1;
	IndexSort( index0, index1, mask );

	unsigned short vertexCount = GetVertexCount();
	NiPoint2 *textures = GetTextures();
	unsigned int i;

	if ( exactTextures || !textures ) {
		// When all components are compared, merge-able vertices must be
		// adjacent.
		for ( i = 1; i < vertexCount; i++ ) {
			if ( VertexCompare( index0[i], index0[i - 1], mask ) == 0 ) {
				index0[i] = index0[i - 1];
			}
		}
	} else {
		for ( i = 1; i < vertexCount; i++ ) {
			for ( int j = i - 1; j >= 0 && VertexCompare( index0[i], index0[j], mask ) == 0; j-- ) {
				bool merge = VertexCompare( index0[i], index0[j], TEXTURE_MASK ) == 0;
				if ( !merge ) {
					// within the tolerance in every texture coordinate set
					unsigned short offset = 0;
					unsigned short textureSets = GetTextureSets();
					merge = true;
					for ( unsigned short k = 0; k < textureSets; k++ ) {
						unsigned short a = index0[i] + offset;
						unsigned short b = index0[j] + offset;
						float du = textures[a].x - textures[b].x;
						float dv = textures[a].y - textures[b].y;
						float distanceSquared = du * du + dv * dv;
						if ( distanceSquared > textureToleranceSquared ) {
							merge = false;
							break;
						}
						offset += vertexCount;
					}
				}
				if ( merge ) {
					index0[i] = index0[j];
					break;
				}
			}
		}
	}

	unsigned int triListLength = GetTriListLength();
	unsigned short *triList = GetTriList();
	for ( i = 0; i < triListLength; i++ ) {
		triList[i] = index0[index1[triList[i]]];
	}

	delete [] index0;
	delete [] index1;

	RemoveUnusedVertices( newToOld );
}

/*
================
NiTriShape::IndexSort

After the sort, vertex index0[i] is the ith smallest and index1[i] tells
where the ith vertex ended up, so index0[index1[i]] == i. Both arrays are
allocated here, for the caller to delete.
Gamebryo 1.2.2: NiGeometryData::IndexSort.
================
*/
// FUNCTION: STBC 0x7F29F0
void NiTriShape::IndexSort( unsigned short *&index0, unsigned short *&index1, unsigned char mask ) {
	unsigned short vertexCount = GetShapeData()->GetVertexCount();
	index0 = new unsigned short[vertexCount];
	index1 = new unsigned short[vertexCount];

	unsigned short i;
	for ( i = 0; i < vertexCount; i++ ) {
		index0[i] = i;
	}

	IndexQSort( index0, mask, 0, vertexCount - 1 );

	for ( i = 0; i < vertexCount; i++ ) {
		index1[index0[i]] = i;
	}
}

/*
================
NiTriShape::IndexQSort

Gamebryo 1.2.2: NiGeometryData::IndexQSort.
================
*/
// FUNCTION: STBC 0x7F2A90
void NiTriShape::IndexQSort( unsigned short *index, unsigned char mask, int l, int r ) {
	if ( r > l ) {
		int i, j;

		i = l - 1;
		j = r + 1;
		int pivot = ChoosePivot( index, mask, l, r );

		for ( ;; ) {
			do {
				j--;
			} while ( VertexCompare( pivot, index[j], mask ) < 0 );

			do {
				i++;
			} while ( VertexCompare( index[i], pivot, mask ) < 0 );

			if ( i < j ) {
				unsigned short temp;
				temp = index[i];
				index[i] = index[j];
				index[j] = temp;
			} else {
				break;
			}
		}

		if ( j == r ) {
			IndexQSort( index, mask, l, j - 1 );
		} else {
			IndexQSort( index, mask, l, j );
			IndexQSort( index, mask, j + 1, r );
		}
	}
}

/*
================
NiTriShape::ChoosePivot

Of the first, middle and last elements, the one between the other two; the
middle when all three are equal.
Gamebryo 1.2.2: NiGeometryData::ChoosePivot.
================
*/
// FUNCTION: STBC 0x7F2B90
int NiTriShape::ChoosePivot( unsigned short *index, unsigned char mask, int l, int r ) {
	int m = ( l + r ) >> 1;

	if ( VertexCompare( index[l], index[m], mask ) < 0 ) {
		if ( VertexCompare( index[m], index[r], mask ) < 0 ) {
			return index[m];
		} else {
			if ( VertexCompare( index[l], index[r], mask ) < 0 ) {
				return index[r];
			} else {
				return index[l];
			}
		}
	} else {
		if ( VertexCompare( index[l], index[r], mask ) < 0 ) {
			return index[l];
		} else {
			if ( VertexCompare( index[m], index[r], mask ) < 0 ) {
				return index[r];
			} else {
				return index[m];
			}
		}
	}
}

/*
================
NiTriShape::RemoveUnusedVertices

Renumbers the vertices in the order the triangles first use them and drops
the rest, telling a skin controller of the shape; newToOld, when given,
receives each new vertex's old index.
Gamebryo 1.2.2: NiOptimize::RemoveUnusedVertices.
================
*/
// FUNCTION: STBC 0x7F2C70
void NiTriShape::RemoveUnusedVertices( unsigned short **newToOld ) {
	unsigned short vertexCount = GetShapeData()->GetVertexCount();
	int *oldToNew = new int[vertexCount];

	unsigned short v;
	for ( v = 0; v < vertexCount; v++ ) {
		oldToNew[v] = -1;
	}

	unsigned short newVertexCount = 0;

	unsigned int triListLength = GetTriListLength();
	unsigned short *triList = GetTriList();
	unsigned int i;
	for ( i = 0; i < triListLength; i++ ) {
		if ( oldToNew[triList[i]] == -1 ) {
			oldToNew[triList[i]] = newVertexCount++;
		}
		triList[i] = oldToNew[triList[i]];
	}

	if ( newToOld ) {
		*newToOld = new unsigned short[newVertexCount];
		for ( v = 0; v < vertexCount; v++ ) {
			if ( oldToNew[v] != -1 ) {
				( *newToOld )[oldToNew[v]] = v;
			}
		}
	}

	// Save a copy for our controller to work with.
	NiPoint3 *oldVertices = GetShapeData()->GetVertices();
	NiPoint3 *old = new NiPoint3[vertexCount];
	for ( i = 0; i < vertexCount; i++ ) {
		old[i] = oldVertices[i];
	}

	ReallocateVertexArrays( newVertexCount, oldToNew );

	for ( NiTimeController *controller = GetControllers(); controller; controller = controller->GetNext() ) {
		NiSkinController *skinController = NiDynamicCast( NiSkinController, controller );
		if ( skinController ) {
			skinController->VirtualSlot20( vertexCount, 0, newVertexCount, oldToNew );
		}
	}

	// BUG: redundant: the copy goes to no controller and is freed unread
	delete [] old;
	delete [] oldToNew;
}

/*
================
NiTriShape::ReallocateVertexArrays

Rebuilds the model data's arrays with every vertex at the position the map
gives it. Gamebryo 1.2.2: NiOptimize::ReallocateVertexArrays.
================
*/
// FUNCTION: STBC 0x7F2E30
// The debug NetImmerse 3.1's statements (AnimOnly 0x434D76, NiTriShape.cpp: its asserts at lines
// 558-580 name pNewVertex, pNewNormal, pNewColor and pNewTexture).
void NiTriShape::ReallocateVertexArrays( unsigned short newVertexCount, int *map ) {
	unsigned short vertexCount = GetVertexCount();
	NiPoint3 *vertices = GetVertices();
	NiPoint3 *normals = GetNormals();
	NiColorA *colors = GetColors();
	unsigned short textureSets = GetTextureSets();
	NiPoint2 *textures = GetTextures();

	NiPoint3 *newVertices = new NiPoint3[newVertexCount];
	assert( newVertices );
	NiPoint3 *newNormals = 0;
	if ( normals ) {
		newNormals = new NiPoint3[newVertexCount];
		assert( newNormals );
	}
	NiColorA *newColors = 0;
	if ( colors ) {
		newColors = new NiColorA[newVertexCount];
		assert( newColors );
	}
	NiPoint2 *newTextures = 0;
	if ( textures ) {
		unsigned short textureCount = newVertexCount * textureSets;
		newTextures = new NiPoint2[textureCount];
		assert( newTextures );
	}

	for ( unsigned short i = 0; i < vertexCount; i++ ) {
		int j = map[i];
		if ( j != -1 ) {
			newVertices[j] = vertices[i];
			if ( normals ) {
				newNormals[j] = normals[i];
			}
			if ( colors ) {
				newColors[j] = colors[i];
			}
			if ( textures ) {
				for ( unsigned short k = 0; k < textureSets; k++ ) {
					unsigned short to = j + k * newVertexCount;
					unsigned short from = i + k * vertexCount;
					newTextures[to] = textures[from];
				}
			}
		}
	}

	modelData->NiGeometryData::Replace( newVertexCount, newVertices, newNormals, newColors, newTextures );
	modelData->SetTextureSets( textureSets );
}

/*
================
NiTriShape::VirtualSlot23

Builds the bounding box tree once, over the model and world vertices.
================
*/
// FUNCTION: STBC 0x7F30D0
void NiTriShape::VirtualSlot23( int unknown ) {
	if ( !obbRoot ) {
		obbRoot = new NiOBBRoot( GetTriangleCount(), GetTriList(), GetVertices(), GetWorldVertices(), unknown );
		assert( obbRoot );
	}
}

/*
================
NiTriShape::CreateClone
================
*/
// FUNCTION: STBC 0x7F3160
NiObject *NiTriShape::CreateClone() {
	NiTriShape *clone = new NiTriShape;
	CopyMembers( clone );
	return clone;
}

/*
================
NiTriShape::CopyMembers
================
*/
// FUNCTION: STBC 0x7F31D0
void NiTriShape::CopyMembers( NiTriShape *dest ) {
	NiTriBasedGeom::CopyMembers( dest );
}

/*
================
NiTriShape::ProcessClone
================
*/
// FUNCTION: STBC 0x7F31E0
void NiTriShape::ProcessClone( NiObject *clone ) {
	assert( NiIsKindOf( NiTriShape, clone ) );
	NiTriBasedGeom::ProcessClone( clone );
}

/*
================
NiTriShape::CreateObject

Creates the shape and loads it.
================
*/
// FUNCTION: STBC 0x7F31F0
void NiTriShape::CreateObject( NiStream &stream, NiObject *&object ) {
	NiTriShape *shape = new NiTriShape;
	stream.SetLoadRecord( new NiTriShapeLoadRecord );
	shape->LoadBinary( stream );
	object = shape;
}

/*
================
NiTriShape::LoadBinary
================
*/
// FUNCTION: STBC 0x7F32D0
void NiTriShape::LoadBinary( NiStream &stream ) {
	NiTriBasedGeom::LoadBinary( stream );
}

/*
================
NiTriShape::LinkObject
================
*/
// FUNCTION: STBC 0x7F32E0
void NiTriShape::LinkObject( NiStream &stream ) {
	NiTriBasedGeom::LinkObject( stream );
}

/*
================
NiTriShape::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7F32F0
bool NiTriShape::RegisterStreamables( NiStream &stream ) {
	return NiTriBasedGeom::RegisterStreamables( stream );
}

/*
================
NiTriShape::SaveBinary
================
*/
// FUNCTION: STBC 0x7F3300
void NiTriShape::SaveBinary( NiStream &stream ) {
	NiTriBasedGeom::SaveBinary( stream );
}

/*
================
NiTriShape::IsEqual
================
*/
// FUNCTION: STBC 0x7F3310
bool NiTriShape::IsEqual( NiObject *object ) {
	return NiTriBasedGeom::IsEqual( object );
}

/*
================
NiTriShape::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7F3320
void NiTriShape::GetViewerStrings( NiTArray<char *> *strings ) {
	NiTriBasedGeom::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
