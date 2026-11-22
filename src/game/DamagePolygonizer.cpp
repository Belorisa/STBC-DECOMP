/*
===============================================================================

	DamagePolygonizer.cpp

	Turns a VisibleDamage's volumes into triangles.

===============================================================================
*/

#include <iostream>
#include "DamagePolygonizer.h"
#include "NiMaterialProperty.h"
#include "NiTextureModeProperty.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"
#include "NiLines.h"
#include "NiLinesData.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004c4bb0 _$E447
// FUNCTION: STBC 0x004c4bd0 _$E446

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004c2510 _$E32
// FUNCTION: STBC 0x004c2520 _$E29
// FUNCTION: STBC 0x004c2530 _$E31
// FUNCTION: STBC 0x004c2540 _$E30
// FUNCTION: STBC 0x004c2900 _$E38
// FUNCTION: STBC 0x004c2910 _$E35
// FUNCTION: STBC 0x004c2920 _$E37
// FUNCTION: STBC 0x004c2930 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004c3b00 _$E45
// FUNCTION: STBC 0x004c3b10 _$E44
// FUNCTION: STBC 0x004c3b20 _$E70
// FUNCTION: STBC 0x004c3b30 _$E69
// FUNCTION: STBC 0x004c3b40 _$E82
// FUNCTION: STBC 0x004c3b50 _$E81
// FUNCTION: STBC 0x004c3b60 _$E86
// FUNCTION: STBC 0x004c3b70 _$E85
// FUNCTION: STBC 0x004c3b80 _$E90
// FUNCTION: STBC 0x004c3b90 _$E89
// FUNCTION: STBC 0x004c3ba0 _$E205
// FUNCTION: STBC 0x004c3bb0 _$E204
// FUNCTION: STBC 0x004c3c90 _$E219
// FUNCTION: STBC 0x004c3ca0 _$E218
// FUNCTION: STBC 0x004c3f90 _$E226
// FUNCTION: STBC 0x004c3fa0 _$E225
// FUNCTION: STBC 0x004c4150 _$E230
// FUNCTION: STBC 0x004c4160 _$E229
// FUNCTION: STBC 0x004c4170 _$E234
// FUNCTION: STBC 0x004c4180 _$E233
// FUNCTION: STBC 0x004c41d0 _$E238
// FUNCTION: STBC 0x004c41e0 _$E237
// FUNCTION: STBC 0x004c42a0 _$E244
// FUNCTION: STBC 0x004c42b0 _$E243
// FUNCTION: STBC 0x004c42c0 _$E210
// FUNCTION: STBC 0x004c42d0 _$E209
// FUNCTION: STBC 0x004c42e0 _$E214
// FUNCTION: STBC 0x004c42f0 _$E213
// FUNCTION: STBC 0x004c43c0 _$E147
// FUNCTION: STBC 0x004c43d0 _$E146
// FUNCTION: STBC 0x004c43e0 _$E160
// FUNCTION: STBC 0x004c43f0 _$E159
// FUNCTION: STBC 0x004c45d0 _$E180
// FUNCTION: STBC 0x004c45e0 _$E179
// FUNCTION: STBC 0x004c4890 _$E74
// FUNCTION: STBC 0x004c48a0 _$E73
// FUNCTION: STBC 0x004c4b90 _$E78
// FUNCTION: STBC 0x004c4ba0 _$E77
// FUNCTION: STBC 0x004c5110 _$E135
// FUNCTION: STBC 0x004c5120 _$E134
// FUNCTION: STBC 0x004c5130 _$E165
// FUNCTION: STBC 0x004c5140 _$E164
// FUNCTION: STBC 0x004c5150 _$E130
// FUNCTION: STBC 0x004c5160 _$E129
// FUNCTION: STBC 0x004c5170 _$E190
// FUNCTION: STBC 0x004c5180 _$E189
// FUNCTION: STBC 0x004c5190 _$E109
// FUNCTION: STBC 0x004c51a0 _$E108
// FUNCTION: STBC 0x004c51b0 _$E200
// FUNCTION: STBC 0x004c51c0 _$E199
// FUNCTION: STBC 0x004c51d0 _$E268
// FUNCTION: STBC 0x004c51e0 _$E267
// FUNCTION: STBC 0x004c51f0 _$E272
// FUNCTION: STBC 0x004c5200 _$E271

// DATA: STBC 0x00983c60
// FUNCTION: STBC 0x004bcbd0 _$E432
FeatureCollector *MetaVolumePolygonizer::mesh = new FeatureCollector( false, false, false );
// DATA: STBC 0x00984478
NiColor MetaVolumePolygonizer::gridColors[5];
// DATA: STBC 0x0098454c
int MetaVolumePolygonizer::gridColorIndex;
// DATA: STBC 0x00984514
unsigned char *MetaVolumePolygonizer::cellBuffers[2];
// DATA: STBC 0x0098451c
unsigned int MetaVolumePolygonizer::cellBufferSize;
// DATA: STBC 0x0098406c
unsigned int MetaVolumePolygonizer::cubeEdges[256];
// DATA: STBC 0x00983c68
unsigned int MetaVolumePolygonizer::cubeTable983C68[256];
// DATA: STBC 0x00984504
DamageBitArray *MetaVolumePolygonizer::unknown984504;
// DATA: STBC 0x00984508
unsigned int *MetaVolumePolygonizer::unknown984508;
// DATA: STBC 0x0098450c
unsigned short MetaVolumePolygonizer::unknown98450C;
// DATA: STBC 0x00984510
unsigned char *MetaVolumePolygonizer::unknown984510;
// DATA: STBC 0x00984520
unsigned char *MetaVolumePolygonizer::unknown984520;
// DATA: STBC 0x00984524
unsigned int MetaVolumePolygonizer::unknown984524;
// DATA: STBC 0x00984528
float *MetaVolumePolygonizer::layerWeightBlocks[2];
// DATA: STBC 0x00984530
unsigned char *MetaVolumePolygonizer::layerWeighedBlocks[2];
// DATA: STBC 0x00984538
int MetaVolumePolygonizer::layerBlockSize;
// DATA: STBC 0x0098453c
unsigned short *MetaVolumePolygonizer::remapBuffer;
// DATA: STBC 0x00984540
int MetaVolumePolygonizer::remapBufferSize;
// DATA: STBC 0x00984544
unsigned char *MetaVolumePolygonizer::usedBuffer;
// DATA: STBC 0x00984548
int MetaVolumePolygonizer::usedBufferSize;

/*
================
MetaVolumePolygonizer::Init
================
*/
// FUNCTION: STBC 0x004bcc70
void MetaVolumePolygonizer::Init() {
	if ( mesh == NULL ) {
		mesh = new FeatureCollector( false, false, false );
	}
	gridColors[0] = NiColor( 1.0f, 0.0f, 0.0f );
	gridColors[1] = NiColor( 0.0f, 1.0f, 0.0f );
	gridColors[2] = NiColor( 1.0f, 1.0f, 0.0f );
	gridColors[3] = NiColor( 1.0f, 0.5f, 0.5f );
	gridColors[4] = NiColor( 0.5f, 0.5f, 0.5f );
	BuildTables();
}

/*
================
MetaVolumePolygonizer::Shutdown
================
*/
// FUNCTION: STBC 0x004bcd80
// PARKED: register choice: retail loads the bit array pointer into EAX and copies it to EDI for its delete, this build loads EDI directly; the rest is exact
void MetaVolumePolygonizer::Shutdown() {
	delete mesh;
	mesh = NULL;
	delete [] unknown984508;
	unknown984508 = NULL;
	unknown98450C = 0;
	delete [] unknown984510;
	unknown984510 = NULL;
	delete unknown984504;
	unknown984504 = NULL;
	delete [] cellBuffers[0];
	cellBuffers[0] = NULL;
	delete [] cellBuffers[1];
	cellBuffers[1] = NULL;
	cellBufferSize = 0;
	delete [] layerWeightBlocks[0];
	layerWeightBlocks[0] = NULL;
	delete [] layerWeightBlocks[1];
	layerWeightBlocks[1] = NULL;
	delete [] layerWeighedBlocks[0];
	layerWeighedBlocks[0] = NULL;
	delete [] layerWeighedBlocks[1];
	layerWeighedBlocks[1] = NULL;
	layerBlockSize = 0;
	delete [] unknown984520;
	unknown984524 = 0;
}

/*
================
MetaVolumePolygonizer::BuildTables

For each pattern of a cell's eight corners (bit n is corner n, x fastest),
one bit per part of the cell's 3x3x3 lattice other than its centre: the
corners, the edges' midpoints and the faces' centres, in lattice order.
cubeEdges has a part's bit when every corner it touches is set,
cubeTable983C68 when any is.
================
*/
// FUNCTION: STBC 0x004bce80
void MetaVolumePolygonizer::BuildTables() {
	for ( int i = 0; i < 256; i++ ) {
		cubeEdges[i] = 0;
		cubeTable983C68[i] = 0;
		bool c0 = ( i & 0x1 ) != 0;
		bool c1 = ( i & 0x2 ) != 0;
		bool c2 = ( i & 0x4 ) != 0;
		bool c3 = ( i & 0x8 ) != 0;
		bool c4 = ( i & 0x10 ) != 0;
		bool c5 = ( i & 0x20 ) != 0;
		bool c6 = ( i & 0x40 ) != 0;
		bool c7 = ( i & 0x80 ) != 0;

		if ( c0 ) { cubeEdges[i] |= 0x1; cubeTable983C68[i] |= 0x1; }
		if ( c0 && c1 ) { cubeEdges[i] |= 0x2; }
		if ( c0 || c1 ) { cubeTable983C68[i] |= 0x2; }
		if ( c1 ) { cubeEdges[i] |= 0x4; cubeTable983C68[i] |= 0x4; }
		if ( c0 && c2 ) { cubeEdges[i] |= 0x8; }
		if ( c0 || c2 ) { cubeTable983C68[i] |= 0x8; }
		if ( c0 && c1 && c2 && c3 ) { cubeEdges[i] |= 0x10; }
		if ( c0 || c1 || c2 || c3 ) { cubeTable983C68[i] |= 0x10; }
		if ( c1 && c3 ) { cubeEdges[i] |= 0x20; }
		if ( c1 || c3 ) { cubeTable983C68[i] |= 0x20; }
		if ( c2 ) { cubeEdges[i] |= 0x40; cubeTable983C68[i] |= 0x40; }
		if ( c2 && c3 ) { cubeEdges[i] |= 0x80; }
		if ( c2 || c3 ) { cubeTable983C68[i] |= 0x80; }
		if ( c3 ) { cubeEdges[i] |= 0x100; cubeTable983C68[i] |= 0x100; }

		if ( c0 && c4 ) { cubeEdges[i] |= 0x200; }
		if ( c0 || c4 ) { cubeTable983C68[i] |= 0x200; }
		if ( c0 && c1 && c4 && c5 ) { cubeEdges[i] |= 0x400; }
		if ( c0 || c1 || c4 || c5 ) { cubeTable983C68[i] |= 0x400; }
		if ( c1 && c5 ) { cubeEdges[i] |= 0x800; }
		if ( c1 || c5 ) { cubeTable983C68[i] |= 0x800; }
		if ( c0 && c2 && c4 && c6 ) { cubeEdges[i] |= 0x1000; }
		if ( c0 || c2 || c4 || c6 ) { cubeTable983C68[i] |= 0x1000; }
		if ( c1 && c3 && c5 && c7 ) { cubeEdges[i] |= 0x2000; }
		if ( c1 || c3 || c5 || c7 ) { cubeTable983C68[i] |= 0x2000; }
		if ( c2 && c6 ) { cubeEdges[i] |= 0x4000; }
		if ( c2 || c6 ) { cubeTable983C68[i] |= 0x4000; }
		if ( c2 && c3 && c6 && c7 ) { cubeEdges[i] |= 0x8000; }
		if ( c2 || c3 || c6 || c7 ) { cubeTable983C68[i] |= 0x8000; }
		if ( c3 && c7 ) { cubeEdges[i] |= 0x10000; }
		if ( c3 || c7 ) { cubeTable983C68[i] |= 0x10000; }

		if ( c4 ) { cubeEdges[i] |= 0x20000; cubeTable983C68[i] |= 0x20000; }
		if ( c4 && c5 ) { cubeEdges[i] |= 0x40000; }
		if ( c4 || c5 ) { cubeTable983C68[i] |= 0x40000; }
		if ( c5 ) { cubeEdges[i] |= 0x80000; cubeTable983C68[i] |= 0x80000; }
		if ( c4 && c6 ) { cubeEdges[i] |= 0x100000; }
		if ( c4 || c6 ) { cubeTable983C68[i] |= 0x100000; }
		if ( c4 && c5 && c6 && c7 ) { cubeEdges[i] |= 0x200000; }
		if ( c4 || c5 || c6 || c7 ) { cubeTable983C68[i] |= 0x200000; }
		if ( c5 && c7 ) { cubeEdges[i] |= 0x400000; }
		if ( c5 || c7 ) { cubeTable983C68[i] |= 0x400000; }
		if ( c6 ) { cubeEdges[i] |= 0x800000; cubeTable983C68[i] |= 0x800000; }
		if ( c6 && c7 ) { cubeEdges[i] |= 0x1000000; }
		if ( c6 || c7 ) { cubeTable983C68[i] |= 0x1000000; }
		if ( c7 ) { cubeEdges[i] |= 0x2000000; cubeTable983C68[i] |= 0x2000000; }
	}
}

/*
================
MetaVolumePolygonizer::MetaVolumePolygonizer
================
*/
// FUNCTION: STBC 0x004bd2d0
MetaVolumePolygonizer::MetaVolumePolygonizer() : volumes( 2 ), unknownAC( 50 ), grids( 2 ) {
	unknown108 = NULL;
	grid = NULL;
	unknown08 = false;
	unknown09 = false;
	cellCount = 0;
	unknownC4 = 0;
}

/*
================
MetaVolumePolygonizer::~MetaVolumePolygonizer
================
*/
// FUNCTION: STBC 0x004bd4d0
MetaVolumePolygonizer::~MetaVolumePolygonizer() {
	NiTMapIterator pos = unknown88.GetFirstPos();
	delete unknown108;
	while ( pos ) {
		__int64 key;
		DamageIndexBlock *block;
		unknown88.GetNext( pos, key, block );
		delete block;
	}
	delete grid;
	DeleteGrids();
}

/*
================
MetaVolumePolygonizer::DeleteGrids
================
*/
// FUNCTION: STBC 0x004bd710
void MetaVolumePolygonizer::DeleteGrids() {
	NiTListIterator pos = grids.GetHeadPos();
	while ( pos ) {
		DamageVoxelGrid *grid_ = grids.GetNext( pos );
		delete grid_;
	}
	grids.RemoveAll();
}

// a cube's lower corners, as offsets in x and y; the upper ones are the
// same at z + 1
// DATA: STBC 0x0088c6f4
static const int cubeCornerOffsets[4][2] = {
	{ 0, 0 },
	{ 0, 1 },
	{ 1, 1 },
	{ 1, 0 }
};
// the order the corners' bits are tested in
// DATA: STBC 0x0088c714
static const int cubeCornerOrder[4] = { 0, 1, 3, 2 };

/*
================
MetaVolumePolygonizer::CreateDebugNode
================
*/
// FUNCTION: STBC 0x004bd7a0
void MetaVolumePolygonizer::CreateDebugNode( NiNode *parent, NiProperty *property ) {
	// MATCH-PENDING: R17, exact only through the invented DamageVoxelGrid::SetDebugNode (DamageVoxelGrid.h), the one more inline site after the AttachChild that keeps SetClampMode's SetField out of line as retail's; no evidence shows it (user's ruling, 2026-11-18)
	debugNode = new NiNode;
	debugNode->AttachProperty( property );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	textureMode->SetClampMode( NiTextureModeProperty::WRAP_S_WRAP_T );
	textureMode->SetFilterMode( NiTextureModeProperty::FILTER_TRILERP );
	debugNode->AttachProperty( textureMode );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	debugNode->AttachProperty( vertexColor );

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetEmittance( NiColor::WHITE );
	debugNode->AttachProperty( material );

	parent->AttachChild( debugNode, false );
	if ( grid != NULL ) {
		grid->SetDebugNode( debugNode );
	}
}

/*
================
MetaVolumePolygonizer::AddVolume
================
*/
// FUNCTION: STBC 0x004bd9f0
void MetaVolumePolygonizer::AddVolume( MetaVolume *volume ) {
	if ( boundMin.x <= volume->GetBoxMax().x && boundMax.x >= volume->GetBoxMin().x
	  && boundMin.y <= volume->GetBoxMax().y && boundMax.y >= volume->GetBoxMin().y
	  && boundMin.z <= volume->GetBoxMax().z && boundMax.z >= volume->GetBoxMin().z ) {
		volumes.AddTail( volume );
	}
}

/*
================
MetaVolumePolygonizer::NextVolume

The cells around the volume run from its box less one cell up to its box
plus two, clamped to the grid.
================
*/
// FUNCTION: STBC 0x004bdab0
bool MetaVolumePolygonizer::NextVolume( void ) {
	nearVolumes.RemoveAll();
	if ( volumes.GetSize() == 0 ) {
		return false;
	}
	MetaVolume *volume = volumes.RemoveHead();
	for ( int i = 0; i < 3; i++ ) {
		int lower = (int)( ( volume->GetBoxMin()[i] - boundMin[i] ) / cellSize );
		boxLower[i] = lower >= 0 ? lower : 0;
		int upper = (int)( ( volume->GetBoxMax()[i] - boundMin[i] ) / cellSize ) + 2;
		if ( upper > size[i] ) {
			upper = size[i];
		}
		boxUpper[i] = upper;
	}
	NiPoint3 low( boxLower[0] * cellSize + boundMin.x, boxLower[1] * cellSize + boundMin.y, boxLower[2] * cellSize + boundMin.z );
	NiPoint3 high( boxUpper[0] * cellSize + boundMin.x, boxUpper[1] * cellSize + boundMin.y, boxUpper[2] * cellSize + boundMin.z );
	volumeList->GetVolumesInBox( &low, &high, &nearVolumes );
	return true;
}

/*
================
MetaVolumePolygonizer::UpdateVoxel

The corners' weights are kept per layer of the box, so each grid point is
weighed once; a point weighed here for the first time loses its bit in
unknown108 when its weight passes unknown04.
================
*/
// FUNCTION: STBC 0x004bdc30
// PARKED: x87 operand order of the corner offsets (retail loads the corner, then adds cellSize; += and both explicit orders give fld cellSize first)
void MetaVolumePolygonizer::UpdateVoxel( Voxel &cube, unsigned short x, unsigned short y, unsigned short z ) {
	cube.x = x;
	cube.y = y;
	cube.z = z;
	float cornerX = x * cellSize + boundMin.x;
	float cornerY = y * cellSize + boundMin.y;
	float cornerZ = z * cellSize + boundMin.z;
	int i;
	for ( i = 0; i < 8; i++ ) {
		cube.corners[i].x = cornerX;
		cube.corners[i].y = cornerY;
		cube.corners[i].z = cornerZ;
	}
	cube.corners[1].y += cellSize;
	cube.corners[2].x += cellSize;
	cube.corners[2].y += cellSize;
	cube.corners[3].x += cellSize;
	cube.corners[4].z += cellSize;
	cube.corners[5].y += cellSize;
	cube.corners[5].z += cellSize;
	cube.corners[6].x += cellSize;
	cube.corners[6].y += cellSize;
	cube.corners[6].z += cellSize;
	cube.corners[7].x += cellSize;
	cube.corners[7].z += cellSize;
	if ( unknown09 ) {
		cube.key = ( (__int64)x << 32 ) | ( (__int64)y << 16 ) | z;
	}

	bool weighed[8] = { false, false, false, false, false, false, false, false };
	for ( i = 0; i < 4; i++ ) {
		int index = ( y + cubeCornerOffsets[i][1] - boxLower[1] ) * layerSize[0] - boxLower[0] + x + cubeCornerOffsets[i][0];
		if ( !layerWeighed[0][index] ) {
			weighed[i] = true;
			layerWeights[0][index] = MetaVolume::GetTotalStrength( &cube.corners[i], &nearVolumes );
			layerWeighed[0][index] = true;
		}
		cube.weights[i] = layerWeights[0][index];
		if ( !layerWeighed[1][index] ) {
			weighed[i] = true;
			layerWeights[1][index] = MetaVolume::GetTotalStrength( &cube.corners[i + 4], &nearVolumes );
			layerWeighed[1][index] = true;
		}
		cube.weights[i + 4] = layerWeights[1][index];
	}
	if ( !unknown08 ) {
		return;
	}

	unsigned int corners = 0;
	for ( i = 0; i < 4; i++ ) {
		int corner = cubeCornerOrder[i];
		if ( cube.weights[corner] > unknown04 ) {
			if ( weighed[corner] ) {
				ClearPoint( x + cubeCornerOffsets[corner][0], y + cubeCornerOffsets[corner][1], z );
			}
			corners |= 1 << corner;
		}
		if ( cube.weights[corner + 4] > unknown04 ) {
			if ( weighed[corner] ) {
				ClearPoint( x + cubeCornerOffsets[corner][0], y + cubeCornerOffsets[corner][1], z + 1 );
			}
			corners |= 1 << ( corner + 4 );
		}
	}
	if ( corners == 0 ) {
		return;
	}
	if ( corners == 0xFF ) {
		if ( !unknown09 ) {
			grid->Clear( x, y, z );
		}
		return;
	}
	unsigned int edges = voxelData->GetVoxelData()->Unknown4A9190( x, y, z );
	if ( ( cubeEdges[~corners & 0xFF] & edges ) && ( cubeTable983C68[corners] & edges ) ) {
		GetCornerBits( x, y, z );
		unknownC8.Set( GetCellIndex( x, y, z ) );
		if ( ++unknownC4 == 1 ) {
			searchLower[0] = x;
			searchLower[1] = y;
			searchLower[2] = z;
			searchUpper[0] = x + 1;
			searchUpper[1] = y + 1;
			searchUpper[2] = z + 1;
			return;
		}
		if ( searchLower[0] > x ) {
			searchLower[0] = x;
		} else if ( searchUpper[0] < x + 1 ) {
			searchUpper[0] = x + 1;
		}
		if ( searchLower[1] > y ) {
			searchLower[1] = y;
		} else if ( searchUpper[1] < y + 1 ) {
			searchUpper[1] = y + 1;
		}
		if ( searchLower[2] > z ) {
			searchLower[2] = z;
		} else if ( searchUpper[2] < z + 1 ) {
			searchUpper[2] = z + 1;
		}
	}
}

/*
================
MetaVolumePolygonizer::Unknown4BE1B0

Builds the first kind of damage around the volume being worked on: each
cell of the box that the current grid has set and that no earlier volume
reached (unknownA4) is weighed and turned into triangles, from the surface
cell's tree when the voxel data has one and the second kind of damage is on.
The weights are kept per layer of z and handed on to the next.
================
*/
// FUNCTION: STBC 0x004be1b0
// DRAFT: reload order at the x loop's exit: retail reloads y before z, this build z before y (one instruction apart); the rest is exact
void MetaVolumePolygonizer::Unknown4BE1B0( void ) {
	layerSize[0] = boxUpper[0] - boxLower[0] + 1;
	layerSize[1] = boxUpper[1] - boxLower[1] + 1;
	int count = layerSize[1] * layerSize[0];
	if ( layerBlockSize < count ) {
		delete [] layerWeightBlocks[0];
		layerWeightBlocks[0] = new float[count];
		delete [] layerWeightBlocks[1];
		layerWeightBlocks[1] = new float[count];
		delete [] layerWeighedBlocks[0];
		layerWeighedBlocks[0] = new unsigned char[count];
		delete [] layerWeighedBlocks[1];
		layerWeighedBlocks[1] = new unsigned char[count];
		layerBlockSize = count;
	}
	layerWeights[0] = layerWeightBlocks[0];
	layerWeights[1] = layerWeightBlocks[1];
	layerWeighed[0] = layerWeighedBlocks[0];
	layerWeighed[1] = layerWeighedBlocks[1];
	memset( layerWeighed[0], 0, count );
	memset( layerWeighed[1], 0, count );

	Voxel cube;
	float *weights;
	unsigned char *weighed;
	if ( unknown09 ) {
		for ( unsigned short z = boxLower[2]; z < boxUpper[2]; z++ ) {
			for ( unsigned short y = boxLower[1]; y < boxUpper[1]; y++ ) {
				for ( unsigned short x = boxLower[0]; x < boxUpper[0]; x++ ) {
					if ( grid->IsSet( x, y, z ) && !unknownA4.TestAndSet( GetCellIndex( x, y, z ) ) ) {
						UpdateVoxel( cube, x, y, z );
						const VoxelBSPNode *cell;
						if ( voxelData->GetVoxelData()->GetSurfaceCell( x, y, z, cell ) ) {
							Polygonize( cube, voxelData->GetVoxelData()->GetTree( cell ) );
						} else {
							Polygonize( cube );
						}
					}
				}
			}
			weights = layerWeights[0];
			layerWeights[0] = layerWeights[1];
			layerWeights[1] = weights;
			weighed = layerWeighed[0];
			layerWeighed[0] = layerWeighed[1];
			layerWeighed[1] = weighed;
			memset( weighed, 0, count );
		}
	} else {
		for ( unsigned short z = boxLower[2]; z < boxUpper[2]; z++ ) {
			for ( unsigned short y = boxLower[1]; y < boxUpper[1]; y++ ) {
				for ( unsigned short x = boxLower[0]; x < boxUpper[0]; x++ ) {
					if ( grid->IsSet( x, y, z ) && !unknownA4.TestAndSet( GetCellIndex( x, y, z ) ) ) {
						UpdateVoxel( cube, x, y, z );
					}
				}
			}
			weights = layerWeights[0];
			layerWeights[0] = layerWeights[1];
			layerWeights[1] = weights;
			weighed = layerWeighed[0];
			layerWeighed[0] = layerWeighed[1];
			layerWeighed[1] = weighed;
			memset( weighed, 0, layerSize[1] * layerSize[0] );
		}
	}
}

/*
================
MetaVolumePolygonizer::CollectFeatures

Packs what the volumes built: the triangles the maps still name move down
to the front and the vertices only they use likewise, the maps' indices
follow, and the packed arrays become the shape's new data.
================
*/
// FUNCTION: STBC 0x004be640
// DRAFT: structure matches (both packs, the map walks with inline SetAt/RemoveAt, the shape rebuild); retail keeps the remap pointer and the vertex/triangle counts in frame slots and loads the mesh counts before copying this, 4 bytes more frame, register and slot order differ
void MetaVolumePolygonizer::CollectFeatures( void ) {
	int meshTriangles = mesh->triangleCount;
	if ( meshTriangles == 0 ) {
		return;
	}
	unsigned short *remap;
	if ( remapBufferSize < meshTriangles * 2 ) {
		delete [] remapBuffer;
		remap = new unsigned short[meshTriangles];
		remapBuffer = remap;
		remapBufferSize = meshTriangles * 2;
	} else {
		remap = remapBuffer;
	}
	memset( remap, 0xFF, meshTriangles * 2 );
	NiTMapIterator pos = unknown68.GetFirstPos();
	int triangleCount = 0;
	while ( pos ) {
		__int64 key;
		DamageVertexIndices triangles;
		unknown68.GetNext( pos, key, triangles );
		for ( int i = 0; triangles.indices[i] != 0xFFFF; i++ ) {
			remap[triangles.indices[i]] = triangleCount;
			triangles.indices[i] = triangleCount++;
		}
		unknown68.SetAt( key, triangles );
	}
	pos = unknown88.GetFirstPos();
	while ( pos ) {
		__int64 key;
		DamageIndexBlock *block;
		unknown88.GetNext( pos, key, block );
		for ( ; block != NULL; block = block->next ) {
			for ( int i = 0; i < 12 && block->indices[i] != 0xFFFF; i++ ) {
				remap[block->indices[i]] = triangleCount;
				block->indices[i] = triangleCount++;
			}
		}
	}
	unsigned short *triList = new unsigned short[triangleCount * 3];
	mesh->GetTriangles( triList, remap );

	int vertexCount = mesh->vertexCount;
	unsigned char *used;
	if ( usedBufferSize < vertexCount ) {
		delete [] usedBuffer;
		used = new unsigned char[vertexCount];
		usedBuffer = used;
		usedBufferSize = vertexCount;
	} else {
		used = usedBuffer;
	}
	memset( used, 0, vertexCount );
	int t;
	for ( t = 0; t < triangleCount; t++ ) {
		used[triList[t * 3]] = 1;
		used[triList[t * 3 + 1]] = 1;
		used[triList[t * 3 + 2]] = 1;
	}
	if ( remapBufferSize < vertexCount * 2 ) {
		delete [] remapBuffer;
		remap = new unsigned short[vertexCount];
		remapBuffer = remap;
		remapBufferSize = vertexCount * 2;
	} else {
		remap = remapBuffer;
	}
	memset( remap, 0xFF, vertexCount * 2 );
	int newVertexCount = 0;
	int v;
	for ( v = 0; v < vertexCount; v++ ) {
		if ( used[v] ) {
			remap[v] = newVertexCount++;
		}
	}
	pos = unknown78.GetFirstPos();
	while ( pos ) {
		__int64 key;
		VertexInfo vertex;
		unknown78.GetNext( pos, key, vertex );
		unsigned short moved = remap[vertex.index];
		if ( moved == 0xFFFF ) {
			unknown78.RemoveAt( key );
		} else {
			vertex.index = moved;
			vertex.unknown02 = 0;
			unknown78.SetAt( key, vertex );
		}
	}
	NiPoint3 *vertices = new NiPoint3[newVertexCount];
	mesh->GetVertices( remap, vertices, NULL, NULL, NULL );
	for ( t = 0; t < triangleCount; t++ ) {
		triList[t * 3] = remap[triList[t * 3]];
		triList[t * 3 + 1] = remap[triList[t * 3 + 1]];
		triList[t * 3 + 2] = remap[triList[t * 3 + 2]];
	}
	if ( unknown9C != NULL && unknown9C->GetParent() != NULL ) {
		unknown9C->GetParent()->DetachChild( unknown9C );
	}
	NiPoint2 *textures = new NiPoint2[newVertexCount];
	for ( v = 0; v < newVertexCount; v++ ) {
		textures[v] = NiPoint2( ( vertices[v].z - boundMin.z ) / 20.0f, ( vertices[v].y - boundMin.y ) / 20.0f );
	}
	unknown9C = new NiTriShape( newVertexCount, vertices, NULL, NULL, textures, triangleCount, triList );
	debugNode->AttachChild( unknown9C, false );
	unknown9C->Update( 0.0f );
	unknown9C->UpdateProperties();
	unknown9C->UpdateEffects();
}

/*
================
MetaVolumePolygonizer::SetUnknown09
================
*/
// FUNCTION: STBC 0x004bee40
void MetaVolumePolygonizer::SetUnknown09( bool enable ) {
	if ( enable == true && voxelData != NULL && debugNode != NULL ) {
		unknown09 = true;
	} else {
		unknown09 = false;
	}
}

/*
================
MetaVolumePolygonizer::SetUnknown08

Starts with every grid point's bit set and every cell's bit clear.
================
*/
// FUNCTION: STBC 0x004bee70
void MetaVolumePolygonizer::SetUnknown08( bool enable ) {
	if ( enable == true && voxelData != NULL ) {
		delete [] unknown108;
		unsigned int points = ( size[0] + 1 ) * ( size[1] + 1 ) * ( size[2] + 1 );
		unsigned int bytes = ( points >> 3 ) + 1;
		unknown108 = new unsigned char[bytes];
		memset( unknown108, 0xFF, bytes );
		unknownC8.SetSize( size[0] * size[1] * size[2] );
		if ( unknownC8.bits != NULL ) {
			memset( unknownC8.bits, 0, unknownC8.GetByteCount() );
		}
		unknown08 = true;
	} else {
		unknown08 = false;
	}
}

/*
================
MetaVolumePolygonizer::Refresh
================
*/
// FUNCTION: STBC 0x004befa0
// PARKED: inline budget: in the children's RemoveAll retail keeps the null smart pointer temporary in memory and destroys it out of line (0x401EA0), so it has no register free for zero; this build folds the temporary and caches zero in EBP; the rest follows
void MetaVolumePolygonizer::Refresh( void ) {
	if ( !unknown09 && !unknown08 ) {
		return;
	}
	if ( unknown08 ) {
		if ( unknownC8.bits != NULL ) {
			memset( unknownC8.bits, 0, unknownC8.GetByteCount() );
		}
		unknownC4 = 0;
		DeleteGrids();
	}
	if ( volumes.GetSize() == 0 ) {
		return;
	}
	if ( unknown09 ) {
		if ( unknown9C != NULL ) {
			mesh->SetVertexArrays( unknown9C->GetShapeData()->GetVertexCount(), unknown9C->GetShapeData()->GetVertices(), NULL, NULL, NULL );
			mesh->SetTriangleArray( unknown9C->GetShapeData()->GetTriangleCount(), unknown9C->GetTriList() );
		} else {
			mesh->SetVertexArrays( 0, NULL, NULL, NULL, NULL );
			mesh->SetTriangleArray( 0, NULL );
		}
	}
	if ( unknownA4.bits != NULL ) {
		memset( unknownA4.bits, 0, unknownA4.GetByteCount() );
	}
	while ( NextVolume() ) {
		Unknown4BE1B0();
	}
	if ( unknown09 ) {
		CollectFeatures();
	}
	if ( !unknown08 ) {
		return;
	}
	if ( unknown138 != NULL ) {
		unknown138->RemoveAllChildren();
	}
	if ( unknownC4 == 0 ) {
		return;
	}
	int cells = size[2] * size[1] * size[0];
	unsigned int bytes = cells / 8 + 1;
	if ( cellBufferSize < bytes ) {
		delete [] cellBuffers[0];
		cellBuffers[0] = new unsigned char[bytes];
		delete [] cellBuffers[1];
		cellBuffers[1] = new unsigned char[bytes];
		cellBufferSize = bytes;
	}
	unknownE4 = cellBuffers[0];
	unknownE8 = cellBuffers[1];
	do {
		unsigned short cell[3];
		FindSetCell( &cell[0], &cell[1], &cell[2] );
		memset( unknownE4, 0, bytes );
		memset( unknownE8, 0, bytes );
		unknownF0[1] = cell[1];
		unknownF6[1] = cell[1];
		unknownEC = 0;
		unknownF0[0] = cell[0];
		unknownF0[2] = cell[2];
		unknownF6[0] = cell[0];
		unknownF6[2] = cell[2];
		Unknown4C0830( cell[0], cell[1], cell[2] );
		DamageVoxelGrid *grid_ = new DamageVoxelGrid( unknownE4, size, unknownEC, unknownF0, unknownF6, boundMin, cellSize, debugNode );
		grids.AddTail( grid_ );
		if ( unknown138 != NULL ) {
			unknown138->AttachChild( grid_->CreateLines( &gridColors[gridColorIndex++ % 5] ), false );
		}
	} while ( unknownC4 > 0 );
}

/*
================
MetaVolumePolygonizer::SetVoxelData
================
*/
// FUNCTION: STBC 0x004bf440
// PARKED: inline budget: the corners go through SetExtents (DamageTool.exe 0x415380) as retail's nested level shows, each argument read through a fresh GetVoxelData as retail reads the sizes after the copies; retail calls both copies out of line (share < 57 at SetExtents, here 99) yet expands DecRefCount in the first assignment (share >= 68, here 51): about 200 units charged between them that this build doesn't have
void MetaVolumePolygonizer::SetVoxelData( NiBinaryVoxelExtraData *data, bool clear ) {
	voxelData = data;
	SetExtents( voxelData->GetVoxelData()->unknown14, voxelData->GetVoxelData()->unknown20, voxelData->GetVoxelData()->sizeX,
				voxelData->GetVoxelData()->sizeY, voxelData->GetVoxelData()->sizeZ );
	cellSize = voxelData->GetVoxelData()->unknown10;
	cellCount = size[0] * size[1] * size[2];
	unknownA4.SetSize( cellCount );

	unsigned short lower[3];
	unsigned short upper[3];
	lower[0] = 0;
	lower[1] = 0;
	lower[2] = 0;
	upper[0] = size[0] - 1;
	upper[1] = size[1] - 1;
	upper[2] = size[2] - 1;
	grid = new DamageVoxelGrid( NULL, size, size[0] * size[1] * size[2], lower, upper, boundMin, cellSize, debugNode );

	if ( clear ) {
		for ( unsigned short z = 0; z < size[2]; z++ ) {
			for ( unsigned short y = 0; y < size[1]; y++ ) {
				for ( unsigned short x = 0; x < size[0]; x++ ) {
					const VoxelBSPNode *cell;
					if ( !voxelData->GetVoxelData()->IsSolid( x, y, z ) && !voxelData->GetVoxelData()->GetSurfaceCell( x, y, z, cell ) ) {
						grid->Clear( x, y, z );
					}
				}
			}
		}
	}
	cellCount = grid->setCount;
}

// The marching-cube tables (invented names). Retail holds them, with neighbourOffsets, as one run
// of .data in this order (0x8DD350-0x8E0CDC), so none of them is const. The edges a pattern of set
// corners makes the surface cross, one bit per edge
// DATA: STBC 0x008dd350
static unsigned int cubeEdgeFlags[256] = {
	0x000, 0x109, 0x203, 0x30A, 0x406, 0x50F, 0x605, 0x70C,
	0x80C, 0x905, 0xA0F, 0xB06, 0xC0A, 0xD03, 0xE09, 0xF00,
	0x190, 0x099, 0x393, 0x29A, 0x596, 0x49F, 0x795, 0x69C,
	0x99C, 0x895, 0xB9F, 0xA96, 0xD9A, 0xC93, 0xF99, 0xE90,
	0x230, 0x339, 0x033, 0x13A, 0x636, 0x73F, 0x435, 0x53C,
	0xA3C, 0xB35, 0x83F, 0x936, 0xE3A, 0xF33, 0xC39, 0xD30,
	0x3A0, 0x2A9, 0x1A3, 0x0AA, 0x7A6, 0x6AF, 0x5A5, 0x4AC,
	0xBAC, 0xAA5, 0x9AF, 0x8A6, 0xFAA, 0xEA3, 0xDA9, 0xCA0,
	0x460, 0x569, 0x663, 0x76A, 0x066, 0x16F, 0x265, 0x36C,
	0xC6C, 0xD65, 0xE6F, 0xF66, 0x86A, 0x963, 0xA69, 0xB60,
	0x5F0, 0x4F9, 0x7F3, 0x6FA, 0x1F6, 0x0FF, 0x3F5, 0x2FC,
	0xDFC, 0xCF5, 0xFFF, 0xEF6, 0x9FA, 0x8F3, 0xBF9, 0xAF0,
	0x650, 0x759, 0x453, 0x55A, 0x256, 0x35F, 0x055, 0x15C,
	0xE5C, 0xF55, 0xC5F, 0xD56, 0xA5A, 0xB53, 0x859, 0x950,
	0x7C0, 0x6C9, 0x5C3, 0x4CA, 0x3C6, 0x2CF, 0x1C5, 0x0CC,
	0xFCC, 0xEC5, 0xDCF, 0xCC6, 0xBCA, 0xAC3, 0x9C9, 0x8C0,
	0x8C0, 0x9C9, 0xAC3, 0xBCA, 0xCC6, 0xDCF, 0xEC5, 0xFCC,
	0x0CC, 0x1C5, 0x2CF, 0x3C6, 0x4CA, 0x5C3, 0x6C9, 0x7C0,
	0x950, 0x859, 0xB53, 0xA5A, 0xD56, 0xC5F, 0xF55, 0xE5C,
	0x15C, 0x055, 0x35F, 0x256, 0x55A, 0x453, 0x759, 0x650,
	0xAF0, 0xBF9, 0x8F3, 0x9FA, 0xEF6, 0xFFF, 0xCF5, 0xDFC,
	0x2FC, 0x3F5, 0x0FF, 0x1F6, 0x6FA, 0x7F3, 0x4F9, 0x5F0,
	0xB60, 0xA69, 0x963, 0x86A, 0xF66, 0xE6F, 0xD65, 0xC6C,
	0x36C, 0x265, 0x16F, 0x066, 0x76A, 0x663, 0x569, 0x460,
	0xCA0, 0xDA9, 0xEA3, 0xFAA, 0x8A6, 0x9AF, 0xAA5, 0xBAC,
	0x4AC, 0x5A5, 0x6AF, 0x7A6, 0x0AA, 0x1A3, 0x2A9, 0x3A0,
	0xD30, 0xC39, 0xF33, 0xE3A, 0x936, 0x83F, 0xB35, 0xA3C,
	0x53C, 0x435, 0x73F, 0x636, 0x13A, 0x033, 0x339, 0x230,
	0xE90, 0xF99, 0xC93, 0xD9A, 0xA96, 0xB9F, 0x895, 0x99C,
	0x69C, 0x795, 0x49F, 0x596, 0x29A, 0x393, 0x099, 0x190,
	0xF00, 0xE09, 0xD03, 0xC0A, 0xB06, 0xA0F, 0x905, 0x80C,
	0x70C, 0x605, 0x50F, 0x406, 0x30A, 0x203, 0x109, 0x000,
};

// per pattern the triangles of the surface as triples of edges, -1 past the last
// DATA: STBC 0x008dd750
static int cubeTriangleTable[256][13] = {
	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 3, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 0, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 3, 1, 8, 1, 9, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 3, 0, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 0, 2, 9, 2, 10, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 2, 8, 2, 10, 8, 8, 10, 9, -1, -1, -1, -1 },
	{ 11, 2, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 2, 0, 11, 0, 8, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 2, 3, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 1, 11, 1, 9, 11, 11, 9, 8, -1, -1, -1, -1 },
	{ 10, 1, 3, 10, 3, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 0, 10, 0, 8, 10, 10, 8, 11, -1, -1, -1, -1 },
	{ 0, 3, 9, 3, 11, 9, 9, 11, 10, -1, -1, -1, -1 },
	{ 8, 10, 9, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 0, 4, 3, 4, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 9, 0, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 4, 1, 4, 7, 1, 1, 7, 3, -1, -1, -1, -1 },
	{ 10, 1, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 10, 1, 0, 4, 7, 0, 7, 3, -1, -1, -1, -1 },
	{ 4, 7, 8, 0, 2, 10, 0, 10, 9, -1, -1, -1, -1 },
	{ 2, 7, 3, 2, 9, 7, 7, 9, 4, 2, 10, 9, -1 },
	{ 2, 3, 11, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 7, 11, 4, 11, 2, 4, 4, 2, 0, -1, -1, -1, -1 },
	{ 3, 11, 2, 4, 7, 8, 9, 0, 1, -1, -1, -1, -1 },
	{ 2, 7, 11, 2, 1, 7, 1, 4, 7, 1, 9, 4, -1 },
	{ 8, 4, 7, 11, 10, 1, 11, 1, 3, -1, -1, -1, -1 },
	{ 11, 4, 7, 1, 4, 11, 1, 11, 10, 1, 0, 4, -1 },
	{ 3, 8, 0, 7, 11, 4, 11, 9, 4, 11, 10, 9, -1 },
	{ 7, 11, 4, 4, 11, 9, 11, 10, 9, -1, -1, -1, -1 },
	{ 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 0, 8, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 4, 0, 5, 0, 1, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 8, 5, 8, 3, 5, 5, 3, 1, -1, -1, -1, -1 },
	{ 2, 10, 1, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 3, 5, 4, 9, 10, 1, 2, -1, -1, -1, -1 },
	{ 10, 5, 2, 5, 4, 2, 2, 4, 0, -1, -1, -1, -1 },
	{ 3, 4, 8, 3, 2, 4, 2, 5, 4, 2, 10, 5, -1 },
	{ 11, 2, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 5, 4, 8, 11, 2, 8, 2, 0, -1, -1, -1, -1 },
	{ 3, 11, 2, 1, 5, 4, 1, 4, 0, -1, -1, -1, -1 },
	{ 8, 5, 4, 2, 5, 8, 2, 8, 11, 2, 1, 5, -1 },
	{ 5, 4, 9, 1, 3, 11, 1, 11, 10, -1, -1, -1, -1 },
	{ 0, 9, 1, 4, 8, 5, 8, 10, 5, 8, 11, 10, -1 },
	{ 3, 4, 0, 3, 10, 4, 4, 10, 5, 3, 11, 10, -1 },
	{ 4, 8, 5, 5, 8, 10, 8, 11, 10, -1, -1, -1, -1 },
	{ 9, 5, 7, 9, 7, 8, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 9, 3, 9, 5, 3, 3, 5, 7, -1, -1, -1, -1 },
	{ 8, 0, 7, 0, 1, 7, 7, 1, 5, -1, -1, -1, -1 },
	{ 1, 7, 3, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 2, 10, 5, 7, 8, 5, 8, 9, -1, -1, -1, -1 },
	{ 9, 1, 0, 10, 5, 2, 5, 3, 2, 5, 7, 3, -1 },
	{ 5, 2, 10, 8, 2, 5, 8, 5, 7, 8, 0, 2, -1 },
	{ 10, 5, 2, 2, 5, 3, 5, 7, 3, -1, -1, -1, -1 },
	{ 11, 2, 3, 8, 9, 5, 8, 5, 7, -1, -1, -1, -1 },
	{ 9, 2, 0, 9, 7, 2, 2, 7, 11, 9, 5, 7, -1 },
	{ 0, 3, 8, 2, 1, 11, 1, 7, 11, 1, 5, 7, -1 },
	{ 2, 1, 11, 11, 1, 7, 1, 5, 7, -1, -1, -1, -1 },
	{ 3, 9, 1, 3, 8, 9, 7, 11, 10, 7, 10, 5, -1 },
	{ 9, 1, 0, 10, 7, 11, 10, 5, 7, -1, -1, -1, -1 },
	{ 3, 8, 0, 7, 10, 5, 7, 11, 10, -1, -1, -1, -1 },
	{ 11, 5, 7, 11, 10, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 3, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 1, 9, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 6, 5, 9, 8, 3, 9, 3, 1, -1, -1, -1, -1 },
	{ 1, 2, 6, 1, 6, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 3, 2, 6, 5, 2, 5, 1, -1, -1, -1, -1 },
	{ 5, 9, 6, 9, 0, 6, 6, 0, 2, -1, -1, -1, -1 },
	{ 9, 6, 5, 3, 6, 9, 3, 9, 8, 3, 2, 6, -1 },
	{ 3, 11, 2, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 6, 5, 10, 2, 0, 8, 2, 8, 11, -1, -1, -1, -1 },
	{ 1, 9, 0, 6, 5, 10, 11, 2, 3, -1, -1, -1, -1 },
	{ 1, 10, 2, 5, 9, 6, 9, 11, 6, 9, 8, 11, -1 },
	{ 11, 6, 3, 6, 5, 3, 3, 5, 1, -1, -1, -1, -1 },
	{ 0, 5, 1, 0, 11, 5, 5, 11, 6, 0, 8, 11, -1 },
	{ 0, 5, 9, 0, 3, 5, 3, 6, 5, 3, 11, 6, -1 },
	{ 5, 9, 6, 6, 9, 11, 9, 8, 11, -1, -1, -1, -1 },
	{ 10, 6, 5, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 10, 6, 7, 3, 0, 7, 0, 4, -1, -1, -1, -1 },
	{ 5, 10, 6, 0, 1, 9, 8, 4, 7, -1, -1, -1, -1 },
	{ 4, 5, 9, 6, 7, 10, 7, 1, 10, 7, 3, 1, -1 },
	{ 7, 8, 4, 5, 1, 2, 5, 2, 6, -1, -1, -1, -1 },
	{ 4, 1, 0, 4, 5, 1, 6, 7, 3, 6, 3, 2, -1 },
	{ 9, 4, 5, 8, 0, 7, 0, 6, 7, 0, 2, 6, -1 },
	{ 4, 5, 9, 6, 3, 2, 6, 7, 3, -1, -1, -1, -1 },
	{ 7, 8, 4, 2, 3, 11, 10, 6, 5, -1, -1, -1, -1 },
	{ 11, 6, 7, 10, 2, 5, 2, 4, 5, 2, 0, 4, -1 },
	{ 11, 6, 7, 8, 0, 3, 1, 10, 2, 9, 4, 5, -1 },
	{ 6, 7, 11, 1, 10, 2, 9, 4, 5, -1, -1, -1, -1 },
	{ 6, 7, 11, 4, 5, 8, 5, 3, 8, 5, 1, 3, -1 },
	{ 6, 7, 11, 4, 1, 0, 4, 5, 1, -1, -1, -1, -1 },
	{ 4, 5, 9, 3, 8, 0, 11, 6, 7, -1, -1, -1, -1 },
	{ 9, 4, 5, 7, 11, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 6, 4, 10, 4, 9, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 3, 0, 9, 10, 6, 9, 6, 4, -1, -1, -1, -1 },
	{ 1, 10, 0, 10, 6, 0, 0, 6, 4, -1, -1, -1, -1 },
	{ 8, 6, 4, 8, 1, 6, 6, 1, 10, 8, 3, 1, -1 },
	{ 9, 1, 4, 1, 2, 4, 4, 2, 6, -1, -1, -1, -1 },
	{ 1, 0, 9, 3, 2, 8, 2, 4, 8, 2, 6, 4, -1 },
	{ 2, 4, 0, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 2, 8, 8, 2, 4, 2, 6, 4, -1, -1, -1, -1 },
	{ 2, 3, 11, 6, 4, 9, 6, 9, 10, -1, -1, -1, -1 },
	{ 0, 10, 2, 0, 9, 10, 4, 8, 11, 4, 11, 6, -1 },
	{ 10, 2, 1, 11, 6, 3, 6, 0, 3, 6, 4, 0, -1 },
	{ 10, 2, 1, 11, 4, 8, 11, 6, 4, -1, -1, -1, -1 },
	{ 1, 4, 9, 11, 4, 1, 11, 1, 3, 11, 6, 4, -1 },
	{ 0, 9, 1, 4, 11, 6, 4, 8, 11, -1, -1, -1, -1 },
	{ 11, 6, 3, 3, 6, 0, 6, 4, 0, -1, -1, -1, -1 },
	{ 8, 6, 4, 8, 11, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 6, 7, 10, 7, 8, 10, 10, 8, 9, -1, -1, -1, -1 },
	{ 9, 3, 0, 6, 3, 9, 6, 9, 10, 6, 7, 3, -1 },
	{ 6, 1, 10, 6, 7, 1, 7, 0, 1, 7, 8, 0, -1 },
	{ 6, 7, 10, 10, 7, 1, 7, 3, 1, -1, -1, -1, -1 },
	{ 7, 2, 6, 7, 9, 2, 2, 9, 1, 7, 8, 9, -1 },
	{ 1, 0, 9, 3, 6, 7, 3, 2, 6, -1, -1, -1, -1 },
	{ 8, 0, 7, 7, 0, 6, 0, 2, 6, -1, -1, -1, -1 },
	{ 2, 7, 3, 2, 6, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 7, 11, 6, 3, 8, 2, 8, 10, 2, 8, 9, 10, -1 },
	{ 11, 6, 7, 10, 0, 9, 10, 2, 0, -1, -1, -1, -1 },
	{ 2, 1, 10, 7, 11, 6, 8, 0, 3, -1, -1, -1, -1 },
	{ 1, 10, 2, 6, 7, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 7, 11, 6, 3, 9, 1, 3, 8, 9, -1, -1, -1, -1 },
	{ 9, 1, 0, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 3, 8, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 8, 3, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 0, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 7, 6, 11, 3, 1, 9, 3, 9, 8, -1, -1, -1, -1 },
	{ 1, 2, 10, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 10, 1, 7, 6, 11, 8, 3, 0, -1, -1, -1, -1 },
	{ 11, 7, 6, 10, 9, 0, 10, 0, 2, -1, -1, -1, -1 },
	{ 7, 6, 11, 3, 2, 8, 8, 2, 10, 8, 10, 9, -1 },
	{ 2, 3, 7, 2, 7, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 7, 0, 7, 6, 0, 0, 6, 2, -1, -1, -1, -1 },
	{ 1, 9, 0, 3, 7, 6, 3, 6, 2, -1, -1, -1, -1 },
	{ 7, 6, 2, 7, 2, 9, 2, 1, 9, 7, 9, 8, -1 },
	{ 6, 10, 7, 10, 1, 7, 7, 1, 3, -1, -1, -1, -1 },
	{ 6, 10, 1, 6, 1, 7, 7, 1, 0, 7, 0, 8, -1 },
	{ 9, 0, 3, 6, 9, 3, 6, 10, 9, 6, 3, 7, -1 },
	{ 6, 10, 7, 7, 10, 8, 10, 9, 8, -1, -1, -1, -1 },
	{ 8, 4, 6, 8, 6, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 3, 6, 3, 0, 6, 6, 0, 4, -1, -1, -1, -1 },
	{ 0, 1, 9, 4, 6, 11, 4, 11, 8, -1, -1, -1, -1 },
	{ 1, 9, 4, 11, 1, 4, 11, 3, 1, 11, 4, 6, -1 },
	{ 10, 1, 2, 11, 8, 4, 11, 4, 6, -1, -1, -1, -1 },
	{ 10, 1, 2, 11, 3, 6, 6, 3, 0, 6, 0, 4, -1 },
	{ 0, 2, 10, 0, 10, 9, 4, 11, 8, 4, 6, 11, -1 },
	{ 2, 11, 3, 6, 9, 4, 6, 10, 9, -1, -1, -1, -1 },
	{ 3, 8, 2, 8, 4, 2, 2, 4, 6, -1, -1, -1, -1 },
	{ 2, 0, 4, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 1, 9, 0, 3, 8, 2, 2, 8, 4, 2, 4, 6, -1 },
	{ 9, 4, 1, 1, 4, 2, 4, 6, 2, -1, -1, -1, -1 },
	{ 8, 4, 6, 8, 6, 1, 6, 10, 1, 8, 1, 3, -1 },
	{ 1, 0, 10, 10, 0, 6, 0, 4, 6, -1, -1, -1, -1 },
	{ 8, 0, 3, 9, 6, 10, 9, 4, 6, -1, -1, -1, -1 },
	{ 10, 4, 6, 10, 9, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 5, 4, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 9, 5, 3, 0, 8, 11, 7, 6, -1, -1, -1, -1 },
	{ 6, 11, 7, 4, 0, 1, 4, 1, 5, -1, -1, -1, -1 },
	{ 6, 11, 7, 4, 8, 5, 5, 8, 3, 5, 3, 1, -1 },
	{ 6, 11, 7, 1, 2, 10, 9, 5, 4, -1, -1, -1, -1 },
	{ 11, 7, 6, 8, 3, 0, 1, 2, 10, 9, 5, 4, -1 },
	{ 11, 7, 6, 10, 5, 2, 2, 5, 4, 2, 4, 0, -1 },
	{ 7, 4, 8, 2, 11, 3, 10, 5, 6, -1, -1, -1, -1 },
	{ 4, 9, 5, 6, 2, 3, 6, 3, 7, -1, -1, -1, -1 },
	{ 9, 5, 4, 8, 7, 0, 0, 7, 6, 0, 6, 2, -1 },
	{ 4, 0, 1, 4, 1, 5, 6, 3, 7, 6, 2, 3, -1 },
	{ 7, 4, 8, 5, 2, 1, 5, 6, 2, -1, -1, -1, -1 },
	{ 4, 9, 5, 6, 10, 7, 7, 10, 1, 7, 1, 3, -1 },
	{ 5, 6, 10, 0, 9, 1, 8, 7, 4, -1, -1, -1, -1 },
	{ 5, 6, 10, 7, 0, 3, 7, 4, 0, -1, -1, -1, -1 },
	{ 10, 5, 6, 4, 8, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 5, 6, 9, 6, 11, 9, 9, 11, 8, -1, -1, -1, -1 },
	{ 0, 9, 5, 0, 5, 3, 3, 5, 6, 3, 6, 11, -1 },
	{ 0, 1, 5, 0, 5, 11, 5, 6, 11, 0, 11, 8, -1 },
	{ 11, 3, 6, 6, 3, 5, 3, 1, 5, -1, -1, -1, -1 },
	{ 1, 2, 10, 5, 6, 9, 9, 6, 11, 9, 11, 8, -1 },
	{ 1, 0, 9, 6, 10, 5, 11, 3, 2, -1, -1, -1, -1 },
	{ 6, 10, 5, 2, 8, 0, 2, 11, 8, -1, -1, -1, -1 },
	{ 3, 2, 11, 10, 5, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 5, 6, 3, 9, 6, 3, 8, 9, 3, 6, 2, -1 },
	{ 5, 6, 9, 9, 6, 0, 6, 2, 0, -1, -1, -1, -1 },
	{ 0, 3, 8, 2, 5, 6, 2, 1, 5, -1, -1, -1, -1 },
	{ 1, 6, 2, 1, 5, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 5, 6, 9, 3, 8, 9, 1, 3, -1, -1, -1, -1 },
	{ 0, 9, 1, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 0, 3, 10, 5, 6, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 7, 5, 11, 5, 10, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 0, 8, 7, 5, 10, 7, 10, 11, -1, -1, -1, -1 },
	{ 9, 0, 1, 10, 11, 7, 10, 7, 5, -1, -1, -1, -1 },
	{ 3, 1, 9, 3, 9, 8, 7, 10, 11, 7, 5, 10, -1 },
	{ 2, 11, 1, 11, 7, 1, 1, 7, 5, -1, -1, -1, -1 },
	{ 0, 8, 3, 2, 11, 1, 1, 11, 7, 1, 7, 5, -1 },
	{ 9, 0, 2, 9, 2, 7, 2, 11, 7, 9, 7, 5, -1 },
	{ 11, 3, 2, 8, 5, 9, 8, 7, 5, -1, -1, -1, -1 },
	{ 10, 2, 5, 2, 3, 5, 5, 3, 7, -1, -1, -1, -1 },
	{ 5, 10, 2, 8, 5, 2, 8, 7, 5, 8, 2, 0, -1 },
	{ 9, 0, 1, 10, 2, 5, 5, 2, 3, 5, 3, 7, -1 },
	{ 1, 10, 2, 5, 8, 7, 5, 9, 8, -1, -1, -1, -1 },
	{ 1, 3, 7, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 7, 0, 0, 7, 1, 7, 5, 1, -1, -1, -1, -1 },
	{ 0, 3, 9, 9, 3, 5, 3, 7, 5, -1, -1, -1, -1 },
	{ 9, 7, 5, 9, 8, 7, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 5, 8, 5, 10, 8, 8, 10, 11, -1, -1, -1, -1 },
	{ 3, 0, 4, 3, 4, 10, 4, 5, 10, 3, 10, 11, -1 },
	{ 0, 1, 9, 4, 5, 8, 8, 5, 10, 8, 10, 11, -1 },
	{ 5, 9, 4, 1, 11, 3, 1, 10, 11, -1, -1, -1, -1 },
	{ 8, 4, 5, 2, 8, 5, 2, 11, 8, 2, 5, 1, -1 },
	{ 3, 2, 11, 1, 4, 5, 1, 0, 4, -1, -1, -1, -1 },
	{ 9, 4, 5, 8, 2, 11, 8, 0, 2, -1, -1, -1, -1 },
	{ 11, 3, 2, 9, 4, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 8, 4, 3, 4, 2, 2, 4, 5, 2, 5, 10, -1 },
	{ 10, 2, 5, 5, 2, 4, 2, 0, 4, -1, -1, -1, -1 },
	{ 0, 3, 8, 5, 9, 4, 10, 2, 1, -1, -1, -1, -1 },
	{ 2, 1, 10, 9, 4, 5, -1, -1, -1, -1, -1, -1, -1 },
	{ 4, 5, 8, 8, 5, 3, 5, 1, 3, -1, -1, -1, -1 },
	{ 5, 0, 4, 5, 1, 0, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 8, 0, 4, 5, 9, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 7, 4, 11, 4, 9, 11, 11, 9, 10, -1, -1, -1, -1 },
	{ 3, 0, 8, 7, 4, 11, 11, 4, 9, 11, 9, 10, -1 },
	{ 11, 7, 4, 1, 11, 4, 1, 10, 11, 1, 4, 0, -1 },
	{ 8, 7, 4, 11, 1, 10, 11, 3, 1, -1, -1, -1, -1 },
	{ 2, 11, 7, 2, 7, 1, 1, 7, 4, 1, 4, 9, -1 },
	{ 3, 2, 11, 4, 8, 7, 9, 1, 0, -1, -1, -1, -1 },
	{ 7, 4, 11, 11, 4, 2, 4, 0, 2, -1, -1, -1, -1 },
	{ 2, 11, 3, 7, 4, 8, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 3, 7, 2, 7, 9, 7, 4, 9, 2, 9, 10, -1 },
	{ 4, 8, 7, 0, 10, 2, 0, 9, 10, -1, -1, -1, -1 },
	{ 2, 1, 10, 0, 7, 4, 0, 3, 7, -1, -1, -1, -1 },
	{ 10, 2, 1, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 1, 4, 4, 1, 7, 1, 3, 7, -1, -1, -1, -1 },
	{ 1, 0, 9, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 4, 0, 3, 7, 4, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 9, 10, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1 },
	{ 0, 9, 3, 3, 9, 11, 9, 10, 11, -1, -1, -1, -1 },
	{ 1, 10, 0, 0, 10, 8, 10, 11, 8, -1, -1, -1, -1 },
	{ 10, 3, 1, 10, 11, 3, -1, -1, -1, -1, -1, -1, -1 },
	{ 2, 11, 1, 1, 11, 9, 11, 8, 9, -1, -1, -1, -1 },
	{ 11, 3, 2, 0, 9, 1, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 0, 2, 11, 8, 0, -1, -1, -1, -1, -1, -1, -1 },
	{ 11, 3, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 3, 8, 2, 2, 8, 10, 8, 9, 10, -1, -1, -1, -1 },
	{ 9, 2, 0, 9, 10, 2, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 0, 3, 1, 10, 2, -1, -1, -1, -1, -1, -1, -1 },
	{ 10, 2, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 1, 3, 8, 9, 1, -1, -1, -1, -1, -1, -1, -1 },
	{ 9, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ 8, 0, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
};

// the offsets of a cell's 26 neighbours, x fastest and z slowest; a bit of the cell masks
// is a neighbour in this order
// DATA: STBC 0x008e0b50
static unsigned short neighbourOffsets[26][3] = {
	{ 0xFFFF, 0xFFFF, 0xFFFF },
	{ 0, 0xFFFF, 0xFFFF },
	{ 1, 0xFFFF, 0xFFFF },
	{ 0xFFFF, 0, 0xFFFF },
	{ 0, 0, 0xFFFF },
	{ 1, 0, 0xFFFF },
	{ 0xFFFF, 1, 0xFFFF },
	{ 0, 1, 0xFFFF },
	{ 1, 1, 0xFFFF },
	{ 0xFFFF, 0xFFFF, 0 },
	{ 0, 0xFFFF, 0 },
	{ 1, 0xFFFF, 0 },
	{ 0xFFFF, 0, 0 },
	{ 1, 0, 0 },
	{ 0xFFFF, 1, 0 },
	{ 0, 1, 0 },
	{ 1, 1, 0 },
	{ 0xFFFF, 0xFFFF, 1 },
	{ 0, 0xFFFF, 1 },
	{ 1, 0xFFFF, 1 },
	{ 0xFFFF, 0, 1 },
	{ 0, 0, 1 },
	{ 1, 0, 1 },
	{ 0xFFFF, 1, 1 },
	{ 0, 1, 1 },
	{ 1, 1, 1 }
};

// each edge's two corners
// DATA: STBC 0x008e0bec
static int cubeEdgeCorners[12][2] = {
	{ 0, 1 },
	{ 1, 2 },
	{ 3, 2 },
	{ 3, 0 },
	{ 4, 5 },
	{ 5, 6 },
	{ 6, 7 },
	{ 7, 4 },
	{ 0, 4 },
	{ 1, 5 },
	{ 2, 6 },
	{ 3, 7 },
};

// each edge's midpoint in half cells from the cell's lower corner
// DATA: STBC 0x008e0c4c
static int cubeEdgeMidpoints[12][3] = {
	{ 0, 1, 0 },
	{ 1, 2, 0 },
	{ 2, 1, 0 },
	{ 1, 0, 0 },
	{ 0, 1, 2 },
	{ 1, 2, 2 },
	{ 2, 1, 2 },
	{ 1, 0, 2 },
	{ 0, 0, 1 },
	{ 0, 2, 1 },
	{ 2, 2, 1 },
	{ 2, 0, 1 },
};

static const float EDGE_EPSILON = 1.0e-5f;

/*
================
MetaVolumePolygonizer::GetEdgePoint
================
*/
inline NiPoint3 MetaVolumePolygonizer::GetEdgePoint( const Voxel &cube, int from, int to ) const {
	const NiPoint3 &a = cube.corners[from];
	const NiPoint3 &b = cube.corners[to];
	float weightA = cube.weights[from];
	float weightB = cube.weights[to];
	float below = unknown00 - weightA;
	if ( (float)fabs( below ) < EDGE_EPSILON ) {
		return a;
	}
	return (float)fabs( unknown00 - weightB ) < EDGE_EPSILON ? b
		: (float)fabs( weightA - weightB ) < EDGE_EPSILON ? a : a + below / ( weightB - weightA ) * ( b - a );
}

/*
================
MetaVolumePolygonizer::Polygonize

Builds the triangles of a cell the model's surface crosses: the surface
cell's tree cuts what the corner pattern gives, and the parts inside the
model are added to the mesh as fans; the cell's key then lists the
triangles added, in blocks of twelve. A cell with every corner set, with
the second kind of damage off, is cleared from the grid.
================
*/
// FUNCTION: STBC 0x004bf700 ?Polygonize@MetaVolumePolygonizer@@QAEXAAVVoxel@@PAUVoxelBSPTree@@@Z
// DRAFT: inline budget: the call set is retail's except two sites: retail expands the closing NiTMap<__int64, DamageIndexBlock *>::SetAt (ours is 103 units short at it) and calls DamageVoxelGrid::Clear's DamageBitArray::Clear out of line (its share here is 85, retail's under 74: a cheap wrapper around the grid's Clear, or 5 more sites after it); the frame is 4 bytes larger and the registers differ from the edge loop on
void MetaVolumePolygonizer::Polygonize( Voxel &cube, VoxelBSPTree *tree ) {
	unsigned int pattern = 0;
	for ( int i = 0; i < 8; i++ ) {
		if ( cube.weights[i] > unknown00 ) {
			pattern |= 1 << i;
		}
	}
	PopVoxelClipedTris( cube.key );
	if ( cubeEdgeFlags[pattern] == 0 ) {
		if ( pattern == 0xFF && !unknown08 ) {
			grid->Clear( cube.x, cube.y, cube.z );
		}
		return;
	}
	unsigned int inverse = ~pattern & 0xFF;
	for ( int edge = 0; edge < 12; edge++ ) {
		if ( cubeEdgeFlags[inverse] & ( 1 << edge ) ) {
			cube.unknown80[edge] = GetEdgePoint( cube, cubeEdgeCorners[edge][0], cubeEdgeCorners[edge][1] );
			cube.edgeVertices[edge] = -1;
		} else {
			cube.edgeVertices[edge] = -2;
		}
	}
	VoxelPolygonList clipped( 2 );
	if ( cubeTriangleTable[inverse][0] != -1 ) {
		int entry = 0;
		do {
			VoxelPolygon *polygon = new VoxelPolygon( 12 );
			polygon->vertices[0] = cube.unknown80[cubeTriangleTable[inverse][entry]];
			polygon->vertices[1] = cube.unknown80[cubeTriangleTable[inverse][entry + 1]];
			polygon->vertices[2] = cube.unknown80[cubeTriangleTable[inverse][entry + 2]];
			polygon->count = 3;
			tree->ClipInside( polygon, &clipped );
			entry += 3;
		} while ( cubeTriangleTable[inverse][entry] != -1 );
		if ( clipped.GetSize() > 0 ) {
			int slot = 0;
			DamageIndexBlock *list = new DamageIndexBlock;
			DamageIndexBlock *block = list;
			NiTListIterator pos = clipped.GetHeadPos();
			while ( pos ) {
				VoxelPolygon *polygon = clipped.GetNext( pos );
				unsigned short *indices = new unsigned short[polygon->count];
				int i;
				for ( i = 0; i < polygon->count; i++ ) {
					indices[i] = mesh->AddVertex( &polygon->vertices[i], NULL, NULL, NULL );
				}
				unsigned short first = indices[0];
				for ( i = 2; i < polygon->count; i++ ) {
					if ( slot == 12 ) {
						slot = 0;
						block->next = new DamageIndexBlock;
						block = block->next;
					}
					block->indices[slot++] = mesh->AddTriangle( first, indices[i - 1], indices[i] );
				}
				delete [] indices;
				delete polygon;
			}
			if ( slot < 12 ) {
				block->indices[slot] = 0xFFFF;
			}
			unknown88.SetAt( cube.key, list );
		}
	}
}

/*
================
MetaVolumePolygonizer::Polygonize

Builds the triangles of a cell from its corner pattern alone: the edges
the surface crosses get a vertex in the mesh (one the neighbouring cells
share, found by the edge's key), and the cell's key then lists the
triangles added. A cell with every corner set, with the second kind of
damage off, is cleared from the grid.
================
*/
// FUNCTION: STBC 0x004bfe80 ?Polygonize@MetaVolumePolygonizer@@QAEXAAVVoxel@@@Z
// DRAFT: inline budget: retail calls NiTMap<__int64, VertexInfo>::SetAt (0x4C3E10) out of line at both sites with 468 units left (nested, or not a candidate: an empty VertexInfo() constructor gives exactly that, no evidence for it), calls DamageBitArray::Clear out of line (share 116 here, needs under 74) and expands the closing SetAt of unknown68 (needs 192, has 49); the interpolated edge point through one out-of-line copy constructor is retail's
void MetaVolumePolygonizer::Polygonize( Voxel &cube ) {
	unsigned int pattern = 0;
	for ( int i = 0; i < 8; i++ ) {
		if ( cube.weights[i] > unknown00 ) {
			pattern |= 1 << i;
		}
	}
	unknown68.RemoveAt( cube.key );
	if ( cubeEdgeFlags[pattern] == 0 ) {
		if ( pattern == 0xFF && !unknown08 ) {
			grid->Clear( cube.x, cube.y, cube.z );
		}
		return;
	}
	unsigned int inverse = ~pattern & 0xFF;
	for ( int edge = 0; edge < 12; edge++ ) {
		if ( cubeEdgeFlags[inverse] & ( 1 << edge ) ) {
			int from = cubeEdgeCorners[edge][0];
			int to = cubeEdgeCorners[edge][1];
			__int64 key = (__int64)(unsigned short)( cube.x * 2 + cubeEdgeMidpoints[edge][0] );
			key = ( key << 16 ) | (unsigned short)( cube.y * 2 + cubeEdgeMidpoints[edge][1] );
			key = ( key << 16 ) | (unsigned short)( cube.z * 2 + cubeEdgeMidpoints[edge][2] );
			VertexInfo vertex;
			if ( !unknown78.GetAt( key, vertex ) ) {
				NiPoint3 point = GetEdgePoint( cube, from, to );
				vertex.index = mesh->AddVertex( &point, NULL, NULL, NULL );
				vertex.unknown02 = 1;
				unknown78.SetAt( key, vertex );
			} else if ( vertex.unknown02 != 1 ) {
				NiPoint3 point = GetEdgePoint( cube, from, to );
				vertex.unknown02 = 1;
				unknown78.SetAt( key, vertex );
				mesh->SetVertex( vertex.index, &point, NULL, NULL, NULL );
			}
			cube.edgeVertices[edge] = vertex.index;
		}
	}
	DamageVertexIndices triangles;
	memset( &triangles, 0xFF, sizeof( triangles ) );
	const int *edges = cubeTriangleTable[inverse];
	int added = 0;
	while ( *edges != -1 ) {
		triangles.indices[added++] = mesh->AddTriangle( (unsigned short)cube.edgeVertices[edges[0]], (unsigned short)cube.edgeVertices[edges[1]], (unsigned short)cube.edgeVertices[edges[2]] );
		edges += 3;
	}
	unknown68.SetAt( cube.key, triangles );
}


/*
================
MetaVolumePolygonizer::GetCornerBits
================
*/
// FUNCTION: STBC 0x004c04c0
int MetaVolumePolygonizer::GetCornerBits( unsigned short x, unsigned short y, unsigned short z ) {
	int corners = 0;
	if ( IsPointSet( x, y, z ) ) {
		corners = 1;
	}
	if ( IsPointSet( x + 1, y, z ) ) {
		corners |= 2;
	}
	if ( IsPointSet( x, y + 1, z ) ) {
		corners |= 4;
	}
	if ( IsPointSet( x + 1, y + 1, z ) ) {
		corners |= 8;
	}
	if ( IsPointSet( x, y, z + 1 ) ) {
		corners |= 0x10;
	}
	if ( IsPointSet( x + 1, y, z + 1 ) ) {
		corners |= 0x20;
	}
	if ( IsPointSet( x, y + 1, z + 1 ) ) {
		corners |= 0x40;
	}
	if ( IsPointSet( x + 1, y + 1, z + 1 ) ) {
		corners |= 0x80;
	}
	return corners;
}

/*
================
MetaVolumePolygonizer::Unknown4C0650
================
*/
// FUNCTION: STBC 0x004c0650
unsigned int MetaVolumePolygonizer::Unknown4C0650( unsigned short x, unsigned short y, unsigned short z ) {
	unsigned int edges = voxelData->GetVoxelData()->Unknown4A9190( x, y, z );
	if ( edges != 0 && grid->IsSet( x, y, z ) ) {
		return cubeEdges[GetCornerBits( x, y, z )] & edges;
	}
	return 0;
}

/*
================
MetaVolumePolygonizer::FindSetCell
================
*/
// FUNCTION: STBC 0x004c06f0
void MetaVolumePolygonizer::FindSetCell( unsigned short *x, unsigned short *y, unsigned short *z ) {
	for ( unsigned short cellZ = searchLower[2]; cellZ < searchUpper[2]; cellZ++ ) {
		for ( unsigned short cellY = searchLower[1]; cellY < searchUpper[1]; cellY++ ) {
			unsigned int index = GetCellIndex( searchLower[0], cellY, cellZ );
			for ( unsigned short cellX = searchLower[0]; cellX < searchUpper[0]; index++, cellX++ ) {
				if ( unknownC8.IsSet( index ) ) {
					*x = cellX;
					*y = cellY;
					*z = cellZ;
					return;
				}
			}
		}
	}
}

/*
================
MetaVolumePolygonizer::Unknown4C0830

Marks in unknownE4 the cells of unknownC8 connected to the cell, through
the 26 neighbours, widening the box of unknownF0 and unknownF6 around
them: a run along x at a time, queueing the cells beside it. Then the
cells of the box that the voxel data gives neighbours are marked in
unknownE8, and unknownE8 is merged into unknownE4.
================
*/
// FUNCTION: STBC 0x004c0830
// DRAFT: retail keeps the row index in a frame slot and reads masks[cx] per row where this build strength-reduces a pointer; structure matches (the run, the eight neighbour rows written out as retail has them, the queue pop, the box scan marking the voxel data's neighbours in unknownE8, the merge; 191 branches against retail's 193); retail keeps NewItem out of line (0x4C47C0) at all 24 queued cells while this build expands it inside AddTail, and keeps the row flags and masks in frame slots; as a helper per row the inline budget ran out (12.5)
void MetaVolumePolygonizer::Unknown4C0830( unsigned short x, unsigned short y, unsigned short z ) {
	if ( unknown98450C < size[0] ) {
		delete [] unknown984508;
		unknown984508 = new unsigned int[size[0]];
		unknown98450C = size[0];
	}
	// Unknown4C2550 leaves each run cell's neighbour bits here
	unsigned int *masks = unknown984508;
	for ( ;; ) {
		unsigned int start;
		unsigned int end;
		Unknown4C2550( x, y, z, &start, &end );
		if ( z > 0 ) {
			if ( y > 0 ) {
				{
					bool previousOpen = (int)start > 0 && !IsCellMarked( start - 1, y - 1, z - 1 );
					bool open = !IsCellMarked( start, y - 1, z - 1 );
					for ( int cx = start; cx < (int)end; cx++ ) {
						unsigned int mask = masks[cx];
						if ( previousOpen && ( mask & 0x1 ) ) {
							DamageCell cell;
							cell.x = cx - 1;
							cell.y = y - 1;
							cell.z = z - 1;
							unknownAC.AddTail( cell );
						}
						if ( open ) {
							if ( mask & 0x2 ) {
								DamageCell cell;
								cell.x = cx;
								cell.y = y - 1;
								cell.z = z - 1;
								unknownAC.AddTail( cell );
								previousOpen = false;
							} else {
								previousOpen = true;
							}
						} else {
							previousOpen = false;
						}
						if ( cx + 1 < size[0] ) {
							if ( !IsCellMarked( cx + 1, y - 1, z - 1 ) ) {
								if ( !( mask & 0x4 ) ) {
									open = true;
									continue;
								}
								DamageCell cell;
								cell.x = cx + 1;
								cell.y = y - 1;
								cell.z = z - 1;
								unknownAC.AddTail( cell );
							}
							open = false;
						}
					}
				}
			}
			{
				bool previousOpen = (int)start > 0 && !IsCellMarked( start - 1, y, z - 1 );
				bool open = !IsCellMarked( start, y, z - 1 );
				for ( int cx = start; cx < (int)end; cx++ ) {
					unsigned int mask = masks[cx];
					if ( previousOpen && ( mask & 0x8 ) ) {
						DamageCell cell;
						cell.x = cx - 1;
						cell.y = y;
						cell.z = z - 1;
						unknownAC.AddTail( cell );
					}
					if ( open ) {
						if ( mask & 0x10 ) {
							DamageCell cell;
							cell.x = cx;
							cell.y = y;
							cell.z = z - 1;
							unknownAC.AddTail( cell );
							previousOpen = false;
						} else {
							previousOpen = true;
						}
					} else {
						previousOpen = false;
					}
					if ( cx + 1 < size[0] ) {
						if ( !IsCellMarked( cx + 1, y, z - 1 ) ) {
							if ( !( mask & 0x20 ) ) {
								open = true;
								continue;
							}
							DamageCell cell;
							cell.x = cx + 1;
							cell.y = y;
							cell.z = z - 1;
							unknownAC.AddTail( cell );
						}
						open = false;
					}
				}
			}
			if ( y + 1 < size[1] ) {
				{
					bool previousOpen = (int)start > 0 && !IsCellMarked( start - 1, y + 1, z - 1 );
					bool open = !IsCellMarked( start, y + 1, z - 1 );
					for ( int cx = start; cx < (int)end; cx++ ) {
						unsigned int mask = masks[cx];
						if ( previousOpen && ( mask & 0x40 ) ) {
							DamageCell cell;
							cell.x = cx - 1;
							cell.y = y + 1;
							cell.z = z - 1;
							unknownAC.AddTail( cell );
						}
						if ( open ) {
							if ( mask & 0x80 ) {
								DamageCell cell;
								cell.x = cx;
								cell.y = y + 1;
								cell.z = z - 1;
								unknownAC.AddTail( cell );
								previousOpen = false;
							} else {
								previousOpen = true;
							}
						} else {
							previousOpen = false;
						}
						if ( cx + 1 < size[0] ) {
							if ( !IsCellMarked( cx + 1, y + 1, z - 1 ) ) {
								if ( !( mask & 0x100 ) ) {
									open = true;
									continue;
								}
								DamageCell cell;
								cell.x = cx + 1;
								cell.y = y + 1;
								cell.z = z - 1;
								unknownAC.AddTail( cell );
							}
							open = false;
						}
					}
				}
			}
		}
		if ( y > 0 ) {
			{
				bool previousOpen = (int)start > 0 && !IsCellMarked( start - 1, y - 1, z );
				bool open = !IsCellMarked( start, y - 1, z );
				for ( int cx = start; cx < (int)end; cx++ ) {
					unsigned int mask = masks[cx];
					if ( previousOpen && ( mask & 0x200 ) ) {
						DamageCell cell;
						cell.x = cx - 1;
						cell.y = y - 1;
						cell.z = z;
						unknownAC.AddTail( cell );
					}
					if ( open ) {
						if ( mask & 0x400 ) {
							DamageCell cell;
							cell.x = cx;
							cell.y = y - 1;
							cell.z = z;
							unknownAC.AddTail( cell );
							previousOpen = false;
						} else {
							previousOpen = true;
						}
					} else {
						previousOpen = false;
					}
					if ( cx + 1 < size[0] ) {
						if ( !IsCellMarked( cx + 1, y - 1, z ) ) {
							if ( !( mask & 0x800 ) ) {
								open = true;
								continue;
							}
							DamageCell cell;
							cell.x = cx + 1;
							cell.y = y - 1;
							cell.z = z;
							unknownAC.AddTail( cell );
						}
						open = false;
					}
				}
			}
		}
		if ( y + 1 < size[1] ) {
			{
				bool previousOpen = (int)start > 0 && !IsCellMarked( start - 1, y + 1, z );
				bool open = !IsCellMarked( start, y + 1, z );
				for ( int cx = start; cx < (int)end; cx++ ) {
					unsigned int mask = masks[cx];
					if ( previousOpen && ( mask & 0x4000 ) ) {
						DamageCell cell;
						cell.x = cx - 1;
						cell.y = y + 1;
						cell.z = z;
						unknownAC.AddTail( cell );
					}
					if ( open ) {
						if ( mask & 0x8000 ) {
							DamageCell cell;
							cell.x = cx;
							cell.y = y + 1;
							cell.z = z;
							unknownAC.AddTail( cell );
							previousOpen = false;
						} else {
							previousOpen = true;
						}
					} else {
						previousOpen = false;
					}
					if ( cx + 1 < size[0] ) {
						if ( !IsCellMarked( cx + 1, y + 1, z ) ) {
							if ( !( mask & 0x10000 ) ) {
								open = true;
								continue;
							}
							DamageCell cell;
							cell.x = cx + 1;
							cell.y = y + 1;
							cell.z = z;
							unknownAC.AddTail( cell );
						}
						open = false;
					}
				}
			}
		}
		if ( z + 1 < size[2] ) {
			if ( y > 0 ) {
				{
					bool previousOpen = (int)start > 0 && !IsCellMarked( start - 1, y - 1, z + 1 );
					bool open = !IsCellMarked( start, y - 1, z + 1 );
					for ( int cx = start; cx < (int)end; cx++ ) {
						unsigned int mask = masks[cx];
						if ( previousOpen && ( mask & 0x20000 ) ) {
							DamageCell cell;
							cell.x = cx - 1;
							cell.y = y - 1;
							cell.z = z + 1;
							unknownAC.AddTail( cell );
						}
						if ( open ) {
							if ( mask & 0x40000 ) {
								DamageCell cell;
								cell.x = cx;
								cell.y = y - 1;
								cell.z = z + 1;
								unknownAC.AddTail( cell );
								previousOpen = false;
							} else {
								previousOpen = true;
							}
						} else {
							previousOpen = false;
						}
						if ( cx + 1 < size[0] ) {
							if ( !IsCellMarked( cx + 1, y - 1, z + 1 ) ) {
								if ( !( mask & 0x80000 ) ) {
									open = true;
									continue;
								}
								DamageCell cell;
								cell.x = cx + 1;
								cell.y = y - 1;
								cell.z = z + 1;
								unknownAC.AddTail( cell );
							}
							open = false;
						}
					}
				}
			}
			{
				bool previousOpen = (int)start > 0 && !IsCellMarked( start - 1, y, z + 1 );
				bool open = !IsCellMarked( start, y, z + 1 );
				for ( int cx = start; cx < (int)end; cx++ ) {
					unsigned int mask = masks[cx];
					if ( previousOpen && ( mask & 0x100000 ) ) {
						DamageCell cell;
						cell.x = cx - 1;
						cell.y = y;
						cell.z = z + 1;
						unknownAC.AddTail( cell );
					}
					if ( open ) {
						if ( mask & 0x200000 ) {
							DamageCell cell;
							cell.x = cx;
							cell.y = y;
							cell.z = z + 1;
							unknownAC.AddTail( cell );
							previousOpen = false;
						} else {
							previousOpen = true;
						}
					} else {
						previousOpen = false;
					}
					if ( cx + 1 < size[0] ) {
						if ( !IsCellMarked( cx + 1, y, z + 1 ) ) {
							if ( !( mask & 0x400000 ) ) {
								open = true;
								continue;
							}
							DamageCell cell;
							cell.x = cx + 1;
							cell.y = y;
							cell.z = z + 1;
							unknownAC.AddTail( cell );
						}
						open = false;
					}
				}
			}
			if ( y + 1 < size[1] ) {
				{
					bool previousOpen = (int)start > 0 && !IsCellMarked( start - 1, y + 1, z + 1 );
					bool open = !IsCellMarked( start, y + 1, z + 1 );
					for ( int cx = start; cx < (int)end; cx++ ) {
						unsigned int mask = masks[cx];
						if ( previousOpen && ( mask & 0x800000 ) ) {
							DamageCell cell;
							cell.x = cx - 1;
							cell.y = y + 1;
							cell.z = z + 1;
							unknownAC.AddTail( cell );
						}
						if ( open ) {
							if ( mask & 0x1000000 ) {
								DamageCell cell;
								cell.x = cx;
								cell.y = y + 1;
								cell.z = z + 1;
								unknownAC.AddTail( cell );
								previousOpen = false;
							} else {
								previousOpen = true;
							}
						} else {
							previousOpen = false;
						}
						if ( cx + 1 < size[0] ) {
							if ( !IsCellMarked( cx + 1, y + 1, z + 1 ) ) {
								if ( !( mask & 0x2000000 ) ) {
									open = true;
									continue;
								}
								DamageCell cell;
								cell.x = cx + 1;
								cell.y = y + 1;
								cell.z = z + 1;
								unknownAC.AddTail( cell );
							}
							open = false;
						}
					}
				}
			}
		}
		for ( ;; ) {
			if ( unknownAC.GetSize() == 0 ) {
				unsigned short lower[3];
				unsigned short upper[3];
				memcpy( lower, unknownF0, sizeof( lower ) );
				int i;
				for ( i = 0; i < 3; i++ ) {
					upper[i] = unknownF6[i] + 1;
				}
				for ( unsigned short cz = lower[2]; cz < upper[2]; cz++ ) {
					for ( unsigned short cy = lower[1]; cy < upper[1]; cy++ ) {
						for ( unsigned short cx = lower[0]; cx < upper[0]; cx++ ) {
							unsigned int index = ( size[1] * cz + cy ) * size[0] + cx;
							if ( unknownE4[index >> 3] & ( 1 << ( index & 7 ) ) ) {
								unsigned int neighbours = voxelData->GetVoxelData()->Unknown4A9190( cx, cy, cz );
								if ( neighbours != 0 ) {
									for ( i = 0; i < 26; i++ ) {
										if ( neighbours & ( 1 << i ) ) {
											unsigned short nx = cx + neighbourOffsets[i][0];
											unsigned short ny = cy + neighbourOffsets[i][1];
											unsigned short nz = cz + neighbourOffsets[i][2];
											unsigned int cell = ( size[1] * nz + ny ) * size[0] + nx;
											unknownE8[cell >> 3] |= 1 << ( cell & 7 );
											if ( nx < unknownF0[0] ) {
												unknownF0[0] = nx;
											} else if ( unknownF6[0] < nx ) {
												unknownF6[0] = nx;
											}
											if ( ny < unknownF0[1] ) {
												unknownF0[1] = ny;
											} else if ( unknownF6[1] < ny ) {
												unknownF6[1] = ny;
											}
											if ( nz < unknownF0[2] ) {
												unknownF0[2] = nz;
											} else if ( unknownF6[2] < nz ) {
												unknownF6[2] = nz;
											}
										}
									}
								}
							}
						}
					}
				}
				int bytes = (int)( size[2] * size[1] * size[0] ) / 8 + 1;
				int words = bytes / 4;
				unsigned int *marked = (unsigned int *)unknownE4;
				unsigned int *added = (unsigned int *)unknownE8;
				for ( i = 0; i < words; i++ ) {
					*marked++ |= *added++;
				}
				int rest = bytes % 4;
				for ( i = 0; i < rest; i++ ) {
					unknownE4[words * 4 + i] |= unknownE8[words * 4 + i];
				}
				return;
			}
			DamageCell cell = unknownAC.RemoveHead();
			x = cell.x;
			y = cell.y;
			z = cell.z;
			if ( !IsCellMarked( x, y, z ) ) {
				break;
			}
		}
	}
}

/*
================
MetaVolumePolygonizer::Unknown4C2550

Grows a run of the current grid's cells along x from the cell, marking each
in unknownE4 and clearing it in unknownC8 while the edge masks
(Unknown4C0650, kept per x in unknown984508) say the surface goes on to
the next cell; the run is from *start up to *end. A cell not set in the
grid gives an empty run (invented name).
================
*/
// FUNCTION: STBC 0x004c2550
// DRAFT: inline budget: the right loop's Unknown4C4D70 is expanded here (77 left, cost 75) where retail calls it, and then IsSet too: retail had 17 to 77 more units spent before it (c2trace)
void MetaVolumePolygonizer::Unknown4C2550( unsigned short x, unsigned short y, unsigned short z, unsigned int *start, unsigned int *end ) {
	if ( unknownC4 != 0 ) {
		Unknown4C4D70( x, y, z );
	}
	if ( !grid->IsSet( x, y, z ) ) {
		*start = x;
		*end = x;
		return;
	}
	Unknown4C4C70( x, y, z );
	unknown984508[x] = Unknown4C0650( x, y, z );

	unsigned short right = x;
	while ( unknown984508[right] & 0x2000 ) {
		right++;
		if ( !grid->IsSet( right, y, z ) ) {
			right--;
			break;
		}
		Unknown4C4C70( right, y, z );
		if ( unknownC4 != 0 ) {
			Unknown4C4D70( right, y, z );
		}
		unknown984508[right] = Unknown4C0650( right, y, z );
	}
	*end = right + 1;

	unsigned short left = x;
	while ( unknown984508[left] & 0x1000 ) {
		left--;
		if ( !grid->IsSet( left, y, z ) ) {
			left++;
			break;
		}
		Unknown4C4C70( left, y, z );
		if ( unknownC4 != 0 ) {
			Unknown4C4D70( left, y, z );
		}
		unknown984508[left] = Unknown4C0650( left, y, z );
	}
	*start = left;
}

/*
================
MetaVolumePolygonizer::CreateUnknown138
================
*/
// FUNCTION: STBC 0x004c2940
// PARKED: PCH boundary: exact only with the unit's leading include block inside the precompiled block (/YX); the game units build without a precompiled header (accepted loss, user's ruling 2026-11-20; docs/c2-backend.md, "A/B over the tree")
NiNode *MetaVolumePolygonizer::CreateUnknown138( void ) {
	unknown138 = new NiNode;
	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( true );
	unknown138->AttachProperty( zBuffer );
	return unknown138;
}

/*
================
MetaVolumePolygonizer::GetCell
================
*/
// FUNCTION: STBC 0x004c2a50
void MetaVolumePolygonizer::GetCell( const NiPoint3 *point, unsigned short *x, unsigned short *y, unsigned short *z ) {
	int cellX = (int)( ( point->x - boundMin.x ) / cellSize );
	int cellY = (int)( ( point->y - boundMin.y ) / cellSize );
	int cellZ = (int)( ( point->z - boundMin.z ) / cellSize );
	if ( cellX < 0 ) {
		cellX = 0;
	} else if ( cellX >= size[0] ) {
		cellX = size[0] - 1;
	}
	if ( cellY < 0 ) {
		cellY = 0;
	} else if ( cellY >= size[1] ) {
		cellY = size[1] - 1;
	}
	if ( cellZ < 0 ) {
		cellZ = 0;
	} else if ( cellZ >= size[2] ) {
		cellZ = size[2] - 1;
	}
	*x = cellX;
	*y = cellY;
	*z = cellZ;
}

/*
================
MetaVolumePolygonizer::Unknown4C2AF0

Whether a point in the model's frame lies in a cell of the current grid
that is set. With the flag, the cell must also be solid in the voxel data,
or a surface cell whose tree holds the point. Without voxel data the
result is the flag's opposite.
================
*/
// FUNCTION: STBC 0x004c2af0
bool MetaVolumePolygonizer::Unknown4C2AF0( const NiPoint3 *point, bool exact ) {
	if ( !voxelData ) {
		return exact ? false : true;
	}
	int x = (int)( ( point->x - boundMin.x ) / cellSize );
	int y = (int)( ( point->y - boundMin.y ) / cellSize );
	int z = (int)( ( point->z - boundMin.z ) / cellSize );
	if ( x < 0 || x >= size[0] || y < 0 || y >= size[1] || z < 0 || z >= size[2] || !grid->IsSet( x, y, z ) ) {
		return false;
	}
	if ( exact ) {
		if ( voxelData->GetVoxelData()->IsSolid( x, y, z ) ) {
			return true;
		}
		const VoxelBSPNode *cell;
		if ( !voxelData->GetVoxelData()->GetSurfaceCell( x, y, z, cell ) ) {
			return false;
		}
		VoxelBSPTree *tree = voxelData->GetVoxelData()->GetTree( cell );
		return tree->IsInside( *point );
	}
	return true;
}

/*
================
MetaVolumePolygonizer::Unknown4C2C40

Sets the grid point's bit in the shared point buffer; the points on the
grid's lower faces and beyond its size are left out (invented name).
================
*/
// FUNCTION: STBC 0x004c2c40
void MetaVolumePolygonizer::Unknown4C2C40( unsigned short x, unsigned short y, unsigned short z ) {
	if ( x > 0 && y > 0 && z > 0 && x < size[0] && y < size[1] && z < size[2] ) {
		unsigned int index = ( ( size[1] - 1 ) * ( z - 1 ) + y - 1 ) * ( size[0] - 1 ) + x - 1;
		unsigned int byteIndex = index >> 3;
		unknown984520[byteIndex] |= 1 << ( index & 7 );
	}
}

/*
================
MetaVolumePolygonizer::Unknown4C2CC0

Keeps in unknown108 only the grid points of the cells the other grid has
set within the box both grids share (no caller in retail; invented name).
================
*/
// FUNCTION: STBC 0x004c2cc0
// DRAFT: the box loop walks four element pointers in retail (the three grid-plane minimum and maximum pairs through one base); the cell loop and the word AND agree
void MetaVolumePolygonizer::Unknown4C2CC0( DamageVoxelGrid *other ) {
	DamageVoxelGrid *current = grid;
	unsigned int byteCount = ( size[2] - 1 ) * ( size[1] - 1 ) * ( size[0] - 1 ) / 8 + 1;
	if ( byteCount > unknown984524 ) {
		delete [] unknown984520;
		unknown984520 = new unsigned char[byteCount];
		unknown984524 = byteCount;
	}
	memset( unknown984520, 0, byteCount );

	unsigned short lower[3];
	unsigned short upper[3];
	int i;
	for ( i = 0; i < 3; i++ ) {
		lower[i] = current->lower[i] < other->lower[i] ? other->lower[i] : current->lower[i];
		upper[i] = current->upper[i] < other->upper[i] ? current->upper[i] : other->upper[i];
	}
	for ( unsigned short z = lower[2]; z < upper[2]; z++ ) {
		for ( unsigned short y = lower[1]; y < upper[1]; y++ ) {
			for ( unsigned short x = lower[0]; x < upper[0]; x++ ) {
				if ( other->IsSet( x, y, z ) ) {
					Unknown4C2C40( x, y, z );
					Unknown4C2C40( x + 1, y, z );
					Unknown4C2C40( x, y + 1, z );
					Unknown4C2C40( x + 1, y + 1, z );
					Unknown4C2C40( x, y, z + 1 );
					Unknown4C2C40( x + 1, y, z + 1 );
					Unknown4C2C40( x, y + 1, z + 1 );
					Unknown4C2C40( x + 1, y + 1, z + 1 );
				}
			}
		}
	}

	int words = (int)byteCount / 4;
	unsigned int *points = (unsigned int *)unknown108;
	unsigned int *buffer = (unsigned int *)unknown984520;
	for ( i = 0; i < words; i++ ) {
		*points++ &= *buffer++;
	}
	int rest = (int)byteCount % 4;
	for ( i = 0; i < rest; i++ ) {
		unknown108[words * 4 + i] &= unknown984520[words * 4 + i];
	}
}

/*
================
MetaVolumePolygonizer::Unknown4C2F80

Keeps only the current grid's cells that the other grid has set and
narrows the box to the other's. With the first kind of damage on, the
triangles built in the cells the other grid doesn't have are dropped from
the two maps and collapsed to a point in the shape's triangle list.
================
*/
// FUNCTION: STBC 0x004c2f80
// DRAFT: structure matches (the AND, the box narrowing, the two map walks collecting the dropped triangles and keys, the removals, the collapse); retail inlines DamageVoxelGrid::IsSet's index (this build calls GetIndex out of line), reloads grid at each box step and keeps the list nodes' links in other registers
void MetaVolumePolygonizer::Unknown4C2F80( DamageVoxelGrid *other ) {
	int words = (int)grid->cells.GetByteCount() / 4;
	unsigned int *bits = (unsigned int *)grid->cells.bits;
	unsigned int *otherBits = (unsigned int *)other->cells.bits;
	int i;
	for ( i = 0; i < words; i++ ) {
		*bits++ &= *otherBits++;
	}
	int rest = (int)grid->cells.GetByteCount() % 4;
	for ( i = 0; i < rest; i++ ) {
		grid->cells.bits[words * 4 + i] &= other->cells.bits[words * 4 + i];
	}
	for ( i = 0; i < 3; i++ ) {
		if ( grid->lower[i] < other->lower[i] ) {
			grid->lower[i] = other->lower[i];
		}
		if ( other->upper[i] < grid->upper[i] ) {
			grid->upper[i] = other->upper[i];
		}
	}
	if ( !unknown09 ) {
		return;
	}
	NiTMapIterator pos = unknown68.GetFirstPos();
	NiTList<__int64> keys( 2 );
	NiTList<int> dropped( 2 );
	while ( pos ) {
		__int64 key;
		DamageVertexIndices triangles;
		unknown68.GetNext( pos, key, triangles );
		__int64 xy = key >> 16;
		if ( !other->IsSet( (unsigned short)( xy >> 16 ), (unsigned short)xy, (unsigned short)key ) ) {
			for ( i = 0; triangles.indices[i] != 0xFFFF; i++ ) {
				dropped.AddTail( triangles.indices[i] );
			}
			keys.AddTail( key );
		}
	}
	NiTListIterator keyPos = keys.GetHeadPos();
	while ( keyPos ) {
		unknown68.RemoveAt( keys.GetNext( keyPos ) );
	}
	keys.RemoveAll();
	pos = unknown88.GetFirstPos();
	while ( pos ) {
		__int64 key;
		DamageIndexBlock *block;
		unknown88.GetNext( pos, key, block );
		__int64 xy = key >> 16;
		if ( !other->IsSet( (unsigned short)( xy >> 16 ), (unsigned short)xy, (unsigned short)key ) ) {
			for ( DamageIndexBlock *b = block; b != NULL; b = b->next ) {
				for ( i = 0; i < 12 && b->indices[i] != 0xFFFF; i++ ) {
					dropped.AddTail( b->indices[i] );
				}
			}
			delete block;
			keys.AddTail( key );
		}
	}
	keyPos = keys.GetHeadPos();
	while ( keyPos ) {
		unknown88.RemoveAt( keys.GetNext( keyPos ) );
	}
	keys.RemoveAll();
	if ( dropped.GetSize() != 0 ) {
		unsigned short *triList = unknown9C->GetTriList();
		NiTListIterator triPos = dropped.GetHeadPos();
		while ( triPos ) {
			int triangle = dropped.GetNext( triPos );
			triList[triangle * 3 + 1] = triList[triangle * 3];
			triList[triangle * 3 + 2] = triList[triangle * 3];
		}
	}
}

/*
================
MetaVolumePolygonizer::Unknown4C3680
================
*/
// FUNCTION: STBC 0x004c3680
// PARKED: register allocation: the call set and the frame (0x40) are retail's through the invented IsInGrid (one charged site after Unitize: Length's share 57 -> 45, operator+='s budget 65 -> under 54); retail keeps this in EBX and x, y in ESI, EDI where this build picks ESI for this (the allocator takes this first)
bool MetaVolumePolygonizer::Unknown4C3680( const NiPoint3 *origin, const NiPoint3 *direction, const NiPoint3 *, int, const float *t, int, int ) {
	if ( !unknown09 ) {
		return true;
	}
	NiNode *node = grid->debugNode;
	float scale = 1.0f / node->GetWorldScale();
	NiPoint3 cell = ( scale * ( ( *t * *direction + *origin - node->GetWorldTranslate() ) * node->GetWorldRotate() ) - boundMin ) / cellSize;
	NiPoint3 step = *direction * node->GetWorldRotate();
	step.Unitize();
	step *= 0.25f;
	while ( true ) {
		int x = (int)cell.x;
		int y = (int)cell.y;
		int z = (int)cell.z;
		if ( !IsInGrid( x, y, z ) ) {
			break;
		}
		if ( grid->IsSet( x, y, z ) ) {
			return true;
		}
		cell += step;
	}
	return false;
}

/*
================
MetaVolumePolygonizer::Unknown4C38C0
================
*/
// FUNCTION: STBC 0x004c38c0
// PARKED: register allocation: the call set and the frame (0x40) are retail's through the invented IsInGrid (one charged site after Unitize: Length's share 57 -> 45, operator+='s budget 65 -> under 54); retail keeps this in EBX and x, y in ESI, EDI where this build picks ESI for this (the allocator takes this first)
bool MetaVolumePolygonizer::Unknown4C38C0( const NiPoint3 *origin, const NiPoint3 *direction, const NiPoint3 *, int, const float *t, int, int ) {
	if ( !unknown09 ) {
		return true;
	}
	NiNode *node = grid->debugNode;
	float scale = 1.0f / node->GetWorldScale();
	NiPoint3 cell = ( scale * ( ( *t * *direction + *origin - node->GetWorldTranslate() ) * node->GetWorldRotate() ) - boundMin ) / cellSize;
	NiPoint3 step = *direction * node->GetWorldRotate();
	step.Unitize();
	step *= 0.25f;
	while ( true ) {
		int x = (int)cell.x;
		int y = (int)cell.y;
		int z = (int)cell.z;
		if ( !IsInGrid( x, y, z ) ) {
			break;
		}
		if ( grid->IsSet( x, y, z ) ) {
			return true;
		}
		cell += step;
	}
	return false;
}
