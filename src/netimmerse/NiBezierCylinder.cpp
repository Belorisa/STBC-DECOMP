// NiBezierCylinder.cpp -- cylindrical Bezier patches

#include "NiRTLib.h"
#include <math.h>

#include "NiStream.h"
#include "NiBezierCylinder.h"
#include "NiBezierRectangle.h"
#include "NiBezierRectangle2.h"
#include "NiNode.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00850cc0 _$E17
// FUNCTION: STBC 0x00850ce0 _$E15
// FUNCTION: STBC 0x00850cf0 _$E23
// FUNCTION: STBC 0x00850d10 _$E21
// FUNCTION: STBC 0x00851070 _$E243

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0084ca20 _$E234
// FUNCTION: STBC 0x0084ca40 _$E231
// FUNCTION: STBC 0x00850e80 _$E37
// FUNCTION: STBC 0x00850e90 _$E41
// FUNCTION: STBC 0x00850ea0 _$E45
// FUNCTION: STBC 0x00850eb0 _$E49
// FUNCTION: STBC 0x00851060 _$E53
// FUNCTION: STBC 0x00851090 _$E177
// FUNCTION: STBC 0x00851130 _$E185
// FUNCTION: STBC 0x00851140 _$E189
// FUNCTION: STBC 0x00851150 _$E193
// FUNCTION: STBC 0x00851160 _$E197
// FUNCTION: STBC 0x00851170 _$E205
// FUNCTION: STBC 0x008511b0 _$E209
// FUNCTION: STBC 0x008511c0 _$E224
// FUNCTION: STBC 0x008511d0 _$E228

// DATA: STBC 0x9B32C0 ?rtti@NiBezierCylinder@@2VNiRTTI@@B
const NiRTTI NiBezierCylinder::rtti( "NiBezierCylinder", &NiBezierPatch::rtti );

/*
================
NiBezierCylinder::Register
================
*/
// FUNCTION: STBC 0x84CA60 ?Register@NiBezierCylinder@@SAHXZ
NiImplementRegister( NiBezierCylinder )

/*
================
NiBezierCylinder::NiBezierCylinder

A patch of the given degree on the mesh's arrays: two rows of degree + 1
control points.
================
*/
// FUNCTION: STBC 0x84CBB0 ??0NiBezierCylinder@@QAE@GPAVNiPoint3@@PAVNiPoint2@@PAVNiColorA@@_N@Z
NiBezierCylinder::NiBezierCylinder( unsigned short degree_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08 )
	: NiBezierPatch( degree_, 2 * ( degree_ + 1 ), vertices_, textures_, colors_, unknown08 ) {
	order = degree_ + 1;
	unknown14E = degree_ - 1;
	unknown160 = 0;
	AllocatePowers();
	ComputeBinomials();
}

/*
================
NiBezierCylinder::NiBezierCylinder
================
*/
// FUNCTION: STBC 0x84CC80 ??0NiBezierCylinder@@QAE@XZ
NiBezierCylinder::NiBezierCylinder() {
	order = 0;
	unknown14E = 0;
	binomials = 0;
	unknown154 = 0;
	powers = 0;
	complementPowers = 0;
	unknown160 = 0;
}

/*
================
NiBezierCylinder::~NiBezierCylinder
================
*/
// FUNCTION: STBC 0x84CCC0
NiBezierCylinder::~NiBezierCylinder() {
	delete [] powers;
	delete [] complementPowers;
	delete [] binomials;
	delete [] unknown154;
}

/*
================
NiBezierCylinder::AllocatePowers

The zeroth powers are 1; slot 43 fills the rest.
================
*/
// FUNCTION: STBC 0x84CD10
void NiBezierCylinder::AllocatePowers() {
	powers = new float[order];
	complementPowers = new float[order];
	powers[0] = 1.0f;
	complementPowers[0] = 1.0f;
}

/*
================
NiBezierCylinder::ComputeBinomials

Builds Pascal's triangle up to the degree in a scratch table and keeps the
rows of the degree and of degree - 2.
================
*/
// FUNCTION: STBC 0x84CD60
// The statements are the debug copy's (AnimOnly.exe 0x4F2C0B).
void NiBezierCylinder::ComputeBinomials() {
	unsigned short i;
	int **triangle = new int *[order];
	triangle[0] = new int[order * order];
	for ( i = 1; i < order; i++ ) {
		triangle[i] = triangle[0] + i * order;
	}

	triangle[0][0] = 1;
	for ( unsigned short n = 1; n < order; n++ ) {
		triangle[n][0] = 1;
		triangle[n][n] = 1;
		for ( i = 1; i < n; i++ ) {
			triangle[n][i] = triangle[n - 1][i] + triangle[n - 1][i - 1];
		}
	}

	binomials = new float[order];
	unknown154 = new float[unknown14E];
	for ( i = 0; i < unknown14E; i++ ) {
		unknown154[i] = triangle[degree - 2][i];
		binomials[i] = triangle[degree][i];
	}
	binomials[unknown14E] = triangle[degree][unknown14E];
	binomials[degree] = triangle[degree][degree];

	delete [] triangle[0];
	delete [] triangle;
}

/*
================
NiBezierCylinder::VirtualSlot40

The level is computed into a float local first, as the SDK's debug copy
shows (AnimOnly.exe 0x4F2F57, NiBezierCylinder.cpp).
================
*/
// FUNCTION: STBC 0x84CF20
unsigned char NiBezierCylinder::VirtualSlot40() {
	float maxLevel = 15.0f - logf( 3.0f * ( unknown160 + 1 ) ) / logf( 2.0f );
	return (unsigned char)maxLevel;
}

/*
================
NiBezierCylinder::VirtualSlot47
================
*/
// FUNCTION: STBC 0x84CF60
unsigned short NiBezierCylinder::VirtualSlot47() {
	return ( 1 << VirtualSlot40() ) + 1;
}

/*
================
NiBezierCylinder::VirtualSlot39
================
*/
// FUNCTION: STBC 0x84CF80
void NiBezierCylinder::VirtualSlot39( unsigned char level_ ) {
	if ( level != level_ && level_ <= VirtualSlot40() ) {
		level = level_;
		Unknown8330E0( level_ );
	}
}

/*
================
NiBezierCylinder::Unknown84CFB0
================
*/
// FUNCTION: STBC 0x84CFB0
void NiBezierCylinder::Unknown84CFB0( unsigned char unknown160_ ) {
	if ( unknown160 == unknown160_ || unknown160_ > Unknown84D020() ) {
		return;
	}

	unknown160 = unknown160_;
	Unknown8330E0( level );
}

/*
================
NiBezierCylinder::Unknown84D020

The SDK's debug copy (AnimOnly.exe 0x4F3114) writes powf( 2.0f, ... ) / 3.0f
into a float local; that spelling reads C2's folded reciprocal instead of the
1/3 constant the tree's other readers share, so the release spelling stays.
================
*/
// FUNCTION: STBC 0x84D020
unsigned char NiBezierCylinder::Unknown84D020() {
	return (unsigned char)( (float)pow( 2.0, 15.0f - level ) * ( 1.0f / 3.0f ) - 1.0f );
}

/*
================
NiBezierCylinder::VirtualSlot48
================
*/
// FUNCTION: STBC 0x84D060
unsigned short NiBezierCylinder::VirtualSlot48() {
	return unknown160 + 2;
}

/*
================
NiBezierCylinder::VirtualSlot43

Fills the power tables for the parameter and builds the Bernstein weights
of the control points (weights) and, when the patch has normals, their
derivative weights (uDerivatives). The second argument is unused.
================
*/
// FUNCTION: STBC 0x84D070
void NiBezierCylinder::VirtualSlot43( float t, float ) {
	float complement = 1.0f - t;
	int i;
	int j;

	for ( i = 0; i < degree; i++ ) {
		powers[i + 1] = t * powers[i];
		complementPowers[i + 1] = complement * complementPowers[i];
	}
	for ( i = 0, j = degree; i <= degree; i++, j-- ) {
		weights[i] = binomials[i] * powers[i] * complementPowers[j];
	}
	if ( normals != NULL ) {
		uDerivatives[0] = -(int)degree * complementPowers[unknown14E];
		uDerivatives[degree] = degree * powers[unknown14E];
		for ( i = 1, j = unknown14E; i < degree; i++, j-- ) {
			uDerivatives[i] = binomials[i] * ( i * complement - j * t ) * powers[i - 1] * complementPowers[j - 1];
		}
	}
}

/*
================
NiBezierCylinder::VirtualSlot41

The evaluator: the point at ( u, v ) lies between the two rows' curves at
u, blended linearly in v. Adds its position, its texture coordinates in
each set, its color and its normal (the unit cross product of the first
row's tangent and the rule between the rows) to the tessellated sets and
returns the index of the new vertex.
================
*/
// FUNCTION: STBC 0x84D1F0 ?VirtualSlot41@NiBezierCylinder@@UAEIMM@Z
// The statements are the debug copy's (AnimOnly.exe 0x4F33C6).
unsigned int NiBezierCylinder::VirtualSlot41( float u, float v ) {
	float w = 1.0f - v;
	VirtualSlot43( u, v );

	int i;
	int j;
	NiPoint3 point0 = weights[0] * vertices[0];	// (name invented; fits the /Od frame order)
	NiPoint3 point1 = weights[0] * vertices[order];	// (name invented; fits the /Od frame order)
	for ( i = 1, j = order + 1; i <= degree; i++, j++ ) {
		point0 += weights[i] * vertices[i];
		point1 += weights[i] * vertices[j];
	}
	unsigned int vertex = positions->Add( w * point0 + v * point1 );	// (name invented; fits the /Od frame order)

	if ( textureCoordinates ) {
		for ( unsigned short set = 0; set < textureSets; set++ ) {
			unsigned short offset = set * unknownD6;	// (name invented; fits the /Od frame order)
			NiPoint2 textureCoord0 = weights[0] * textures[offset];	// (name invented; fits the /Od frame order)
			NiPoint2 textureCoord1 = weights[0] * textures[order + offset];	// (name invented; fits the /Od frame order)
			// BUG: the later control points are read from the first set
			// whatever set is being evaluated
			for ( i = 1, j = order + 1; i <= degree; i++, j++ ) {
				textureCoord0 += weights[i] * textures[i];
				textureCoord1 += weights[i] * textures[j];
			}
			textureCoordinates->Add( set, w * textureCoord0 + v * textureCoord1 );
		}
	}

	if ( vertexColors ) {
		NiColorA color0 = weights[0] * colors[0];
		NiColorA color1 = weights[0] * colors[order];
		for ( i = 1, j = order + 1; i <= degree; i++, j++ ) {
			color0 += weights[i] * colors[i];
			color1 += weights[i] * colors[j];
		}
		vertexColors->Add( w * color0 + v * color1 );
	}

	if ( normals ) {
		NiPoint3 tangent = uDerivatives[0] * vertices[0];
		for ( i = 1; i <= degree; i++ ) {
			tangent += uDerivatives[i] * vertices[i];
		}
		NiPoint3 rule = weights[0] * ( vertices[order] - vertices[0] );
		for ( i = 1, j = order + 1; i <= degree; i++, j++ ) {
			rule += weights[i] * ( vertices[j] - vertices[i] );
		}
		normals->Add( tangent.UnitCross( rule ) );
	}
	return vertex;
}

/*
================
NiBezierCylinder::VirtualSlot44

Tessellates the patch as a regular grid at the level: 2 ^ level segments
along the curve and unknown160 + 1 rows across it, two triangles to a cell.
================
*/
// FUNCTION: STBC 0x84D9F0
// The statements are the debug copy's (AnimOnly.exe 0x4F3B02).
void NiBezierCylinder::VirtualSlot44( unsigned char level_ ) {
	unsigned short segments = 1 << level_;
	unsigned short rowCount = unknown160 + 1;	// (name invented; fits the /Od frame order)
	unsigned short row;	// (name invented; fits the /Od frame order)
	unsigned short segment;	// (name invented; fits the /Od frame order)

	vertexCount = ( segments + 1 ) * ( rowCount + 1 );
	triangleCount = 2 * segments * rowCount;
	CreateSets();

	float rowStep = 1.0f / rowCount;
	float segmentStep = 1.0f / segments;
	for ( row = 0; row <= rowCount; row++ ) {
		float v = row * rowStep;
		for ( segment = 0; segment <= segments; segment++ ) {
			float u = segment * segmentStep;
			VirtualSlot41( u, v );
		}
	}

	unsigned short rowLen = segments + 1;	// (name invented; fits the /Od frame order)
	unsigned short rowStart = 0;
	for ( row = 0; row < rowCount; row++ ) {
		unsigned short bottom0 = rowStart;	// (name invented; fits the /Od frame order)
		unsigned short bottom1 = bottom0 + 1;	// (name invented; fits the /Od frame order)
		rowStart += rowLen;
		unsigned short top0 = rowStart;	// (name invented; fits the /Od frame order)
		for ( segment = 0; segment < segments; segment++ ) {
			triangles->Add( bottom0 );
			triangles->Add( bottom1 );
			triangles->Add( top0 );
			triangles->Add( bottom1 );
			triangles->Add( top0 + 1 );
			triangles->Add( top0 );
			bottom0++;
			bottom1++;
			top0++;
		}
	}

	NiPoint3 *localVertices = 0;	// (name invented; fits the /Od frame order)
	if ( positions ) {
		localVertices = positions->GetBase();
	}
	modelBound.ComputeFromData( vertexCount, localVertices );
	worldSetsStale = true;
}

/*
================
NiBezierCylinder::VirtualSlot45

Sizes the sets for the level, evaluates the points of the two edge curves
into the vertex cache, then has slot 49 build the rest, and recomputes the
model bound.
================
*/
// FUNCTION: STBC 0x84DDA0
// The statements are the debug copy's (AnimOnly.exe 0x4F3E03).
void NiBezierCylinder::VirtualSlot45( unsigned char level_ ) {
	unsigned short segments = 1 << level_;
	unsigned short rowCount = unknown160 + 1;	// (name invented; fits the /Od frame order)

	vertexCount = ( segments + 1 ) * ( rowCount + 1 );
	triangleCount = 2 * segments * rowCount;
	float rowStep = 1.0f / rowCount;	// (name invented; fits the /Od frame order)
	unsigned short curvePoints = VirtualSlot47();
	unsigned short edgeCount = VirtualSlot48();	// (name invented; fits the /Od frame order)
	CreateSets();

	vertexCache.cache->NextStamp();
	for ( int i = 0; i < 2 * edgeCount; i++ ) {
		float u = ( i % 2 ) ? 1.0f : 0.0f;
		float v = i / 2 * rowStep;
		vertexCache.cache->Set( curvePoints * ( i / 2 ) + ( curvePoints - 1 ) * ( i % 2 ), VirtualSlot41( u, v ) );
	}

	unsigned short *scratch = new unsigned short[3 * edgeCount];	// (name invented; fits the /Od frame order)
	VirtualSlot49( 0, level_, 0, scratch );
	delete [] scratch;

	vertexCount = positions->GetSize();
	triangleCount = triangles->GetSize() / 3;
	modelBound.ComputeFromData( vertexCount, positions ? positions->GetBase() : 0 );
	worldSetsStale = true;
}

/*
================
NiBezierCylinder::VirtualSlot49

Refines the strip of the tessellation grid that starts at the grid point
first and spans 2 ^ ( finest level - depth ) steps along the curve, over
every row. columns is scratch of three words a row: the strip's first
grid point in each row, its last, and the midpoint between. Above the
wanted level every row is split at its midpoint; below it, down to the
finest level, a row is split while slot 46 finds its segment bends enough,
scanning from the first row and then back from the last. A strip whose
rows all split recurses into its two halves; the rest is filled with
triangles: four to a cell between two split rows, three where a split row
meets an unsplit one and two between unsplit rows, the diagonals
alternating from cell to cell.
================
*/
// FUNCTION: STBC 0x84DF60
// PARKED: frame and inline budget: /Od identical to the debug copy but for frame slots; retail packs the cell index arrays 0x10 higher and the row bounds one slot apart, calls one Add out of line where this build expands it, and schedules one copy in the seam differently
// The statements are the debug copy's (AnimOnly.exe 0x4F40A1).
void NiBezierCylinder::VirtualSlot49( unsigned char depth, unsigned char level_, unsigned short first, unsigned short *columns ) {
	unsigned short rows = unknown160 + 1;
	unsigned short size = 1 << ( VirtualSlot40() - depth );
	unsigned short curvePoints = VirtualSlot47();
	unsigned short edgePoints = VirtualSlot48();
	float uStep = 1.0f / ( curvePoints - 1 );
	float vStep = 1.0f / rows;
	unsigned short half = size >> 1;
	unsigned short vertex;

	for ( unsigned int row = 0; row < edgePoints; row++ ) {
		columns[row] = row * curvePoints + first;
		columns[edgePoints + row] = row * curvePoints + first + size;
		columns[row + 2 * edgePoints] = row * curvePoints + first + half;
	}

	// the rows split from the first one and back from the last one
	bool split = true;
	unsigned short upper = rows;
	unsigned short lower = 0;
	if ( depth < level_ ) {
		// BUG: redundant: split is already true; the debug copy stores it again
		// (AnimOnly.exe 0x4F40A1), the release build drops the dead store
		split = true;
		for ( unsigned int row = 0; row < edgePoints; row++ ) {
			unsigned short middle = row + 2 * edgePoints;
			if ( !vertexCache.cache->Get( columns[middle], vertex ) ) {
				float s = columns[middle] % curvePoints * uStep;
				float t = row * vStep;
				vertexCache.cache->Set( columns[middle], VirtualSlot41( s, t ) );
			}
		}
	} else if ( depth >= VirtualSlot40() ) {
		split = false;
	} else {
		unsigned int row;
		for ( row = 0; row < edgePoints; row++ ) {
			if ( VirtualSlot46( columns[row] % curvePoints, columns[row] / curvePoints, columns[row + edgePoints] % curvePoints, columns[row + edgePoints] / curvePoints ) ) {
				unsigned short middle = row + 2 * edgePoints;
				if ( !vertexCache.cache->Get( columns[middle], vertex ) ) {
					float s = columns[middle] % curvePoints * uStep;
					float t = row * vStep;
					vertexCache.cache->Set( columns[middle], VirtualSlot41( s, t ) );
				}
			} else {
				split = false;
				break;
			}
		}
		upper = row;
		lower = upper;
		if ( lower != edgePoints ) {
			unsigned int back;	// (name invented)
			for ( back = rows; back > lower; back-- ) {
				if ( VirtualSlot46( columns[back] % curvePoints, columns[back] / curvePoints, columns[back + edgePoints] % curvePoints, columns[back + edgePoints] / curvePoints ) ) {
					unsigned short middle = back + 2 * edgePoints;
					if ( !vertexCache.cache->Get( columns[middle], vertex ) ) {
						float s = columns[middle] % curvePoints * uStep;
						float t = back * vStep;
						vertexCache.cache->Set( columns[middle], VirtualSlot41( s, t ) );
					}
				} else {
					break;
				}
			}
			upper = back;
		}
	}

	if ( split ) {
		// the recursion rewrites columns
		unsigned short second = columns[2 * edgePoints];
		VirtualSlot49( depth + 1, level_, columns[0], columns );
		VirtualSlot49( depth + 1, level_, second, columns );
		return;
	}

	// the cells between rows split from the first one
	for ( int i = 0; i < lower - 1; i++ ) {
		unsigned short index[6];		// the cell's first, middle and last grid points in row i, then in row i + 1
		index[0] = i;
		index[1] = i + 2 * edgePoints;
		index[2] = edgePoints + i;
		index[3] = i + 1;
		index[4] = i + 2 * edgePoints + 1;
		index[5] = edgePoints + i + 1;
		if ( i % 2 ) {
			vertexCache.cache->Get( columns[index[0]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[1]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[3]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[4]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[3]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[1]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[4]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[1]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[5]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[2]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[5]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[1]], vertex );
			triangles->Add( vertex );
		} else {
			vertexCache.cache->Get( columns[index[3]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[0]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[4]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[1]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[4]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[0]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[1]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[2]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[4]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[5]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[4]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[index[2]], vertex );
			triangles->Add( vertex );
		}
	}

	// the cells between unsplit rows, after the one that meets the last row
	// split from the first one
	if ( i < upper ) {
		if ( i == lower - 1 ) {
			unsigned short seam[6];		// as index, for the cell where a split row meets an unsplit one
			seam[0] = i;
			seam[1] = i + 2 * edgePoints;
			seam[2] = edgePoints + i;
			seam[3] = i + 1;
			seam[4] = i + 2 * edgePoints + 1;
			seam[5] = edgePoints + i + 1;
			vertexCache.cache->Get( columns[seam[0]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[seam[1]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[seam[3]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[seam[5]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[seam[3]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[seam[1]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[seam[2]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[seam[5]], vertex );
			triangles->Add( vertex );
			vertexCache.cache->Get( columns[seam[1]], vertex );
			triangles->Add( vertex );
			i++;
		}
		for ( ; i < upper; i++ ) {
			unsigned short index[6];
			index[0] = i;
			index[1] = i + 2 * edgePoints;
			index[2] = edgePoints + i;
			index[3] = i + 1;
			index[4] = i + 2 * edgePoints + 1;
			index[5] = edgePoints + i + 1;
			if ( i % 2 ) {
				vertexCache.cache->Get( columns[index[0]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[2]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[3]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[5]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[3]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[2]], vertex );
				triangles->Add( vertex );
			} else {
				vertexCache.cache->Get( columns[index[3]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[0]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[5]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[2]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[5]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[0]], vertex );
				triangles->Add( vertex );
			}
		}
	}

	// the cell that meets the first row split back from the last one, and
	// the cells after it
	if ( i < rows ) {
		unsigned short seam[6];		// as index, for the cell where a split row meets an unsplit one
		seam[0] = i;
		seam[1] = i + 2 * edgePoints;
		seam[2] = edgePoints + i;
		seam[3] = i + 1;
		seam[4] = i + 2 * edgePoints + 1;
		seam[5] = edgePoints + i + 1;
		vertexCache.cache->Get( columns[seam[3]], vertex );
		triangles->Add( vertex );
		vertexCache.cache->Get( columns[seam[0]], vertex );
		triangles->Add( vertex );
		vertexCache.cache->Get( columns[seam[4]], vertex );
		triangles->Add( vertex );
		vertexCache.cache->Get( columns[seam[2]], vertex );
		triangles->Add( vertex );
		vertexCache.cache->Get( columns[seam[4]], vertex );
		triangles->Add( vertex );
		vertexCache.cache->Get( columns[seam[0]], vertex );
		triangles->Add( vertex );
		vertexCache.cache->Get( columns[seam[5]], vertex );
		triangles->Add( vertex );
		vertexCache.cache->Get( columns[seam[4]], vertex );
		triangles->Add( vertex );
		vertexCache.cache->Get( columns[seam[2]], vertex );
		triangles->Add( vertex );
		i++;
		for ( ; i < rows; i++ ) {
			unsigned short index[6];
			index[0] = i;
			index[1] = i + 2 * edgePoints;
			index[2] = edgePoints + i;
			index[3] = i + 1;
			index[4] = i + 2 * edgePoints + 1;
			index[5] = edgePoints + i + 1;
			if ( i % 2 ) {
				vertexCache.cache->Get( columns[index[0]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[1]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[3]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[4]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[3]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[1]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[4]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[1]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[5]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[2]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[5]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[1]], vertex );
				triangles->Add( vertex );
			} else {
				vertexCache.cache->Get( columns[index[3]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[0]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[4]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[1]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[4]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[0]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[1]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[2]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[4]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[5]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[4]], vertex );
				triangles->Add( vertex );
				vertexCache.cache->Get( columns[index[2]], vertex );
				triangles->Add( vertex );
			}
		}
	}
}

/*
================
NiBezierCylinder::VirtualSlot46

Whether the curve segment from u0 to u1 on row v0 of the tessellation grid
bends enough to split: the second derivative of the blended curve at the
segment's midpoint (the rows' second differences weighted by the Bernstein
basis of degree - 2), scaled by the segment's parameter length squared,
against unknownF8 times the squared chord between the vertices already
made for its ends. A split adds the midpoint's vertex to the cache; a
midpoint already there splits at once.
================
*/
// FUNCTION: STBC 0x8500F0 ?VirtualSlot46@NiBezierCylinder@@UAE_NGGGG@Z
// The statements are the debug copy's (AnimOnly.exe 0x4F5C09).
bool NiBezierCylinder::VirtualSlot46( unsigned short u0, unsigned short v0, unsigned short u1, unsigned short v1 ) {
	assert( v0 == v1 );
	unsigned short u = ( u1 + u0 ) >> 1;
	unsigned short curvePoints = VirtualSlot47();
	unsigned short index = v0 * curvePoints + u;
	bool split = true;
	unsigned short vertex;
	if ( !vertexCache.cache->Get( index, vertex ) ) {
		unsigned short length = u1 - u0;
		float step = 1.0f / ( curvePoints - 1 );
		float rowStep = 1.0f / ( GetUnknown160() + 1 );
		float t = u * step;
		float s = 1.0f - t;
		float row = v0 * rowStep;
		float rowComplement = 1.0f - row;

		int i;
		for ( i = 0; i < degree; i++ ) {
			powers[i + 1] = t * powers[i];
			complementPowers[i + 1] = s * complementPowers[i];
		}
		float *basis = new float[unknown14E];
		int j;
		for ( i = 0, j = degree - 2; i < unknown14E; i++, j-- ) {
			basis[i] = unknown154[i] * powers[i] * complementPowers[j];
		}

		NiPoint3 bend0 = basis[0] * ( vertices[2] - 2.0f * vertices[1] + vertices[0] );
		NiPoint3 bend1 = basis[0] * ( vertices[order + 2] - 2.0f * vertices[order + 1] + vertices[order] );
		// BUG: the second row's later differences start at vertices[degree],
		// one control point before that row's own
		for ( i = 1, j = degree; i < unknown14E; i++, j++ ) {
			bend0 += basis[i] * ( vertices[i + 2] - 2.0f * vertices[i + 1] + vertices[i] );
			bend1 += basis[i] * ( vertices[j + 2] - 2.0f * vertices[j + 1] + vertices[j] );
		}
		delete [] basis;
		NiPoint3 bend = (float)( degree * unknown14E ) * ( rowComplement * bend0 + row * bend1 );
		float bendLength = bend.x * bend.x + bend.y * bend.y + bend.z * bend.z;	// squared

		NiPoint3 start;
		NiPoint3 end;
		unsigned short first = v0 * curvePoints + u0;	// (name invented)
		unsigned short last = v1 * curvePoints + u1;	// (name invented)
		vertexCache.cache->Get( first, vertex );
		start = positions->GetAt( vertex );
		vertexCache.cache->Get( last, vertex );
		end = positions->GetAt( vertex );
		NiPoint3 chord = end - start;
		float chordLength = chord.x * chord.x + chord.y * chord.y + chord.z * chord.z;	// squared
		float segment = step * step * ( length * length );
		if ( bendLength * segment * segment > chordLength * unknownF8 ) {
			split = true;
		} else {
			split = false;
		}
		if ( split ) {
			vertexCache.cache->Set( index, VirtualSlot41( t, row ) );
		}
	}
	return split;
}

/*
================
NiBezierCylinder::CreateClone
================
*/
// FUNCTION: STBC 0x850870
NiObject *NiBezierCylinder::CreateClone() {
	NiBezierCylinder *cyl = new NiBezierCylinder;
	assert( cyl );
	CopyMembers( cyl );
	return cyl;
}

/*
================
NiBezierCylinder::CopyMembers

A bug of the original: after copying unknown160 it rebuilds the tables and
the tessellation of the original, not of the clone, so the clone keeps no
tables and the original leaks its old ones.
================
*/
// FUNCTION: STBC 0x850990
void NiBezierCylinder::CopyMembers( NiBezierCylinder *dest ) {
	NiBezierPatch::CopyMembers( dest );

	dest->unknown160 = unknown160;
	order = degree + 1;
	unknown14E = degree - 1;
	AllocatePowers();
	ComputeBinomials();
	Unknown8330E0( level );
}

/*
================
NiBezierCylinder::CreateObject

Creates the patch and loads it.
================
*/
// FUNCTION: STBC 0x850A30
void NiBezierCylinder::CreateObject( NiStream &stream, NiObject *&object ) {
	NiBezierCylinder *cylinder = new NiBezierCylinder;
	stream.SetLoadRecord( new NiBezierCylinderLoadRecord );
	cylinder->LoadBinary( stream );
	object = cylinder;
}

/*
================
NiBezierCylinder::LoadBinary

Builds the tables for the loaded degree and tessellates at the loaded level.
================
*/
// FUNCTION: STBC 0x850B80
void NiBezierCylinder::LoadBinary( NiStream &stream ) {
	NiBezierPatch::LoadBinary( stream );

	NiStreamLoadBinary( stream, unknown160 );
	order = degree + 1;
	unknown14E = degree - 1;
	AllocatePowers();
	ComputeBinomials();
	Unknown8330E0( level );
}

/*
================
NiBezierCylinder::LinkObject
================
*/
// FUNCTION: STBC 0x850C20
void NiBezierCylinder::LinkObject( NiStream &stream ) {
	NiBezierPatch::LinkObject( stream );
}

/*
================
NiBezierCylinder::RegisterStreamables
================
*/
// FUNCTION: STBC 0x850C30
bool NiBezierCylinder::RegisterStreamables( NiStream &stream ) {
	return NiBezierPatch::RegisterStreamables( stream );
}

/*
================
NiBezierCylinder::SaveBinary
================
*/
// FUNCTION: STBC 0x850C40
void NiBezierCylinder::SaveBinary( NiStream &stream ) {
	NiBezierPatch::SaveBinary( stream );

	NiStreamSaveBinary( stream, unknown160 );
}

/*
================
NiBezierCylinder::IsEqual
================
*/
// FUNCTION: STBC 0x850C70
bool NiBezierCylinder::IsEqual( NiObject *object ) {
	if ( !NiBezierPatch::IsEqual( object ) ) {
		return false;
	}

	NiBezierCylinder *cylinder = (NiBezierCylinder *)object;
	if ( unknown160 != cylinder->unknown160 || order != cylinder->order || unknown14E != cylinder->unknown14E ) {
		return false;
	}
	return true;
}
