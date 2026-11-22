// NiBezierPatch.cpp -- Bezier surface patches of an NiBezierMesh

#include "NiRTLib.h"
#include "NiStream.h"
#include "NiBezierPatch.h"
#include "NiTriShapeData.h"
#include "NiTriShapeDynamicData.h"
#include "NiColor.h"
#include "NiPoint2.h"
#include "NiCamera.h"
#include "renderer/NiRender.h"
#include "NiBezierCylinder.h"
#include "NiDynamicEffect.h"
#include "NiNode.h"
#include "NiPick.h"
#include "NiTriBasedGeom.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00834c00 _$E275
// FUNCTION: STBC 0x008321e0 _$E17
// FUNCTION: STBC 0x00832200 _$E15
// FUNCTION: STBC 0x00832210 _$E23
// FUNCTION: STBC 0x00832230 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00832320 _$E255
// FUNCTION: STBC 0x00832340 _$E252
// FUNCTION: STBC 0x008324b0 _$E260
// FUNCTION: STBC 0x008324d0 _$E258
// FUNCTION: STBC 0x00832170 _$E37
// FUNCTION: STBC 0x00832180 _$E41
// FUNCTION: STBC 0x00832190 _$E45
// FUNCTION: STBC 0x008321a0 _$E49
// FUNCTION: STBC 0x008321b0 _$E53
// FUNCTION: STBC 0x008321c0 _$E177
// FUNCTION: STBC 0x008321d0 _$E209
// FUNCTION: STBC 0x00832240 _$E119
// FUNCTION: STBC 0x00832250 _$E132
// FUNCTION: STBC 0x00832260 _$E214
// FUNCTION: STBC 0x00832270 _$E219
// FUNCTION: STBC 0x00832280 _$E223
// FUNCTION: STBC 0x00832290 _$E227
// FUNCTION: STBC 0x008322a0 _$E234
// FUNCTION: STBC 0x008322b0 _$E185
// FUNCTION: STBC 0x008322c0 _$E245
// FUNCTION: STBC 0x008322d0 _$E189
// FUNCTION: STBC 0x008322e0 _$E249
// FUNCTION: STBC 0x008322f0 _$E193
// FUNCTION: STBC 0x00832300 _$E197
// FUNCTION: STBC 0x00832310 _$E205

// DATA: STBC 0x9B2F98 ?rtti@NiBezierPatch@@2VNiRTTI@@B
const NiRTTI NiBezierPatch::rtti( "NiBezierPatch", &NiAVObject::rtti );

// the evaluator for each combination of SETS_ bits
// DATA: STBC 0x979904 ?evaluators@NiBezierPatch@@1PAP81@AEIMM@ZA
unsigned int ( NiBezierPatch::*NiBezierPatch::evaluators[16] )( float u, float v ) = {
	&NiBezierPatch::Unknown851330,
	&NiBezierPatch::Unknown851400,
	&NiBezierPatch::Unknown851620,
	&NiBezierPatch::Unknown851840,
	&NiBezierPatch::Unknown851B80,
	&NiBezierPatch::Unknown851D90,
	&NiBezierPatch::Unknown852130,
	&NiBezierPatch::Unknown8524D0,
	&NiBezierPatch::Unknown852980,
	&NiBezierPatch::Unknown852A80,
	&NiBezierPatch::Unknown852CD0,
	&NiBezierPatch::Unknown852F00,
	&NiBezierPatch::Unknown853260,
	&NiBezierPatch::Unknown8534B0,
	&NiBezierPatch::Unknown853870,
	&NiBezierPatch::Unknown853C20,
};

/*
================
NiBezierPatch::Register
================
*/
// FUNCTION: STBC 0x832360 ?Register@NiBezierPatch@@SAHXZ
NiImplementRegister( NiBezierPatch )

// DATA: STBC 0x9B2F80 ?vertexCache@@3VNiBezierVertexCacheOwner@@A
NiBezierVertexCacheOwner vertexCache;

/*
================
NiBezierPatch::NiBezierPatch

A patch on the mesh's arrays, which it does not own.
================
*/
// FUNCTION: STBC 0x8324F0 ??0NiBezierPatch@@QAE@GGPAVNiPoint3@@PAVNiPoint2@@PAVNiColorA@@_N@Z
NiBezierPatch::NiBezierPatch( unsigned short degree_, unsigned short controlPointCount_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08 ) {
	degree = degree_;
	controlPointCount = controlPointCount_;
	unknownD6 = controlPointCount_;
	unknownE4 = 0;
	vertices = vertices_;
	textures = textures_;
	if ( textures_ ) {
		textureSets = 1;
	} else {
		textureSets = 0;
	}
	colors = colors_;
	if ( unknown08 ) {
		flags |= UNKNOWN_08_MASK;
	} else {
		flags &= ~UNKNOWN_08_MASK;
	}
	SetUnknown10( 0 );
	level = 0;
	unknownF8 = 0.1f;
	unknownFC = 4.0f;
	weights = new float[controlPointCount];
	uDerivatives = new float[controlPointCount];
	vDerivatives = new float[controlPointCount];
	unknown128 = 0;
	positions = 0;
	normals = 0;
	textureCoordinates = 0;
	vertexColors = 0;
	triangles = 0;
	unknown11C = 0;
	worldPositions = 0;
	worldNormals = 0;
	worldSetsStale = false;
}

/*
================
NiBezierPatch::NiBezierPatch

A patch that owns unknownE4 (TODO identity).
================
*/
// FUNCTION: STBC 0x8326D0 ??0NiBezierPatch@@QAE@GGPAGPAVNiPoint3@@PAVNiPoint2@@PAVNiColorA@@_NG@Z
NiBezierPatch::NiBezierPatch( unsigned short degree_, unsigned short controlPointCount_, unsigned short *unknownE4_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08, unsigned short unknownD6_ ) {
	degree = degree_;
	controlPointCount = controlPointCount_;
	unknownD6 = unknownD6_;
	unknownE4 = unknownE4_;
	vertices = vertices_;
	textures = textures_;
	if ( textures_ ) {
		textureSets = 1;
	} else {
		textureSets = 0;
	}
	colors = colors_;
	if ( unknown08 ) {
		flags |= UNKNOWN_08_MASK;
	} else {
		flags &= ~UNKNOWN_08_MASK;
	}
	SetUnknown10( 0 );
	level = 0;
	unknownF8 = 0.1f;
	unknownFC = 4.0f;
	weights = new float[controlPointCount];
	uDerivatives = new float[controlPointCount];
	vDerivatives = new float[controlPointCount];
	unknown128 = 0;
	positions = 0;
	normals = 0;
	textureCoordinates = 0;
	vertexColors = 0;
	triangles = 0;
	unknown11C = 0;
	worldPositions = 0;
	worldNormals = 0;
	worldSetsStale = false;
}

/*
================
NiBezierPatch::NiBezierPatch
================
*/
// FUNCTION: STBC 0x832880 ??0NiBezierPatch@@QAE@XZ
NiBezierPatch::NiBezierPatch() {
	degree = 0;
	controlPointCount = 0;
	unknownD6 = 0;
	textureSets = 0;
	unknownE4 = 0;
	vertices = 0;
	textures = 0;
	colors = 0;
	weights = 0;
	uDerivatives = 0;
	vDerivatives = 0;
	flags &= ~UNKNOWN_08_MASK;
	SetUnknown10( 0 );
	level = 0;
	unknownF8 = 0.1f;
	unknownFC = 4.0f;
	vertexCount = 0;
	unknown128 = 0;
	positions = 0;
	normals = 0;
	textureCoordinates = 0;
	vertexColors = 0;
	triangleCount = 0;
	triangles = 0;
	unknown11C = 0;
	worldSetsStale = false;
	worldPositions = 0;
	worldNormals = 0;
}

/*
================
NiBezierPatch::~NiBezierPatch

The patch owns unknownE4 when it has one, else the control arrays; the
triangle data gives up the tessellated sets before it is released.
================
*/
// FUNCTION: STBC 0x8329C0
NiBezierPatch::~NiBezierPatch() {
	if ( unknownE4 ) {
		delete [] unknownE4;
	} else {
		delete [] vertices;
		delete [] textures;
		delete [] colors;
	}

	delete positions;
	delete normals;
	delete textureCoordinates;
	delete vertexColors;
	delete triangles;
	delete worldPositions;
	delete worldNormals;

	delete [] weights;
	delete [] uDerivatives;
	delete [] vDerivatives;

	if ( data ) {
		data->SetData( 0 );
		data->NiGeometryData::SetData( 0, 0, 0, 0, 0 );
	}
	data = 0;
}

/*
================
NiBezierPatch::SetArrays
================
*/
// FUNCTION: STBC 0x832BC0
void NiBezierPatch::SetArrays( NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_ ) {
	vertices = vertices_;
	textures = textures_;
	colors = colors_;
}

/*
================
NiBezierPatch::SetUnknown10
================
*/
// FUNCTION: STBC 0x832BF0
void NiBezierPatch::SetUnknown10( int unknown10 ) {
	if ( unknown10 != ( ( flags >> UNKNOWN_10_POS ) & 1 ) ) {
		flags = ( flags & ~UNKNOWN_10_MASK ) | ( unknown10 << UNKNOWN_10_POS );
		Unknown8330E0( level );
	}
}

/*
================
NiBezierPatch::CreateSets
================
*/
// FUNCTION: STBC 0x832C30
// The debug NetImmerse 3.1's statements (AnimOnly 0x4D21BE): the texture flag is or'ed in like the
// others.
void NiBezierPatch::CreateSets() {
	delete positions;
	delete normals;
	delete textureCoordinates;
	delete vertexColors;
	delete triangles;
	delete worldPositions;
	delete worldNormals;

	positions = new NiTSet<NiPoint3>( vertexCount, 2 );
	worldPositions = new NiTSet<NiPoint3>( vertexCount, 2 );

	unknown11C = 0;
	if ( textures ) {
		unknown11C |= SETS_TEXTURES;
		textureCoordinates = new NiBezierTextureSets( textureSets, vertexCount, 2 );
	} else {
		textureCoordinates = 0;
	}
	if ( colors ) {
		unknown11C |= SETS_COLORS;
		vertexColors = new NiTSet<NiColorA>( vertexCount, 2 );
	} else {
		vertexColors = 0;
	}
	if ( GetUnknown08() ) {
		unknown11C |= SETS_NORMALS;
		normals = new NiTSet<NiPoint3>( vertexCount, 2 );
		worldNormals = new NiTSet<NiPoint3>( vertexCount, 2 );
	} else {
		normals = 0;
		worldNormals = 0;
	}
	if ( unknownE4 ) {
		unknown11C |= SETS_UNKNOWN_E4;
	}
	unknown120 = evaluators[unknown11C];

	unsigned short indexCount = triangleCount * 3;
	triangles = new NiTSet<unsigned short>( indexCount, 2 );
}

/*
================
NiBezierPatch::VirtualSlot39
================
*/
// FUNCTION: STBC 0x8330A0
void NiBezierPatch::VirtualSlot39( unsigned char level_ ) {
	if ( level == level_ || level_ > VirtualSlot40() ) {
		return;
	}

	Unknown8330E0( level_ );
	level = level_;
}

/*
================
NiBezierPatch::VirtualSlot43
================
*/
// FUNCTION: STBC 0x8330D0
void NiBezierPatch::VirtualSlot43( float, float ) {
}

/*
================
NiBezierPatch::Unknown8330E0

Tessellates at the level by the tessellator the patch's flag picks, then
hands the sets to the triangle data: a new one on the first call, else the
arrays of the one the patch has.
================
*/
// FUNCTION: STBC 0x8330E0
void NiBezierPatch::Unknown8330E0( unsigned char level_ ) {
	if ( !( flags & UNKNOWN_10_MASK ) ) {
		VirtualSlot44( level_ );
	} else {
		VirtualSlot45( level_ );
	}

	if ( !data ) {
		NiPoint3 *newNormals = 0;
		NiColorA *newColors = 0;
		NiPoint2 *newTextures = 0;
		unsigned short *newTriangles = 0;
		unsigned int maxVertices = positions->GetMaxSize();
		NiPoint3 *newVertices = positions->GetBase();
		if ( normals ) {
			newNormals = normals->GetBase();
		}
		if ( vertexColors ) {
			newColors = vertexColors->GetBase();
		}
		if ( textureCoordinates ) {
			newTextures = textureCoordinates->GetBase();
		}
		if ( triangles ) {
			newTriangles = triangles->GetBase();
		}
		data = new NiTriShapeDynamicData( maxVertices, newVertices, newNormals, newColors, newTextures, triangleCount, newTriangles, vertexCount, triangleCount );
		data->SetTextureSets( textureSets );
	} else {
		NiPoint3 *newNormals = 0;
		NiColorA *newColors = 0;
		NiPoint2 *newTextures = 0;
		unsigned int maxVertices = positions->GetMaxSize();
		NiPoint3 *newVertices = positions->GetBase();
		if ( normals ) {
			newNormals = normals->GetBase();
		}
		if ( vertexColors ) {
			newColors = vertexColors->GetBase();
		}
		if ( textureCoordinates ) {
			newTextures = textureCoordinates->GetBase();
		}
		data->NiGeometryData::SetData( maxVertices, newVertices, newNormals, newColors, newTextures );
		data->SetTriangleCount( triangleCount );
		data->SetData( triangles ? triangles->GetBase() : 0 );
		data->SetActiveVertexCount( vertexCount );
		data->SetActiveTriangleCount( triangleCount );
		data->SetTextureSets( textureSets );
		data->IncRevisionID();
	}
}

/*
================
NiBezierPatch::VirtualSlot44
================
*/
// FUNCTION: STBC 0x8332F0
void NiBezierPatch::VirtualSlot44( unsigned char ) {
}

/*
================
NiBezierPatch::VirtualSlot45
================
*/
// FUNCTION: STBC 0x833300
void NiBezierPatch::VirtualSlot45( unsigned char ) {
}

/*
================
NiBezierPatch::VirtualSlot46
================
*/
// FUNCTION: STBC 0x833310
bool NiBezierPatch::VirtualSlot46( unsigned short, unsigned short, unsigned short, unsigned short ) {
	return false;
}

/*
================
NiBezierPatch::VirtualSlot42
================
*/
// FUNCTION: STBC 0x833320
bool NiBezierPatch::VirtualSlot42( unsigned short, unsigned short, unsigned char, unsigned char, unsigned char, short ) {
	return false;
}

/*
================
NiBezierPatch::GetWorldPositions

The tessellated positions in world space, brought up to date first.
================
*/
// FUNCTION: STBC 0x833330
NiPoint3 *NiBezierPatch::GetWorldPositions() {
	if ( worldSetsStale ) {
		UpdateWorldSets();
	}
	return worldPositions ? worldPositions->GetBase() : 0;
}

/*
================
NiBezierPatch::GetWorldNormals

The tessellated normals in world space, brought up to date first.
================
*/
// FUNCTION: STBC 0x833360
NiPoint3 *NiBezierPatch::GetWorldNormals() {
	if ( worldSetsStale ) {
		UpdateWorldSets();
	}
	return worldNormals ? worldNormals->GetBase() : 0;
}

/*
================
NiBezierPatch::UpdateWorldBound

The model bound carried into world space.
================
*/
// FUNCTION: STBC 0x833390
void NiBezierPatch::UpdateWorldBound() {
	assert( !modelBound.GetNeedUpdate() );
	worldBound.Update( modelBound, worldRotate, worldTranslate, worldScale );
}

/*
================
NiBezierPatch::UpdateWorldData
================
*/
// FUNCTION: STBC 0x8333C0
void NiBezierPatch::UpdateWorldData() {
	NiAVObject::UpdateWorldData();
	worldSetsStale = true;
}

/*
================
NiBezierPatch::VirtualSlot35

Passes the call on to the controllers, keeps the first camera, brings the
world sets up to date and hands the patch's properties and lights to the
camera's renderer.
================
*/
// FUNCTION: STBC 0x8333E0
void NiBezierPatch::VirtualSlot35( NiCamera *camera ) {
	NiAVObject::VirtualSlot35( camera );
	if ( !unknown128 ) {
		unknown128 = camera;
	}
	if ( worldSetsStale ) {
		UpdateWorldSets();
	}
	assert( propertyState );
	if ( propertyState ) {
		camera->GetRenderer()->SetPropertyState( propertyState );
	}
	camera->GetRenderer()->SetEffectState( effectState );
}

/*
================
NiBezierPatch::Display

Draws the tessellated triangle data unless the camera's sorter takes the
object it is given.
================
*/
// FUNCTION: STBC 0x833490
void NiBezierPatch::Display( NiCamera *camera ) {
	NiRender *renderer = camera->GetRenderer();
	assert( renderer );
	// BUG: registers the camera with the sorter, not the patch; it compiles
	// because RegisterObject takes any NiAVObject
	if ( !renderer->RegisterObject( camera ) ) {
		camera->UpdateActivePlanes();
		renderer->VirtualSlot42( GetWorldTranslate(), GetWorldRotate(), GetWorldScale() );
		renderer->VirtualSlot45( data, GetWorldBound() );
	}
}

/*
================
NiBezierPatch::GetPropertyState
================
*/
// FUNCTION: STBC 0x833500
NiPointer<NiPropertyState> NiBezierPatch::GetPropertyState() {
	return propertyState;
}

/*
================
NiBezierPatch::UpdatePropertiesDownward

Takes the parent's state with this object's own properties applied.
================
*/
// FUNCTION: STBC 0x833520
void NiBezierPatch::UpdatePropertiesDownward( NiPropertyState *parentState ) {
	propertyState = PushLocalProperties( parentState, true );
}

/*
================
NiBezierPatch::GetEffectState
================
*/
// FUNCTION: STBC 0x8335E0
NiPointer<NiDynamicEffectState> NiBezierPatch::GetEffectState() {
	return effectState;
}

/*
================
NiBezierPatch::UpdateEffectsDownward

Takes the parent's state, or the last state shared when it is equal, as
NiGeometry does.
================
*/
// FUNCTION: STBC 0x833600
void NiBezierPatch::UpdateEffectsDownward( NiDynamicEffectState *parentState ) {
	if ( parentState ) {
		NiDynamicEffectState *lastState = NiDynamicEffectState::GetCachedState();
		if ( parentState->Equal( lastState ) ) {
			effectState = lastState;
		} else {
			effectState = parentState;
			NiDynamicEffectState::SetCachedState( parentState );
		}
	} else {
		effectState = 0;
	}
}

/*
================
NiBezierPatch::VirtualSlot32
================
*/
// FUNCTION: STBC 0x8336F0
bool NiBezierPatch::TestCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 ) {
	assert( 0 );
	return false;
}

/*
================
NiBezierPatch::VirtualSlot33
================
*/
// FUNCTION: STBC 0x833700
int NiBezierPatch::FindCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 ) {
	assert( 0 );
	return 0;
}

/*
================
NiBezierPatch::FindIntersections

Records each tessellated triangle the ray hits, in model space, with the
interpolated texture coordinates, normal and color the pick asks for; a
bound pick records the patch itself.
================
*/
// FUNCTION: STBC 0x833710 ?FindIntersections@NiBezierPatch@@UAE_NABVNiPoint3@@0AAVNiPick@@@Z
// Gamebryo 1.2.2: FindIntersectionsTriBasedGeom; the statements are the
// debug copy's (AnimOnly.exe 0x4D321C)
bool NiBezierPatch::FindIntersections( const NiPoint3 &origin, const NiPoint3 &dir, NiPick &pick ) {
	if ( pick.GetObserveAppCullFlag() && GetAppCulled() ) {
		return false;
	}
	if ( !TestRayBound( origin, dir ) ) {
		return false;
	}

	if ( pick.GetIntersectType() == NiPick::BOUND_INTERSECT ) {
		NiPick::Record *record = new NiPick::Record( this );
		assert( record );
		pick.Add( record );
		return true;
	}

	float invScale = 1.0f / GetWorldScale();
	NiPoint3 offset = origin - GetWorldTranslate();	// (name invented)
	NiPoint3 modelOrigin = ( offset * GetWorldRotate() ) * invScale;
	NiPoint3 modelDir = ( dir * GetWorldRotate() ) * invScale;

	bool found = false;
	unsigned short index = 0;
	for ( unsigned short i = 0; i < triangleCount; i++ ) {
		unsigned short i0 = triangles->GetAt( index++ );
		unsigned short i1 = triangles->GetAt( index++ );
		unsigned short i2 = triangles->GetAt( index++ );

		NiPoint3 intersect;
		float distance;
		float u;
		float v;
		bool hit = NiTriBasedGeom::IntersectTriangle( modelOrigin, modelDir, positions->GetAt( i0 ), positions->GetAt( i1 ),
													  positions->GetAt( i2 ), pick.GetFrontOnly(), intersect, distance, u,
													  v );	// (name invented)
		if ( hit ) {
			found = true;
			NiPick::Record *record = new NiPick::Record( this );
			if ( pick.GetCoordinateType() == NiPick::WORLD_COORDINATES ) {
				intersect = GetWorldScale() * ( GetWorldRotate() * intersect ) + GetWorldTranslate();
			}
			record->SetIntersection( intersect );
			record->SetDistance( distance );
			record->SetTriangleIndex( i );
			record->SetVertexIndices( i0, i1, i2 );

			float w = 1.0f - ( u + v );
			if ( pick.GetReturnTexture() && textureCoordinates ) {
				NiPoint2 texture = w * textureCoordinates->GetAt( 0, i0 ) + u * textureCoordinates->GetAt( 0, i1 ) +
								   v * textureCoordinates->GetAt( 0, i2 );
				record->SetTexture( texture );
			} else {
				record->SetTexture( NiPoint2::ZERO );
			}

			if ( pick.GetReturnNormal() ) {
				NiPoint3 normal;
				if ( pick.GetReturnSmoothNormal() && normals ) {
					normal = w * normals->GetAt( i0 ) + u * normals->GetAt( i1 ) + v * normals->GetAt( i2 );
				} else {
					NiPoint3 edge0;
					NiPoint3 edge1;
					edge0 = positions->GetAt( i1 ) - positions->GetAt( i0 );
					edge1 = positions->GetAt( i2 ) - positions->GetAt( i1 );
					normal = edge0.Cross( edge1 );
				}
				normal.Unitize();
				if ( pick.GetCoordinateType() == NiPick::WORLD_COORDINATES ) {
					normal = GetWorldRotate() * normal;
				}
				record->SetNormal( normal );
			}

			if ( pick.GetReturnColor() && vertexColors ) {
				NiColorA color = w * vertexColors->GetAt( i0 ) + u * vertexColors->GetAt( i1 ) + v * vertexColors->GetAt( i2 );
				record->SetColor( color );
			} else {
				record->SetColor( NiColorA::WHITE );
			}
			pick.Add( record );
		}
	}
	return found;
}

/*
================
NiBezierPatch::CreateClone

The base patch has no clone of its own; the derived classes create theirs
and call CopyMembers.
================
*/
// FUNCTION: STBC 0x834300
NiObject *NiBezierPatch::CreateClone() {
	assert( false );
	return 0;
}

/*
================
NiBezierPatch::CopyMembers

The clone gets its own copies of the control arrays: of unknownE4 alone
when the patch owns it, else of the vertex, texture coordinate and color
arrays, and always of the three word arrays.
================
*/
// FUNCTION: STBC 0x834310
void NiBezierPatch::CopyMembers( NiBezierPatch *dest ) {
	NiAVObject::CopyMembers( dest );

	dest->degree = degree;
	dest->controlPointCount = controlPointCount;
	dest->textureSets = textureSets;
	dest->unknownD6 = unknownD6;
	dest->level = level;

	if ( unknownE4 ) {
		dest->unknownE4 = new unsigned short[controlPointCount];
		memcpy( dest->unknownE4, unknownE4, controlPointCount * sizeof( unsigned short ) );
	} else {
		dest->vertices = new NiPoint3[controlPointCount];
		memcpy( dest->vertices, vertices, controlPointCount * sizeof( NiPoint3 ) );
		if ( textures ) {
			dest->textures = new NiPoint2[controlPointCount];
			memcpy( dest->textures, textures, controlPointCount * sizeof( NiPoint2 ) );
		}
		if ( colors ) {
			dest->colors = new NiColorA[controlPointCount];
			memcpy( dest->colors, colors, controlPointCount * sizeof( NiColorA ) );
		}
	}

	dest->weights = new float[controlPointCount];
	memcpy( dest->weights, weights, controlPointCount * sizeof( float ) );
	dest->uDerivatives = new float[controlPointCount];
	memcpy( dest->uDerivatives, uDerivatives, controlPointCount * sizeof( float ) );
	dest->vDerivatives = new float[controlPointCount];
	memcpy( dest->vDerivatives, vDerivatives, controlPointCount * sizeof( float ) );
}

/*
================
NiBezierPatch::CreateObject

A patch is created only through its derived classes; the stream gets
nothing from this one.
================
*/
// FUNCTION: STBC 0x834570
void NiBezierPatch::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiBezierPatch::LoadBinary

The texture coordinates and colors are read as arrays of floats. The
pointers are read as presence flags: unknownE4 alone when it was
saved, else the vertex array and the optional texture coordinates and
colors. The three word arrays are allocated, not read.
================
*/
// FUNCTION: STBC 0x834580
void NiBezierPatch::LoadBinary( NiStream &stream ) {
	NiAVObject::LoadBinary( stream );

	NiStreamLoadBinary( stream, degree );
	NiStreamLoadBinary( stream, controlPointCount );
	NiStreamLoadBinary( stream, textureSets );
	NiStreamLoadBinary( stream, unknownD6 );
	NiStreamLoadBinary( stream, level );

	NiStreamLoadBinary( stream, unknownE4 );
	if ( unknownE4 ) {
		unknownE4 = new unsigned short[controlPointCount];
		NiStreamLoadBinary( stream, unknownE4, controlPointCount );
	} else {
		vertices = new NiPoint3[controlPointCount];
		NiStreamLoadBinary( stream, &vertices->x, controlPointCount * 3 );

		NiStreamLoadBinary( stream, textures );
		if ( textures ) {
			textureSets = 1;
			textures = new NiPoint2[controlPointCount];
			NiStreamLoadBinary( stream, &textures->x, controlPointCount * 2 );
		}

		NiStreamLoadBinary( stream, colors );
		if ( colors ) {
			colors = new NiColorA[controlPointCount];
			NiStreamLoadBinary( stream, &colors->r, controlPointCount * 4 );
		}
	}

	weights = new float[controlPointCount];
	uDerivatives = new float[controlPointCount];
	vDerivatives = new float[controlPointCount];
}

/*
================
NiBezierPatch::LinkObject
================
*/
// FUNCTION: STBC 0x834780
void NiBezierPatch::LinkObject( NiStream &stream ) {
	NiAVObject::LinkObject( stream );
}

/*
================
NiBezierPatch::RegisterStreamables
================
*/
// FUNCTION: STBC 0x834790
bool NiBezierPatch::RegisterStreamables( NiStream &stream ) {
	return NiAVObject::RegisterStreamables( stream );
}

/*
================
NiBezierPatch::SaveBinary

The pointers follow as presence flags, each array after its own.
================
*/
// FUNCTION: STBC 0x8347A0
void NiBezierPatch::SaveBinary( NiStream &stream ) {
	NiAVObject::SaveBinary( stream );

	NiStreamSaveBinary( stream, degree );
	NiStreamSaveBinary( stream, controlPointCount );
	NiStreamSaveBinary( stream, textureSets );
	NiStreamSaveBinary( stream, unknownD6 );
	NiStreamSaveBinary( stream, level );

	NiStreamSaveBinary( stream, unknownE4 );
	if ( unknownE4 ) {
		NiStreamSaveBinary( stream, unknownE4, controlPointCount );
		return;
	}

	NiStreamSaveBinary( stream, (float *)vertices, controlPointCount * 3 );

	NiStreamSaveBinary( stream, textures );
	if ( textures ) {
		NiStreamSaveBinary( stream, (float *)textures, controlPointCount * 2 );
	}

	NiStreamSaveBinary( stream, colors );
	if ( colors ) {
		NiStreamSaveBinary( stream, (float *)colors, controlPointCount * 4 );
	}
}

/*
================
NiBezierPatch::IsEqual

When the patches own unknownE4, only it is compared; otherwise the control
vertices and the optional texture coordinates and colors.
================
*/
// FUNCTION: STBC 0x8348B0
bool NiBezierPatch::IsEqual( NiObject *object ) {
	if ( !NiAVObject::IsEqual( object ) ) {
		return false;
	}

	NiBezierPatch *patch = (NiBezierPatch *)object;
	if ( degree != patch->degree
		|| ( flags & UNKNOWN_10_MASK ) != ( patch->flags & UNKNOWN_10_MASK )
		|| controlPointCount != patch->controlPointCount
		|| GetUnknown08() != patch->GetUnknown08()
		|| textureSets != patch->textureSets
		|| unknownD6 != patch->unknownD6
		|| level != patch->level ) {
		return false;
	}

	if ( ( unknownE4 && !patch->unknownE4 ) || ( !unknownE4 && patch->unknownE4 ) ) {
		return false;
	}

	unsigned short i;
	if ( unknownE4 ) {
		for ( i = 0; i < controlPointCount; i++ ) {
			if ( unknownE4[i] != patch->unknownE4[i] ) {
				return false;
			}
		}
		return true;
	}

	if ( ( textures && !patch->textures ) || ( !textures && patch->textures ) ) {
		return false;
	}
	if ( ( colors && !patch->colors ) || ( !colors && patch->colors ) ) {
		return false;
	}

	for ( i = 0; i < controlPointCount; i++ ) {
		if ( vertices[i] != patch->vertices[i] ) {
			return false;
		}
	}
	if ( textures ) {
		for ( i = 0; i < controlPointCount; i++ ) {
			if ( textures[i] != patch->textures[i] ) {
				return false;
			}
		}
	}
	if ( colors ) {
		for ( i = 0; i < controlPointCount; i++ ) {
			if ( colors[i] != patch->colors[i] ) {
				return false;
			}
		}
	}
	return true;
}
