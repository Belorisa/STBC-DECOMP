/*
===============================================================================

	NiD3DTexturePipeline.h

	The renderer's texture pipeline manager: it maps texturing properties to
	texture stage setups, single pass or multipass, from what the device's
	multitexturing can do.

	Evidence: the creation function (0x7CCD10, "(Creation): Attempting to
	create texture pipeline manager") allocates 0x1D90 bytes; the
	constructor (0x7CBE60) and the destructor (0x7CCA90) give the layout:
	six alpha properties, eight 0x30-byte records and nine arrays or single
	members of 0x68-byte texture stages, each with its own unwind state; the
	capability checks log the multitexture modes it found ("(Creation):
	Number of available single-pass multitexture modes", ...). The class
	names are invented from the log.

===============================================================================
*/

#ifndef __NID3DTEXTUREPIPELINE_H__
#define __NID3DTEXTUREPIPELINE_H__

#include "NiD3D7.h"
#include "NiPointer.h"
#include "NiImage.h"
#include "NiAlphaProperty.h"
#include "NiMatrix3.h"
#include "NiPoint3.h"
#include "NiMultiTextureProperty.h"
#include "NiTextureModeProperty.h"
#include <string.h>

class NiD3DRenderState;
class NiPropertyState;
class NiDynamicEffectState;
class NiTextureEffect;

class NiTextureManager;
class NiD3DVertexBufferManager;

/*
================
NiD3DTextureStage

One texture stage's setup: the image and the stage's texture transform.
The constructor is 0x7D0EE0 and the destructor 0x7CCA70, both emitted
for the pipeline's arrays. The name is invented.
================
*/
// FUNCTION: STBC 0x7CCA70 ??1NiD3DTextureStage@@QAE@XZ
class NiD3DTextureStage {
public:
							NiD3DTextureStage();

	NiPointer<NiImage>		image;
	unsigned int			unknown04;		// the texture manager's answer (slot 1) for the image; 7 at construction
	unsigned int			filterMode;		// NiTextureModeProperty::FilterMode
	DWORD					addressU;		// D3DTADDRESS_; wrap at construction
	DWORD					addressV;		// D3DTADDRESS_; wrap at construction
	DWORD					texCoordIndex;	// D3DTSS_TEXCOORDINDEX: the coordinate set, or a D3DTSS_TCI_ generation mode
	D3DMATRIX				transform;		// identity at construction
	bool					unknown58;
	unsigned int			blend;			// the pass's blending setup (BLEND_); BLEND_MULTIPLY at construction
	bool					unknown60;		// true at construction
	const void *			source;			// what the stage was set up from: the texture property, a multi-texture map or a texture effect (name invented)
};

/*
================
NiD3DTextureStage::NiD3DTextureStage
================
*/
// FUNCTION: STBC 0x7D0EE0
inline NiD3DTextureStage::NiD3DTextureStage() {
	unknown04 = 7;
	filterMode = 0;
	addressU = D3DTADDRESS_WRAP;
	addressV = D3DTADDRESS_WRAP;
	texCoordIndex = 0;
	blend = 3;
	memset( &transform, 0, sizeof( transform ) );
	transform._11 = transform._22 = transform._33 = transform._44 = 1.0f;
	unknown58 = false;
	unknown60 = true;
	source = NULL;
}

/*
================
NiD3DTextureStageSetup

The texture stage states last set on one of the device's stages; the
pipeline keeps one per stage and sets a state on the device only when it
changes. Only its reference at 0x18 is constructed (0x7CCA40) and
destroyed (0x7CCA50). The class and member names are invented from the
stage states 0x7CFD60 sets from them.
================
*/
// FUNCTION: STBC 0x7CCA40 ??0NiD3DTextureStageSetup@@QAE@XZ
// FUNCTION: STBC 0x7CCA50 ??1NiD3DTextureStageSetup@@QAE@XZ
class NiD3DTextureStageSetup {
public:
	// a stage's operations and coordinate set, with the texture cleared
	// and the addressing and filtering at their defaults (name invented)
	void					Set( DWORD colorOp_, DWORD colorArg1_, DWORD colorArg2_, DWORD alphaOp_, DWORD alphaArg1_,
								 DWORD alphaArg2_, DWORD texCoordIndex_ );

	DWORD					colorOp;		// D3DTOP_
	DWORD					colorArg1;		// D3DTA_
	DWORD					colorArg2;		// D3DTA_
	DWORD					alphaOp;		// D3DTOP_
	DWORD					alphaArg1;		// D3DTA_
	DWORD					alphaArg2;		// D3DTA_
	NiPointer<NiImage>		texture;		// set on the device stage
	DWORD					texCoordIndex;	// D3DTSS_TEXCOORDINDEX
	DWORD					addressU;		// D3DTADDRESS_; set on the device for both directions
	DWORD					addressV;		// D3DTADDRESS_
	unsigned int			filterMode;		// the NetImmerse filter mode, a row of filterModes
	unsigned int			transformMode;	// D3DTSS_TEXTURETRANSFORMFLAGS as set: 0 disabled, 1 two coordinates, 2 the pipeline's textureTransformFlags (name invented)
};

/*
================
NiD3DTextureStageSetup::Set
================
*/
inline void NiD3DTextureStageSetup::Set( DWORD colorOp_, DWORD colorArg1_, DWORD colorArg2_, DWORD alphaOp_, DWORD alphaArg1_,
										  DWORD alphaArg2_, DWORD texCoordIndex_ ) {
	colorOp = colorOp_;
	colorArg1 = colorArg1_;
	colorArg2 = colorArg2_;
	alphaOp = alphaOp_;
	alphaArg1 = alphaArg1_;
	alphaArg2 = alphaArg2_;
	texture = NULL;
	texCoordIndex = texCoordIndex_;
	addressU = D3DTADDRESS_WRAP;
	addressV = D3DTADDRESS_WRAP;
	filterMode = 0;
	transformMode = 0;
}

/*
================
NiD3DTextureOps

A texture mode's stage operations. The name is invented.
================
*/
struct NiD3DTextureOps {
	unsigned int			unknown00;
	DWORD					colorOp;		// D3DTOP_
	DWORD					alphaOp;		// D3DTOP_
};

class NiD3DTexturePipeline {
public:
	// flags (names invented from the device capabilities they copy)
	enum {
		INDEPENDENT_UV		= 0x01,			// D3DPTADDRESSCAPS_INDEPENDENTUV
		TRILINEAR			= 0x02,			// both stages filter trilinearly in a single pass
		SEPARATE_MEMORIES	= 0x04,			// D3DDEVCAPS_SEPARATETEXTUREMEMORIES
		VERTEX_COLOR_LAST	= 0x08,			// single passes take the vertex color in the last stage
		STAGE1_TRILINEAR	= 0x10,			// stage 1 filters trilinearly in a single pass
		INDEPENDENT_ADDRESSING	= 0x20,		// the stages address apart in a single pass
		HARDWARE			= 0x40,			// the device is a hardware device
		DECAL_ALPHA			= 0x80,			// D3DPTBLENDCAPS_DECALALPHA
		PROJECTED			= 0x100			// D3DPTEXTURECAPS_PROJECTED
	};

	// the flags set, cleared and tested by mask (names invented): AnimOnly.exe's debug copies
	// 0x52B910, 0x52B940, 0x52B970 and 0x52B9A0 (D3DTexturePipeline.cpp), which every flag
	// change and test of the pipeline calls
	void					SetFlag( unsigned int mask ) { flags |= mask; }
	void					ClearFlag( unsigned int mask ) { flags &= ~mask; }
	bool					IsFlagSet( unsigned int mask ) const { return ( mask & flags ) != 0; }
	bool					IsFlagClear( unsigned int mask ) const { return ( mask & flags ) == 0; }

	// the blending setups of the passes, indices of alphaProperties (names
	// invented from the blend modes)
	// a multitexture mode a single pass draws (multiTexturePasses)
	enum {
		SINGLE_PASS			= 0x80
	};

	enum {
		BLEND_ADD			= 0,			// one, one
		BLEND_ALPHA_ADD		= 1,			// source alpha, one
		BLEND_ALPHA			= 2,			// source alpha, inverse source alpha
		BLEND_MULTIPLY		= 3				// zero, source color
	};

	// "(Creation): Attempting to create texture pipeline manager" (name
	// invented)
	static NiD3DTexturePipeline *	Create( IDirect3DDevice7 *device, const D3DDEVICEDESC7 *deviceDesc,
											NiD3DRenderState *renderState, NiTextureManager *textureManager, bool hardware );

							NiD3DTexturePipeline( IDirect3DDevice7 *device_, const D3DDEVICEDESC7 *deviceDesc_,
												  NiD3DRenderState *renderState_, NiTextureManager *textureManager_ );
							~NiD3DTexturePipeline();

	// sets the stages up for an object's properties and effects and returns
	// the flags of what they ask for that the device can't give
	unsigned int			Unknown7D0A00( const NiPropertyState &state, const NiDynamicEffectState *effects );
	void					Unknown7CFD30();
	// the default states of the first three stages, kept and set on the
	// device (name invented)
	void					ResetStageStates();
	// draws two screen triangles to validate the stage setup with (name
	// invented)
	void					DrawTestTriangles();

	// the camera's frame: its right, up and direction vectors as the
	// columns of unknown1D4C, its location as unknown1D70
	void					Unknown7D0320( const NiPoint3 &direction, const NiPoint3 &up, const NiPoint3 &right, float x, float y, float z );


	// reads the texture capabilities of the device description (name
	// invented)
	void					CheckTextureCapabilities();
	// the filter table from the DirectX 5 or the DirectX 6 filter
	// capabilities (names invented)
	void					SetFilterModesDX5();
	void					SetFilterModesDX6();

	// a stage's addressing for a NetImmerse clamp mode (name invented)
	void					SetAddressModes( unsigned int clampMode, NiD3DTextureStage *stage );
	void					SetFilterMode( unsigned int filterMode, NiD3DTextureStage *stage );	// name invented
	// set a device stage's addressing and filtering from a pass stage
	// (unknown4DC); names invented
	void					ApplyAddressModes( unsigned int stage, unsigned int index );
	void					ApplyTextureCoordinates( unsigned int stage, unsigned int index );
	// sets stages 0 and 1 up for two pass stages (name invented)
	void					SetStageTextures( unsigned int index0, unsigned int index1 );
	// sets stage 0 up for one pass stage
	void					Unknown7CE4E0( unsigned int index );
	void					ApplyFilterMode( bool mipmapped, bool trilinear, unsigned int stage, unsigned int index );
	// a map with an image that none of the stages in unknown4DC was set up
	// from
	bool					Unknown7D08E0( const NiMultiTextureProperty::Map *map );
	// a stage of unknown530 for an image, or NULL when the texture manager
	// can't take the image
	NiD3DTextureStage *		Unknown7CD580( NiImage *image, NiTextureModeProperty::ClampMode clamp, NiTextureModeProperty::FilterMode filter,
										   unsigned int blend, unsigned int texCoordIndex, const void *source );
	// whether a texture effect still needs stages: on, with an image, and
	// not yet set up in unknown4DC; sets the flags of what the effect asks
	// for that the passes can't give (names invented)
	bool					Unknown7D0920( NiTextureEffect *effect, bool &unknown1, bool &projected );
	// sets a stage up for a texture effect's image and coordinates; false
	// when the effect stages are full, the effect is off or the texture
	// manager can't take its image
	bool					Unknown7CF8A0( NiTextureEffect *effect, NiD3DTextureStage *stage );
	// the next clipping plane stage of unknownF68 and of unknown15EC: a
	// texture effect's clipping plane in camera space, the second one
	// facing the other way
	bool					Unknown7CF9A0( NiTextureEffect *effect );
	bool					Unknown7CFA70( NiTextureEffect *effect );
	// sets the effect stages up for an effect state's projected lights and
	// shadows, environment map and fog map, unless they are already set up
	// for it
	void					Unknown7CFB50( const NiDynamicEffectState *effects );
	// adds one or two stages to unknown4DC, in one pass when the device can
	// draw them together; returns which were added
	unsigned int			Unknown7CD350( NiD3DTextureStage *stage0, NiD3DTextureStage *stage1, bool onlyPair );
	// adds two stages to unknown4DC, in one pass when the device can draw
	// them together; returns how many were added
	unsigned int			Unknown7CD1A0( NiD3DTextureStage *tex0, NiD3DTextureStage *tex1 );
	// adds a stage to unknown4DC in a pass of its own, when there is room
	// (name invented)
	bool					AddStage( NiD3DTextureStage *stage );
	// the passes the device needs for a blending setup (an index of
	// alphaProperties): two when it can blend them, none when it can't
	// (name invented)
	unsigned char			GetBlendPasses( unsigned int blend );
	bool					Unknown7CE9C0( bool unknown );

	// stage 0: the operations of a mode (a row of textureOps), or the
	// texture modulated or selected, or no texture (the diffuse color)
	void					Unknown7CE780( unsigned int mode, bool textured, bool unknown );
	// stage 1 on, with the color operation of a multitexture mode
	void					Unknown7CE640( unsigned int mode );
	// stages 1 and 2 off, and stage 1's texture cleared
	void					Unknown7CE6A0();
	// stage 2: the operations of a mode, or the current color passed
	void					Unknown7CE580( unsigned int mode, bool unknown );
	// the blending of the current pass, after the first
	void					Unknown7CE750();

	// "(Creation): Attempting to initialize multitexturing" precedes it
	void					Unknown7CCE50();

	// set up a stage's texture coordinates (the third argument is the stage)
	typedef void			( NiD3DTexturePipeline::*StageFunction )( const NiMatrix3 &rotate, const NiPoint3 &translate, NiD3DTextureStage *stage );

	void					Unknown7D03A0( const NiMatrix3 &rotate, const NiPoint3 &translate, NiD3DTextureStage *stage );
	void					Unknown7D0560( const NiMatrix3 &rotate, const NiPoint3 &translate, NiD3DTextureStage *stage );
	void					Unknown7D0730( const NiMatrix3 &rotate, const NiPoint3 &translate, NiD3DTextureStage *stage );
	void					Unknown7D08A0( const NiMatrix3 &rotate, const NiPoint3 &translate, NiD3DTextureStage *stage );

	// sets up the stages for an object's properties; true when the object
	// is drawn in passes through the pipeline
	bool					Unknown7CD640( const NiPropertyState &state, const NiDynamicEffectState *effects, bool normals, unsigned int textureSets );
	// the vertex buffer the stages draw from, set up from the object's

	IDirect3DVertexBuffer7 *	Unknown7D0170( bool lighting, bool clip, unsigned int vertexCount, unsigned int textureSets,
										   IDirect3DVertexBuffer7 *vertexBuffer );
	// sets up the next pass; false after the last one
	bool					Unknown7CE040();

	// resets the pass state, at the start of each frame and after each
	// object drawn in passes (name invented)
	void					ResetPasses() { effectState = NULL; Unknown7CFD30(); }

	IDirect3DDevice7 *		device;					// referenced at construction
	D3DDEVICEDESC7			deviceDesc;				// copied at construction
	NiD3DRenderState *		renderState;			// referenced at construction
	unsigned int			flags;					// INDEPENDENT_UV, HARDWARE, ...
	unsigned int			unknown0F8;
	DWORD					addressModes[4][2];	// D3DTADDRESS_ u and v per NetImmerse clamp mode (name invented)
	DWORD					filterModes[6][3];	// D3DTFN_, D3DTFG_, D3DTFP_ per NetImmerse filter mode (name invented)
	NiD3DTextureOps			textureOps[6];			// by texture apply mode (NiTextureModeProperty::APPLY_)
	NiD3DTextureOps			multiTextureOps[5];		// stage 1's, by multitexture mode
	unsigned int			unknown1E8;
	unsigned int			unknown1EC;
	unsigned int			unknown1F0;
	// the passes of each multitexture mode by the pixel formats of the two
	// stages' textures: SINGLE_PASS, or the blend passes (name invented)
	unsigned char			multiTexturePasses[5][8][8];
	unsigned int			unknown334;
	NiPointer<NiAlphaProperty>	alphaProperties[6];	// one per blending setup, made at construction
	NiTextureManager *		textureManager;			// referenced at construction
	unsigned int			unknown354;				// 8 at construction
	DWORD					textureTransformFlags;	// D3DTTFF_ flags of the texture coordinates
	NiD3DTextureStageSetup	stageStates[8];		// the states last set on each device stage (name invented)
	NiD3DTextureStage *		unknown4DC[8];
	unsigned int			unknown4FC;				// stages in unknown4DC
	unsigned int			unknown500;
	unsigned int			unknown504[8];
	unsigned int			unknown524;
	unsigned int			unknown528;
	unsigned int			unknown52C;
	NiD3DTextureStage		unknown530[8];
	unsigned int			unknown870;
	NiD3DTextureStage		unknown874;
	unsigned int			unknown8DC;
	NiD3DTextureStage		unknown8E0[8];
	unsigned int			unknownC20;
	NiPointer<NiImage>		unknownC24;				// a 2x1 black and white image made at construction
	NiD3DTextureStage		unknownC28[8];
	NiD3DTextureStage		unknownF68[8];
	unsigned int			unknown12A8;
	NiD3DTextureStage		unknown12AC[8];
	NiD3DTextureStage		unknown15EC[8];
	unsigned int			unknown192C;
	NiD3DTextureStage		unknown1930[8];
	unsigned int			unknown1C70;
	NiD3DTextureStage		unknown1C74;
	bool					unknown1CDC;
	NiD3DTextureStage		unknown1CE0;
	bool					unknown1D48;
	NiMatrix3				unknown1D4C;			// identity at construction
	NiPoint3				unknown1D70;			// zero at construction
	// the stage setup of each texture effect coordinate mode
	// (NiTextureEffect::CoordGenType): Unknown7D03A0, Unknown7D0560 and
	// Unknown7D08A0 at construction; NiD3DRender::Unknown7C3380 picks the
	// third of two (name invented)
	StageFunction			coordGenFunctions[3];
	NiD3DVertexBufferManager *	vertexBufferManager;	// made by Create
	const NiDynamicEffectState *	effectState;			// the effect state the effect stages were set up for; cleared at the start of each frame (name invented)
};

#endif /* !__NID3DTEXTUREPIPELINE_H__ */
