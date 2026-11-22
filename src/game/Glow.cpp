/*
===============================================================================

	Glow.cpp

===============================================================================
*/

#include "Glow.h"
#include "NiCamera.h"
#include "NiColor.h"
#include "NiNode.h"
#include "NiPropertyState.h"
#include "NiVertexColorProperty.h"
#include "NiMaterialProperty.h"
#include "NiTextureProperty.h"
#include "NiTextureModeProperty.h"
#include "NiAlphaProperty.h"
#include "NiZBufferProperty.h"
#include "NiFogProperty.h"
#include <iostream>

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004e1080 _$E234
// FUNCTION: STBC 0x004e1090 _$E231
// FUNCTION: STBC 0x004e10a0 _$E233
// FUNCTION: STBC 0x004e10b0 _$E232
// FUNCTION: STBC 0x004e10c0 _$E240
// FUNCTION: STBC 0x004e10d0 _$E237
// FUNCTION: STBC 0x004e10e0 _$E239
// FUNCTION: STBC 0x004e10f0 _$E238

// std::ctype<unsigned short>::id (<string>, through <iostream>).
// FUNCTION: STBC 0x004e1490 _$E287
// FUNCTION: STBC 0x004e14b0 _$E286

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004e1360 _$E21
// FUNCTION: STBC 0x004e1370 _$E20
// FUNCTION: STBC 0x004e13a0 _$E114
// FUNCTION: STBC 0x004e13b0 _$E113
// FUNCTION: STBC 0x004e13e0 _$E127
// FUNCTION: STBC 0x004e13f0 _$E126
// FUNCTION: STBC 0x004e1470 _$E25
// FUNCTION: STBC 0x004e1480 _$E24
// FUNCTION: STBC 0x004e14c0 _$E36
// FUNCTION: STBC 0x004e14d0 _$E35
// FUNCTION: STBC 0x004e14e0 _$E40
// FUNCTION: STBC 0x004e14f0 _$E39
// FUNCTION: STBC 0x004e1500 _$E44
// FUNCTION: STBC 0x004e1510 _$E43
// FUNCTION: STBC 0x004e1520 _$E48
// FUNCTION: STBC 0x004e1530 _$E47
// FUNCTION: STBC 0x004e1540 _$E172
// FUNCTION: STBC 0x004e1550 _$E171
// FUNCTION: STBC 0x004e1560 _$E211
// FUNCTION: STBC 0x004e1570 _$E210
// FUNCTION: STBC 0x004e15f0 _$E216
// FUNCTION: STBC 0x004e1600 _$E215
// FUNCTION: STBC 0x004e1610 _$E265
// FUNCTION: STBC 0x004e1620 _$E264
// FUNCTION: STBC 0x004e1630 _$E269
// FUNCTION: STBC 0x004e1640 _$E268
// FUNCTION: STBC 0x004e1650 _$E276
// FUNCTION: STBC 0x004e1660 _$E275
// FUNCTION: STBC 0x004e1670 _$E181
// FUNCTION: STBC 0x004e1680 _$E180
// FUNCTION: STBC 0x004e1690 _$E188
// FUNCTION: STBC 0x004e16a0 _$E187
// FUNCTION: STBC 0x004e16b0 _$E192
// FUNCTION: STBC 0x004e16c0 _$E191
// FUNCTION: STBC 0x004e16d0 _$E196
// FUNCTION: STBC 0x004e16e0 _$E195
// FUNCTION: STBC 0x004e16f0 _$E200
// FUNCTION: STBC 0x004e1700 _$E199
// FUNCTION: STBC 0x004e1710 _$E206
// FUNCTION: STBC 0x004e1720 _$E205
// FUNCTION: STBC 0x004e1760 _$E147
// FUNCTION: STBC 0x004e1770 _$E146
// FUNCTION: STBC 0x004e1780 _$E102
// FUNCTION: STBC 0x004e1790 _$E101
// FUNCTION: STBC 0x004e17a0 _$E157
// FUNCTION: STBC 0x004e17b0 _$E156
// FUNCTION: STBC 0x004e17c0 _$E72
// FUNCTION: STBC 0x004e17d0 _$E71
// FUNCTION: STBC 0x004e17e0 _$E137
// FUNCTION: STBC 0x004e17f0 _$E136
// FUNCTION: STBC 0x004e1800 _$E97
// FUNCTION: STBC 0x004e1810 _$E96
// FUNCTION: STBC 0x004e1820 _$E132
// FUNCTION: STBC 0x004e1830 _$E131
// FUNCTION: STBC 0x004e1840 _$E91
// FUNCTION: STBC 0x004e1850 _$E90
// FUNCTION: STBC 0x004e1860 _$E167
// FUNCTION: STBC 0x004e1870 _$E166

/*
================
GlowAccumulator::GlowAccumulator
================
*/
// FUNCTION: STBC 0x004df130
GlowAccumulator::GlowAccumulator() {
}

/*
================
GlowAccumulator::~GlowAccumulator
================
*/
// FUNCTION: STBC 0x004df1b0
GlowAccumulator::~GlowAccumulator() {
	while ( !properties.IsEmpty() ) {
		properties.RemoveHead();
	}
}

/*
================
GlowAccumulator::RegisterObject

Draws the geometry at once, with the properties of its GlowPass (or the
accumulator's own, without one) swapped into the renderer's state, and
without the renderer's sorter and lights. A geometry whose GlowPass has
its flag set is skipped.
================
*/
// FUNCTION: STBC 0x004df290
// DRAFT: inline budget: retail expands IsEmpty but calls its NiPointer( 0 ) and the loop's NiPointer copy, assignment and destructor out of line; the extra data search keeps NiIsExactKindOf's NULL test apart (as OBBModel::AddShapes)
bool GlowAccumulator::RegisterObject( NiAVObject *geometry ) {
	NiRender *renderer = camera->GetRenderer();

	NiExtraData *extra = geometry->GetExtraData();
	while ( extra != NULL && !NiIsExactKindOf( GlowPass, extra ) ) {
		extra = extra->GetNext();
	}
	GlowPass *pass = (GlowPass *)extra;

	NiPropertyList *list;
	if ( pass != NULL ) {
		if ( pass->unknown14 ) {
			return true;
		}
		list = &pass->properties;
	} else {
		list = &properties;
	}

	renderer->GetPropertyState()->SwapProperties( *list );

	NiPointer<NiAccumulator> sorter = this;
	renderer->SetSorter( NULL );
	NiPointer<NiDynamicEffectState> effects = renderer->GetEffectState();
	renderer->SetEffectState( NULL );

	geometry->Display( camera );

	renderer->SetEffectState( effects );
	renderer->SetSorter( sorter );

	renderer->GetPropertyState()->SwapProperties( *list );
	return true;
}

// The texel offsets of the samples each downsampling quad adds up: four
// diagonal and four straight neighbours (invented name).
// DATA: STBC 0x008e0f6c
static const float DOWNSAMPLE_OFFSETS[8][2] = {
	{ 0.5f, 0.5f }, { 0.5f, -0.5f }, { -0.5f, -0.5f }, { -0.5f, 0.5f },
	{ 0.0f, 1.0f }, { 0.0f, -1.0f }, { 1.0f, 0.0f }, { -1.0f, 0.0f }
};

// The texel offsets of the blur quads' samples: the centre and the four
// diagonals (invented name).
// DATA: STBC 0x008e0fac
static const float BLUR_OFFSETS[5][2] = {
	{ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, -1.0f }, { -1.0f, -1.0f }, { -1.0f, 1.0f }
};

/*
================
GlowRenderer::Create

Makes the glow for the renderer: the sorter with its default properties,
the five frame buffers and their images, the quad that adds the glow over
the scene (shapes[6]), the three quads that shrink the glow through the
smaller frame buffers (shapes[0] to [2], eight samples each, four at 16
bits) and the three that blur it back (shapes[3] to [5], five samples, one
at 16 bits). NULL when the renderer is not an NiD3DRender or a frame buffer
can't be made or drawn into.
================
*/
// FUNCTION: STBC 0x004df510
// DRAFT: inline budget, then registers: retail expands one more NiColor constructor (in the fog constructor at site 114, share 46 against 47) and one more DecRefCount (site 185, share 63 against 66); c2trace --whatif flips exactly those two with own cost +89 to about +180 tuples (ours 4763): retail's body is that much costlier, or has fewer inline sites after them; retail also keeps 0 in edi from the start
GlowRenderer *GlowRenderer::Create( NiRender *renderer_ ) {
	GlowRenderer *glow = new GlowRenderer;
	glow->renderer = NiDynamicCast( NiD3DRender, renderer_ );
	if ( glow->renderer == NULL ) {
		goto fail;
	}

	{
		GlowAccumulator *accumulator = new GlowAccumulator;
		NiPointer<NiVertexColorProperty> vertexColor = new NiVertexColorProperty;
		vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
		vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
		accumulator->properties.AddHead( (NiVertexColorProperty *)vertexColor );
		NiPointer<NiMaterialProperty> material = new NiMaterialProperty;
		material->SetEmittance( NiColor::BLACK );
		accumulator->properties.AddHead( (NiMaterialProperty *)material );
		NiPointer<NiTextureProperty> texture = new NiTextureProperty;
		accumulator->properties.AddHead( (NiTextureProperty *)texture );
		NiPointer<NiFogProperty> fog = new NiFogProperty;
		fog->SetFog( false );
		accumulator->properties.AddHead( (NiFogProperty *)fog );
		glow->accumulator = accumulator;
	}

	int i;
	for ( i = 0; i < 5; i++ ) {
		glow->frameBuffers[i] = 0;
	}
	glow->frameBuffers[0] = glow->renderer->CreateOffscreenFrameBuffer( 256, 256 );
	if ( glow->frameBuffers[0] == 0 ) {
		goto fail;
	}
	glow->frameBuffers[1] = glow->renderer->CreateOffscreenFrameBuffer( 128, 128 );
	glow->frameBuffers[2] = glow->renderer->CreateOffscreenFrameBuffer( 64, 64 );
	glow->frameBuffers[3] = glow->renderer->CreateOffscreenFrameBuffer( 32, 32 );
	glow->frameBuffers[4] = glow->renderer->CreateOffscreenFrameBuffer( 256, 256 );
	if ( glow->frameBuffers[1] == 0 || glow->frameBuffers[2] == 0 || glow->frameBuffers[3] == 0 || glow->frameBuffers[4] == 0 ) {
		goto fail;
	}

	bool is16Bit;
	for ( i = 0; i < 5; i++ ) {
		if ( !glow->renderer->SetFrameBuffer( glow->frameBuffers[i] ) ) {
			glow->renderer->SetFrameBuffer( 0 );
			goto fail;
		}
		is16Bit = glow->renderer->VirtualSlot21() == 16;
		glow->images[i] = (NiImage *)glow->renderer->VirtualSlot22( 2 );
	}
	glow->renderer->SetFrameBuffer( 0 );

	{
		NiPointer<NiTextureModeProperty> textureMode = new NiTextureModeProperty;
		textureMode->SetClampMode( NiTextureModeProperty::CLAMP_S_CLAMP_T );
		textureMode->SetFilterMode( NiTextureModeProperty::FILTER_BILERP );
		textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
		NiPointer<NiVertexColorProperty> vertexColor = new NiVertexColorProperty;
		vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
		vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
		NiPointer<NiZBufferProperty> zBuffer = new NiZBufferProperty;
		zBuffer->SetZBufferTest( false );
		zBuffer->SetZBufferWrite( false );

		// how bright the blurred glow starts, and how much of it each blur pass keeps
		float brightness;
		float falloff;
		if ( is16Bit ) {
			brightness = 1.5f;
			falloff = 0.125f;
		} else {
			brightness = 1.2f;
			falloff = 0.25f;
		}

		NiPoint3 *vertices;
		NiPoint2 *textures;
		unsigned short *triangles;

		// the quad that adds the blurred glow over the scene
		{
			vertices = new NiPoint3[4];
			textures = new NiPoint2[4];
			triangles = new unsigned short[6];
			vertices[0] = NiPoint3( 0.0f, 0.0f, 1.0f );
			vertices[1] = NiPoint3( 1.0f, 0.0f, 1.0f );
			vertices[2] = NiPoint3( 1.0f, 1.0f, 1.0f );
			vertices[3] = NiPoint3( 0.0f, 1.0f, 1.0f );
			textures[0] = NiPoint2( 0.0f, 1.0f );
			textures[1] = NiPoint2( 1.0f, 1.0f );
			textures[2] = NiPoint2( 1.0f, 0.0f );
			textures[3] = NiPoint2( 0.0f, 0.0f );
			triangles[0] = 0;
			triangles[1] = 1;
			triangles[2] = 2;
			triangles[3] = 2;
			triangles[4] = 3;
			triangles[5] = 0;
			glow->shapes[6] = new NiTriShape( 4, vertices, NULL, NULL, textures, 2, triangles );
			glow->shapes[6]->AttachProperty( textureMode );
			glow->shapes[6]->AttachProperty( vertexColor );
			NiPointer<NiTextureProperty> texture = new NiTextureProperty;
			texture->SetImage( glow->images[4] );
			glow->shapes[6]->AttachProperty( texture );
			NiPointer<NiMaterialProperty> material = new NiMaterialProperty;
			material->SetEmittance( NiColor::WHITE );
			material->SetAlpha( 1.0f );
			glow->shapes[6]->AttachProperty( material );
			NiPointer<NiAlphaProperty> alpha = new NiAlphaProperty;
			alpha->SetAlphaBlending( true );
			alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_INVDESTCOLOR );
			alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );
			glow->shapes[6]->AttachProperty( alpha );
			NiFogProperty *fog = new NiFogProperty;
			fog->SetFog( false );
			glow->shapes[6]->AttachProperty( zBuffer );
			glow->shapes[6]->AttachProperty( fog );
			glow->shapes[6]->Update( 0.0f );
			glow->shapes[6]->UpdateProperties();
			// a card that can't blend this way adds the glow plainly, and more faintly
			unsigned int caps = renderer_->VirtualSlot16( glow->shapes[6]->GetPropertyState() );
			if ( caps != 0 && ( caps & 2 ) != 0 ) {
				alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_ONE );
				if ( !is16Bit ) {
					brightness = 1.0f;
					falloff = 0.1f;
				}
			}
		}

		int quad;
		int pass;

		// the quads that shrink the glow into the smaller frame buffers
		NiPointer<NiAlphaProperty> alpha = new NiAlphaProperty;
		alpha->SetAlphaBlending( true );
		alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
		alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );
		int samples;
		float gain;
		if ( is16Bit ) {
			samples = 4;
			gain = 1.25f;
		} else {
			samples = 8;
			gain = 1.1f;
		}
		NiPointer<NiMaterialProperty> material = new NiMaterialProperty;
		material->SetEmittance( NiColor( 1.0f, 1.0f, 1.0f ) );
		material->SetAlpha( 1.0f / samples * gain );
		float size = 128.0f;
		for ( pass = 0; pass < 3; pass++ ) {
			vertices = new NiPoint3[samples * 4];
			textures = new NiPoint2[samples * 4];
			triangles = new unsigned short[samples * 6];
			unsigned short vertex = 0;
			unsigned short *triangle = triangles;
			for ( quad = 0; quad < samples; quad++ ) {
				vertices[vertex] = NiPoint3( 0.0f, 0.0f, 1.0f );
				vertices[vertex + 1] = NiPoint3( 1.0f, 0.0f, 1.0f );
				vertices[vertex + 2] = NiPoint3( 1.0f, 1.0f, 1.0f );
				vertices[vertex + 3] = NiPoint3( 0.0f, 1.0f, 1.0f );
				float u = ( DOWNSAMPLE_OFFSETS[quad][0] + 0.5f ) / size;
				float v = ( DOWNSAMPLE_OFFSETS[quad][1] + 0.5f ) / size;
				textures[vertex] = NiPoint2( u, v + 1.0f );
				textures[vertex + 1] = NiPoint2( u + 1.0f, v + 1.0f );
				textures[vertex + 2] = NiPoint2( u + 1.0f, v );
				textures[vertex + 3] = NiPoint2( u, v );
				*triangle++ = vertex;
				*triangle++ = vertex + 1;
				*triangle++ = vertex + 2;
				*triangle++ = vertex + 2;
				*triangle++ = vertex + 3;
				*triangle++ = vertex;
				vertex += 4;
			}
			glow->shapes[pass] = new NiTriShape( samples * 4, vertices, NULL, NULL, textures, samples * 2, triangles );
			glow->shapes[pass]->AttachProperty( textureMode );
			glow->shapes[pass]->AttachProperty( vertexColor );
			glow->shapes[pass]->AttachProperty( material );
			glow->shapes[pass]->AttachProperty( alpha );
			NiPointer<NiTextureProperty> texture = new NiTextureProperty;
			texture->SetImage( glow->images[pass] );
			glow->shapes[pass]->AttachProperty( texture );
			glow->shapes[pass]->Update( 0.0f );
			glow->shapes[pass]->UpdateProperties();
			size *= 0.5f;
		}

		// the quads that blur the glow back up into the last frame buffer
		{
			NiPointer<NiAlphaProperty> alpha = new NiAlphaProperty;
			alpha->SetAlphaBlending( true );
			alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
			alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );
			int samples = 5;
			if ( is16Bit ) {
				samples = 1;
			}
			for ( pass = 0; pass < 3; pass++ ) {
				vertices = new NiPoint3[samples * 4];
				textures = new NiPoint2[samples * 4];
				triangles = new unsigned short[samples * 6];
				unsigned short vertex = 0;
				unsigned short *triangle = triangles;
				for ( quad = 0; quad < samples; quad++ ) {
					vertices[vertex] = NiPoint3( 0.0f, 0.0f, 1.0f );
					vertices[vertex + 1] = NiPoint3( 1.0f, 0.0f, 1.0f );
					vertices[vertex + 2] = NiPoint3( 1.0f, 1.0f, 1.0f );
					vertices[vertex + 3] = NiPoint3( 0.0f, 1.0f, 1.0f );
					float u = ( BLUR_OFFSETS[quad][0] + 0.5f ) * ( 1.0f / 256.0f );
					float v = ( BLUR_OFFSETS[quad][1] + 0.5f ) * ( 1.0f / 256.0f );
					textures[vertex] = NiPoint2( u, v + 1.0f );
					textures[vertex + 1] = NiPoint2( u + 1.0f, v + 1.0f );
					textures[vertex + 2] = NiPoint2( u + 1.0f, v );
					textures[vertex + 3] = NiPoint2( u, v );
					*triangle++ = vertex;
					*triangle++ = vertex + 1;
					*triangle++ = vertex + 2;
					*triangle++ = vertex + 2;
					*triangle++ = vertex + 3;
					*triangle++ = vertex;
					vertex += 4;
				}
				glow->shapes[pass + 3] = new NiTriShape( samples * 4, vertices, NULL, NULL, textures, samples * 2, triangles );
				float passAlpha = brightness / samples;
				if ( passAlpha > 1.0f ) {
					passAlpha = 1.0f;
				}
				brightness *= falloff;
				NiPointer<NiMaterialProperty> material = new NiMaterialProperty;
				material->SetEmittance( NiColor( 1.0f, 1.0f, 1.0f ) );
				material->SetAlpha( passAlpha );
				glow->shapes[pass + 3]->AttachProperty( material );
				glow->shapes[pass + 3]->AttachProperty( alpha );
				glow->shapes[pass + 3]->AttachProperty( textureMode );
				glow->shapes[pass + 3]->AttachProperty( vertexColor );
				NiPointer<NiTextureProperty> texture = new NiTextureProperty;
				texture->SetImage( glow->images[3 - pass] );
				glow->shapes[pass + 3]->AttachProperty( texture );
				glow->shapes[pass + 3]->Update( 0.0f );
				glow->shapes[pass + 3]->UpdateProperties();
			}
		}
		return glow;
	}

fail:
	delete glow;
	return NULL;
}

/*
================
GlowRenderer::~GlowRenderer
================
*/
// FUNCTION: STBC 0x004e0ac0
GlowRenderer::~GlowRenderer() {
	int i;
	for ( i = 0; i < 5; i++ ) {
		images[i] = NULL;
	}
	for ( i = 0; i < 7; i++ ) {
		shapes[i] = NULL;
	}
	if ( renderer ) {
		for ( i = 0; i < 5; i++ ) {
			if ( frameBuffers[i] ) {
				renderer->RemoveFrameBuffer( frameBuffers[i] );
			}
		}
	}
}

/*
================
GlowRenderer::SetIntensity

Does nothing: SetClass's draw passes its glow intensity.
================
*/
// FUNCTION: STBC 0x004e0c10
void GlowRenderer::SetIntensity( float intensity ) {
}

/*
================
GlowRenderer::BeginPass
================
*/
// FUNCTION: STBC 0x004e0c20
void GlowRenderer::BeginPass() {
	renderer->SetFrameBuffer( frameBuffers[0] );
}

/*
================
GlowRenderer::EndPass
================
*/
// FUNCTION: STBC 0x004e0c30
void GlowRenderer::EndPass() {
	renderer->VirtualSlot17();
	Blur();
}

/*
================
GlowRenderer::RenderPass

Clicks the camera with the glow's sorter in place of the renderer's.
================
*/
// FUNCTION: STBC 0x004e0c50
// DRAFT: inline budget: retail calls NiObject::DecRefCount out of line in the first SetSorter and expands the second; this build expands both, and keeps this in esi
void GlowRenderer::RenderPass( NiCamera *camera, bool clear ) {
	NiPointer<NiAccumulator> sorter = renderer->GetSorter();
	renderer->SetSorter( accumulator );
	if ( clear ) {
		renderer->SetBackgroundColor( NiColor::BLACK );
		camera->Clear( 5 );
	} else {
		camera->Clear( 4 );
	}
	camera->Click();
	renderer->SetSorter( sorter );
}

/*
================
GlowRenderer::Blur

Draws the glow down through the smaller frame buffers with the first four
quads, then the three blur quads into the last frame buffer.
================
*/
// FUNCTION: STBC 0x004e0d40
void GlowRenderer::Blur() {
	NiPointer<NiAccumulator> sorter = renderer->GetSorter();
	renderer->SetBackgroundColor( NiColor::BLACK );
	renderer->SetSorter( NULL );

	int i;
	for ( i = 0; i < 4; i++ ) {
		renderer->SetFrameBuffer( frameBuffers[i + 1] );
		renderer->ClearBuffer( NULL, 1 );
		renderer->VirtualSlot38( NULL );
		renderer->VirtualSlot43( true );
		renderer->SetPropertyState( shapes[i]->GetPropertyState() );
		renderer->VirtualSlot45( shapes[i]->GetShapeData(), shapes[i]->GetWorldBound() );
		renderer->VirtualSlot44();
		renderer->VirtualSlot39();
	}

	renderer->SetFrameBuffer( frameBuffers[4] );
	renderer->VirtualSlot38( NULL );
	renderer->VirtualSlot43( true );
	renderer->ClearBuffer( NULL, 1 );
	for ( i = 0; i < 3; i++ ) {
		renderer->SetPropertyState( shapes[i + 3]->GetPropertyState() );
		renderer->VirtualSlot45( shapes[i + 3]->GetShapeData(), shapes[i + 3]->GetWorldBound() );
	}
	renderer->VirtualSlot44();
	renderer->VirtualSlot39();

	renderer->SetFrameBuffer( 0 );
	renderer->SetSorter( sorter );
}

/*
================
GlowRenderer::Composite

Draws the last quad, the blurred glow, over the scene.
================
*/
// FUNCTION: STBC 0x004e0fa0
void GlowRenderer::Composite() {
	renderer->VirtualSlot38( NULL );
	renderer->VirtualSlot43( true );
	renderer->SetPropertyState( shapes[6]->GetPropertyState() );
	renderer->VirtualSlot45( shapes[6]->GetShapeData(), shapes[6]->GetWorldBound() );
	renderer->VirtualSlot44();
	renderer->VirtualSlot39();
}
