// NiD3DTexturePipeline.cpp -- the renderer's texture pipeline manager

#include "NiRTLib.h"
#include <stdio.h>
#include <string.h>

#include "NiD3DTexturePipeline.h"
#include "NiD3DRender.h"
#include "NiD3DRenderState.h"
#include "NiD3DVertexBufferManager.h"
#include "NiTextureManager.h"
#include "NiRawImageData.h"
#include "NiTextureEffect.h"
#include "NiDDImage.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007d0e50 _$E223
// FUNCTION: STBC 0x007cfcd0 _$E17
// FUNCTION: STBC 0x007cfcf0 _$E15
// FUNCTION: STBC 0x007cfd00 _$E23
// FUNCTION: STBC 0x007cfd20 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007d0720 _$E30
// FUNCTION: STBC 0x007d0890 _$E61
// FUNCTION: STBC 0x007d0da0 _$E65
// FUNCTION: STBC 0x007d0db0 _$E69
// FUNCTION: STBC 0x007d0dc0 _$E73
// FUNCTION: STBC 0x007d0dd0 _$E78
// FUNCTION: STBC 0x007d0de0 _$E95
// FUNCTION: STBC 0x007d0df0 _$E104
// FUNCTION: STBC 0x007d0e00 _$E44
// FUNCTION: STBC 0x007d0e10 _$E57
// FUNCTION: STBC 0x007d0e40 _$E181
// FUNCTION: STBC 0x007d0e70 _$E186
// FUNCTION: STBC 0x007d0e80 _$E191
// FUNCTION: STBC 0x007d0e90 _$E90
// FUNCTION: STBC 0x007d0ea0 _$E100
// FUNCTION: STBC 0x007d0ed0 _$E216
// FUNCTION: STBC 0x007d0f40 _$E135
// FUNCTION: STBC 0x007d0f50 _$E156

/*
================
NiD3DTexturePipeline::NiD3DTexturePipeline

Takes references to the device, the render state and the texture
manager, makes the alpha properties of the blending setups and a black
and white image for the default stages, and copies the device
description.
================
*/
// FUNCTION: STBC 0x7CBE60
NiD3DTexturePipeline::NiD3DTexturePipeline( IDirect3DDevice7 *device_, const D3DDEVICEDESC7 *deviceDesc_,
											NiD3DRenderState *renderState_, NiTextureManager *textureManager_ ) {
	int i;

	device = device_;
	device->AddRef();
	renderState = renderState_;
	renderState->IncRefCount();
	textureManager = textureManager_;
	textureManager->AddReference();

	unknown0F8 = 0;
	for ( i = 0; i < 6; i++ ) {
		filterModes[i][0] = D3DTFN_POINT;
		filterModes[i][1] = D3DTFG_POINT;
		filterModes[i][2] = D3DTFP_NONE;
	}
	addressModes[3][0] = D3DTADDRESS_WRAP;
	addressModes[3][1] = D3DTADDRESS_WRAP;
	addressModes[2][0] = D3DTADDRESS_WRAP;
	addressModes[2][1] = D3DTADDRESS_CLAMP;
	addressModes[1][0] = D3DTADDRESS_CLAMP;
	addressModes[1][1] = D3DTADDRESS_WRAP;
	addressModes[0][0] = D3DTADDRESS_CLAMP;
	addressModes[0][1] = D3DTADDRESS_CLAMP;

	alphaProperties[BLEND_ADD] = new NiAlphaProperty;
	alphaProperties[BLEND_ADD]->SetAlphaBlending( true );
	alphaProperties[BLEND_ADD]->SetSrcBlendMode( NiAlphaProperty::ALPHA_ONE );
	alphaProperties[BLEND_ADD]->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );

	alphaProperties[BLEND_ALPHA_ADD] = new NiAlphaProperty;
	alphaProperties[BLEND_ALPHA_ADD]->SetAlphaBlending( true );
	alphaProperties[BLEND_ALPHA_ADD]->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alphaProperties[BLEND_ALPHA_ADD]->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );

	alphaProperties[BLEND_ALPHA] = new NiAlphaProperty;
	alphaProperties[BLEND_ALPHA]->SetAlphaBlending( true );
	alphaProperties[BLEND_ALPHA]->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alphaProperties[BLEND_ALPHA]->SetDestBlendMode( NiAlphaProperty::ALPHA_INVSRCALPHA );

	alphaProperties[BLEND_MULTIPLY] = new NiAlphaProperty;
	alphaProperties[BLEND_MULTIPLY]->SetAlphaBlending( true );
	alphaProperties[BLEND_MULTIPLY]->SetSrcBlendMode( NiAlphaProperty::ALPHA_ZERO );
	alphaProperties[BLEND_MULTIPLY]->SetDestBlendMode( NiAlphaProperty::ALPHA_SRCCOLOR );

	alphaProperties[4] = new NiAlphaProperty;
	alphaProperties[4]->SetAlphaBlending( false );

	alphaProperties[5] = new NiAlphaProperty;
	alphaProperties[5]->SetAlphaBlending( false );

	unknown4FC = 0;
	unknown500 = 0;
	unknown334 = 0;
	unknown8DC = 0;
	unknownC20 = 0;
	unknown1C70 = 0;
	unknown1CDC = false;
	unknown1D48 = false;
	unknown1C74.blend = 0;
	unknown1CE0.blend = 2;
	for ( i = 0; i < 8; i++ ) {
		unknown8E0[i].blend = 0;
	}
	for ( i = 0; i < 8; i++ ) {
		unknown1930[i].blend = 3;
	}
	for ( i = 0; i < 8; i++ ) {
		unknownC28[i].blend = 0;
	}

	NiPointer<NiRawImageData> raw = new NiRawImageData( NiRawImageData::FORMAT_RGB, 2, 1 );
	unsigned char *pixels = raw->GetPixels();
	pixels[3] = pixels[4] = pixels[5] = 0xFF;
	pixels[0] = pixels[1] = pixels[2] = 0;
	unknownC24 = NiImage::Create( raw, 0, 7, 2 );

	unsigned int texture = textureManager->VirtualSlot01( unknownC24 );
	for ( i = 0; i < 8; i++ ) {
		unknownF68[i].image = unknownC24;
		unknownF68[i].unknown04 = texture;
		unknownF68[i].filterMode = 0;
		unknownF68[i].addressU = D3DTADDRESS_CLAMP;
		unknownF68[i].addressV = D3DTADDRESS_CLAMP;
		unknownF68[i].texCoordIndex = D3DTSS_TCI_CAMERASPACEPOSITION;
		unknownF68[i].unknown58 = false;
		unknownF68[i].unknown60 = false;
		unknownF68[i].source = NULL;
		unknownF68[i].blend = 3;
		memset( &unknownF68[i].transform, 0, sizeof( unknownF68[i].transform ) );
	}
	unknown12A8 = 0;
	for ( i = 0; i < 8; i++ ) {
		unknown12AC[i].blend = 3;
	}
	for ( i = 0; i < 8; i++ ) {
		unknown15EC[i].image = unknownC24;
		unknown15EC[i].unknown04 = texture;
		unknown15EC[i].filterMode = 0;
		unknown15EC[i].addressU = D3DTADDRESS_CLAMP;
		unknown15EC[i].addressV = D3DTADDRESS_CLAMP;
		unknown15EC[i].texCoordIndex = D3DTSS_TCI_CAMERASPACEPOSITION;
		unknown15EC[i].unknown58 = false;
		unknown15EC[i].unknown60 = false;
		unknown15EC[i].source = NULL;
		unknown15EC[i].blend = 0;
		memset( &unknown15EC[i].transform, 0, sizeof( unknown15EC[i].transform ) );
	}
	unknown192C = 0;

	textureTransformFlags = D3DTTFF_COUNT2;
	deviceDesc = *deviceDesc_;
	flags = 0;
	unknown354 = 8;
	memset( unknown504, 0, sizeof( unknown504 ) );
	unknown528 = 0;
	unknown52C = 0;
	unknown1D4C = NiMatrix3::IDENTITY;
	unknown1D70 = NiPoint3::ZERO;
	vertexBufferManager = NULL;
	effectState = NULL;

	// the stage operations of each apply mode, the decal and modulate
	// alpha ones by the blend caps, and stage 1's color operation of each
	// multitexture mode
	textureOps[0].colorOp = D3DTOP_SELECTARG1;
	textureOps[0].alphaOp = D3DTOP_SELECTARG1;
	textureOps[1].colorOp = ( deviceDesc.dpcTriCaps.dwTextureBlendCaps & D3DPTBLENDCAPS_DECALALPHA ) ? D3DTOP_BLENDTEXTUREALPHA : D3DTOP_SELECTARG1;
	textureOps[1].alphaOp = D3DTOP_SELECTARG1;
	textureOps[2].colorOp = D3DTOP_MODULATE;
	textureOps[2].alphaOp = ( deviceDesc.dpcTriCaps.dwTextureBlendCaps & D3DPTBLENDCAPS_MODULATEALPHA ) ? D3DTOP_MODULATE : D3DTOP_SELECTARG1;
	textureOps[3].colorOp = D3DTOP_MODULATE;
	textureOps[3].alphaOp = D3DTOP_SELECTARG2;
	textureOps[4].colorOp = D3DTOP_MODULATE;
	textureOps[4].alphaOp = D3DTOP_SELECTARG1;
	textureOps[5].colorOp = D3DTOP_MODULATE;
	textureOps[5].alphaOp = D3DTOP_SELECTARG1;
	multiTextureOps[3].colorOp = D3DTOP_MODULATE;
	multiTextureOps[0].colorOp = D3DTOP_ADD;
	multiTextureOps[1].colorOp = multiTextureOps[2].colorOp = D3DTOP_BLENDTEXTUREALPHA;
	multiTextureOps[4].colorOp = D3DTOP_MODULATE2X;

	coordGenFunctions[0] = Unknown7D03A0;
	coordGenFunctions[1] = Unknown7D0560;
	coordGenFunctions[2] = Unknown7D08A0;
}

/*
================
NiD3DTexturePipeline::~NiD3DTexturePipeline

Clears the device's first two texture stages and releases the device, the
texture manager and the render state, then deletes the vertex buffer
manager.
================
*/
// FUNCTION: STBC 0x7CCA90
NiD3DTexturePipeline::~NiD3DTexturePipeline() {
	if ( device ) {
		device->SetTexture( 0, NULL );
		device->SetTexture( 1, NULL );
		device->Release();
		device = NULL;
	}

	if ( textureManager ) {
		textureManager->Release();
		textureManager = NULL;
	}

	if ( renderState ) {
		renderState->DecRefCount();
		renderState = NULL;
	}

	delete vertexBufferManager;
}

/*
================
NiD3DTexturePipeline::Create

Makes the pipeline of a device, with its own vertex buffer manager, and
checks the device's multitexturing; null when the check fails.
================
*/
// FUNCTION: STBC 0x7CCD10
NiD3DTexturePipeline *NiD3DTexturePipeline::Create( IDirect3DDevice7 *device, const D3DDEVICEDESC7 *deviceDesc,
													NiD3DRenderState *renderState, NiTextureManager *textureManager, bool hardware ) {
	NiD3DRender::Message( "(Creation): Attempting to create texture pipeline manager" );

	NiD3DTexturePipeline *pipeline = new NiD3DTexturePipeline( device, deviceDesc, renderState, textureManager );
	if ( hardware ) {
		pipeline->SetFlag( HARDWARE );
	}

	IDirect3D7 *d3d = NULL;	// (name invented; fits the /Od frame order)
	pipeline->device->GetDirect3D( &d3d );
	pipeline->vertexBufferManager = new NiD3DVertexBufferManager( d3d, pipeline->device, true );
	d3d->Release();

	pipeline->CheckTextureCapabilities();
	if ( !pipeline->Unknown7CE9C0( false ) ) {
		goto failed;
	}
	NiD3DRender::Message( "(Creation): Texture pipeline manager creation succeeded" );
	return pipeline;

failed:
	delete pipeline;
	return NULL;
}

/*
================
NiD3DTexturePipeline::Unknown7CCE50
================
*/
// FUNCTION: STBC 0x7CCE50
void NiD3DTexturePipeline::Unknown7CCE50() {
	Unknown7CE9C0( true );
}

/*
================
NiD3DTexturePipeline::CheckTextureCapabilities

Picks the filter setup by whether the device reports DirectX 6 filter
capabilities, and copies the addressing, blending and projection
capabilities into the flags.
================
*/
// FUNCTION: STBC 0x7CCE60
void NiD3DTexturePipeline::CheckTextureCapabilities() {
	if ( deviceDesc.dpcTriCaps.dwTextureFilterCaps & ( D3DPTFILTERCAPS_MINFPOINT | D3DPTFILTERCAPS_MINFLINEAR | D3DPTFILTERCAPS_MINFANISOTROPIC
		| D3DPTFILTERCAPS_MIPFPOINT | D3DPTFILTERCAPS_MIPFLINEAR | D3DPTFILTERCAPS_MAGFPOINT | D3DPTFILTERCAPS_MAGFLINEAR
		| D3DPTFILTERCAPS_MAGFANISOTROPIC | D3DPTFILTERCAPS_MAGFAFLATCUBIC | D3DPTFILTERCAPS_MAGFGAUSSIANCUBIC ) ) {
		SetFilterModesDX6();
	} else {
		SetFilterModesDX5();
	}

	if ( deviceDesc.dpcTriCaps.dwTextureAddressCaps & D3DPTADDRESSCAPS_INDEPENDENTUV ) {
		SetFlag( INDEPENDENT_UV );
	} else {
		ClearFlag( INDEPENDENT_UV );
	}

	if ( deviceDesc.dpcTriCaps.dwTextureBlendCaps & D3DPTBLENDCAPS_DECALALPHA ) {
		SetFlag( DECAL_ALPHA );
	} else {
		ClearFlag( DECAL_ALPHA );
	}

	if ( deviceDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_PROJECTED ) {
		SetFlag( PROJECTED );
		textureTransformFlags = D3DTTFF_COUNT3 | D3DTTFF_PROJECTED;
	} else {
		textureTransformFlags = D3DTTFF_COUNT2;
		ClearFlag( PROJECTED );
	}
}

/*
================
NiD3DTexturePipeline::SetFilterModesDX5

Fills the filter table from the DirectX 5 filter capabilities: bilinear
modes fall back to point sampling, and the mipmap modes to the nearest
mipmap mode the device has.
================
*/
// FUNCTION: STBC 0x7CCEF0
void NiD3DTexturePipeline::SetFilterModesDX5() {
	filterModes[0][1] = D3DTFG_POINT;
	filterModes[3][1] = D3DTFG_POINT;
	filterModes[4][1] = D3DTFG_POINT;
	filterModes[0][0] = D3DTFN_POINT;
	filterModes[3][0] = D3DTFN_POINT;
	filterModes[4][0] = D3DTFN_POINT;
	filterModes[0][2] = D3DTFP_NONE;
	filterModes[1][2] = D3DTFP_NONE;

	if ( deviceDesc.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_LINEAR ) {
		filterModes[2][1] = D3DTFG_LINEAR;
		filterModes[5][1] = D3DTFG_LINEAR;
		filterModes[1][1] = D3DTFG_LINEAR;
		filterModes[2][0] = D3DTFN_LINEAR;
		filterModes[5][0] = D3DTFN_LINEAR;
		filterModes[1][0] = D3DTFN_LINEAR;
	} else if ( deviceDesc.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_NEAREST ) {
		filterModes[2][1] = D3DTFG_POINT;
		filterModes[5][1] = D3DTFG_POINT;
		filterModes[1][1] = D3DTFG_POINT;
		filterModes[2][0] = D3DTFN_POINT;
		filterModes[5][0] = D3DTFN_POINT;
		filterModes[1][0] = D3DTFN_POINT;
	} else {
		NiD3DRender::Warning( "(Creation): Device does not support texture filtering" );
		assert( 0 );
	}

	if ( deviceDesc.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_MIPNEAREST ) {
		filterModes[3][2] = D3DTFP_POINT;
	} else {
		filterModes[3][2] = D3DTFP_NONE;
	}
	if ( deviceDesc.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_MIPLINEAR ) {
		filterModes[5][2] = D3DTFP_POINT;
	} else {
		filterModes[5][2] = D3DTFP_NONE;
	}
	if ( deviceDesc.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_LINEARMIPNEAREST ) {
		filterModes[4][2] = D3DTFP_LINEAR;
	} else {
		filterModes[4][2] = filterModes[5][2];
	}
	if ( deviceDesc.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_LINEARMIPLINEAR ) {
		filterModes[2][2] = D3DTFP_LINEAR;
	} else {
		filterModes[2][2] = filterModes[5][2];
	}
}

/*
================
NiD3DTexturePipeline::SetFilterModesDX6

Fills the filter table from the DirectX 6 filter capabilities, which give
the magnification, minification and mipmap filters apart.
================
*/
// FUNCTION: STBC 0x7CD000
void NiD3DTexturePipeline::SetFilterModesDX6() {
	filterModes[0][1] = D3DTFG_POINT;
	filterModes[3][1] = D3DTFG_POINT;
	filterModes[4][1] = D3DTFG_POINT;
	filterModes[0][0] = D3DTFN_POINT;
	filterModes[3][0] = D3DTFN_POINT;
	filterModes[4][0] = D3DTFN_POINT;
	filterModes[0][2] = D3DTFP_NONE;
	filterModes[1][2] = D3DTFP_NONE;

	if ( deviceDesc.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_MAGFLINEAR ) {
		filterModes[2][1] = D3DTFG_LINEAR;
		filterModes[5][1] = D3DTFG_LINEAR;
		filterModes[1][1] = D3DTFG_LINEAR;
	} else if ( deviceDesc.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_MAGFPOINT ) {
		filterModes[2][1] = D3DTFG_POINT;
		filterModes[5][1] = D3DTFG_POINT;
		filterModes[1][1] = D3DTFG_POINT;
	} else {
		NiD3DRender::Warning( "(Creation): Device does not support texture filtering" );
		assert( 0 );
	}

	if ( deviceDesc.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_MINFLINEAR ) {
		filterModes[2][0] = D3DTFN_LINEAR;
		filterModes[5][0] = D3DTFN_LINEAR;
		filterModes[1][0] = D3DTFN_LINEAR;
	} else if ( deviceDesc.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_MINFPOINT ) {
		filterModes[2][0] = D3DTFN_POINT;
		filterModes[5][0] = D3DTFN_POINT;
		filterModes[1][0] = D3DTFN_POINT;
	} else {
		NiD3DRender::Warning( "(Creation): Device does not support texture filtering" );
		assert( 0 );
	}

	if ( deviceDesc.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR ) {
		filterModes[5][2] = D3DTFP_POINT;
		filterModes[3][2] = D3DTFP_POINT;
		filterModes[2][2] = D3DTFP_LINEAR;
		filterModes[4][2] = D3DTFP_LINEAR;
	} else if ( deviceDesc.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_MIPFPOINT ) {
		filterModes[2][2] = D3DTFP_POINT;
		filterModes[4][2] = D3DTFP_POINT;
		filterModes[5][2] = D3DTFP_POINT;
		filterModes[3][2] = D3DTFP_POINT;
	} else {
		filterModes[2][2] = D3DTFP_NONE;
		filterModes[4][2] = D3DTFP_NONE;
		filterModes[5][2] = D3DTFP_NONE;
		filterModes[3][2] = D3DTFP_NONE;
	}
}

/*
================
NiD3DTexturePipeline::AddStage
================
*/
// FUNCTION: STBC 0x7CD140
bool NiD3DTexturePipeline::AddStage( NiD3DTextureStage *stage ) {
	if ( unknown4FC >= unknown354 || !stage ) {
		return false;
	}
	unknown4DC[unknown4FC++] = stage;
	unknown504[unknown528++] = 1;
	return true;
}

/*
================
NiD3DTexturePipeline::Unknown7CD1A0

Adds two stages to unknown4DC: in one pass when the device can draw them
together (as Unknown7CD350 tests), else in a pass each; only the first
when a single stage is left. Returns how many stages were added.
The parameters are named by AnimOnly.exe 0x5252D8's assert (pTex0, pTex1);
its ways out of the pair test are the gotos of Unknown7CD350.
================
*/
// FUNCTION: STBC 0x7CD1A0
unsigned int NiD3DTexturePipeline::Unknown7CD1A0( NiD3DTextureStage *tex0, NiD3DTextureStage *tex1 ) {
	unsigned char passes;

	assert( tex0 && tex1 );
	if ( unknown4FC >= unknown354 ) {
		return 0;
	}
	if ( unknown4FC >= unknown354 - 1 ) {
		unknown4DC[unknown4FC++] = tex0;
		unknown504[unknown528++] = 1;
		return 1;
	}
	if ( IsFlagSet( SEPARATE_MEMORIES ) ) {
		if ( tex0->image && NiIsExactKindOf( NiDDBufferImage, tex0->image ) ) {
			goto separate;
		}
		if ( NiIsExactKindOf( NiDDBufferImage, tex1->image ) ) {
			goto separate;
		}
	}
	passes = multiTexturePasses[tex1->blend][tex0->unknown04][tex1->unknown04];
	if ( IsFlagClear( INDEPENDENT_ADDRESSING ) ) {
		if ( tex0->addressU != tex1->addressU || tex0->addressV != tex1->addressV ) {
			goto separate;
		}
	}
	if ( passes & SINGLE_PASS ) {
		unknown504[unknown528++] = 2;
		unknown4DC[unknown4FC++] = tex0;
		unknown4DC[unknown4FC] = tex1;
		unknown4FC++;
		return 2;
	}
separate:
	unknown504[unknown528++] = 1;
	unknown4DC[unknown4FC++] = tex0;
	unknown504[unknown528++] = 1;
	unknown4DC[unknown4FC] = tex1;
	unknown4FC++;
	return 2;
}

/*
================
NiD3DTexturePipeline::Unknown7CD350

Adds one or two stages to unknown4DC: two stages go into one pass when the
device can draw them together (their textures in one memory, their
addressing alike unless the stages address apart, and their pixel formats
a single pass of the second stage's multitexture mode), else each takes a
pass of its own; with onlyPair the second stage isn't added alone. Returns
which stages were added: 1 the first, 2 the second, 3 both, 0 none.
The three ways out of the pair test are gotos, as AnimOnly.exe 0x525613
(D3DTexturePipeline.cpp) shows them (its jump stubs after the returns).
================
*/
// FUNCTION: STBC 0x7CD350
unsigned int NiD3DTexturePipeline::Unknown7CD350( NiD3DTextureStage *stage0, NiD3DTextureStage *stage1, bool onlyPair ) {
	unsigned char passes;

	if ( unknown4FC >= unknown354 ) {
		return 0;
	}
	if ( !stage1 ) {
		if ( stage0 ) {
			unknown4DC[unknown4FC++] = stage0;
			unknown504[unknown528++] = 1;
			return 1;
		}
		return 0;
	}
	if ( !stage0 ) {
		if ( onlyPair ) {
			return 0;
		}
		unknown4DC[unknown4FC++] = stage1;
		unknown504[unknown528++] = 1;
		return 2;
	}
	if ( unknown4FC >= unknown354 - 1 ) {
		unknown4DC[unknown4FC++] = stage0;
		unknown504[unknown528++] = 1;
		return 1;
	}
	if ( IsFlagSet( SEPARATE_MEMORIES ) ) {
		if ( stage0->image && NiIsExactKindOf( NiDDBufferImage, stage0->image ) ) {
			goto separate;
		}
		if ( NiIsExactKindOf( NiDDBufferImage, stage1->image ) ) {
			goto separate;
		}
	}
	passes = multiTexturePasses[stage1->blend][stage0->unknown04][stage1->unknown04];
	if ( IsFlagClear( INDEPENDENT_ADDRESSING ) ) {
		if ( stage0->addressU != stage1->addressU || stage0->addressV != stage1->addressV ) {
			goto separate;
		}
	}
	if ( passes & SINGLE_PASS ) {
		unknown504[unknown528++] = 2;
		unknown4DC[unknown4FC++] = stage0;
		unknown4DC[unknown4FC] = stage1;
		unknown4FC++;
		return 3;
	}
separate:
	unknown504[unknown528++] = 1;
	unknown4DC[unknown4FC++] = stage0;
	if ( !onlyPair ) {
		unknown504[unknown528++] = 1;
		unknown4DC[unknown4FC] = stage1;
		unknown4FC++;
		return 3;
	}
	return 1;
}

/*
================
NiD3DTexturePipeline::Unknown7CD580

Takes the next stage of unknown530 for an image the texture manager can
take, with the addressing and filtering of the given modes.
================
*/
// FUNCTION: STBC 0x7CD580
NiD3DTextureStage *NiD3DTexturePipeline::Unknown7CD580( NiImage *image, NiTextureModeProperty::ClampMode clamp,
	NiTextureModeProperty::FilterMode filter, unsigned int blend, unsigned int texCoordIndex, const void *source ) {
	if ( !image ) {
		return NULL;
	}
	unsigned int texture = textureManager->VirtualSlot01( image );
	if ( texture != 7 ) {
		NiD3DTextureStage *stage = &unknown530[unknown870++];
		stage->unknown04 = texture;
		stage->image = image;
		SetAddressModes( clamp, stage );
		SetFilterMode( filter, stage );
		stage->blend = blend;
		stage->texCoordIndex = texCoordIndex;
		stage->unknown58 = false;
		stage->unknown60 = false;
		stage->source = source;
		return stage;
	}
	return NULL;
}

/*
================
NiD3DTexturePipeline::Unknown7CD640

Sets up the passes for an object's properties and effects: the base
texture alone when the object has no multi-texture maps and no effects,
else the dark map with the base texture and the glow alpha or detail map,
or the projected lights with the base texture, then the decal maps, the
projected shadows, the environment map with the gloss map, or the glow
map, and the fog map. Each map uses its own texture coordinate set when
the object has it, else the last one. True when the object takes more
than one pass and none of them is a clipped effect's.
================
*/
// FUNCTION: STBC 0x7CD640
bool NiD3DTexturePipeline::Unknown7CD640( const NiPropertyState &state, const NiDynamicEffectState *effects, bool normals, unsigned int textureSets ) {
	bool result = true;
	unsigned int i;

	Unknown7CFB50( effects );

	NiMultiTextureProperty *multiTexture = state.GetMultiTexture();
	unknown870 = 0;
	unknown500 = 0;
	unknown528 = 0;
	unknown52C = 0;

	NiTextureProperty *texture = state.GetTexture();
	NiTextureModeProperty *mode = state.GetTextureMode();
	// a second pointer to the property, read for its maps: the debug copy
	// (AnimOnly.exe 0x525BDD) copies it into a local of its own (name invented)
	NiMultiTextureProperty *maps = multiTexture;
	NiImage *image = texture->GetImage();
	unknown0F8 = mode->GetApplyMode();
	bool vertexColor = mode->GetApplyMode() != NiTextureModeProperty::APPLY_REPLACE;
	NiD3DTextureStage *baseStage = Unknown7CD580( image, mode->GetClampMode(), mode->GetFilterMode(),
		BLEND_MULTIPLY, 0, texture );
	unknown4FC = 0;

	if ( unknown8DC || multiTexture != NiMultiTextureProperty::GetDefault() ) {
		unsigned int lastSet = textureSets ? textureSets - 1 : 0;
		NiD3DTextureStage *darkStage = NULL;
		NiD3DTextureStage *detailStage = NULL;
		NiD3DTextureStage *glowStage = NULL;
		NiD3DTextureStage *glowAlphaStage = NULL;
		NiMultiTextureProperty::Map *gloss;
		NiMultiTextureProperty::Map *glow;
		unsigned int decals;
		if ( textureSets ) {
			decals = maps->GetDecalMapCount();
			NiMultiTextureProperty::Map *dark = maps->GetDarkMap();
			gloss = maps->GetGlossMap();
			glow = maps->GetGlowMap();
			NiMultiTextureProperty::Map *glowAlpha = maps->GetGlowAlphaMap();
			NiMultiTextureProperty::Map *detail = maps->GetDetailMap();
			if ( detail ) {
				unsigned int texCoord = detail->GetTextureIndex();
				detailStage = Unknown7CD580( detail->GetImage(), detail->GetClampMode(), detail->GetFilterMode(), 4,
					texCoord < textureSets ? texCoord : lastSet, detail );
			}
			if ( glow ) {
				unsigned int texCoord = glow->GetTextureIndex();
				glowStage = Unknown7CD580( glow->GetImage(), glow->GetClampMode(), glow->GetFilterMode(), 0,
					texCoord < textureSets ? texCoord : lastSet, glow );
			}
			if ( glowAlpha ) {
				unsigned int texCoord = glowAlpha->GetTextureIndex();
				glowAlphaStage = Unknown7CD580( glowAlpha->GetImage(), glowAlpha->GetClampMode(), glowAlpha->GetFilterMode(), 1,
					texCoord < textureSets ? texCoord : lastSet, glowAlpha );
			}
			if ( dark ) {
				unsigned int texCoord = dark->GetTextureIndex();
				darkStage = Unknown7CD580( dark->GetImage(), dark->GetClampMode(), dark->GetFilterMode(), 3,
					texCoord < textureSets ? texCoord : lastSet, dark );
			}
		} else {
			decals = 0;
			gloss = NULL;
			glow = NULL;
		}

		unknown4FC = 0;
		if ( !unknownC20 && !unknown12A8 ) {
			if ( glowAlphaStage ) {
				AddStage( darkStage );
				unsigned int added = Unknown7CD350( baseStage, glowAlphaStage, true );
				if ( !added ) {
					Unknown7CD350( &unknown874, glowAlphaStage, false );
				} else if ( added & 1 ) {
					baseStage->unknown60 = vertexColor;
				}
			} else if ( detailStage ) {
				AddStage( darkStage );
				if ( !Unknown7CD350( baseStage, detailStage, true ) ) {
					AddStage( &unknown874 );
				} else {
					baseStage->unknown60 = vertexColor;
				}
			} else {
				unsigned int added = Unknown7CD350( baseStage, darkStage, false );
				if ( added & 1 ) {
					baseStage->unknown60 = vertexColor;
				} else if ( added == 2 ) {
					darkStage->unknown60 = vertexColor;
				} else {
					AddStage( &unknown874 );
				}
			}
		} else {
			unknown354 = 7;
			if ( unknownC20 ) {
				i = 0;
				if ( darkStage ) {
					darkStage->blend = BLEND_ADD;
					Unknown7CD350( darkStage, &unknown8E0[0], false );
					i = 1;
				}
				unsigned int last = unknownC20 - 1;
				for ( ; i < last; i += 2 ) {
					Unknown7CD1A0( &unknown8E0[i], &unknown8E0[i + 1] );
				}
				if ( i < unknownC20 ) {
					AddStage( &unknown8E0[i] );
				}
			} else if ( darkStage ) {
				darkStage->blend = BLEND_ADD;
				AddStage( darkStage );
			}
			for ( i = 0; i < unknown12A8; i++ ) {
				result = false;
				Unknown7CD350( &unknownC28[i], &unknownF68[i], true );
			}
			unknown354 = 8;
			if ( !Unknown7CD350( baseStage, detailStage, true ) ) {
				AddStage( &unknown874 );
			} else {
				baseStage->unknown60 = vertexColor;
			}
		}

		for ( i = 0; i < decals; i++ ) {
			NiMultiTextureProperty::Map *decal = maps->GetDecalMap( i );
			if ( decal ) {
				unsigned int texCoord = decal->GetTextureIndex();
				NiD3DTextureStage *stage = Unknown7CD580( decal->GetImage(), decal->GetClampMode(), decal->GetFilterMode(), 2,
					texCoord < textureSets ? texCoord : lastSet, decal );
				if ( stage ) {
					stage->unknown60 = vertexColor;
				}
				AddStage( stage );
			}
		}

		if ( unknown1C70 ) {
			unsigned int last = unknown1C70 - 1;
			for ( i = 0; i < last; i += 2 ) {
				Unknown7CD1A0( &unknown1930[i], &unknown1930[i + 1] );
			}
			if ( i < unknown1C70 ) {
				AddStage( &unknown1930[i] );
			}
		}
		for ( i = 0; i < unknown192C; i++ ) {
			result = false;
			Unknown7CD350( &unknown12AC[i], &unknown15EC[i], true );
		}

		if ( unknown1CDC && normals ) {
			if ( gloss ) {
				unsigned int texCoord = gloss->GetTextureIndex();
				NiD3DTextureStage *glossStage = Unknown7CD580( gloss->GetImage(), gloss->GetClampMode(), gloss->GetFilterMode(), 3,
					texCoord < textureSets ? texCoord : lastSet, gloss );
				Unknown7CD350( &unknown1C74, glossStage, true );
				if ( glowStage ) {
					AddStage( glowStage );
				}
			} else if ( glowStage ) {
				Unknown7CD350( glowStage, &unknown1C74, false );
			} else {
				AddStage( &unknown1C74 );
			}
		} else if ( glow ) {
			AddStage( glowStage );
		}
		if ( unknown1D48 ) {
			AddStage( &unknown1CE0 );
		}

		if ( unknown4FC ) {
			if ( unknown4FC < 8 ) {
				unknown4DC[unknown4FC] = NULL;
			}
		} else {
			AddStage( &unknown874 );
			unknown4DC[1] = NULL;
		}
	} else {
		if ( !AddStage( baseStage ) ) {
			AddStage( &unknown874 );
		} else {
			baseStage->unknown60 = vertexColor;
		}
		unknown4DC[1] = NULL;
	}

	unknown52C = unknown528;
	unknown500 = 0;
	unknown528 = 0;
	return unknown52C > 1 && result ? true : false;
}

/*
================
NiD3DTexturePipeline::Unknown7CE040

Sets up the next pass: its blending after the first pass, then one stage
(the stage's texture on device stage 0) or two (stage 1 on with the second
stage's multitexture mode); with the vertex color last, the vertex color
goes to stage 2. True while passes remain.
The asserts and the vertex color's ternary are AnimOnly.exe 0x526607's
(D3DTexturePipeline.cpp).
================
*/
// FUNCTION: STBC 0x7CE040
bool NiD3DTexturePipeline::Unknown7CE040() {
	assert( !unknown528 || unknown504[unknown528] );
	if ( unknown504[unknown528] > 1 ) {
		assert( unknown504[unknown528] == 2 );
		Unknown7CE750();
		bool vertexColor = ( unknown4DC[unknown500]->unknown60 || unknown4DC[unknown500 + 1]->unknown60 ) ? true : false;
		Unknown7CE640( unknown4DC[unknown500 + 1]->blend );
		SetStageTextures( unknown500, unknown500 + 1 );
		if ( IsFlagSet( VERTEX_COLOR_LAST ) ) {
			Unknown7CE780( 0, stageStates[0].texture != NULL, vertexColor );
			Unknown7CE580( unknown0F8, vertexColor );
		} else {
			Unknown7CE780( unknown0F8, stageStates[0].texture != NULL, vertexColor );
		}
		unknown500 += 2;
	} else {
		Unknown7CE6A0();
		Unknown7CE750();
		bool vertexColor = unknown4DC[unknown500]->unknown60;
		Unknown7CE4E0( unknown500 );
		Unknown7CE780( unknown0F8, stageStates[0].texture != NULL, vertexColor );
		unknown500++;
	}
	unknown528++;
	if ( unknown500 < unknown4FC ) {
		return true;
	}
	return false;
}

/*
================
NiD3DTexturePipeline::ApplyTextureCoordinates

Sets a device stage's texture coordinates to a pass stage's: the stage's
texture transform when it generates camera-space coordinates, none
otherwise, and its coordinate index.
================
*/
// FUNCTION: STBC 0x7CE2A0
void NiD3DTexturePipeline::ApplyTextureCoordinates( unsigned int stage, unsigned int index ) {
	DWORD texCoordIndex = unknown4DC[index]->texCoordIndex;
	if ( texCoordIndex == D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR || texCoordIndex == D3DTSS_TCI_CAMERASPACEPOSITION ) {
		device->SetTransform( (D3DTRANSFORMSTATETYPE)( D3DTRANSFORMSTATE_TEXTURE0 + stage ), &unknown4DC[index]->transform );
		if ( unknown4DC[index]->unknown58 ) {
			if ( stageStates[stage].transformMode != 2 ) {
				stageStates[stage].transformMode = 2;
				device->SetTextureStageState( stage, D3DTSS_TEXTURETRANSFORMFLAGS, textureTransformFlags );
			}
		} else if ( stageStates[stage].transformMode != 1 ) {
			stageStates[stage].transformMode = 1;
			device->SetTextureStageState( stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );
		}
	} else if ( stageStates[stage].transformMode != 0 ) {
		stageStates[stage].transformMode = 0;
		device->SetTextureStageState( stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
	}

	if ( texCoordIndex != stageStates[stage].texCoordIndex ) {
		stageStates[stage].texCoordIndex = texCoordIndex;
		device->SetTextureStageState( stage, D3DTSS_TEXCOORDINDEX, texCoordIndex );
	}
}

/*
================
NiD3DTexturePipeline::SetStageTextures

Sets the textures of two pass stages on device stages 0 and 1, then the
coordinates, filtering and addressing of each stage that has a texture.
Stage 1 filters by the second pass stage only when it can filter
trilinearly in a single pass, by the first one otherwise.
================
*/
// FUNCTION: STBC 0x7CE390
void NiD3DTexturePipeline::SetStageTextures( unsigned int index0, unsigned int index1 ) {
	IDirectDrawSurface7 *surfaces[2];
	bool newTexture0;	// (name invented; fits the /Od frame order)
	bool newTexture1;	// (name invented; fits the /Od frame order)
	bool hasMips0;	// (name invented; fits the /Od frame order)
	bool hasMips1;	// (name invented; fits the /Od frame order)

	surfaces[0] = NULL;
	surfaces[1] = NULL;

	if ( !textureManager->VirtualSlot06( 0, unknown4DC[index0]->image, stageStates[0].texture, surfaces[0], newTexture0, hasMips0 ) ) {
		surfaces[0] = NULL;
	}
	if ( !textureManager->VirtualSlot06( 1, unknown4DC[index1]->image, stageStates[1].texture, surfaces[1], newTexture1, hasMips1 ) ) {
		surfaces[1] = NULL;
	}

	if ( newTexture0 ) {
		device->SetTexture( 0, surfaces[0] );
	}
	if ( surfaces[0] ) {
		ApplyTextureCoordinates( 0, index0 );
		ApplyFilterMode( hasMips0, IsFlagSet( TRILINEAR ), 0, index0 );
		ApplyAddressModes( 0, index0 );
	}

	if ( newTexture1 ) {
		device->SetTexture( 1, surfaces[1] );
	}
	if ( surfaces[1] ) {
		// stage 1 filters as the first pass stage unless it filters
		// trilinearly in a single pass
		unsigned int filterIndex = IsFlagSet( STAGE1_TRILINEAR ) ? index1 : index0;
		ApplyTextureCoordinates( 1, index1 );
		ApplyFilterMode( hasMips1, IsFlagSet( TRILINEAR ), 1, filterIndex );
		ApplyAddressModes( 1, index1 );
	}
}

/*
================
NiD3DTexturePipeline::Unknown7CE4E0

Sets the texture of one pass stage on device stage 0, then its
coordinates, filtering and addressing when it has a texture: the
single-stage form of SetStageTextures.
================
*/
// FUNCTION: STBC 0x7CE4E0
void NiD3DTexturePipeline::Unknown7CE4E0( unsigned int index ) {
	IDirectDrawSurface7 *surface = NULL;
	bool changed;
	bool mipmapped;

	if ( !textureManager->VirtualSlot06( -1, unknown4DC[index]->image, stageStates[0].texture, surface, changed, mipmapped ) ) {
		surface = NULL;
	}
	if ( changed ) {
		device->SetTexture( 0, surface );
	}
	if ( surface ) {
		ApplyTextureCoordinates( 0, index );
		ApplyFilterMode( mipmapped, true, 0, index );
		ApplyAddressModes( 0, index );
	}
}

/*
================
NiD3DTexturePipeline::Unknown7CE580

Sets the third stage's color and alpha operations, from the texture
operations of the mode or to select the first argument, sending each to
the device only when it changed. The operations are written out, as in
AnimOnly.exe 0x526D37 (D3DTexturePipeline.cpp).
================
*/
// FUNCTION: STBC 0x7CE580
void NiD3DTexturePipeline::Unknown7CE580( unsigned int mode, bool unknown ) {
	if ( unknown ) {
		const NiD3DTextureOps &ops = textureOps[mode];
		if ( stageStates[2].colorOp != ops.colorOp ) {
			device->SetTextureStageState( 2, D3DTSS_COLOROP, ops.colorOp );
			stageStates[2].colorOp = ops.colorOp;
		}
		if ( stageStates[2].alphaOp != ops.alphaOp ) {
			device->SetTextureStageState( 2, D3DTSS_ALPHAOP, ops.alphaOp );
			stageStates[2].alphaOp = ops.alphaOp;
		}
	} else {
		if ( stageStates[2].colorOp != D3DTOP_SELECTARG1 ) {
			device->SetTextureStageState( 2, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
			stageStates[2].colorOp = D3DTOP_SELECTARG1;
		}
		if ( stageStates[2].alphaOp != D3DTOP_SELECTARG1 ) {
			device->SetTextureStageState( 2, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
			stageStates[2].alphaOp = D3DTOP_SELECTARG1;
		}
	}
}

/*
================
NiD3DTexturePipeline::Unknown7CE640
================
*/
// FUNCTION: STBC 0x7CE640
void NiD3DTexturePipeline::Unknown7CE640( unsigned int mode ) {
	if ( stageStates[1].colorOp == D3DTOP_DISABLE ) {
		device->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );
	}
	const NiD3DTextureOps &ops = multiTextureOps[mode];
	if ( stageStates[1].colorOp != ops.colorOp ) {
		device->SetTextureStageState( 1, D3DTSS_COLOROP, ops.colorOp );
		stageStates[1].colorOp = ops.colorOp;
	}

}

/*
================
NiD3DTexturePipeline::Unknown7CE6A0
================
*/
// FUNCTION: STBC 0x7CE6A0
void NiD3DTexturePipeline::Unknown7CE6A0() {
	if ( stageStates[1].colorOp != D3DTOP_DISABLE ) {
		stageStates[1].colorOp = D3DTOP_DISABLE;
		device->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
		device->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
		if ( stageStates[2].colorOp != D3DTOP_DISABLE ) {
			stageStates[2].colorOp = D3DTOP_DISABLE;
			device->SetTextureStageState( 2, D3DTSS_COLOROP, D3DTOP_DISABLE );
		}
		if ( stageStates[2].alphaOp != D3DTOP_DISABLE ) {
			stageStates[2].alphaOp = D3DTOP_DISABLE;
			device->SetTextureStageState( 2, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
		}
		device->SetTexture( 1, NULL );
		stageStates[1].texture = NULL;
	}
}

/*
================
NiD3DTexturePipeline::Unknown7CE750
================
*/
// FUNCTION: STBC 0x7CE750
void NiD3DTexturePipeline::Unknown7CE750() {
	if ( unknown500 ) {
		renderState->ApplyAlpha( alphaProperties[unknown4DC[unknown500]->blend] );
	}
}

/*
================
NiD3DTexturePipeline::Unknown7CE780
================
*/
// FUNCTION: STBC 0x7CE780
void NiD3DTexturePipeline::Unknown7CE780( unsigned int mode, bool textured, bool unknown ) {
	// AnimOnly.exe 0x5270BB (D3DTexturePipeline.cpp) writes each operation
	// out, as Unknown7CE580 does
	if ( textured ) {
		if ( unknown ) {
			const NiD3DTextureOps &ops = textureOps[mode];
			if ( stageStates[0].colorOp != ops.colorOp ) {
				device->SetTextureStageState( 0, D3DTSS_COLOROP, ops.colorOp );
				stageStates[0].colorOp = ops.colorOp;
			}
			if ( stageStates[0].alphaOp != ops.alphaOp ) {
				device->SetTextureStageState( 0, D3DTSS_ALPHAOP, ops.alphaOp );
				stageStates[0].alphaOp = ops.alphaOp;
			}
		} else if ( mode == NiTextureModeProperty::APPLY_MODULATE_VERTEX ) {
			if ( stageStates[0].colorOp != D3DTOP_SELECTARG1 ) {
				device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
				stageStates[0].colorOp = D3DTOP_SELECTARG1;
			}
			if ( stageStates[0].alphaOp != D3DTOP_MODULATE ) {
				device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
				stageStates[0].alphaOp = D3DTOP_MODULATE;
			}
		} else {
			if ( stageStates[0].colorOp != D3DTOP_SELECTARG1 ) {
				device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
				stageStates[0].colorOp = D3DTOP_SELECTARG1;
			}
			if ( stageStates[0].alphaOp != D3DTOP_SELECTARG1 ) {
				device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
				stageStates[0].alphaOp = D3DTOP_SELECTARG1;
			}
		}
	} else {
		if ( stageStates[0].colorOp != D3DTOP_SELECTARG2 ) {
			device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
			stageStates[0].colorOp = D3DTOP_SELECTARG2;
		}
		if ( stageStates[0].alphaOp != D3DTOP_SELECTARG2 ) {
			device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );
			stageStates[0].alphaOp = D3DTOP_SELECTARG2;
		}
		device->SetTexture( 0, NULL );
		stageStates[0].texture = NULL;
	}
}

/*
================
NiD3DTexturePipeline::GetBlendPasses

The cases are in retail's order (its block layout); each returns on its
own.
================
*/
// FUNCTION: STBC 0x7CE920
unsigned char NiD3DTexturePipeline::GetBlendPasses( unsigned int blend ) {
	switch ( blend ) {
	case BLEND_MULTIPLY:
		if ( ( renderState->GetSourceBlendModes() & ( 1 << NiAlphaProperty::ALPHA_ZERO ) ) && ( renderState->GetDestBlendModes() & ( 1 << NiAlphaProperty::ALPHA_SRCCOLOR ) ) ) {
			return 2;
		}
		return 0;
	case BLEND_ALPHA:
		if ( ( renderState->GetSourceBlendModes() & ( 1 << NiAlphaProperty::ALPHA_SRCALPHA ) ) && ( renderState->GetDestBlendModes() & ( 1 << NiAlphaProperty::ALPHA_INVSRCALPHA ) ) ) {
			return 2;
		}
		return 0;
	case BLEND_ADD:
		if ( ( renderState->GetSourceBlendModes() & ( 1 << NiAlphaProperty::ALPHA_ONE ) ) && ( renderState->GetDestBlendModes() & ( 1 << NiAlphaProperty::ALPHA_ONE ) ) ) {
			return 2;
		}
		return 0;
	case BLEND_ALPHA_ADD:
		if ( ( renderState->GetSourceBlendModes() & ( 1 << NiAlphaProperty::ALPHA_SRCALPHA ) ) && ( renderState->GetDestBlendModes() & ( 1 << NiAlphaProperty::ALPHA_ONE ) ) ) {
			return 2;
		}
		return 0;
	}
	return 0;
}

/*
================
NiD3DTexturePipeline::DrawTestTriangles

Two screen triangles, one of each winding, so that one of them is drawn
whatever the culling.
================
*/
inline void NiD3DTexturePipeline::DrawTestTriangles() {
	device->BeginScene();

	D3DTLVERTEX vertices[6];
	memset( vertices, 0, sizeof( vertices ) );
	vertices[1].sy = 2.0f;
	vertices[2].sx = 2.0f;
	vertices[4].sx = 2.0f;
	vertices[5].sy = 2.0f;
	vertices[0].rhw = vertices[1].rhw = vertices[2].rhw = vertices[3].rhw = vertices[4].rhw = vertices[5].rhw = 0.5f;
	vertices[0].sz = vertices[1].sz = vertices[2].sz = vertices[3].sz = vertices[4].sz = vertices[5].sz = 0.5f;
	device->DrawPrimitive( D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX, vertices, 6, D3DDP_WAIT );

	device->EndScene();
}

/*
================
NiD3DTexturePipeline::Unknown7CE9C0

Finds the passes each multitexture mode takes with each pair of texture
pixel formats: a mode the device validates in one pass is SINGLE_PASS,
any other takes the passes of its blending setup. Without a test, or on
a device with fewer than two stages, every mode takes its blend passes.
The test runs with the vertex color in the first stage, then in the last
one, and keeps the order that validates more single-pass modes. It also
checks trilinear filtering and independent addressing in a single pass.
================
*/
// FUNCTION: STBC 0x7CE9C0
bool NiD3DTexturePipeline::Unknown7CE9C0( bool test ) {
	ResetStageStates();
	SetFlag( TRILINEAR );
	ClearFlag( SEPARATE_MEMORIES );
	ClearFlag( VERTEX_COLOR_LAST );
	SetFlag( INDEPENDENT_ADDRESSING );
	SetFlag( STAGE1_TRILINEAR );
	device->SetTexture( 0, NULL );
	device->SetTexture( 1, NULL );
	if ( stageStates[0].colorOp != D3DTOP_SELECTARG2 ) {
		device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
		stageStates[0].colorOp = D3DTOP_SELECTARG2;
	}
	if ( stageStates[0].alphaOp != D3DTOP_SELECTARG2 ) {
		device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );
		stageStates[0].alphaOp = D3DTOP_SELECTARG2;
	}

	DWORD passes = 0;
	HRESULT result;
	if ( !( deviceDesc.dwTextureOpCaps & D3DTEXOPCAPS_SELECTARG2 ) ) {
		result = device->ValidateDevice( &passes );
		if ( result != D3D_OK ) {
			NiD3DRender::Warning( "(Creation): Device cannot support required SELECTARG2 mode" );
			NiD3DRender::Error( "Creation failed: 3D Device cannot support required capabilities" );
			return false;
		}
	}

	// the test needs two stages
	if ( deviceDesc.wMaxTextureBlendStages < 2 ) {
		goto noTest;
	}
	if ( deviceDesc.wMaxSimultaneousTextures < 2 ) {
		goto noTest;
	}
	if ( deviceDesc.dwDevCaps & D3DDEVCAPS_SEPARATETEXTUREMEMORIES ) {
		SetFlag( SEPARATE_MEMORIES );
	}
	if ( !test ) {
		goto noTest;
	}

	{
		// unused: the debug copy (AnimOnly.exe 0x527691) reads it too
		bool hardware = IsFlagSet( HARDWARE );

		// a test texture of each pixel format for each stage
		int memory0;
		int memory1;
		if ( IsFlagSet( SEPARATE_MEMORIES ) ) {
			memory0 = 0;
			memory1 = 1;
		} else {
			memory0 = -1;
			memory1 = -1;
		}
		unsigned int format;
		IDirectDrawSurface7 *textures0[8];
		IDirectDrawSurface7 *textures1[8];
		for ( format = 0; format < 7; format++ ) {
			textures0[format] = textureManager->VirtualSlot04( format, memory0 );
			textures1[format] = textureManager->VirtualSlot04( format, memory1 );
		}
		textures0[7] = NULL;
		textures1[7] = NULL;

		unsigned char modes[2][5][8][8];
		unsigned int singlePass[2];
		unsigned int multiPass[2];
		unsigned int invalid[2];
		singlePass[0] = 0;
		multiPass[0] = 0;
		invalid[0] = 0;
		singlePass[1] = 0;
		multiPass[1] = 0;
		invalid[1] = 0;

		stageStates[0].texCoordIndex = 0;
		device->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
		stageStates[1].texCoordIndex = 0;
		device->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );

		unsigned int order;
		unsigned int mode;
		unsigned int i;
		unsigned int j;
		for ( order = 0; order < 2; order++ ) {
			for ( mode = 0; mode < 5; mode++ ) {
				Unknown7CE640( mode );
				for ( i = 0; i < 8; i++ ) {
					for ( j = 0; j < 8; j++ ) {
						// the original tests stage 0's texture of the second
						// format, then sets stage 1's
						if ( textures0[i] && textures0[j] ) {
							device->SetTexture( 0, textures0[i] );
							device->SetTexture( 1, textures1[j] );
						} else {
							modes[order][mode][i][j] = 0;
							continue;
						}
						// unused: the debug copy (AnimOnly.exe 0x5279AC) computes it too
						bool alphaFormat = ( i == 4 || i == 3 || i == 5 );		// (name invented)
						if ( IsFlagSet( VERTEX_COLOR_LAST ) ) {
							Unknown7CE580( NiTextureModeProperty::APPLY_MODULATE, true );
							Unknown7CE780( NiTextureModeProperty::APPLY_REPLACE, true, true );
						} else {
							Unknown7CE780( NiTextureModeProperty::APPLY_MODULATE, true, true );
						}
						DrawTestTriangles();

						passes = 0;
						result = device->ValidateDevice( &passes );
						if ( result != D3D_OK ) {
							modes[order][mode][i][j] = GetBlendPasses( mode );
							if ( modes[order][mode][i][j] ) {
								multiPass[order]++;
							} else {
								invalid[order]++;
							}
						} else if ( passes > 1 ) {
							modes[order][mode][i][j] = GetBlendPasses( mode );
							if ( modes[order][mode][i][j] ) {
								multiPass[order]++;
							} else {
								invalid[order]++;
							}
						} else {
							if ( !singlePass[order] ) {
								// the first single pass: whether stage 1 filters
								// trilinearly too
								device->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTFG_LINEAR );
								device->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTFN_LINEAR );
								device->SetTextureStageState( 1, D3DTSS_MIPFILTER, D3DTFP_LINEAR );
								DrawTestTriangles();
								passes = 0;
								result = device->ValidateDevice( &passes );
								if ( result != D3D_OK ) {
									ClearFlag( STAGE1_TRILINEAR );
								}
								device->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTFG_POINT );
								device->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTFN_POINT );
								device->SetTextureStageState( 1, D3DTSS_MIPFILTER, D3DTFP_NONE );
							}
							modes[order][mode][i][j] = GetBlendPasses( mode ) | SINGLE_PASS;
							singlePass[order]++;
						}
					}
				}
			}

			if ( singlePass[order] ) {
				char message[256];
				if ( IsFlagSet( VERTEX_COLOR_LAST ) ) {
					sprintf( message, "(Creation): Device supports vertex-lastsingle pass multitexturing: %u modes", singlePass[order] );
				} else {
					sprintf( message, "(Creation): Device supports vertex-firstsingle pass multitexturing: %u modes", singlePass[order] );
				}
				NiD3DRender::Message( message );
			}

			if ( IsFlagClear( VERTEX_COLOR_LAST ) && deviceDesc.wMaxSimultaneousTextures < deviceDesc.wMaxTextureBlendStages ) {
				SetFlag( VERTEX_COLOR_LAST );
				NiD3DRender::Message( "(Creation): - trying single-pass vertex-last" );
				continue;
			}
			break;
		}

		if ( singlePass[0] > singlePass[1] ) {
			order = 0;
			ClearFlag( VERTEX_COLOR_LAST );
			NiD3DRender::Message( "(Creation): Device supports more single-pass vertex-first than vertex-last modes" );
		} else if ( singlePass[0] == singlePass[1] ) {
			order = 0;
			ClearFlag( VERTEX_COLOR_LAST );
			NiD3DRender::Message( "(Creation): Device supports an equal number ofsingle-pass vertex-first and vertex-last modes" );
		} else if ( !singlePass[0] && !singlePass[1] ) {
			order = 0;
			ClearFlag( VERTEX_COLOR_LAST );
			NiD3DRender::Message( "(Creation): Device supports no single-pass modes" );
		} else {
			order = 1;
			SetFlag( VERTEX_COLOR_LAST );
			NiD3DRender::Message( "(Creation): Device supports more single-pass vertex-last than vertex-first modes" );
		}
		memcpy( multiTexturePasses, modes[order], sizeof( multiTexturePasses ) );

		char message[256];
		sprintf( message, "(Creation): Number of available single-pass multitexture modes: %u", singlePass[order] );
		NiD3DRender::Message( message );
		sprintf( message, "(Creation): Number of available multi-pass multitexture modes: %u", multiPass[order] );
		NiD3DRender::Message( message );
		sprintf( message, "(Creation): Number of available invalid multitexture modes: %u", invalid[order] );
		NiD3DRender::Message( message );

		if ( textures0[1] && textures1[1] ) {
			device->SetTexture( 0, textures0[1] );
			device->SetTexture( 1, textures1[1] );
			if ( IsFlagSet( VERTEX_COLOR_LAST ) ) {
				Unknown7CE780( NiTextureModeProperty::APPLY_REPLACE, true, true );
			} else {
				Unknown7CE780( NiTextureModeProperty::APPLY_MODULATE, true, true );
			}
			device->SetTextureStageState( 2, D3DTSS_COLOROP, D3DTOP_DISABLE );
			stageStates[2].colorOp = D3DTOP_DISABLE;
			device->SetTextureStageState( 2, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
			stageStates[2].alphaOp = D3DTOP_DISABLE;

			// the stages addressed apart
			device->SetTextureStageState( 0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP );
			device->SetTextureStageState( 1, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP );
			for ( mode = 0; mode < 5; mode++ ) {
				if ( multiTexturePasses[mode][1][1] & SINGLE_PASS ) {
					Unknown7CE640( mode );
					DrawTestTriangles();
					result = device->ValidateDevice( &passes );
					if ( result != D3D_OK ) {
						ClearFlag( INDEPENDENT_ADDRESSING );
						break;
					}
				}
			}
			device->SetTextureStageState( 0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP );
			device->SetTextureStageState( 1, D3DTSS_ADDRESS, D3DTADDRESS_WRAP );

			// both stages filtered trilinearly
			if ( deviceDesc.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR ) {
				device->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTFG_LINEAR );
				device->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTFN_LINEAR );
				device->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTFP_LINEAR );
				device->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTFG_LINEAR );
				device->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTFN_LINEAR );
				device->SetTextureStageState( 1, D3DTSS_MIPFILTER, D3DTFP_LINEAR );
				for ( mode = 0; mode < 5; mode++ ) {
					if ( multiTexturePasses[mode][1][1] & SINGLE_PASS ) {
						Unknown7CE640( mode );
						DrawTestTriangles();
						result = device->ValidateDevice( &passes );
						if ( result != D3D_OK ) {
							ClearFlag( TRILINEAR );
							break;
						}
					}
				}
			}
		}

		if ( IsFlagClear( TRILINEAR ) ) {
			NiD3DRender::Message( "(Creation): Device cannot do trilerp in multitexture mode" );
		}
		if ( IsFlagClear( INDEPENDENT_ADDRESSING ) ) {
			NiD3DRender::Message( "(Creation): Device cannot do independent texture addressing inmultitexture mode" );
			NiD3DRender::Message( "(Creation): NI D3D Renderer will multipass these cases" );
		}

		for ( format = 0; format < 7; format++ ) {
			if ( textures0[format] ) {
				textures0[format]->Release();
			}
			if ( textures1[format] ) {
				textures1[format]->Release();
			}
		}
		device->SetTexture( 0, NULL );
		device->SetTexture( 1, NULL );
		device->SetTexture( 0, NULL );
		device->SetTexture( 1, NULL );

		ResetStageStates();
		return true;
	}

noTest:
	// every mode takes the passes of its blending setup
	for ( unsigned int mode = 0; mode < 5; mode++ ) {
		for ( unsigned int i = 0; i < 8; i++ ) {
			for ( unsigned int j = 0; j < 8; j++ ) {
				multiTexturePasses[mode][i][j] = GetBlendPasses( mode );
			}
		}
	}
	ResetStageStates();
	return true;
}

/*
================
NiD3DTexturePipeline::SetAddressModes
================
*/
// FUNCTION: STBC 0x7CF6E0
void NiD3DTexturePipeline::SetAddressModes( unsigned int clampMode, NiD3DTextureStage *stage ) {
	stage->addressU = addressModes[clampMode][0];
	stage->addressV = addressModes[clampMode][1];
}

/*
================
NiD3DTexturePipeline::ApplyAddressModes

Sets a device stage's addressing to a pass stage's, in both directions at
once when the device can't address them apart.
================
*/
// FUNCTION: STBC 0x7CF710
void NiD3DTexturePipeline::ApplyAddressModes( unsigned int stage, unsigned int index ) {
	DWORD addressU = unknown4DC[index]->addressU;
	DWORD addressV = unknown4DC[index]->addressV;
	if ( IsFlagSet( INDEPENDENT_UV ) ) {
		if ( stageStates[stage].addressU != addressU ) {
			stageStates[stage].addressU = addressU;
			device->SetTextureStageState( stage, D3DTSS_ADDRESSU, addressU );
		}
		if ( stageStates[stage].addressV != addressV ) {
			stageStates[stage].addressV = addressV;
			device->SetTextureStageState( stage, D3DTSS_ADDRESSV, addressV );
		}
	} else if ( stageStates[stage].addressU != addressU ) {
		stageStates[stage].addressU = addressU;
		stageStates[stage].addressV = addressU;
		device->SetTextureStageState( stage, D3DTSS_ADDRESS, addressU );
	}
}

/*
================
NiD3DTexturePipeline::SetFilterMode
================
*/
// FUNCTION: STBC 0x7CF7C0
void NiD3DTexturePipeline::SetFilterMode( unsigned int filterMode, NiD3DTextureStage *stage ) {
	stage->filterMode = filterMode;
}

/*
================
NiD3DTexturePipeline::ApplyFilterMode

Sets a device stage's filtering to a pass stage's filter mode: without
mipmaps the mipmapped modes fall back to their plain filter, and without
trilinear filtering the modes that blend mipmaps take the nearest one.
================
*/
// FUNCTION: STBC 0x7CF7D0
void NiD3DTexturePipeline::ApplyFilterMode( bool mipmapped, bool trilinear, unsigned int stage, unsigned int index ) {
	unsigned int filterMode = unknown4DC[index]->filterMode;
	if ( mipmapped ) {
		if ( !trilinear ) {
			if ( filterMode == NiTextureModeProperty::FILTER_TRILERP ) {
				filterMode = NiTextureModeProperty::FILTER_BILERP_MIPNEAREST;
			} else if ( filterMode == NiTextureModeProperty::FILTER_NEAREST_MIPLERP ) {
				filterMode = NiTextureModeProperty::FILTER_NEAREST_MIPNEAREST;
			}
		}
	} else if ( filterMode == NiTextureModeProperty::FILTER_NEAREST_MIPNEAREST || filterMode == NiTextureModeProperty::FILTER_NEAREST_MIPLERP ) {
		filterMode = NiTextureModeProperty::FILTER_NEAREST;
	} else if ( filterMode == NiTextureModeProperty::FILTER_BILERP_MIPNEAREST || filterMode == NiTextureModeProperty::FILTER_TRILERP ) {
		filterMode = NiTextureModeProperty::FILTER_BILERP;
	}

	if ( stageStates[stage].filterMode != filterMode ) {
		stageStates[stage].filterMode = filterMode;
		device->SetTextureStageState( stage, D3DTSS_MAGFILTER, filterModes[filterMode][1] );
		device->SetTextureStageState( stage, D3DTSS_MINFILTER, filterModes[filterMode][0] );
		device->SetTextureStageState( stage, D3DTSS_MIPFILTER, filterModes[filterMode][2] );
	}
}

/*
================
NiD3DTexturePipeline::Unknown7CF8A0

Sets a stage up for a texture effect: its image, addressing and filtering,
and its coordinates through the setup of the effect's coordinate mode.
================
*/
// FUNCTION: STBC 0x7CF8A0
bool NiD3DTexturePipeline::Unknown7CF8A0( NiTextureEffect *effect, NiD3DTextureStage *stage ) {
	if ( unknown8DC >= 8 ) {
		return false;
	}
	if ( !effect->GetSwitch() ) {
		return false;
	}
	NiImage *image = effect->GetEffectTexture();
	if ( !image ) {
		return false;
	}
	unsigned int texture = textureManager->VirtualSlot01( image );
	if ( texture == 7 ) {
		return false;
	}

	stage->texCoordIndex = 1;
	( this->*coordGenFunctions[effect->GetTextureCoordGen()] )( effect->GetWorldProjectionMatrix(), effect->GetWorldProjectionTranslation(), stage );
	unknown8DC++;
	stage->image = image;
	stage->source = effect;
	stage->unknown04 = texture;
	stage->unknown60 = false;
	SetFilterMode( effect->GetTextureFilter(), stage );
	SetAddressModes( effect->GetTextureClamp(), stage );
	return true;
}

/*
================
NiD3DTexturePipeline::Unknown7CF9A0

The next stage of unknownF68 takes the effect's clipping plane, moved
into camera space, as its u coordinate: the distance from the plane, a
half texel row in front of it.
================
*/
// FUNCTION: STBC 0x7CF9A0
bool NiD3DTexturePipeline::Unknown7CF9A0( NiTextureEffect *effect ) {
	NiD3DTextureStage *stage = &unknownF68[unknown12A8];
	const NiPlane &plane = effect->GetWorldClippingPlane();
	NiPoint3 normal = plane.GetNormal();
	float constant = plane.GetConstant();

	stage->transform._11 = unknown1D4C.GetEntry( 0, 0 ) * normal.x + unknown1D4C.GetEntry( 1, 0 ) * normal.y + unknown1D4C.GetEntry( 2, 0 ) * normal.z;
	stage->transform._21 = unknown1D4C.GetEntry( 0, 1 ) * normal.x + unknown1D4C.GetEntry( 1, 1 ) * normal.y + unknown1D4C.GetEntry( 2, 1 ) * normal.z;
	stage->transform._31 = unknown1D4C.GetEntry( 0, 2 ) * normal.x + unknown1D4C.GetEntry( 1, 2 ) * normal.y + unknown1D4C.GetEntry( 2, 2 ) * normal.z;
	stage->transform._41 = normal.x * unknown1D70.x + normal.y * unknown1D70.y + normal.z * unknown1D70.z + 0.5f - constant;
	unknown8DC++;
	stage->source = effect;
	return true;
}

/*
================
NiD3DTexturePipeline::Unknown7CFA70

As Unknown7CF9A0, for the next stage of unknown15EC and the plane facing
the other way.
================
*/
// FUNCTION: STBC 0x7CFA70
bool NiD3DTexturePipeline::Unknown7CFA70( NiTextureEffect *effect ) {
	NiD3DTextureStage *stage = &unknown15EC[unknown192C];
	const NiPlane &plane = effect->GetWorldClippingPlane();
	NiPoint3 normal = -plane.GetNormal();
	float constant = -plane.GetConstant();

	stage->transform._11 = unknown1D4C.GetEntry( 0, 0 ) * normal.x + unknown1D4C.GetEntry( 1, 0 ) * normal.y + unknown1D4C.GetEntry( 2, 0 ) * normal.z;
	stage->transform._21 = unknown1D4C.GetEntry( 0, 1 ) * normal.x + unknown1D4C.GetEntry( 1, 1 ) * normal.y + unknown1D4C.GetEntry( 2, 1 ) * normal.z;
	stage->transform._31 = unknown1D4C.GetEntry( 0, 2 ) * normal.x + unknown1D4C.GetEntry( 1, 2 ) * normal.y + unknown1D4C.GetEntry( 2, 2 ) * normal.z;
	stage->transform._41 = normal.x * unknown1D70.x + normal.y * unknown1D70.y + normal.z * unknown1D70.z + 0.5f - constant;
	unknown8DC++;
	stage->source = effect;
	return true;
}

/*
================
NiD3DTexturePipeline::Unknown7CFB50

Sets the effect stages up for an effect state: each projected light and
shadow takes a stage, a clipping one also a clipping plane stage, and the
environment and fog maps take their own stages.
================
*/
// FUNCTION: STBC 0x7CFB50
void NiD3DTexturePipeline::Unknown7CFB50( const NiDynamicEffectState *effects ) {
	if ( effects == effectState ) {
		return;
	}

	unknownC20 = 0;
	unknown12A8 = 0;
	unknown1C70 = 0;
	unknown192C = 0;
	unknown1CDC = false;
	unknown1D48 = false;
	unknown8DC = 0;
	if ( !effects ) {
		return;
	}
	effectState = effects;

	NiDynamicEffectState::ListItem *pos = effects->GetProjLightHeadPos();
	while ( pos ) {
		NiTextureEffect *effect = effects->GetNextProjLight( pos );
		if ( effect->GetClippingPlaneEnable() ) {
			if ( Unknown7CF8A0( effect, &unknownC28[unknown12A8] ) ) {
				Unknown7CF9A0( effect );
				unknown12A8++;
			}
		} else if ( Unknown7CF8A0( effect, &unknown8E0[unknownC20] ) ) {
			unknownC20++;
		}
	}

	pos = effects->GetProjShadowHeadPos();
	while ( pos ) {
		NiTextureEffect *effect = effects->GetNextProjShadow( pos );
		if ( effect->GetClippingPlaneEnable() ) {
			if ( Unknown7CF8A0( effect, &unknown12AC[unknown192C] ) ) {
				Unknown7CFA70( effect );
				unknown192C++;
			}
		} else if ( Unknown7CF8A0( effect, &unknown1930[unknown1C70] ) ) {
			unknown1C70++;
		}
	}

	if ( effects->GetEnvironmentMap() ) {
		unknown1CDC = Unknown7CF8A0( effects->GetEnvironmentMap(), &unknown1C74 );
	}
	if ( effects->GetFogMap() ) {
		unknown1D48 = Unknown7CF8A0( effects->GetFogMap(), &unknown1CE0 );
	}
}

/*
================
NiD3DTexturePipeline::Unknown7CFD30

Clears the per-frame state.
================
*/
// FUNCTION: STBC 0x7CFD30
void NiD3DTexturePipeline::Unknown7CFD30() {
	unknownC20 = 0;
	unknown12A8 = 0;
	unknown1C70 = 0;
	unknown192C = 0;
	unknown1CDC = false;
	unknown1D48 = false;
	unknown8DC = 0;
}

/*
================
NiD3DTexturePipeline::ResetStageStates

Stage 0 passes its texture through, and stages 1 and 2 are off; the
states are kept and set on the device. Stage 3 is turned off and the
texture wrapping of the first two coordinate sets cleared.
================
*/
// FUNCTION: STBC 0x7CFD60
void NiD3DTexturePipeline::ResetStageStates() {
	stageStates[0].Set( D3DTOP_SELECTARG1, D3DTA_TEXTURE, D3DTA_DIFFUSE, D3DTOP_SELECTARG1, D3DTA_TEXTURE, D3DTA_DIFFUSE, 0 );
	stageStates[1].Set( D3DTOP_DISABLE, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTOP_DISABLE, D3DTA_TEXTURE, D3DTA_CURRENT, 1 );
	stageStates[2].Set( D3DTOP_DISABLE, D3DTA_CURRENT, D3DTA_DIFFUSE, D3DTOP_DISABLE, D3DTA_CURRENT, D3DTA_DIFFUSE, 0 );

	device->SetTextureStageState( 0, D3DTSS_COLOROP, stageStates[0].colorOp );
	device->SetTextureStageState( 0, D3DTSS_COLORARG1, stageStates[0].colorArg1 );
	device->SetTextureStageState( 0, D3DTSS_COLORARG2, stageStates[0].colorArg2 );
	device->SetTextureStageState( 0, D3DTSS_ALPHAOP, stageStates[0].alphaOp );
	device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, stageStates[0].alphaArg1 );
	device->SetTextureStageState( 0, D3DTSS_ALPHAARG2, stageStates[0].alphaArg2 );
	device->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, stageStates[0].texCoordIndex );
	device->SetTextureStageState( 0, D3DTSS_ADDRESS, stageStates[0].addressU );
	device->SetTextureStageState( 0, D3DTSS_MAGFILTER, filterModes[stageStates[0].filterMode][1] );
	device->SetTextureStageState( 0, D3DTSS_MINFILTER, filterModes[stageStates[0].filterMode][0] );
	device->SetTextureStageState( 0, D3DTSS_MIPFILTER, filterModes[stageStates[0].filterMode][2] );

	device->SetTextureStageState( 1, D3DTSS_COLOROP, stageStates[1].colorOp );
	device->SetTextureStageState( 1, D3DTSS_COLORARG1, stageStates[1].colorArg1 );
	device->SetTextureStageState( 1, D3DTSS_COLORARG2, stageStates[1].colorArg2 );
	device->SetTextureStageState( 1, D3DTSS_ALPHAOP, stageStates[1].alphaOp );
	device->SetTextureStageState( 1, D3DTSS_ALPHAARG1, stageStates[1].alphaArg1 );
	device->SetTextureStageState( 1, D3DTSS_ALPHAARG2, stageStates[1].alphaArg2 );
	device->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, stageStates[1].texCoordIndex );
	device->SetTextureStageState( 1, D3DTSS_ADDRESS, stageStates[1].addressU );
	device->SetTextureStageState( 1, D3DTSS_MAGFILTER, filterModes[stageStates[1].filterMode][1] );
	device->SetTextureStageState( 1, D3DTSS_MINFILTER, filterModes[stageStates[1].filterMode][0] );
	device->SetTextureStageState( 1, D3DTSS_MIPFILTER, filterModes[stageStates[1].filterMode][2] );

	device->SetTextureStageState( 2, D3DTSS_COLOROP, stageStates[2].colorOp );
	device->SetTextureStageState( 2, D3DTSS_COLORARG1, stageStates[2].colorArg1 );
	device->SetTextureStageState( 2, D3DTSS_COLORARG2, stageStates[2].colorArg2 );
	device->SetTextureStageState( 2, D3DTSS_ALPHAOP, stageStates[2].alphaOp );
	device->SetTextureStageState( 2, D3DTSS_ALPHAARG1, stageStates[2].alphaArg1 );
	device->SetTextureStageState( 2, D3DTSS_ALPHAARG2, stageStates[2].alphaArg2 );

	device->SetTextureStageState( 3, D3DTSS_COLOROP, D3DTOP_DISABLE );
	device->SetTextureStageState( 3, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

	device->SetRenderState( D3DRENDERSTATE_WRAP0, 0 );
	device->SetRenderState( D3DRENDERSTATE_WRAP1, 0 );
}

/*
================
NiD3DTexturePipeline::Unknown7D0170

Transforms (and lights and clips, when asked) an object's vertices into
a screen-space vertex buffer of the vertex buffer manager. Each texture
set takes a stage that passes its coordinates through the first pass
stage's coordinate setup, and each pass stage that generates its
coordinates takes the next stage; its coordinates are then read from
that set. Returns NULL when the processing fails. As the debug copy has
it (AnimOnly.exe 0x529916, D3DTexturePipeline.cpp: one counter for both
loops, the stage disabled through its cached state written out, the
vertex format and flags in locals, assert( !hErr )).
================
*/
// FUNCTION: STBC 0x7D0170
IDirect3DVertexBuffer7 *NiD3DTexturePipeline::Unknown7D0170( bool lighting, bool clip, unsigned int vertexCount, unsigned int textureSets,
															 IDirect3DVertexBuffer7 *vertexBuffer ) {
	unsigned int i;
	for ( i = 0; i < textureSets; i++ ) {
		DWORD texCoordIndex = unknown4DC[0]->texCoordIndex;
		unknown4DC[0]->texCoordIndex = i;
		ApplyTextureCoordinates( i, 0 );
		unknown4DC[0]->texCoordIndex = texCoordIndex;
		device->SetTextureStageState( i, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		stageStates[i].colorOp = D3DTOP_SELECTARG1;
	}

	// the generated coordinates take the stages after the texture sets
	unsigned int stage = textureSets;
	for ( i = 0; i < unknown4FC; i++ ) {
		if ( unknown4DC[i]->texCoordIndex >= 8 ) {
			if ( stage < 8 ) {
				ApplyTextureCoordinates( stage, i );
				unknown4DC[i]->texCoordIndex = stage;
				device->SetTextureStageState( stage, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
				stageStates[stage].colorOp = D3DTOP_SELECTARG1;
				stage++;
			} else {
				unknown4DC[i]->texCoordIndex = 7;
				break;
			}
		}
	}

	if ( stage < 8 && stageStates[stage].colorOp != D3DTOP_DISABLE ) {
		device->SetTextureStageState( stage, D3DTSS_COLOROP, D3DTOP_DISABLE );
		stageStates[stage].colorOp = D3DTOP_DISABLE;
	}

	DWORD fvf = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | ( ( ( stage ? stage : 1 ) << D3DFVF_TEXCOUNT_SHIFT ) & 0xF00 );	// 0xF00: the texture count's bits
	IDirect3DVertexBuffer7 *screenBuffer = vertexBufferManager->Unknown7CB570( fvf, vertexCount );
	DWORD flags = ( clip ? D3DVOP_CLIP : 0 ) | D3DVOP_TRANSFORM | ( lighting ? D3DVOP_LIGHT : 0 );
	HRESULT result = screenBuffer->ProcessVertices( flags, 0, vertexCount, vertexBuffer, 0, device, 0 );
	assert( !result );

	device->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
	stageStates[1].colorOp = D3DTOP_DISABLE;
	device->SetTextureStageState( 2, D3DTSS_COLOROP, D3DTOP_DISABLE );
	stageStates[2].colorOp = D3DTOP_DISABLE;

	if ( result ) {
		return NULL;
	}
	return screenBuffer;
}

/*
================
NiD3DTexturePipeline::Unknown7D0320

Keeps the camera's frame, which the projected texture stages use.
================
*/
// FUNCTION: STBC 0x7D0320
void NiD3DTexturePipeline::Unknown7D0320( const NiPoint3 &direction, const NiPoint3 &up, const NiPoint3 &right, float x, float y, float z ) {
	unknown1D4C.SetCol( 0, right.x, right.y, right.z );
	unknown1D4C.SetCol( 1, up.x, up.y, up.z );
	unknown1D4C.SetCol( 2, direction.x, direction.y, direction.z );
	unknown1D70.x = x;
	unknown1D70.y = y;
	unknown1D70.z = z;
}

/*
================
NiD3DTexturePipeline::Unknown7D03A0

Generates the stage's coordinates from the camera-space position, through
the given transform followed by the pipeline's own (unknown1D4C,
unknown1D70).
================
*/
// FUNCTION: STBC 0x7D03A0
void NiD3DTexturePipeline::Unknown7D03A0( const NiMatrix3 &rotate, const NiPoint3 &translate, NiD3DTextureStage *stage ) {
	D3DMATRIX &transform = stage->transform;
	stage->texCoordIndex = D3DTSS_TCI_CAMERASPACEPOSITION;
	stage->unknown58 = false;
	transform._11 = rotate.GetEntry( 0, 0 ) * unknown1D4C.GetEntry( 0, 0 ) + rotate.GetEntry( 0, 1 ) * unknown1D4C.GetEntry( 1, 0 )
		+ rotate.GetEntry( 0, 2 ) * unknown1D4C.GetEntry( 2, 0 );
	transform._21 = rotate.GetEntry( 0, 0 ) * unknown1D4C.GetEntry( 0, 1 ) + rotate.GetEntry( 0, 1 ) * unknown1D4C.GetEntry( 1, 1 )
		+ rotate.GetEntry( 0, 2 ) * unknown1D4C.GetEntry( 2, 1 );
	transform._31 = rotate.GetEntry( 0, 0 ) * unknown1D4C.GetEntry( 0, 2 ) + rotate.GetEntry( 0, 1 ) * unknown1D4C.GetEntry( 1, 2 )
		+ rotate.GetEntry( 0, 2 ) * unknown1D4C.GetEntry( 2, 2 );
	transform._12 = rotate.GetEntry( 1, 0 ) * unknown1D4C.GetEntry( 0, 0 ) + rotate.GetEntry( 1, 1 ) * unknown1D4C.GetEntry( 1, 0 )
		+ rotate.GetEntry( 1, 2 ) * unknown1D4C.GetEntry( 2, 0 );
	transform._22 = rotate.GetEntry( 1, 0 ) * unknown1D4C.GetEntry( 0, 1 ) + rotate.GetEntry( 1, 1 ) * unknown1D4C.GetEntry( 1, 1 )
		+ rotate.GetEntry( 1, 2 ) * unknown1D4C.GetEntry( 2, 1 );
	transform._32 = rotate.GetEntry( 1, 0 ) * unknown1D4C.GetEntry( 0, 2 ) + rotate.GetEntry( 1, 1 ) * unknown1D4C.GetEntry( 1, 2 )
		+ rotate.GetEntry( 1, 2 ) * unknown1D4C.GetEntry( 2, 2 );
	transform._13 = rotate.GetEntry( 2, 0 ) * unknown1D4C.GetEntry( 0, 0 ) + rotate.GetEntry( 2, 1 ) * unknown1D4C.GetEntry( 1, 0 )
		+ rotate.GetEntry( 2, 2 ) * unknown1D4C.GetEntry( 2, 0 );
	transform._23 = rotate.GetEntry( 2, 0 ) * unknown1D4C.GetEntry( 0, 1 ) + rotate.GetEntry( 2, 1 ) * unknown1D4C.GetEntry( 1, 1 )
		+ rotate.GetEntry( 2, 2 ) * unknown1D4C.GetEntry( 2, 1 );
	transform._33 = rotate.GetEntry( 2, 0 ) * unknown1D4C.GetEntry( 0, 2 ) + rotate.GetEntry( 2, 1 ) * unknown1D4C.GetEntry( 1, 2 )
		+ rotate.GetEntry( 2, 2 ) * unknown1D4C.GetEntry( 2, 2 );
	transform._41 = rotate.GetEntry( 0, 0 ) * unknown1D70.x + rotate.GetEntry( 0, 1 ) * unknown1D70.y + rotate.GetEntry( 0, 2 ) * unknown1D70.z
		+ translate.x;
	transform._42 = rotate.GetEntry( 1, 0 ) * unknown1D70.x + rotate.GetEntry( 1, 1 ) * unknown1D70.y + rotate.GetEntry( 1, 2 ) * unknown1D70.z
		+ translate.y;
	transform._43 = rotate.GetEntry( 2, 0 ) * unknown1D70.x + rotate.GetEntry( 2, 1 ) * unknown1D70.y + rotate.GetEntry( 2, 2 ) * unknown1D70.z
		+ translate.z;
}

/*
================
NiD3DTexturePipeline::Unknown7D0560

As Unknown7D03A0, with the stage's unknown58 set.
================
*/
// FUNCTION: STBC 0x7D0560
void NiD3DTexturePipeline::Unknown7D0560( const NiMatrix3 &rotate, const NiPoint3 &translate, NiD3DTextureStage *stage ) {
	D3DMATRIX &transform = stage->transform;
	stage->texCoordIndex = D3DTSS_TCI_CAMERASPACEPOSITION;
	stage->unknown58 = true;
	transform._11 = rotate.GetEntry( 0, 0 ) * unknown1D4C.GetEntry( 0, 0 ) + rotate.GetEntry( 0, 1 ) * unknown1D4C.GetEntry( 1, 0 )
		+ rotate.GetEntry( 0, 2 ) * unknown1D4C.GetEntry( 2, 0 );
	transform._21 = rotate.GetEntry( 0, 0 ) * unknown1D4C.GetEntry( 0, 1 ) + rotate.GetEntry( 0, 1 ) * unknown1D4C.GetEntry( 1, 1 )
		+ rotate.GetEntry( 0, 2 ) * unknown1D4C.GetEntry( 2, 1 );
	transform._31 = rotate.GetEntry( 0, 0 ) * unknown1D4C.GetEntry( 0, 2 ) + rotate.GetEntry( 0, 1 ) * unknown1D4C.GetEntry( 1, 2 )
		+ rotate.GetEntry( 0, 2 ) * unknown1D4C.GetEntry( 2, 2 );
	transform._12 = rotate.GetEntry( 1, 0 ) * unknown1D4C.GetEntry( 0, 0 ) + rotate.GetEntry( 1, 1 ) * unknown1D4C.GetEntry( 1, 0 )
		+ rotate.GetEntry( 1, 2 ) * unknown1D4C.GetEntry( 2, 0 );
	transform._22 = rotate.GetEntry( 1, 0 ) * unknown1D4C.GetEntry( 0, 1 ) + rotate.GetEntry( 1, 1 ) * unknown1D4C.GetEntry( 1, 1 )
		+ rotate.GetEntry( 1, 2 ) * unknown1D4C.GetEntry( 2, 1 );
	transform._32 = rotate.GetEntry( 1, 0 ) * unknown1D4C.GetEntry( 0, 2 ) + rotate.GetEntry( 1, 1 ) * unknown1D4C.GetEntry( 1, 2 )
		+ rotate.GetEntry( 1, 2 ) * unknown1D4C.GetEntry( 2, 2 );
	transform._13 = rotate.GetEntry( 2, 0 ) * unknown1D4C.GetEntry( 0, 0 ) + rotate.GetEntry( 2, 1 ) * unknown1D4C.GetEntry( 1, 0 )
		+ rotate.GetEntry( 2, 2 ) * unknown1D4C.GetEntry( 2, 0 );
	transform._23 = rotate.GetEntry( 2, 0 ) * unknown1D4C.GetEntry( 0, 1 ) + rotate.GetEntry( 2, 1 ) * unknown1D4C.GetEntry( 1, 1 )
		+ rotate.GetEntry( 2, 2 ) * unknown1D4C.GetEntry( 2, 1 );
	transform._33 = rotate.GetEntry( 2, 0 ) * unknown1D4C.GetEntry( 0, 2 ) + rotate.GetEntry( 2, 1 ) * unknown1D4C.GetEntry( 1, 2 )
		+ rotate.GetEntry( 2, 2 ) * unknown1D4C.GetEntry( 2, 2 );
	transform._41 = rotate.GetEntry( 0, 0 ) * unknown1D70.x + rotate.GetEntry( 0, 1 ) * unknown1D70.y + rotate.GetEntry( 0, 2 ) * unknown1D70.z
		+ translate.x;
	transform._42 = rotate.GetEntry( 1, 0 ) * unknown1D70.x + rotate.GetEntry( 1, 1 ) * unknown1D70.y + rotate.GetEntry( 1, 2 ) * unknown1D70.z
		+ translate.y;
	transform._43 = rotate.GetEntry( 2, 0 ) * unknown1D70.x + rotate.GetEntry( 2, 1 ) * unknown1D70.y + rotate.GetEntry( 2, 2 ) * unknown1D70.z
		+ translate.z;
}

/*
================
NiD3DTexturePipeline::Unknown7D0730

Generates the stage's coordinates from the camera-space reflection vector,
through the given rotation followed by the pipeline's own (unknown1D4C),
then the given translation.
================
*/
// FUNCTION: STBC 0x7D0730
void NiD3DTexturePipeline::Unknown7D0730( const NiMatrix3 &rotate, const NiPoint3 &translate, NiD3DTextureStage *stage ) {
	D3DMATRIX &transform = stage->transform;
	stage->texCoordIndex = D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR;
	stage->unknown58 = false;
	transform._11 = rotate.GetEntry( 0, 0 ) * unknown1D4C.GetEntry( 0, 0 ) + rotate.GetEntry( 0, 1 ) * unknown1D4C.GetEntry( 1, 0 )
		+ rotate.GetEntry( 0, 2 ) * unknown1D4C.GetEntry( 2, 0 );
	transform._21 = rotate.GetEntry( 0, 0 ) * unknown1D4C.GetEntry( 0, 1 ) + rotate.GetEntry( 0, 1 ) * unknown1D4C.GetEntry( 1, 1 )
		+ rotate.GetEntry( 0, 2 ) * unknown1D4C.GetEntry( 2, 1 );
	transform._31 = rotate.GetEntry( 0, 0 ) * unknown1D4C.GetEntry( 0, 2 ) + rotate.GetEntry( 0, 1 ) * unknown1D4C.GetEntry( 1, 2 )
		+ rotate.GetEntry( 0, 2 ) * unknown1D4C.GetEntry( 2, 2 );
	transform._12 = rotate.GetEntry( 1, 0 ) * unknown1D4C.GetEntry( 0, 0 ) + rotate.GetEntry( 1, 1 ) * unknown1D4C.GetEntry( 1, 0 )
		+ rotate.GetEntry( 1, 2 ) * unknown1D4C.GetEntry( 2, 0 );
	transform._22 = rotate.GetEntry( 1, 0 ) * unknown1D4C.GetEntry( 0, 1 ) + rotate.GetEntry( 1, 1 ) * unknown1D4C.GetEntry( 1, 1 )
		+ rotate.GetEntry( 1, 2 ) * unknown1D4C.GetEntry( 2, 1 );
	transform._32 = rotate.GetEntry( 1, 0 ) * unknown1D4C.GetEntry( 0, 2 ) + rotate.GetEntry( 1, 1 ) * unknown1D4C.GetEntry( 1, 2 )
		+ rotate.GetEntry( 1, 2 ) * unknown1D4C.GetEntry( 2, 2 );
	transform._13 = rotate.GetEntry( 2, 0 ) * unknown1D4C.GetEntry( 0, 0 ) + rotate.GetEntry( 2, 1 ) * unknown1D4C.GetEntry( 1, 0 )
		+ rotate.GetEntry( 2, 2 ) * unknown1D4C.GetEntry( 2, 0 );
	transform._23 = rotate.GetEntry( 2, 0 ) * unknown1D4C.GetEntry( 0, 1 ) + rotate.GetEntry( 2, 1 ) * unknown1D4C.GetEntry( 1, 1 )
		+ rotate.GetEntry( 2, 2 ) * unknown1D4C.GetEntry( 2, 1 );
	transform._33 = rotate.GetEntry( 2, 0 ) * unknown1D4C.GetEntry( 0, 2 ) + rotate.GetEntry( 2, 1 ) * unknown1D4C.GetEntry( 1, 2 )
		+ rotate.GetEntry( 2, 2 ) * unknown1D4C.GetEntry( 2, 2 );
	transform._41 = translate.x;
	transform._42 = translate.y;
	transform._43 = translate.z;
}

/*
================
NiD3DTexturePipeline::Unknown7D08A0

Maps the camera-space reflection vector into the texture: u and v from x
and y, scaled by a half and moved to the middle of the texture.
================
*/
// FUNCTION: STBC 0x7D08A0
void NiD3DTexturePipeline::Unknown7D08A0( const NiMatrix3 &rotate, const NiPoint3 &translate, NiD3DTextureStage *stage ) {
	stage->texCoordIndex = D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR;
	stage->unknown58 = false;
	stage->transform._11 = stage->transform._12 = stage->transform._13 = stage->transform._21 = stage->transform._23
		= stage->transform._32 = stage->transform._33 = 0.0f;
	stage->transform._22 = 0.5f;
	stage->transform._31 = 0.5f;
	stage->transform._41 = 0.5f;
	stage->transform._42 = 0.5f;
	stage->transform._43 = 0.0f;
}

/*
================
NiD3DTexturePipeline::Unknown7D08E0

Whether the map has an image and none of the stages in unknown4DC was
set up from it.
================
*/
// FUNCTION: STBC 0x7D08E0
bool NiD3DTexturePipeline::Unknown7D08E0( const NiMultiTextureProperty::Map *map ) {
	if ( !map || !map->image ) {
		return false;
	}
	for ( unsigned int i = 0; i < unknown4FC; i++ ) {
		if ( unknown4DC[i]->source == map ) {
			return false;
		}
	}
	return true;
}

/*
================
NiD3DTexturePipeline::Unknown7D0920

Whether a texture effect is on, has an image and none of the stages in
unknown4DC was set up from it yet. For an effect already set up, flags
what the passes can't give it: projection from the camera, and its
clipping plane when the stage after it isn't one of the plane stages of
its kind.
================
*/
// FUNCTION: STBC 0x7D0920
bool NiD3DTexturePipeline::Unknown7D0920( NiTextureEffect *effect, bool &unknown1, bool &projected ) {
	if ( !effect || !effect->GetSwitch() || !effect->GetEffectTexture() ) {
		return false;
	}
	for ( unsigned int i = 0; i < unknown4FC; i++ ) {
		if ( unknown4DC[i]->source == effect ) {
			if ( effect->GetTextureCoordGen() == NiTextureEffect::WORLD_PERSPECTIVE ) {
				projected = true;
			}
			if ( effect->GetClippingPlaneEnable() ) {
				if ( i < unknown4FC - 1 ) {
					NiD3DTextureStage *next = unknown4DC[i + 1];
					unsigned int j;
					if ( effect->GetEffectType() == NiDynamicEffect::AMBIENT_LIGHT ) {
						for ( j = 0; j < 8; j++ ) {
							if ( &unknownF68[j] == next ) {
								return false;
							}
						}
					} else {
						for ( j = 0; j < 8; j++ ) {
							if ( &unknown15EC[j] == next ) {
								return false;
							}
						}
					}
				}
				unknown1 = true;
			}
			return false;
		}
	}
	return true;
}

/*
================
NiD3DTexturePipeline::Unknown7D0A00

Sets the stages up for an object's properties and effects, then returns
the flags of what they ask for that the passes or the device can't give:
multi-texture maps and texture effects left without a stage, addressing
the device can't do, a stage without a source, and too many stages. The
effect stages are cleared again for the next object.
================
*/
// FUNCTION: STBC 0x7D0A00
unsigned int NiD3DTexturePipeline::Unknown7D0A00( const NiPropertyState &state, const NiDynamicEffectState *effects ) {
	NiMultiTextureProperty *multiTexture = state.GetMultiTexture();
	unsigned int result = 0;
	unsigned int i;

	effectState = NULL;
	Unknown7CD640( state, effects, true, 16 );

	if ( Unknown7D08E0( multiTexture->GetDarkMap() ) ) {
		result |= 0x2000;
	}
	if ( Unknown7D08E0( multiTexture->GetDetailMap() ) ) {
		result |= 0x4000;
	}
	if ( Unknown7D08E0( multiTexture->GetGlossMap() ) ) {
		result |= 0x8000;
	}
	if ( Unknown7D08E0( multiTexture->GetGlowMap() ) ) {
		result |= 0x10000;
	}
	for ( i = 0; i < multiTexture->GetDecalMapCount(); i++ ) {
		if ( Unknown7D08E0( multiTexture->GetDecalMap( i ) ) ) {
			result |= 0x10000;
			break;
		}
	}

	if ( effects ) {
		bool projected = false;
		bool unknown1 = false;
		if ( Unknown7D0920( effects->GetEnvironmentMap(), unknown1, projected ) ) {
			result |= 0x800;
		}
		if ( Unknown7D0920( effects->GetFogMap(), unknown1, projected ) ) {
			result |= 0x1000;
		}
		NiDynamicEffectState::ListItem *pos = effects->GetProjLightHeadPos();
		while ( pos ) {
			if ( Unknown7D0920( effects->GetNextProjLight( pos ), unknown1, projected ) ) {
				result |= 0x200;
			}
		}
		pos = effects->GetProjShadowHeadPos();
		while ( pos ) {
			if ( Unknown7D0920( effects->GetNextProjShadow( pos ), unknown1, projected ) ) {
				result |= 0x400;
			}
		}
		if ( projected && !IsFlagSet( PROJECTED ) ) {
			result |= 0x80000;
		}
		if ( unknown1 ) {
			result |= 0x100000;
		}
	}

	if ( !IsFlagSet( INDEPENDENT_UV ) ) {
		for ( i = 0; i < unknown4FC; i++ ) {
			if ( unknown4DC[i]->addressU != unknown4DC[i]->addressV ) {
				result |= 0x20;
				break;
			}
		}
	}
	if ( !IsFlagSet( DECAL_ALPHA ) ) {
		for ( i = 0; i < unknown4FC; i++ ) {
			if ( unknown4DC[i]->addressU == D3DTADDRESS_CLAMP || unknown4DC[i]->addressV == D3DTADDRESS_CLAMP ) {
				result |= 0x10;
				break;
			}
		}
	}
	if ( unknown52C > 1 ) {
		result |= 0x01;
		if ( state.GetAlpha()->GetAlphaBlending() ) {
			result |= 0x08;
		}
		if ( state.GetFog()->GetFog() ) {
			result |= 0x40;
		}
	}
	for ( i = 0; i < unknown4FC; i++ ) {
		if ( !unknown4DC[i]->source ) {
			result |= 0x80;
			break;
		}
	}
	if ( ( result & 0x77600 ) && unknown4FC >= 8 ) {
		result |= 0x100;
	}

	unknownC20 = 0;
	unknown12A8 = 0;
	unknown1C70 = 0;
	unknown192C = 0;
	unknown1CDC = false;
	unknown1D48 = false;
	unknown8DC = 0;
	effectState = NULL;
	return result;
}
