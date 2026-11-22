/*
===============================================================================

	TorpedoGraphic.cpp

===============================================================================
*/

#include <iostream>
#include <stdlib.h>
#include <math.h>

#include "TorpedoGraphic.h"
#include "NiPointLight.h"
#include "NiTriShapeData.h"
#include "NiMath.h"
#include "NiBillboardNode.h"
#include "GlowPass.h"
#include "ImageManagerClass.h"
#include "TGColorA.h"
#include "NiAlphaProperty.h"
#include "NiMaterialProperty.h"
#include "NiTextureModeProperty.h"
#include "NiTextureProperty.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004f59d0 _$E45
// FUNCTION: STBC 0x004f59e0 _$E44
// FUNCTION: STBC 0x004f59f0 _$E56
// FUNCTION: STBC 0x004f5a00 _$E55
// FUNCTION: STBC 0x004f5a10 _$E60
// FUNCTION: STBC 0x004f5a20 _$E59
// FUNCTION: STBC 0x004f5a30 _$E64
// FUNCTION: STBC 0x004f5a40 _$E63
// FUNCTION: STBC 0x004f5a50 _$E68
// FUNCTION: STBC 0x004f5a60 _$E67
// FUNCTION: STBC 0x004f5a70 _$E192
// FUNCTION: STBC 0x004f5a80 _$E191
// FUNCTION: STBC 0x004f5a90 _$E197
// FUNCTION: STBC 0x004f5aa0 _$E196
// FUNCTION: STBC 0x004f5ab0 _$E201
// FUNCTION: STBC 0x004f5ac0 _$E200
// FUNCTION: STBC 0x004f5ad0 _$E209
// FUNCTION: STBC 0x004f5ae0 _$E208
// FUNCTION: STBC 0x004f5af0 _$E216
// FUNCTION: STBC 0x004f5b00 _$E215
// FUNCTION: STBC 0x004f5b10 _$E220
// FUNCTION: STBC 0x004f5b20 _$E219
// FUNCTION: STBC 0x004f5b30 _$E224
// FUNCTION: STBC 0x004f5b40 _$E223
// FUNCTION: STBC 0x004f5b50 _$E228
// FUNCTION: STBC 0x004f5b60 _$E227
// FUNCTION: STBC 0x004f5b70 _$E234
// FUNCTION: STBC 0x004f5b80 _$E233
// FUNCTION: STBC 0x004f5c50 _$E134
// FUNCTION: STBC 0x004f5c60 _$E133
// FUNCTION: STBC 0x004f5c70 _$E147
// FUNCTION: STBC 0x004f5c80 _$E146
// FUNCTION: STBC 0x004f5c90 _$E167
// FUNCTION: STBC 0x004f5ca0 _$E166
// FUNCTION: STBC 0x004f5cb0 _$E122
// FUNCTION: STBC 0x004f5cc0 _$E121
// FUNCTION: STBC 0x004f5cd0 _$E247
// FUNCTION: STBC 0x004f5ce0 _$E246
// FUNCTION: STBC 0x004f5cf0 _$E177
// FUNCTION: STBC 0x004f5d00 _$E176
// FUNCTION: STBC 0x004f5d10 _$E187
// FUNCTION: STBC 0x004f5d20 _$E186
// FUNCTION: STBC 0x004f5d30 _$E92
// FUNCTION: STBC 0x004f5d40 _$E91
// FUNCTION: STBC 0x004f5d50 _$E117
// FUNCTION: STBC 0x004f5d60 _$E116
// FUNCTION: STBC 0x004f5d70 _$E111
// FUNCTION: STBC 0x004f5d80 _$E110
// FUNCTION: STBC 0x004f5d90 _$E239
// FUNCTION: STBC 0x004f5da0 _$E238
// FUNCTION: STBC 0x004f5db0 _$E243
// FUNCTION: STBC 0x004f5dc0 _$E242

// The standard streams' initialisers and exit-time destructors (<iostream>),
// and the guard of std::ctype<unsigned short>::id with its exit-time thunk.
// FUNCTION: STBC 0x004f5920 _$E32
// FUNCTION: STBC 0x004f5930 _$E29
// FUNCTION: STBC 0x004f5940 _$E31
// FUNCTION: STBC 0x004f5950 _$E30
// FUNCTION: STBC 0x004f5960 _$E38
// FUNCTION: STBC 0x004f5970 _$E35
// FUNCTION: STBC 0x004f5980 _$E37
// FUNCTION: STBC 0x004f5990 _$E36
// FUNCTION: STBC 0x004f59a0 _$E264
// FUNCTION: STBC 0x004f59c0 _$E263

// The NiRTTI's static initialiser, its exit-time thunk, the exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x004f45d0 _$E254
// FUNCTION: STBC 0x004f45e0 _$E250
// FUNCTION: STBC 0x004f4600 _$E253
// FUNCTION: STBC 0x004f4610 _$E251
// DATA: STBC 0x986970 ?rtti@TorpedoGraphic@@2VNiRTTI@@B
const NiRTTI TorpedoGraphic::rtti( "TorpedoGraphic", &NiNode::rtti );

// The vertices of one flare's quad in the flare shape (invented name). Not static: UpdateFlares reads it again
// after its rand() calls, as for a global another module may change.
// DATA: STBC 0x8E1060
int verticesPerFlare = 4;

/*
================
TorpedoGraphic::TorpedoGraphic

A light in the glow colour, then a camera-facing pair of quads: the glow,
pulsed by the controller, and a copy of it for the core, scaled to the
core's size; then numFlares two-sided streaks in random directions around
the centre, faded in and out by the controller, which also spins the
whole graphic.
================
*/
// FUNCTION: STBC 0x004f4630
// DRAFT: inline budget: from the core's alpha property on retail calls the nested inlines out of line (NiProperty's constructor, SetBit/SetField, NiColor::operator=); the light's diffuse colour goes through the FPU in retail; frame 12 bytes smaller
TorpedoGraphic::TorpedoGraphic( char *coreTexture, TGColorA *coreColor, float coreSize, float coreRotationSpeed,
								char *glowTexture, TGColorA *glowColor, float glowPulseRate, float glowMinScale,
								float glowMaxScale, char *flareTexture, TGColorA *flareColor, int numFlares,
								float flareLength, float flareLife ) {
	NiPointLight *light = new NiPointLight;
	light->SetAmbientColor( NiColor::BLACK );
	light->SetDiffuseColor( NiColor( glowColor->GetR(), glowColor->GetG(), glowColor->GetB() ) );
	light->SetConstantAttenuation( 0.0f );
	light->SetLinearAttenuation( coreSize );
	light->SetQuadraticAttenuation( 0.0f );
	AttachChild( light, false );

	NiBillboardNode *billboard = new NiBillboardNode;
	billboard->SetMode( NiBillboardNode::RIGID_FACE_CAMERA );
	AttachChild( billboard, false );

	NiPoint3 *vertices = new NiPoint3[4];
	vertices[0] = NiPoint3( -0.5f, -0.5f, 0.0f );
	vertices[1] = NiPoint3( 0.5f, -0.5f, 0.0f );
	vertices[2] = NiPoint3( 0.5f, 0.5f, 0.0f );
	vertices[3] = NiPoint3( -0.5f, 0.5f, 0.0f );

	NiPoint2 *textures = new NiPoint2[4];
	textures[0] = NiPoint2( 0.0f, 0.0f );
	textures[1] = NiPoint2( 0.0f, 1.0f );
	textures[2] = NiPoint2( 1.0f, 1.0f );
	textures[3] = NiPoint2( 1.0f, 0.0f );

	unsigned short *triangles = new unsigned short[6];
	triangles[0] = 0;
	triangles[1] = 1;
	triangles[2] = 2;
	triangles[3] = 0;
	triangles[4] = 2;
	triangles[5] = 3;

	NiTriShape *glow = new NiTriShape( 4, vertices, NULL, NULL, textures, 2, triangles );
	GlowPass *glowPass = new GlowPass;
	glowPass->unknown14 = false;
	glow->AddExtraData( glowPass );
	NiTriShape *core = (NiTriShape *)glow->Clone();

	NiImage *image = g_kImageManager.GetImage( glowTexture, true, true );
	NiTextureProperty *texture = new NiTextureProperty;
	texture->SetImage( image );
	glow->AttachProperty( texture );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	glow->AttachProperty( vertexColor );

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetEmittance( NiColor( glowColor->GetR(), glowColor->GetG(), glowColor->GetB() ) );
	material->SetAlpha( 0.8f );
	glow->AttachProperty( material );

	material = new NiMaterialProperty;
	material->SetEmittance( NiColor( glowColor->GetR(), glowColor->GetG(), glowColor->GetB() ) );
	material->SetAlpha( 0.4f );
	glowPass->properties.AddHead( material );

	image = g_kImageManager.GetImage( coreTexture, true, true );
	texture = new NiTextureProperty;
	texture->SetImage( image );
	core->AttachProperty( texture );
	core->AttachProperty( vertexColor );

	glowPass = new GlowPass;
	glowPass->unknown14 = true;
	core->AddExtraData( glowPass );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_INVSRCALPHA );
	alpha->SetAlphaBlending( true );
	core->AttachProperty( alpha );

	material = new NiMaterialProperty;
	material->SetEmittance( NiColor( coreColor->GetR(), coreColor->GetG(), coreColor->GetB() ) );
	material->SetAlpha( 1.0f );
	core->AttachProperty( material );

	if ( coreSize > 0.0f ) {
		core->SetScale( coreSize );
	} else {
		core->SetScale( -coreSize );
	}

	billboard->AttachChild( glow, false );
	billboard->AttachChild( core, false );

	int numVertices = verticesPerFlare * numFlares;
	vertices = new NiPoint3[numVertices];
	textures = new NiPoint2[numVertices];
	NiColorA *colors = new NiColorA[numVertices];

	float flareSize = coreSize * 0.4f;
	NiPoint3 *vertex = vertices;
	NiPoint2 *textureCoord = textures;
	NiColorA *color = colors;
	for ( int i = 0; i < numFlares; i++ ) {
		NiMatrix3 zRotation;
		zRotation.MakeZRotation( rand() * NI_TWO_PI * ( 1.0f / RAND_MAX ) );
		NiMatrix3 yRotation;
		yRotation.MakeYRotation( rand() * NI_TWO_PI * ( 1.0f / RAND_MAX ) );

		vertex->x = 0.0f;
		vertex->y = -flareSize;
		vertex->z = 0.0f;
		*vertex = yRotation * *vertex;
		*vertex = zRotation * *vertex;
		*textureCoord = NiPoint2( 0.0f, 0.0f );
		*color = *flareColor;
		vertex++;
		textureCoord++;
		color++;

		vertex->x = 0.0f;
		vertex->y = -flareSize;
		vertex->z = flareLength;
		*vertex = yRotation * *vertex;
		*vertex = zRotation * *vertex;
		*textureCoord = NiPoint2( 0.0f, 1.0f );
		*color = *flareColor;
		vertex++;
		textureCoord++;
		color++;

		vertex->x = 0.0f;
		vertex->y = flareSize;
		vertex->z = flareLength;
		*vertex = yRotation * *vertex;
		*vertex = zRotation * *vertex;
		*textureCoord = NiPoint2( 1.0f, 1.0f );
		*color = *flareColor;
		vertex++;
		textureCoord++;
		color++;

		vertex->x = 0.0f;
		vertex->y = flareSize;
		vertex->z = 0.0f;
		*vertex = yRotation * *vertex;
		*vertex = zRotation * *vertex;
		*textureCoord = NiPoint2( 1.0f, 0.0f );
		*color = *flareColor;
		vertex++;
		textureCoord++;
		color++;
	}

	// both faces of each streak
	triangles = new unsigned short[numFlares * 12];
	unsigned short *triangle = triangles;
	for ( int j = 0; j < numFlares; j++ ) {
		unsigned short first = j * 4;
		*triangle++ = first;
		*triangle++ = first + 1;
		*triangle++ = first + 2;
		*triangle++ = first;
		*triangle++ = first + 2;
		*triangle++ = first + 3;
		*triangle++ = first;
		*triangle++ = first + 2;
		*triangle++ = first + 1;
		*triangle++ = first;
		*triangle++ = first + 3;
		*triangle++ = first + 2;
	}

	NiTriShape *flares = new NiTriShape( numVertices, vertices, NULL, colors, textures, numFlares * 4, triangles );
	texture = new NiTextureProperty;
	// BUG: the flares are drawn with the core's texture; flareTexture is
	// never used
	texture->SetImage( g_kImageManager.GetImage( coreTexture, true, true ) );
	flares->AttachProperty( texture );
	AttachChild( flares, false );

	glowPass = new GlowPass;
	glowPass->unknown14 = false;
	flares->AddExtraData( glowPass );

	TorpedoController *controller = new TorpedoController( coreRotationSpeed, this, glowMinScale, glowMaxScale, glowPulseRate,
		glow, flareLife, flareLife * 2.0f, numFlares, flares );
	controller->SetTarget( this );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( false );
	AttachProperty( zBuffer );

	vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	AttachProperty( vertexColor );

	alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );
	AttachProperty( alpha );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	textureMode->SetFilterMode( g_kImageManager.GetTextureFilterMode() );
	AttachProperty( textureMode );
}

/*
================
TorpedoController::TorpedoController
================
*/
// FUNCTION: STBC 0x004f5520
TorpedoController::TorpedoController( float rotationSpeed_, NiAVObject *spinObject_, float minScale_, float maxScale_,
									  float pulseRate_, NiAVObject *pulseObject_, float flareLife_, float flareLifeVariance_,
									  int numFlares_, NiTriShape *flareShape_ ) {
	rotationSpeed = rotationSpeed_;
	angle = 0.0f;
	spinObject = spinObject_;
	lastTime = -NI_INFINITY;
	minScale = minScale_;
	maxScale = maxScale_;
	pulsePhase = 0.0f;
	pulseRate = pulseRate_;
	pulseObject = pulseObject_;
	numFlares = numFlares_;
	flareTimes = new float[numFlares];
	flareLife = flareLife_;
	flareLifeVariance = flareLifeVariance_;
	flareShape = flareShape_;
}

/*
================
TorpedoController::UpdateFlares
================
*/
// FUNCTION: STBC 0x004f5600
void TorpedoController::UpdateFlares( float time ) {
	NiColorA *colors = flareShape->GetShapeData()->GetColors();

	for ( int i = 0; i < numFlares; i++ ) {
		float age = time - flareTimes[i];
		if ( age > flareLife ) {
			age -= flareLife;
			if ( age > flareLifeVariance + flareLife ) {
				float random = rand() * ( 1.0f / RAND_MAX );
				age = ( flareLifeVariance + flareLife ) * random;
			}
			flareTimes[i] = ( ( rand() * ( 1.0f / RAND_MAX ) ) * flareLifeVariance * 0.5f ) + ( flareLifeVariance * 0.5f ) - age + time;
			age = 0.0f;
		}

		float fraction = age / flareLife;
		float alpha;
		if ( fraction < 0.0f ) {
			alpha = 0.0f;
		} else if ( fraction < 0.3f ) {
			alpha = fraction * ( 1.0f / 0.3f );
		} else if ( fraction < 0.7f ) {
			alpha = 1.0f;
		} else if ( fraction < 1.0f ) {
			alpha = 1.0f - ( fraction - 0.7f ) * ( 1.0f / 0.3f );
		} else {
			alpha = 0.0f;
		}

		int first = verticesPerFlare * i;
		int last = first + verticesPerFlare;
		for ( int j = first; j < last; j++ ) {
			colors[j].a = alpha;
		}
	}

	flareShape->GetShapeData()->IncRevisionID();
}

/*
================
TorpedoController::Update
================
*/
// FUNCTION: STBC 0x004f5780
void TorpedoController::Update( float time ) {
	if ( lastTime < 0.0f ) {
		lastTime = time;
		for ( int i = 0; i < numFlares; i++ ) {
			flareTimes[i] = time - rand() * flareLife * ( 1.0f / RAND_MAX );
		}
	}

	float deltaTime = time - lastTime;

	float newAngle = deltaTime * rotationSpeed + angle;
	angle = newAngle;
	if ( newAngle > NI_TWO_PI ) {
		if ( NI_TWO_PI + NI_TWO_PI < newAngle ) {
			angle = rand() * NI_TWO_PI * ( 1.0f / RAND_MAX );
		} else {
			angle = newAngle - NI_TWO_PI;
		}
	}
	NiMatrix3 rotation;
	rotation.MakeYRotation( angle );
	spinObject->SetRotate( rotation );

	pulsePhase += deltaTime * pulseRate;
	while ( pulsePhase > NI_PI ) {
		pulsePhase -= NI_PI;
	}
	// MATCH-PENDING: R17, single-use local pulse renames pulseObject only to move its load before the sign test; the original's spelling is unknown
	NiAVObject *pulse = pulseObject;
	float scale = (float)sin( pulsePhase ) * ( maxScale - minScale ) + minScale;
	if ( scale <= 0.0f ) {
		scale = -scale;
	}
	pulse->SetScale( scale );

	UpdateFlares( time );
	lastTime = time;
}

/*
================
TorpedoGraphic::GetLight
================
*/
// FUNCTION: STBC 0x004f58d0
NiPointLight *TorpedoGraphic::GetLight() {
	return NiDynamicCast( NiPointLight, GetAt( 0 ) );
}

/*
================
TorpedoController::~TorpedoController
================
*/
// FUNCTION: STBC 0x004f5bd0
TorpedoController::~TorpedoController() {
	delete [] flareTimes;
}
