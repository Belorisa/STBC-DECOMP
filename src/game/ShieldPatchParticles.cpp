/*
===============================================================================

	ShieldPatchParticles.cpp

	The shield hit effect.

===============================================================================
*/

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "ShieldPatchParticles.h"
#include "GlowPass.h"
#include "NiDynamicEffect.h"
#include "NiMaterialProperty.h"
#include "NiMath.h"
#include "NiMatrix3.h"
#include "TGStream.h"
#include "NiAlphaProperty.h"
#include "NiPointer.h"
#include "NiTextureModeProperty.h"
#include "NiTextureProperty.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004ec6e0 _$E45
// FUNCTION: STBC 0x004ec6f0 _$E44
// FUNCTION: STBC 0x004ec700 _$E56
// FUNCTION: STBC 0x004ec710 _$E55
// FUNCTION: STBC 0x004ec720 _$E60
// FUNCTION: STBC 0x004ec730 _$E59
// FUNCTION: STBC 0x004ec770 _$E64
// FUNCTION: STBC 0x004ec780 _$E63
// FUNCTION: STBC 0x004ec790 _$E68
// FUNCTION: STBC 0x004ec7a0 _$E67
// FUNCTION: STBC 0x004ec7b0 _$E192
// FUNCTION: STBC 0x004ec7c0 _$E191
// FUNCTION: STBC 0x004ec7d0 _$E197
// FUNCTION: STBC 0x004ec7e0 _$E196
// FUNCTION: STBC 0x004ec7f0 _$E201
// FUNCTION: STBC 0x004ec800 _$E200
// FUNCTION: STBC 0x004ec810 _$E209
// FUNCTION: STBC 0x004ec820 _$E208
// FUNCTION: STBC 0x004ec830 _$E216
// FUNCTION: STBC 0x004ec840 _$E215
// FUNCTION: STBC 0x004ec850 _$E220
// FUNCTION: STBC 0x004ec860 _$E219
// FUNCTION: STBC 0x004ec870 _$E224
// FUNCTION: STBC 0x004ec880 _$E223
// FUNCTION: STBC 0x004ec890 _$E228
// FUNCTION: STBC 0x004ec8a0 _$E227
// FUNCTION: STBC 0x004ec8b0 _$E234
// FUNCTION: STBC 0x004ec8c0 _$E233
// FUNCTION: STBC 0x004ec8e0 _$E177
// FUNCTION: STBC 0x004ec8f0 _$E176
// FUNCTION: STBC 0x004ec900 _$E92
// FUNCTION: STBC 0x004ec910 _$E91
// FUNCTION: STBC 0x004ec920 _$E117
// FUNCTION: STBC 0x004ec930 _$E116
// FUNCTION: STBC 0x004ec940 _$E134
// FUNCTION: STBC 0x004ec950 _$E133
// FUNCTION: STBC 0x004ec960 _$E147
// FUNCTION: STBC 0x004ec970 _$E146
// FUNCTION: STBC 0x004ec980 _$E167
// FUNCTION: STBC 0x004ec990 _$E166
// FUNCTION: STBC 0x004ec9a0 _$E122
// FUNCTION: STBC 0x004ec9b0 _$E121
// FUNCTION: STBC 0x004ec9c0 _$E187
// FUNCTION: STBC 0x004ec9d0 _$E186

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004ec660 _$E32
// FUNCTION: STBC 0x004ec670 _$E29
// FUNCTION: STBC 0x004ec680 _$E31
// FUNCTION: STBC 0x004ec690 _$E30
// FUNCTION: STBC 0x004ec6a0 _$E38
// FUNCTION: STBC 0x004ec6b0 _$E35
// FUNCTION: STBC 0x004ec6c0 _$E37
// FUNCTION: STBC 0x004ec6d0 _$E36

// The guard of std::ctype<unsigned short>::id and its exit-time thunk (<iostream>).
// FUNCTION: STBC 0x004ec740 _$E259
// FUNCTION: STBC 0x004ec760 _$E258

// how long a patch stays lit, in seconds
// DATA: STBC 0x88D658
static const float PATCH_LIFE = 3.0f;

// The RTTI's dynamic initialiser, its construction, the registration of its
// destructor and the guarded destructor.
// FUNCTION: STBC 0x004e9c00 _$E242
// FUNCTION: STBC 0x004e9c10 _$E238
// FUNCTION: STBC 0x004e9c30 _$E241
// FUNCTION: STBC 0x004e9c40 _$E239
// DATA: STBC 0x9865C8 ?rtti@PatchInstance@ShieldPatchParticles@@2VNiRTTI@@B
const NiRTTI ShieldPatchParticles::PatchInstance::rtti( "ShieldPatchParticles::PatchInstance", &NiTriShape::rtti );

/*
================
ShieldPatchParticles::ShieldPatchParticles

Draws additive-looking patches unlit over the bubble: vertex colours as
emissive, alpha blended, the image modulated, no depth write.
================
*/
// FUNCTION: STBC 0x004e9c60 ??0ShieldPatchParticles@@QAE@ABVNiColorA@@ABVNiPoint3@@1PAVNiImage@@@Z
ShieldPatchParticles::ShieldPatchParticles( const NiColorA &color_, const NiPoint3 &extent_, const NiPoint3 &center, NiImage *image ) : extent( extent_ ), color( color_ ) {
	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	AttachProperty( vertexColor );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_INVSRCALPHA );
	AttachProperty( alpha );

	NiPointer<NiTextureProperty> texture = new NiTextureProperty;
	texture->SetImage( image );
	AttachProperty( texture );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	textureMode->SetFilterMode( NiTextureModeProperty::FILTER_BILERP );
	textureMode->SetClampMode( NiTextureModeProperty::CLAMP_S_CLAMP_T );
	AttachProperty( textureMode );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( false );
	AttachProperty( zBuffer );

	SetTranslate( center );
}

/*
================
ShieldPatchParticles::Unknown4E9FB0

Lights the patch that faces the direction, or a new one.
================
*/
// FUNCTION: STBC 0x004e9fb0
void ShieldPatchParticles::Unknown4E9FB0( float time, NiPoint3 direction, int size, float scale ) {
	direction.Unitize();

	PatchInstance *patch = NULL;
	for ( unsigned int i = 0; i < GetArrayCount(); i++ ) {
		PatchInstance *child = (PatchInstance *)GetAt( i );
		if ( child->direction.Dot( direction ) > 0.9f ) {
			patch = child;
			break;
		}
	}
	if ( patch == NULL ) {
		patch = new PatchInstance( extent, color );
		AttachChild( patch, true );
		patch->UpdateProperties();
	}
	patch->Show( time, direction, size, scale );
}

/*
================
ShieldPatchParticles::AttachChild
================
*/
// FUNCTION: STBC 0x004ea150
void ShieldPatchParticles::AttachChild( NiAVObject *child, bool firstAvail ) {
	NiNode::AttachChild( child, firstAvail );
}

/*
================
ShieldPatchParticles::Unknown4EA170

Hides the patches that face the direction.
================
*/
// FUNCTION: STBC 0x004ea170
void ShieldPatchParticles::Unknown4EA170( const NiPoint3 &direction ) {
	for ( unsigned int i = 0; i < GetArrayCount(); i++ ) {
		if ( ( (PatchInstance *)GetAt( i ) )->direction.Dot( direction ) > 0.9f ) {
			( (PatchInstance *)GetAt( i ) )->Hide();
		}
	}
}

/*
================
ShieldPatchParticles::PatchInstance::PatchInstance

The patch glows: its GlowPass draws it into the glow half transparent.
================
*/
// FUNCTION: STBC 0x004ea1e0
ShieldPatchParticles::PatchInstance::PatchInstance( const NiPoint3 &extent_, const NiColorA &color_ ) : extent( extent_ ), color( color_ ), size( 0 ), scale( 0.0f ), baseVertices( NULL ) {
	GlowPass *glowPass = new GlowPass;
	glowPass->unknown14 = false;
	AddExtraData( glowPass );

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetAlpha( 0.5f );
	glowPass->properties.AddHead( material );
}

/*
================
ShieldPatchParticles::PatchInstance::~PatchInstance
================
*/
// FUNCTION: STBC 0x004ea310
ShieldPatchParticles::PatchInstance::~PatchInstance() {
	delete [] baseVertices;
}

/*
================
ShieldPatchParticles::PatchInstance::Hide
================
*/
// FUNCTION: STBC 0x004ea340
void ShieldPatchParticles::PatchInstance::Hide() {
	SetAppCulled( true );
}

/*
================
ShieldPatchParticles::PatchInstance::Show

Rebuilds the unit patch only when the size or the scale changes.
================
*/
// FUNCTION: STBC 0x004ea350
void ShieldPatchParticles::PatchInstance::Show( float time, const NiPoint3 &direction_, unsigned int size_, float scale_ ) {
	startTime = time;
	direction = direction_;
	SetAppCulled( false );
	if ( size_ != size || scale != scale_ ) {
		size = size_;
		scale = scale_;
		BuildGeometry();
	}
	Orient();
	Update( time );
}

/*
================
ShieldPatchParticles::PatchInstance::UpdateDownwardPass

Flickers the vertex alphas, fading out over the patch's life.
================
*/
// FUNCTION: STBC 0x004ea3e0
void ShieldPatchParticles::PatchInstance::UpdateDownwardPass( float time ) {
	float age = time - startTime;
	if ( age < PATCH_LIFE ) {
		NiTriShapeData *data = GetShapeData();
		int count = data->GetVertexCount();
		NiColorA *colors = data->GetColors();
		float alpha = ( 1.0f - age / PATCH_LIFE ) * color.a;
		for ( int i = 0; i < count; i++ ) {
			colors[i].a = rand() * alpha * ( 1.0f / RAND_MAX );
		}
		GetShapeData()->IncRevisionID();
	} else {
		Hide();
	}
	NiAVObject::UpdateDownwardPass( time );
}

/*
================
ShieldPatchParticles::PatchInstance::Orient

Maps the unit patch, built around +Z, onto the box: Z to the direction,
then each axis scaled by the extent.
================
*/
// FUNCTION: STBC 0x004ea4a0
void ShieldPatchParticles::PatchInstance::Orient() {
	NiTriShapeData *data = GetShapeData();
	memcpy( data->GetVertices(), baseVertices, data->GetVertexCount() * sizeof( NiPoint3 ) );

	NiPoint3 forward = direction;
	NiPoint3 right = forward.Perpendicular();
	NiPoint3 up = right.Cross( forward );
	NiMatrix3 rotation;
	rotation.SetCol( 0, right );
	rotation.SetCol( 1, forward );
	rotation.SetCol( 2, up );

	NiMatrix3 scaling;
	scaling.MakeDiagonal( extent.x, extent.y, extent.z );
	NiMatrix3 transform = scaling * rotation;
	ApplyTransform( transform, NiPoint3::ZERO, false );
	GetShapeData()->IncRevisionID();
}

/*
================
ShieldPatchParticles::PatchInstance::GetGridSize

The patch is a square grid with two triangles per cell.
================
*/
// FUNCTION: STBC 0x004ea650
void ShieldPatchParticles::PatchInstance::GetGridSize( int &columns, int &rows ) {
	columns = (int)( sqrtf( size >> 1 ) + 1.0f );
	rows = columns;
}

/*
================
ShieldPatchParticles::PatchInstance::BuildGeometry

A grid over the part of the unit sphere around +Z that the scale gives,
each cell drawn from both sides: the second half of the triangle list
holds the back faces.
================
*/
// FUNCTION: STBC 0x004ea690
// PARKED: register/frame: arithmetic, calls and loops agree; retail keeps this in EBP, triList in a register and one temporary in column's slot (frame 0x4C against 0x50), where this build spills an extra value
void ShieldPatchParticles::PatchInstance::BuildGeometry() {
	int columns;
	int rows;
	GetGridSize( columns, rows );
	int vertexCount = columns * rows;
	int triangleCount = ( columns * 2 - 2 ) * ( rows - 1 );

	NiPoint3 *vertices = new NiPoint3[vertexCount];
	NiPoint2 *textures = new NiPoint2[vertexCount];
	NiColorA *colors = new NiColorA[vertexCount];
	unsigned short *triList = new unsigned short[triangleCount * 6];
	int backFaces = triangleCount * 3;

	float arc = NI_PI * scale;
	float pitch = NI_HALF_PI - arc * 0.5f;
	float yawStep = NI_TWO_PI * ( scale * 0.5f ) / ( columns - 1 );
	float pitchStep = arc / ( rows - 1 );
	int index = 0;
	for ( int row = 0; row < rows; row++ ) {
		float yaw = -( NI_TWO_PI * ( scale * 0.5f ) * 0.5f );
		float v = (float)row / ( rows - 1 );
		float cosPitch = cos( pitch );
		float sinPitch = sin( pitch );
		for ( int column = 0; column < columns; column++ ) {
			float sinYaw = sin( yaw );
			float cosYaw = cos( yaw );
			int vertex = row * columns + column;
			vertices[vertex].z = cosPitch;
			vertices[vertex].x = sinYaw * sinPitch;
			vertices[vertex].y = cosYaw * sinPitch;
			textures[vertex].x = (float)column / ( columns - 1 );
			textures[vertex].y = v;
			colors[vertex] = color;
			if ( row > 0 && column > 0 ) {
				triList[index] = vertex;
				triList[index + 1] = vertex - columns;
				triList[index + 2] = vertex - 1;
				triList[index + 3] = vertex - columns;
				triList[index + 4] = vertex - columns - 1;
				triList[index + 5] = vertex - 1;

				triList[backFaces + index] = vertex;
				triList[backFaces + index + 1] = vertex - 1;
				triList[backFaces + index + 2] = vertex - columns;
				triList[backFaces + index + 3] = vertex - columns;
				triList[backFaces + index + 4] = vertex - 1;
				triList[backFaces + index + 5] = vertex - columns - 1;
				index += 6;
			}
			yaw += yawStep;
		}
		pitch += pitchStep;
	}

	delete [] baseVertices;
	baseVertices = new NiPoint3[vertexCount];
	memcpy( baseVertices, vertices, vertexCount * sizeof( NiPoint3 ) );

	if ( modelData != NULL ) {
		modelData->Replace( vertexCount, vertices, NULL, colors, textures );
		GetShapeData()->SetTriangleCount( triangleCount * 2 );
		GetShapeData()->Replace( triList );
	} else {
		SetModelData( new NiTriShapeData( vertexCount, vertices, NULL, colors, textures, triangleCount * 2, triList ) );
	}
}

/*
================
ShieldPatchParticles::PatchInstance::Save
================
*/
// FUNCTION: STBC 0x004eaaa0
bool ShieldPatchParticles::PatchInstance::Save( TGStream *stream ) {
	stream->WriteFloat( startTime );
	stream->WriteFloat( direction.x );
	stream->WriteFloat( direction.y );
	stream->WriteFloat( direction.z );
	stream->WriteFloat( scale );
	stream->WriteLong( size );
	return true;
}

/*
================
ShieldPatchParticles::PatchInstance::Load
================
*/
// FUNCTION: STBC 0x004eab10
bool ShieldPatchParticles::PatchInstance::Load( TGStream *stream ) {
	startTime = stream->ReadFloat();
	direction.x = stream->ReadFloat();
	direction.y = stream->ReadFloat();
	direction.z = stream->ReadFloat();
	scale = stream->ReadFloat();
	size = stream->ReadLong();
	BuildGeometry();
	Orient();
	return true;
}

/*
================
ShieldPatchParticles::Save

Writes the patches that are lit.
================
*/
// FUNCTION: STBC 0x004eab80
bool ShieldPatchParticles::Save( TGStream *stream ) {
	stream->WriteFloat( extent.x );
	stream->WriteFloat( extent.y );
	stream->WriteFloat( extent.z );
	stream->WriteFloat( color.r );
	stream->WriteFloat( color.g );
	stream->WriteFloat( color.b );
	stream->WriteFloat( color.a );

	int count = 0;
	unsigned int i;
	for ( i = 0; i < GetArrayCount(); i++ ) {
		if ( !GetAt( i )->GetAppCulled() ) {
			count++;
		}
	}
	stream->WriteLong( count );
	for ( i = 0; i < GetArrayCount(); i++ ) {
		if ( !GetAt( i )->GetAppCulled() ) {
			( (PatchInstance *)GetAt( i ) )->Save( stream );
		}
	}
	return true;
}

/*
================
ShieldPatchParticles::Load
================
*/
// FUNCTION: STBC 0x004eac70
bool ShieldPatchParticles::Load( TGStream *stream ) {
	extent.x = stream->ReadFloat();
	extent.y = stream->ReadFloat();
	extent.z = stream->ReadFloat();
	color.r = stream->ReadFloat();
	color.g = stream->ReadFloat();
	color.b = stream->ReadFloat();
	color.a = stream->ReadFloat();

	int count = stream->ReadLong();
	for ( int i = 0; i < count; i++ ) {
		PatchInstance *patch = new PatchInstance( extent, color );
		patch->Load( stream );
		AttachChild( patch, false );
	}
	return true;
}

/*
================
AtmosphereEffect::AtmosphereEffect

Half the count goes to the shock wave.
================
*/
// FUNCTION: STBC 0x004ead80
AtmosphereEffect::AtmosphereEffect( float time, float intensity, const NiPoint3 &extent, NiImage *shockWaveImage, NiImage *image, unsigned int count ) {
	shockWave = new AtmosphereShockWave( time, intensity, extent, shockWaveImage, 1, count >> 1 );
	AttachChild( shockWave->shape, false );

	streaks = new AtmosphereStreaks( time, intensity, extent, image, 1, 3, count );
	AttachChild( streaks->shape, false );
}

/*
================
AtmosphereEffect::~AtmosphereEffect
================
*/
// FUNCTION: STBC 0x004eae90
AtmosphereEffect::~AtmosphereEffect() {
	delete shockWave;
	delete streaks;
}

/*
================
AtmosphereEffect::UpdateDownwardPass
================
*/
// FUNCTION: STBC 0x004eaf40
void AtmosphereEffect::UpdateDownwardPass( float time ) {
	streaks->Update( time );
	shockWave->Update( time );
	NiNode::UpdateDownwardPass( time );
}

/*
================
AtmosphereEffect::Set

The shock wave is drawn a little larger than the streaks.
================
*/
// FUNCTION: STBC 0x004eaf70
void AtmosphereEffect::Set( float time, float intensity, const NiPoint3 &extent ) {
	streaks->Set( time, intensity, extent );

	NiPoint3 shockWaveExtent = extent;
	shockWaveExtent.x *= 1.1f;
	shockWaveExtent.y *= 1.3f;
	shockWaveExtent.z *= 1.3f;
	shockWave->Set( time, intensity, shockWaveExtent );
}

/*
================
AtmosphereShockWave::AtmosphereShockWave

Builds the cap's triangles, a fan around the tip and then a strip per
row, each from both sides: the second half of the list holds the back
faces. The shape is drawn added to the frame, unlit and without depth
writes.
================
*/
// FUNCTION: STBC 0x004eaff0
// PARKED: EH states (structure, set by the build): without /YX this unit treats SetBit/SetField as non-throwing, so the inline NiAlphaProperty, NiTextureModeProperty and NiZBufferProperty constructors lose retail's states 3, 7 and 9 (99.2 under /YX, 96.1 now); left besides: the spill homes of this and triangleCount swapped
AtmosphereShockWave::AtmosphereShockWave( float time_, float intensity_, const NiPoint3 &extent_, NiImage *image, unsigned int textureTurns_, unsigned int count ) {
	time = time_;
	intensity = intensity_;
	textureTurns = textureTurns_;
	extent = extent_;

	float cells = (float)( count >> 1 );
	int numColumns = (int)sqrt( cells * 2.0f );
	columns = numColumns;
	int numRows = (int)sqrt( cells * 0.5f );
	rows = numRows;

	int triangleCount = ( numColumns * 2 - 2 ) * ( numRows - 1 ) - numColumns + 1;
	unsigned short *triList = new unsigned short[triangleCount * 6];
	int index = 0;
	int backFaces = triangleCount * 3;
	int vertex = 1;
	for ( int row = 1; row < numRows; row++ ) {
		for ( int column = 0; column < numColumns; column++ ) {
			if ( row > 0 && column > 0 ) {
				if ( row == 1 ) {
					triList[index] = vertex;
					triList[index + 1] = 0;
					triList[index + 2] = vertex - 1;

					triList[backFaces + index] = vertex;
					triList[backFaces + index + 1] = vertex - 1;
					triList[backFaces + index + 2] = 0;
					index += 3;
				} else {
					int above = vertex - numColumns;
					triList[index] = vertex;
					triList[index + 1] = above;
					triList[index + 2] = vertex - 1;
					triList[index + 3] = above;
					triList[index + 4] = above - 1;
					triList[index + 5] = vertex - 1;

					triList[backFaces + index] = vertex;
					triList[backFaces + index + 1] = vertex - 1;
					triList[backFaces + index + 2] = above;
					triList[backFaces + index + 3] = above;
					triList[backFaces + index + 4] = vertex - 1;
					triList[backFaces + index + 5] = above - 1;
					index += 6;
				}
			}
			vertex++;
		}
	}

	int vertexCount = ( numRows - 1 ) * numColumns + 1;
	NiPoint3 *vertices = new NiPoint3[vertexCount];
	NiPoint2 *textures = new NiPoint2[vertexCount];
	NiColorA *colors = new NiColorA[vertexCount];
	float random = rand() * ( 1.0f / RAND_MAX );
	Fill( GetScale(), random * NI_TWO_PI, GetColor(), vertices, colors, textures );

	shape = new NiTriShape( vertexCount, vertices, NULL, colors, textures, triangleCount * 2, triList );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );
	shape->AttachProperty( alpha );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	shape->AttachProperty( vertexColor );

	NiTextureProperty *texture = new NiTextureProperty;
	texture->SetImage( image );
	shape->AttachProperty( texture );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	textureMode->SetFilterMode( NiTextureModeProperty::FILTER_BILERP );
	textureMode->SetClampMode( NiTextureModeProperty::CLAMP_S_CLAMP_T );
	shape->AttachProperty( textureMode );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( false );
	shape->AttachProperty( zBuffer );
}

/*
================
AtmosphereShockWave::GetScale
================
*/
// FUNCTION: STBC 0x004eb650
float AtmosphereShockWave::GetScale() {
	return intensity * 0.5f + 0.25f;
}

/*
================
AtmosphereShockWave::GetColor

White, opaque from half the intensity up.
================
*/
// FUNCTION: STBC 0x004eb660
NiColorA AtmosphereShockWave::GetColor() {
	float alpha = intensity * 2.0f;
	if ( alpha > 1.0f ) {
		alpha = 1.0f;
	}
	return NiColorA( 1.0f, 1.0f, 1.0f, alpha );
}

/*
================
AtmosphereShockWave::Fill

A cap around +X: the tip, then a ring of vertices per row. The texture
is mapped in circles around its middle, turned by the angle.
================
*/
// FUNCTION: STBC 0x004eb6a0
void AtmosphereShockWave::Fill( float scale, float angle, const NiColorA &color, NiPoint3 *vertices, NiColorA *colors, NiPoint2 *textures ) {
	int numColumns = columns;
	int numRows = rows;
	unsigned int turns = textureTurns;
	float yawStep = NI_TWO_PI / ( numColumns - 1 );
	float textureStep = turns * NI_TWO_PI / ( numColumns - 1 );
	float pitch = 0.0f;
	float pitchStep = NI_PI * scale / ( numRows - 1 );
	float radius = 0.0f;
	float radiusStep = 1.0f / ( numRows - 1 );

	vertices[0] = extent.x * NiPoint3::UNIT_X;
	textures[0] = NiPoint2( 0.5f, 0.5f );
	colors[0] = color;

	int index = 1;
	for ( int row = 1; row < numRows; row++ ) {
		float yaw = 0.0f;
		float textureAngle = angle;
		float cosPitch = cosf( pitch );
		float sinPitch = sinf( pitch );
		pitch += pitchStep;
		radius += radiusStep;
		for ( int column = 0; column < numColumns; column++ ) {
			vertices[index] = NiPoint3( cosPitch * extent.x, extent.y * ( cos( yaw ) * sinPitch ), extent.z * ( sin( yaw ) * sinPitch ) );
			textures[index] = NiPoint2( ( sinf( textureAngle ) + 0.5f ) * radius, ( cosf( textureAngle ) + 0.5f ) * radius );
			colors[index] = color;
			index++;
			yaw += yawStep;
			textureAngle += textureStep;
		}
	}
}

/*
================
AtmosphereShockWave::Update

Refills the shape turned by a random angle.
================
*/
// FUNCTION: STBC 0x004eb8f0
void AtmosphereShockWave::Update( float time_ ) {
	NiTriShapeData *data = shape->GetShapeData();
	NiPoint2 *textures = data->GetTextures();
	NiColorA *colors = data->GetColors();
	NiPoint3 *vertices = data->GetVertices();
	float random = rand() * ( 1.0f / RAND_MAX );
	Fill( GetScale(), NI_TWO_PI * random, GetColor(), vertices, colors, textures );
	shape->GetShapeData()->IncRevisionID();
}

/*
================
AtmosphereShockWave::Set
================
*/
// FUNCTION: STBC 0x004eb980
void AtmosphereShockWave::Set( float time_, float intensity_, const NiPoint3 &extent_ ) {
	time = time_;
	intensity = intensity_;
	extent = extent_;
}

/*
================
AtmosphereStreaks::AtmosphereStreaks

The shock wave's cap with the tail's rows of cells behind it, each
triangle from both sides. The texture wraps around the rings.
================
*/
// FUNCTION: STBC 0x004eb9b0
// DRAFT: retail computes numTailRows before the column term and swaps the two dead parameter homes it reuses for the strip pointers and the random number (frame and register picks)
AtmosphereStreaks::AtmosphereStreaks( float time_, float intensity_, const NiPoint3 &extent_, NiImage *image, unsigned int textureTurns_, int tailRows_, unsigned int count ) {
	time = time_;
	intensity = intensity_;
	textureTurns = textureTurns_;
	tailRows = tailRows_;
	extent = extent_;

	float cells = (float)( count / 3 );
	int numColumns = (int)sqrt( cells * 1.5f );
	columns = numColumns;
	int numRows = (int)sqrt( cells * ( 2.0f / 3.0f ) );
	int numTailRows = tailRows_ + 1;
	rows = numRows;

	int triangleCount = ( numRows + numTailRows - 2 ) * ( numColumns * 2 - 2 ) + numColumns - 1;
	unsigned short *triList = new unsigned short[triangleCount * 6];
	int index = 0;
	int backFaces = triangleCount * 3;
	int vertex = 1;
	int column;
	for ( int row = 1; row < numRows; row++ ) {
		for ( column = 0; column < numColumns; column++ ) {
			if ( row > 0 && column > 0 ) {
				if ( row == 1 ) {
					triList[index] = vertex;
					triList[index + 1] = 0;
					triList[index + 2] = vertex - 1;

					triList[backFaces + index] = vertex;
					triList[backFaces + index + 1] = vertex - 1;
					triList[backFaces + index + 2] = 0;
					index += 3;
				} else {
					int above = vertex - numColumns;
					triList[index] = vertex;
					triList[index + 1] = above;
					triList[index + 2] = vertex - 1;
					triList[index + 3] = above;
					triList[index + 4] = above - 1;
					triList[index + 5] = vertex - 1;

					triList[backFaces + index] = vertex;
					triList[backFaces + index + 1] = vertex - 1;
					triList[backFaces + index + 2] = above;
					triList[backFaces + index + 3] = above;
					triList[backFaces + index + 4] = vertex - 1;
					triList[backFaces + index + 5] = above - 1;
					index += 6;
				}
			}
			vertex++;
		}
	}
	for ( int tailRow = 0; tailRow < numTailRows; tailRow++ ) {
		for ( column = 0; column < numColumns; column++ ) {
			if ( column > 0 ) {
				int above = vertex - numColumns;
				triList[index] = vertex;
				triList[index + 1] = above;
				triList[index + 2] = vertex - 1;
				triList[index + 3] = above;
				triList[index + 4] = above - 1;
				triList[index + 5] = vertex - 1;

				triList[backFaces + index] = vertex;
				triList[backFaces + index + 1] = vertex - 1;
				triList[backFaces + index + 2] = above;
				triList[backFaces + index + 3] = above;
				triList[backFaces + index + 4] = vertex - 1;
				triList[backFaces + index + 5] = above - 1;
				index += 6;
			}
			vertex++;
		}
	}

	int vertexCount = ( numRows + numTailRows - 1 ) * numColumns + 1;
	NiPoint3 *vertices = new NiPoint3[vertexCount];
	NiPoint2 *textures = new NiPoint2[vertexCount];
	NiColorA *colors = new NiColorA[vertexCount];
	float random = rand() * ( 1.0f / RAND_MAX );
	Fill( GetScale(), random * NI_TWO_PI, GetLength(), GetColor(), vertices, colors, textures );

	shape = new NiTriShape( vertexCount, vertices, NULL, colors, textures, triangleCount * 2, triList );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );
	shape->AttachProperty( alpha );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	shape->AttachProperty( vertexColor );

	NiTextureProperty *texture = new NiTextureProperty;
	texture->SetImage( image );
	shape->AttachProperty( texture );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	textureMode->SetFilterMode( NiTextureModeProperty::FILTER_BILERP );
	textureMode->SetClampMode( NiTextureModeProperty::WRAP_S_CLAMP_T );
	shape->AttachProperty( textureMode );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( false );
	shape->AttachProperty( zBuffer );
}

/*
================
AtmosphereStreaks::Fill

The same cap as the shock wave's, with a tail of rings behind it that
runs back along X by the length and widens a little. The texture goes
around the rings and down the rows.
================
*/
// FUNCTION: STBC 0x004ec0d0
void AtmosphereStreaks::Fill( float scale, float angle, float length, const NiColorA &color, NiPoint3 *vertices, NiColorA *colors, NiPoint2 *textures ) {
	int numColumns = columns;
	int numRows = rows;
	int numTailRows = tailRows + 1;
	float columnSpan = (float)( numColumns - 1 );
	float yawStep = NI_TWO_PI / columnSpan;
	float pitch = 0.0f;
	float pitchStep = NI_PI * scale / ( numRows - 1 );
	float v = 0.0f;
	float vStep = 1.0f / ( numRows + numTailRows - 1 );
	float turns = (float)textureTurns;

	vertices[0] = NiPoint3::UNIT_X * extent.x;
	textures[0] = NiPoint2( 0.5f, 0.0f );
	colors[0] = color;

	int index = 1;
	int column;
	float yaw;
	float x;
	for ( int row = 1; row < numRows; row++ ) {
		pitch += pitchStep;
		v += vStep;
		yaw = 0.0f;
		x = cos( pitch );
		float sinPitch = sin( pitch );
		for ( column = 0; column < numColumns; column++ ) {
			vertices[index] = NiPoint3( x * extent.x, cos( yaw ) * extent.y * sinPitch, sin( yaw ) * extent.z * sinPitch );
			textures[index] = NiPoint2( column / columnSpan * turns + angle, v );
			colors[index] = color;
			index++;
			yaw += yawStep;
		}
	}

	float xStep = -( length / numTailRows );
	float radius = sinf( pitch ) * 0.8f;
	float radiusStep = radius * 0.2f / numTailRows;
	for ( int tailRow = 0; tailRow < numTailRows; tailRow++ ) {
		yaw = 0.0f;
		v += vStep;
		x += xStep;
		for ( column = 0; column < numColumns; column++ ) {
			vertices[index] = NiPoint3( x * extent.x, cosf( yaw ) * extent.y * radius, sinf( yaw ) * extent.z * radius );
			textures[index] = NiPoint2( column / columnSpan * turns + angle, v );
			colors[index] = color;
			index++;
			yaw += yawStep;
		}
		radius += radiusStep;
	}
}

/*
================
AtmosphereStreaks::Set
================
*/
// FUNCTION: STBC 0x004ec490
void AtmosphereStreaks::Set( float time_, float intensity_, const NiPoint3 &extent_ ) {
	time = time_;
	intensity = intensity_;
	extent = extent_;

	NiTriShapeData *data = shape->GetShapeData();
	NiPoint2 *textures = data->GetTextures();
	NiColorA *colors = data->GetColors();
	NiPoint3 *vertices = data->GetVertices();
	float random = rand() * ( 1.0f / RAND_MAX );
	Fill( GetScale(), NI_TWO_PI * random, GetLength(), GetColor(), vertices, colors, textures );
	shape->GetShapeData()->IncRevisionID();
}

/*
================
AtmosphereStreaks::Update
================
*/
// FUNCTION: STBC 0x004ec550
void AtmosphereStreaks::Update( float time_ ) {
	NiTriShapeData *data = shape->GetShapeData();
	NiPoint2 *textures = data->GetTextures();
	NiColorA *colors = data->GetColors();
	NiPoint3 *vertices = data->GetVertices();
	float random = rand() * ( 1.0f / RAND_MAX );
	Fill( GetScale(), NI_TWO_PI * random, GetLength(), GetColor(), vertices, colors, textures );
	shape->GetShapeData()->IncRevisionID();
}

/*
================
AtmosphereStreaks::GetScale
================
*/
// FUNCTION: STBC 0x004ec5f0
float AtmosphereStreaks::GetScale() {
	return intensity * 0.25f + 0.5f;
}

/*
================
AtmosphereStreaks::GetLength
================
*/
// FUNCTION: STBC 0x004ec600
float AtmosphereStreaks::GetLength() {
	return intensity * 5.0f + 1.0f;
}

/*
================
AtmosphereStreaks::GetColor
================
*/
// FUNCTION: STBC 0x004ec610
NiColorA AtmosphereStreaks::GetColor() {
	NiColorA color = NiColorA::WHITE;
	color.a = intensity * 0.75f + 0.25f;
	return color;
}
