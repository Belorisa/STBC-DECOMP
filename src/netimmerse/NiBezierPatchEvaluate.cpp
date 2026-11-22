// NiBezierPatchEvaluate.cpp -- NiBezierPatch's evaluation at the parameters (invented file name)
//
// Retail keeps these in a unit of their own (0x8511E0-0x8540F0), after
// NiBezierCylinder's, with its <iostream> quads, locale pair, registrations
// and the inline copies UpdateWorldSets calls (NiTSet::GetAt, the
// NiAVObject world getters).

#include "NiRTLib.h"
#include "NiBezierPatch.h"
#include "NiTriShapeDynamicData.h"
#include "NiTriBasedGeom.h"
#include "NiCamera.h"
#include "NiNode.h"
#include "NiDynamicEffect.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00854150 _$E30
// FUNCTION: STBC 0x00854160 _$E41
// FUNCTION: STBC 0x00854170 _$E45
// FUNCTION: STBC 0x00854180 _$E49
// FUNCTION: STBC 0x00854190 _$E53
// FUNCTION: STBC 0x008541d0 _$E177
// FUNCTION: STBC 0x00854210 _$E220
// FUNCTION: STBC 0x00854240 _$E119
// FUNCTION: STBC 0x00854250 _$E132
// FUNCTION: STBC 0x00854260 _$E225
// FUNCTION: STBC 0x00854270 _$E230
// FUNCTION: STBC 0x00854280 _$E234
// FUNCTION: STBC 0x00854290 _$E238
// FUNCTION: STBC 0x008542a0 _$E245
// FUNCTION: STBC 0x008542b0 _$E185
// FUNCTION: STBC 0x008542c0 _$E212
// FUNCTION: STBC 0x008542d0 _$E189
// FUNCTION: STBC 0x008542e0 _$E216
// FUNCTION: STBC 0x008542f0 _$E193
// FUNCTION: STBC 0x00854300 _$E197
// FUNCTION: STBC 0x00854310 _$E205

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x008540f0 _$E17
// FUNCTION: STBC 0x00854110 _$E15
// FUNCTION: STBC 0x00854120 _$E23
// FUNCTION: STBC 0x00854140 _$E21
// FUNCTION: STBC 0x00854220 _$E262


/*
================
NiBezierPatch::VirtualSlot41

Does slot 43, then the function chosen for the sets the patch has, and
returns the index of the vertex it added.
================
*/
// FUNCTION: STBC 0x8511E0
unsigned int NiBezierPatch::VirtualSlot41( float u, float v ) {
	VirtualSlot43( u, v );
	return ( this->*unknown120 )( u, v );
}

/*
================
NiBezierPatch::UpdateWorldSets

Carries the tessellated positions, and the normals when the patch has
them, into world space, and marks the world sets current.
================
*/
// FUNCTION: STBC 0x851210
void NiBezierPatch::UpdateWorldSets() {
	unsigned short i;

	worldPositions->RemoveAll();
	for ( i = 0; i < vertexCount; i++ ) {
		worldPositions->Add( GetWorldScale() * ( GetWorldRotate() * positions->GetAt( i ) ) + GetWorldTranslate() );
	}
	if ( normals ) {
		worldNormals->RemoveAll();
		for ( i = 0; i < vertexCount; i++ ) {
			worldNormals->Add( GetWorldRotate() * normals->GetAt( i ) );
		}
	}
	worldSetsStale = false;
}

/*
================
NiBezierPatch::Unknown851330

The position at the parameters: the control points weighted by slot 43's
weights.
================
*/
// FUNCTION: STBC 0x851330
unsigned int NiBezierPatch::Unknown851330( float u, float v ) {
	int i;

	NiPoint3 vertex = vertices[0];
	float weight = weights[0];
	NiPoint3 position;
	position.x = vertex.x * weight;
	position.y = vertex.y * weight;
	position.z = vertex.z * weight;
	for ( i = 1; i < controlPointCount; i++ ) {
		vertex = vertices[i];
		weight = weights[i];
		position.x += vertex.x * weight;
		position.y += vertex.y * weight;
		position.z += vertex.z * weight;
	}
	return positions->Add( position );
}

/*
================
NiBezierPatch::Unknown851400

The position and the texture coordinates of each set at the parameters.
================
*/
// FUNCTION: STBC 0x851400
unsigned int NiBezierPatch::Unknown851400( float u, float v ) {
	int i;
	unsigned short set;

	NiPoint3 vertex = vertices[0];
	float weight = weights[0];
	NiPoint3 position;
	position.x = vertex.x * weight;
	position.y = vertex.y * weight;
	position.z = vertex.z * weight;
	NiPoint2 *coords = new NiPoint2[textureSets];
	NiPoint2 texture;
	for ( set = 0; set < textureSets; set++ ) {
		texture = textures[set * unknownD6];
		coords[set].x = texture.x * weight;
		coords[set].y = texture.y * weight;
	}
	for ( i = 1; i < controlPointCount; i++ ) {
		vertex = vertices[i];
		weight = weights[i];
		position.x += vertex.x * weight;
		position.y += vertex.y * weight;
		position.z += vertex.z * weight;
		for ( set = 0; set < textureSets; set++ ) {
			texture = textures[set * unknownD6 + i];
			coords[set].x += texture.x * weight;
			coords[set].y += texture.y * weight;
		}
	}
	for ( set = 0; set < textureSets; set++ ) {
		textureCoordinates->Add( set, coords[set] );
	}
	delete [] coords;
	return positions->Add( position );
}

/*
================
NiBezierPatch::Unknown851620

The position and the colour at the parameters.
================
*/
// FUNCTION: STBC 0x851620
// PARKED: x87 operand order: the first component of the colour sum loads the product before the sum (retail the reverse); the loop's product goes through the friend operator* for retail's out-of-line result copy
unsigned int NiBezierPatch::Unknown851620( float u, float v ) {
	int i;

	NiPoint3 vertex = vertices[0];
	float weight = weights[0];
	NiColorA color = colors[0] * weight;
	NiPoint3 position;
	position.x = vertex.x * weight;
	position.y = vertex.y * weight;
	position.z = vertex.z * weight;
	for ( i = 1; i < controlPointCount; i++ ) {
		vertex = vertices[i];
		weight = weights[i];
		position.x += vertex.x * weight;
		position.y += vertex.y * weight;
		position.z += vertex.z * weight;
		color += weight * colors[i];
	}
	vertexColors->Add( color );
	return positions->Add( position );
}

/*
================
NiBezierPatch::Unknown851840

The position, the colour and the texture coordinates of each set at the
parameters.
================
*/
// FUNCTION: STBC 0x851840
unsigned int NiBezierPatch::Unknown851840( float u, float v ) {
	int i;
	unsigned short set;

	NiPoint3 vertex = vertices[0];
	float weight = weights[0];
	NiColorA color = weight * colors[0];
	NiPoint3 position;
	position.x = vertex.x * weight;
	position.y = vertex.y * weight;
	position.z = vertex.z * weight;
	NiPoint2 *coords = new NiPoint2[textureSets];
	NiPoint2 texture;
	for ( set = 0; set < textureSets; set++ ) {
		texture = textures[set * unknownD6];
		coords[set].x = texture.x * weight;
		coords[set].y = texture.y * weight;
	}
	for ( i = 1; i < controlPointCount; i++ ) {
		vertex = vertices[i];
		weight = weights[i];
		position.x += vertex.x * weight;
		position.y += vertex.y * weight;
		position.z += vertex.z * weight;
		color += weight * colors[i];
		for ( set = 0; set < textureSets; set++ ) {
			texture = textures[set * unknownD6 + i];
			coords[set].x += texture.x * weight;
			coords[set].y += texture.y * weight;
		}
	}
	for ( set = 0; set < textureSets; set++ ) {
		textureCoordinates->Add( set, coords[set] );
	}
	vertexColors->Add( color );
	delete [] coords;
	return positions->Add( position );
}

/*
================
NiBezierPatch::Unknown851B80

The position and the normal at the parameters: the normal is the unit
cross product of the surface's tangents along the two parameters, summed
from the derivative weights.
================
*/
// FUNCTION: STBC 0x851B80
unsigned int NiBezierPatch::Unknown851B80( float u, float v ) {
	int i;

	NiPoint3 vertex = vertices[0];
	float weight = weights[0];
	float uWeight = uDerivatives[0];
	float vWeight = vDerivatives[0];
	NiPoint3 position;
	NiPoint3 uTangent;
	NiPoint3 vTangent;
	position.x = vertex.x * weight;
	uTangent.x = vertex.x * uWeight;
	vTangent.x = vertex.x * vWeight;
	position.y = vertex.y * weight;
	uTangent.y = vertex.y * uWeight;
	vTangent.y = vertex.y * vWeight;
	position.z = vertex.z * weight;
	uTangent.z = vertex.z * uWeight;
	vTangent.z = vertex.z * vWeight;
	for ( i = 1; i < controlPointCount; i++ ) {
		vertex = vertices[i];
		weight = weights[i];
		uWeight = uDerivatives[i];
		vWeight = vDerivatives[i];
		position.x += vertex.x * weight;
		uTangent.x += vertex.x * uWeight;
		vTangent.x += vertex.x * vWeight;
		position.y += vertex.y * weight;
		uTangent.y += vertex.y * uWeight;
		vTangent.y += vertex.y * vWeight;
		position.z += vertex.z * weight;
		uTangent.z += vertex.z * uWeight;
		vTangent.z += vertex.z * vWeight;
	}
	normals->Add( uTangent.UnitCross( vTangent ) );
	return positions->Add( position );
}

/*
================
NiBezierPatch::Unknown851D90

The position, the normal and the texture coordinates of each set at the
parameters.
================
*/
// FUNCTION: STBC 0x851D90
unsigned int NiBezierPatch::Unknown851D90( float u, float v ) {
	int i;
	unsigned short set;

	NiPoint3 vertex = vertices[0];
	float weight = weights[0];
	float uWeight = uDerivatives[0];
	float vWeight = vDerivatives[0];
	NiPoint3 position;
	NiPoint3 uTangent;
	NiPoint3 vTangent;
	position.x = vertex.x * weight;
	uTangent.x = vertex.x * uWeight;
	vTangent.x = vertex.x * vWeight;
	position.y = vertex.y * weight;
	uTangent.y = vertex.y * uWeight;
	vTangent.y = vertex.y * vWeight;
	position.z = vertex.z * weight;
	uTangent.z = vertex.z * uWeight;
	vTangent.z = vertex.z * vWeight;
	NiPoint2 *coords = new NiPoint2[textureSets];
	NiPoint2 texture;
	for ( set = 0; set < textureSets; set++ ) {
		texture = textures[set * unknownD6];
		coords[set].x = texture.x * weight;
		coords[set].y = texture.y * weight;
	}
	for ( i = 1; i < controlPointCount; i++ ) {
		vertex = vertices[i];
		weight = weights[i];
		uWeight = uDerivatives[i];
		vWeight = vDerivatives[i];
		position.x += vertex.x * weight;
		uTangent.x += vertex.x * uWeight;
		vTangent.x += vertex.x * vWeight;
		position.y += vertex.y * weight;
		uTangent.y += vertex.y * uWeight;
		vTangent.y += vertex.y * vWeight;
		position.z += vertex.z * weight;
		uTangent.z += vertex.z * uWeight;
		vTangent.z += vertex.z * vWeight;
		for ( set = 0; set < textureSets; set++ ) {
			texture = textures[set * unknownD6 + i];
			coords[set].x += texture.x * weight;
			coords[set].y += texture.y * weight;
		}
	}
	for ( set = 0; set < textureSets; set++ ) {
		textureCoordinates->Add( set, coords[set] );
	}
	normals->Add( uTangent.UnitCross( vTangent ) );
	delete [] coords;
	return positions->Add( position );
}

/*
================
NiBezierPatch::Unknown852130

The position, the colour and the normal at the parameters.
================
*/
// FUNCTION: STBC 0x852130
// PARKED: x87 operand order of the colour sum's first component (as Unknown851620), and the inline budget in UnitCross: retail calls NiPoint3's constructor out of line for the scaled cross product and builds the zero inline, this build the reverse
unsigned int NiBezierPatch::Unknown852130( float u, float v ) {
	int i;

	NiPoint3 vertex = vertices[0];
	float weight = weights[0];
	float uWeight = uDerivatives[0];
	float vWeight = vDerivatives[0];
	NiColorA color = colors[0] * weight;
	NiPoint3 position;
	NiPoint3 uTangent;
	NiPoint3 vTangent;
	position.x = vertex.x * weight;
	uTangent.x = vertex.x * uWeight;
	vTangent.x = vertex.x * vWeight;
	position.y = vertex.y * weight;
	uTangent.y = vertex.y * uWeight;
	vTangent.y = vertex.y * vWeight;
	position.z = vertex.z * weight;
	uTangent.z = vertex.z * uWeight;
	vTangent.z = vertex.z * vWeight;
	for ( i = 1; i < controlPointCount; i++ ) {
		vertex = vertices[i];
		weight = weights[i];
		uWeight = uDerivatives[i];
		vWeight = vDerivatives[i];
		position.x += vertex.x * weight;
		uTangent.x += vertex.x * uWeight;
		vTangent.x += vertex.x * vWeight;
		position.y += vertex.y * weight;
		uTangent.y += vertex.y * uWeight;
		vTangent.y += vertex.y * vWeight;
		position.z += vertex.z * weight;
		uTangent.z += vertex.z * uWeight;
		vTangent.z += vertex.z * vWeight;
		color += weight * colors[i];
	}
	vertexColors->Add( color );
	normals->Add( uTangent.UnitCross( vTangent ) );
	return positions->Add( position );
}

/*
================
NiBezierPatch::Unknown8524D0

The position, the colour, the normal and the texture coordinates of each
set at the parameters.
================
*/
// FUNCTION: STBC 0x8524D0
// PARKED: x87 stray value: the first component of the colour sum is added on the FPU stack and popped (retail adds from memory), and one register swap in the weight loads
unsigned int NiBezierPatch::Unknown8524D0( float u, float v ) {
	int i;
	unsigned short set;

	NiPoint3 vertex = vertices[0];
	float weight = weights[0];
	float uWeight = uDerivatives[0];
	float vWeight = vDerivatives[0];
	NiColorA color = colors[0] * weight;
	NiPoint3 position;
	NiPoint3 uTangent;
	NiPoint3 vTangent;
	position.x = vertex.x * weight;
	uTangent.x = vertex.x * uWeight;
	vTangent.x = vertex.x * vWeight;
	position.y = vertex.y * weight;
	uTangent.y = vertex.y * uWeight;
	vTangent.y = vertex.y * vWeight;
	position.z = vertex.z * weight;
	uTangent.z = vertex.z * uWeight;
	vTangent.z = vertex.z * vWeight;
	NiPoint2 *coords = new NiPoint2[textureSets];
	NiPoint2 texture;
	for ( set = 0; set < textureSets; set++ ) {
		texture = textures[set * unknownD6];
		coords[set].x = texture.x * weight;
		coords[set].y = texture.y * weight;
	}
	for ( i = 1; i < controlPointCount; i++ ) {
		vertex = vertices[i];
		weight = weights[i];
		uWeight = uDerivatives[i];
		vWeight = vDerivatives[i];
		position.x += vertex.x * weight;
		uTangent.x += vertex.x * uWeight;
		vTangent.x += vertex.x * vWeight;
		position.y += vertex.y * weight;
		uTangent.y += vertex.y * uWeight;
		vTangent.y += vertex.y * vWeight;
		position.z += vertex.z * weight;
		uTangent.z += vertex.z * uWeight;
		vTangent.z += vertex.z * vWeight;
		color += colors[i] * weight;
		for ( set = 0; set < textureSets; set++ ) {
			texture = textures[set * unknownD6 + i];
			coords[set].x += texture.x * weight;
			coords[set].y += texture.y * weight;
		}
	}
	for ( set = 0; set < textureSets; set++ ) {
		textureCoordinates->Add( set, coords[set] );
	}
	vertexColors->Add( color );
	normals->Add( uTangent.UnitCross( vTangent ) );
	delete [] coords;
	return positions->Add( position );
}

/*
================
NiBezierPatch::Unknown852980

As Unknown851330, for a patch that reaches its control points through
the index table unknownE4.
================
*/
// FUNCTION: STBC 0x852980
unsigned int NiBezierPatch::Unknown852980( float u, float v ) {
	int i;

	NiPoint3 vertex = vertices[unknownE4[0]];
	float weight = weights[0];
	NiPoint3 position;
	position.x = vertex.x * weight;
	position.y = vertex.y * weight;
	position.z = vertex.z * weight;
	for ( i = 1; i < controlPointCount; i++ ) {
		vertex = vertices[unknownE4[i]];
		weight = weights[i];
		position.x += vertex.x * weight;
		position.y += vertex.y * weight;
		position.z += vertex.z * weight;
	}
	return positions->Add( position );
}

/*
================
NiBezierPatch::Unknown852A80

As Unknown851400, through the index table unknownE4.
================
*/
// FUNCTION: STBC 0x852A80
unsigned int NiBezierPatch::Unknown852A80( float u, float v ) {
	int i;
	unsigned short set;

	unsigned short index = unknownE4[0];
	NiPoint3 vertex = vertices[index];
	float weight = weights[0];
	NiPoint3 position;
	position.x = vertex.x * weight;
	position.y = vertex.y * weight;
	position.z = vertex.z * weight;
	NiPoint2 *coords = new NiPoint2[textureSets];
	NiPoint2 texture;
	for ( set = 0; set < textureSets; set++ ) {
		texture = textures[set * unknownD6 + index];
		coords[set].x = texture.x * weight;
		coords[set].y = texture.y * weight;
	}
	for ( i = 1; i < controlPointCount; i++ ) {
		index = unknownE4[i];
		vertex = vertices[index];
		weight = weights[i];
		position.x += vertex.x * weight;
		position.y += vertex.y * weight;
		position.z += vertex.z * weight;
		for ( set = 0; set < textureSets; set++ ) {
			texture = textures[set * unknownD6 + index];
			coords[set].x += texture.x * weight;
			coords[set].y += texture.y * weight;
		}
	}
	for ( set = 0; set < textureSets; set++ ) {
		textureCoordinates->Add( set, coords[set] );
	}
	delete [] coords;
	return positions->Add( position );
}

/*
================
NiBezierPatch::Unknown852CD0

As Unknown851620, through the index table unknownE4.
================
*/
// FUNCTION: STBC 0x852CD0
// PARKED: x87 operand order of the colour sum's first component, as in Unknown851620
unsigned int NiBezierPatch::Unknown852CD0( float u, float v ) {
	int i;

	unsigned short index = unknownE4[0];
	NiPoint3 vertex = vertices[index];
	float weight = weights[0];
	NiColorA color = colors[index] * weight;
	NiPoint3 position;
	position.x = vertex.x * weight;
	position.y = vertex.y * weight;
	position.z = vertex.z * weight;
	for ( i = 1; i < controlPointCount; i++ ) {
		index = unknownE4[i];
		vertex = vertices[index];
		weight = weights[i];
		position.x += vertex.x * weight;
		position.y += vertex.y * weight;
		position.z += vertex.z * weight;
		color += weight * colors[index];
	}
	vertexColors->Add( color );
	return positions->Add( position );
}

/*
================
NiBezierPatch::Unknown852F00

As Unknown851840, through the index table unknownE4.
================
*/
// FUNCTION: STBC 0x852F00
unsigned int NiBezierPatch::Unknown852F00( float u, float v ) {
	int i;
	unsigned short set;

	unsigned short index = unknownE4[0];
	NiPoint3 vertex = vertices[index];
	float weight = weights[0];
	NiColorA color = weight * colors[index];
	NiPoint3 position;
	position.x = vertex.x * weight;
	position.y = vertex.y * weight;
	position.z = vertex.z * weight;
	NiPoint2 *coords = new NiPoint2[textureSets];
	NiPoint2 texture;
	for ( set = 0; set < textureSets; set++ ) {
		texture = textures[set * unknownD6 + index];
		coords[set].x = texture.x * weight;
		coords[set].y = texture.y * weight;
	}
	for ( i = 1; i < controlPointCount; i++ ) {
		index = unknownE4[i];
		vertex = vertices[index];
		weight = weights[i];
		position.x += vertex.x * weight;
		position.y += vertex.y * weight;
		position.z += vertex.z * weight;
		color += weight * colors[index];
		for ( set = 0; set < textureSets; set++ ) {
			texture = textures[set * unknownD6 + index];
			coords[set].x += texture.x * weight;
			coords[set].y += texture.y * weight;
		}
	}
	for ( set = 0; set < textureSets; set++ ) {
		textureCoordinates->Add( set, coords[set] );
	}
	vertexColors->Add( color );
	delete [] coords;
	return positions->Add( position );
}

/*
================
NiBezierPatch::Unknown853260

As Unknown851B80, through the index table unknownE4.
================
*/
// FUNCTION: STBC 0x853260
unsigned int NiBezierPatch::Unknown853260( float u, float v ) {
	int i;

	unsigned short index = unknownE4[0];
	NiPoint3 vertex = vertices[index];
	float weight = weights[0];
	float uWeight = uDerivatives[0];
	float vWeight = vDerivatives[0];
	NiPoint3 position;
	NiPoint3 uTangent;
	NiPoint3 vTangent;
	position.x = vertex.x * weight;
	uTangent.x = vertex.x * uWeight;
	vTangent.x = vertex.x * vWeight;
	position.y = vertex.y * weight;
	uTangent.y = vertex.y * uWeight;
	vTangent.y = vertex.y * vWeight;
	position.z = vertex.z * weight;
	uTangent.z = vertex.z * uWeight;
	vTangent.z = vertex.z * vWeight;
	for ( i = 1; i < controlPointCount; i++ ) {
		index = unknownE4[i];
		vertex = vertices[index];
		weight = weights[i];
		uWeight = uDerivatives[i];
		vWeight = vDerivatives[i];
		position.x += vertex.x * weight;
		uTangent.x += vertex.x * uWeight;
		vTangent.x += vertex.x * vWeight;
		position.y += vertex.y * weight;
		uTangent.y += vertex.y * uWeight;
		vTangent.y += vertex.y * vWeight;
		position.z += vertex.z * weight;
		uTangent.z += vertex.z * uWeight;
		vTangent.z += vertex.z * vWeight;
	}
	normals->Add( uTangent.UnitCross( vTangent ) );
	return positions->Add( position );
}

/*
================
NiBezierPatch::Unknown8534B0

As Unknown851D90, through the index table unknownE4.
================
*/
// FUNCTION: STBC 0x8534B0
unsigned int NiBezierPatch::Unknown8534B0( float u, float v ) {
	int i;
	unsigned short set;

	unsigned short index = unknownE4[0];
	NiPoint3 vertex = vertices[index];
	float weight = weights[0];
	float uWeight = uDerivatives[0];
	float vWeight = vDerivatives[0];
	NiPoint3 position;
	NiPoint3 uTangent;
	NiPoint3 vTangent;
	position.x = vertex.x * weight;
	uTangent.x = vertex.x * uWeight;
	vTangent.x = vertex.x * vWeight;
	position.y = vertex.y * weight;
	uTangent.y = vertex.y * uWeight;
	vTangent.y = vertex.y * vWeight;
	position.z = vertex.z * weight;
	uTangent.z = vertex.z * uWeight;
	vTangent.z = vertex.z * vWeight;
	NiPoint2 *coords = new NiPoint2[textureSets];
	NiPoint2 texture;
	for ( set = 0; set < textureSets; set++ ) {
		texture = textures[set * unknownD6 + index];
		coords[set].x = texture.x * weight;
		coords[set].y = texture.y * weight;
	}
	for ( i = 1; i < controlPointCount; i++ ) {
		index = unknownE4[i];
		vertex = vertices[index];
		weight = weights[i];
		uWeight = uDerivatives[i];
		vWeight = vDerivatives[i];
		position.x += vertex.x * weight;
		uTangent.x += vertex.x * uWeight;
		vTangent.x += vertex.x * vWeight;
		position.y += vertex.y * weight;
		uTangent.y += vertex.y * uWeight;
		vTangent.y += vertex.y * vWeight;
		position.z += vertex.z * weight;
		uTangent.z += vertex.z * uWeight;
		vTangent.z += vertex.z * vWeight;
		for ( set = 0; set < textureSets; set++ ) {
			texture = textures[set * unknownD6 + index];
			coords[set].x += texture.x * weight;
			coords[set].y += texture.y * weight;
		}
	}
	for ( set = 0; set < textureSets; set++ ) {
		textureCoordinates->Add( set, coords[set] );
	}
	normals->Add( uTangent.UnitCross( vTangent ) );
	delete [] coords;
	return positions->Add( position );
}

/*
================
NiBezierPatch::Unknown853870

As Unknown852130, through the index table unknownE4.
================
*/
// FUNCTION: STBC 0x853870
// PARKED: x87 operand order of the colour sum's first component, as in Unknown851620
unsigned int NiBezierPatch::Unknown853870( float u, float v ) {
	int i;

	unsigned short index = unknownE4[0];
	NiPoint3 vertex = vertices[index];
	float weight = weights[0];
	float uWeight = uDerivatives[0];
	float vWeight = vDerivatives[0];
	NiColorA color = colors[index] * weight;
	NiPoint3 position;
	NiPoint3 uTangent;
	NiPoint3 vTangent;
	position.x = vertex.x * weight;
	uTangent.x = vertex.x * uWeight;
	vTangent.x = vertex.x * vWeight;
	position.y = vertex.y * weight;
	uTangent.y = vertex.y * uWeight;
	vTangent.y = vertex.y * vWeight;
	position.z = vertex.z * weight;
	uTangent.z = vertex.z * uWeight;
	vTangent.z = vertex.z * vWeight;
	for ( i = 1; i < controlPointCount; i++ ) {
		index = unknownE4[i];
		vertex = vertices[index];
		weight = weights[i];
		uWeight = uDerivatives[i];
		vWeight = vDerivatives[i];
		position.x += vertex.x * weight;
		uTangent.x += vertex.x * uWeight;
		vTangent.x += vertex.x * vWeight;
		position.y += vertex.y * weight;
		uTangent.y += vertex.y * uWeight;
		vTangent.y += vertex.y * vWeight;
		position.z += vertex.z * weight;
		uTangent.z += vertex.z * uWeight;
		vTangent.z += vertex.z * vWeight;
		color += weight * colors[index];
	}
	vertexColors->Add( color );
	normals->Add( uTangent.UnitCross( vTangent ) );
	return positions->Add( position );
}

/*
================
NiBezierPatch::Unknown853C20

As Unknown8524D0, through the index table unknownE4.
================
*/
// FUNCTION: STBC 0x853C20
// PARKED: x87 stray value in the colour sum's first component, as in Unknown8524D0
unsigned int NiBezierPatch::Unknown853C20( float u, float v ) {
	int i;
	unsigned short set;

	unsigned short index = unknownE4[0];
	NiPoint3 vertex = vertices[index];
	float weight = weights[0];
	float uWeight = uDerivatives[0];
	float vWeight = vDerivatives[0];
	NiColorA color = colors[index] * weight;
	NiPoint3 position;
	NiPoint3 uTangent;
	NiPoint3 vTangent;
	position.x = vertex.x * weight;
	uTangent.x = vertex.x * uWeight;
	vTangent.x = vertex.x * vWeight;
	position.y = vertex.y * weight;
	uTangent.y = vertex.y * uWeight;
	vTangent.y = vertex.y * vWeight;
	position.z = vertex.z * weight;
	uTangent.z = vertex.z * uWeight;
	vTangent.z = vertex.z * vWeight;
	NiPoint2 *coords = new NiPoint2[textureSets];
	NiPoint2 texture;
	for ( set = 0; set < textureSets; set++ ) {
		texture = textures[set * unknownD6 + index];
		coords[set].x = texture.x * weight;
		coords[set].y = texture.y * weight;
	}
	for ( i = 1; i < controlPointCount; i++ ) {
		index = unknownE4[i];
		vertex = vertices[index];
		weight = weights[i];
		uWeight = uDerivatives[i];
		vWeight = vDerivatives[i];
		position.x += vertex.x * weight;
		uTangent.x += vertex.x * uWeight;
		vTangent.x += vertex.x * vWeight;
		position.y += vertex.y * weight;
		uTangent.y += vertex.y * uWeight;
		vTangent.y += vertex.y * vWeight;
		position.z += vertex.z * weight;
		uTangent.z += vertex.z * uWeight;
		vTangent.z += vertex.z * vWeight;
		color += colors[index] * weight;
		for ( set = 0; set < textureSets; set++ ) {
			texture = textures[set * unknownD6 + index];
			coords[set].x += texture.x * weight;
			coords[set].y += texture.y * weight;
		}
	}
	for ( set = 0; set < textureSets; set++ ) {
		textureCoordinates->Add( set, coords[set] );
	}
	vertexColors->Add( color );
	normals->Add( uTangent.UnitCross( vTangent ) );
	delete [] coords;
	return positions->Add( position );
}
