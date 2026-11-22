/*
===============================================================================

	Glow.h

	The enhanced glow: the set's glowing parts drawn a second time into an
	offscreen frame buffer, blurred through smaller ones and added over the
	scene. SetClass keeps one (0x97E91C) while glow is enabled and the card
	can do it (SetClass::CanGlowBeEnabled, "CanDoEnhancedGlows"), and its
	draw (SetClass::VirtualSlot25) drives it.

	Evidence: GlowRenderer::Create (0x4DF510), which SetClass::SetGlowEnabled
	and SetClass::CanGlowBeEnabled call with the renderer, allocates 0x54
	bytes, keeps the renderer as an NiD3DRender at 8 and makes five frame
	buffers (256, 128, 64, 32 and 256 pixels square) at 0x0C, their images
	at 0x20, seven screen quads at 0x34 and a GlowAccumulator at 0x50. The
	vtable 0x88D2E0 has NiObject's twelve slots with only the destructor
	(0x4E0AC0) overridden. The class has no RTTI or strings of its own; its
	name and its members' names are invented.

===============================================================================
*/

#ifndef __GLOW_H__
#define __GLOW_H__

#include "NiAccumulator.h"
#include "NiAVObject.h"
#include "GlowPass.h"
#include "NiPointer.h"
#include "NiTriShape.h"
#include "renderer/NiD3DRender.h"
#include "renderer/NiImage.h"

class NiCamera;
class NiPropertyState;

/*
================
GlowAccumulator

The sorter the glow's camera clicks with (invented name). 0x14 bytes:
NiAccumulator's and the default properties at 0x0C, for geometry without a
GlowPass. The vtable 0x88D264 keeps NiAccumulator's slots and RTTI and
overrides the destructor (0x4DF1B0) and RegisterObject (0x4DF290).
================
*/
// FUNCTION: STBC 0x004df190 ??_GGlowAccumulator@@UAEPAXI@Z
class GlowAccumulator : public NiAccumulator {
public:
							GlowAccumulator();
	virtual					~GlowAccumulator();

	virtual bool			RegisterObject( NiAVObject *geometry );

	NiPropertyList			properties;		// 0x0C the properties of geometry without a GlowPass
};

/*
================
GlowRenderer

The glow's frame buffers and the quads it draws them with (invented
name). SetClass's draw sets the intensity, begins the pass, clicks the
glow's cameras through RenderPass, ends the pass and composites the
result.
================
*/
// FUNCTION: STBC 0x004e1730 ??_GGlowRenderer@@UAEPAXI@Z
class GlowRenderer : public NiObject {
public:
	// a glow on the renderer, or NULL when it is not an NiD3DRender or a
	// frame buffer can't be made
	static GlowRenderer *	Create( NiRender *renderer_ );

	virtual					~GlowRenderer();

	void					SetIntensity( float intensity );
	// draws into the first frame buffer from here on
	void					BeginPass();
	// clicks the camera with the glow's sorter, clearing first when asked
	void					RenderPass( NiCamera *camera, bool clear );
	// finishes the glow's drawing and blurs it
	void					EndPass();
	// draws the glow over the scene
	void					Composite();

protected:
							GlowRenderer() {}

	// shrinks the glow through the smaller frame buffers and blurs it
	void					Blur();

	NiPointer<NiD3DRender>	renderer;			// 0x08
	unsigned int			frameBuffers[5];	// 0x0C the renderer's frame buffer indices: 256, 128, 64, 32 and 256 pixels
	NiPointer<NiImage>		images[5];			// 0x20 what each frame buffer draws into
	NiPointer<NiTriShape>	shapes[7];			// 0x34 the screen quads
	NiPointer<GlowAccumulator>	accumulator;	// 0x50
};

#endif /* !__GLOW_H__ */
