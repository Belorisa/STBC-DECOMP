/*
===============================================================================

	NiRender.h

	The abstract renderer that the scene graph draws through. NiD3DRender is
	the only implementation in the image.

	Evidence: the static initializer at 0x7E2A00 registers "NiRender" with
	NiObject's NiRTTI as its base; the constructor (0x7E2C40) stores the
	vtable at 0x898E50 (55 slots, 34 of them _purecall) and adds the renderer
	to a static list; the scalar deleting destructor is slot 10 (0x7E2D80).
	No string names the virtual functions, so the slots NiRender adds keep
	placeholder names.

	This header lives with the renderer until the NiRender unit itself is
	reconstructed.

===============================================================================
*/

#ifndef __NIRENDER_H__
#define __NIRENDER_H__

#include "NiObject.h"
#include "NiPointer.h"
#include "NiTList.h"
#include "NiFrustum.h"
#include "NiRect.h"
#include "NiAccumulator.h"
#include "NiImage.h"
#include "NiDynamicEffect.h"
#include "NiPropertyState.h"
#include "NiDynamicEffectState.h"
#include "NiStreamMacros.h"

class NiPlane;



class NiColor;
class NiColorA;
class NiPoint2;
class NiDynamicEffect;
class NiAVObject;
class NiGeometryData;
class NiTriBasedGeomData;
class NiTriShapeData;
class NiTriStripsData;
class NiParticlesData;
class NiLinesData;
class NiCamera;
class NiSortedObject;

class NiBound;

class NiImage;
class NiPoint3;
class NiMatrix3;


// FUNCTION: STBC 0x7E2D80 ??_GNiRender@@UAEPAXI@Z
class NiRender : public NiObject {
public:
	// registers the class's loader once; every unit that includes this
	// header calls it from a static initializer (0x7E2A40)
	static int				Register();
	// the loader Register registers; it creates nothing, the class being
	// abstract (0x7E31B0)
	static void				CreateObject( NiStream &stream, NiObject *&object );

	// FUNCTION: STBC 0x7E3520 ?GetRTTI@NiRender@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiRender();

	virtual const char *	VirtualSlot12() = 0;	// the display device's name
	virtual unsigned int	VirtualSlot13() = 0;
	virtual bool			VirtualSlot14( NiImage::texFormat_t format ) = 0;
	// FUNCTION: STBC 0x7E35E0 ?VirtualSlot15@NiRender@@UAEIPAVNiPropertyState@@PAVNiDynamicEffectState@@@Z
	virtual unsigned int	VirtualSlot15( NiPropertyState *state, NiDynamicEffectState *effects ) { return 0x800000; }
	// FUNCTION: STBC 0x7E3530 ?VirtualSlot16@NiRender@@UAEIPAVNiPropertyState@@@Z
	virtual unsigned int	VirtualSlot16( NiPropertyState *state ) { return 0x800000; }
	virtual bool			VirtualSlot17() = 0;
	virtual void			ClearBuffer( const NiRect<float> *rect, unsigned int flags ) = 0;
	virtual unsigned int	VirtualSlot19() = 0;
	virtual unsigned int	VirtualSlot20() = 0;
	// FUNCTION: STBC 0x7E3540 ?VirtualSlot21@NiRender@@UAEIXZ
	virtual unsigned int	VirtualSlot21() { return 0; }
	virtual void *			VirtualSlot22( int which ) = 0;
	virtual void			VirtualSlot23( NiImage *image, const NiRect<unsigned int> *rect, unsigned int x, unsigned int y, bool keyed ) = 0;
	virtual bool			SetBackgroundColor( const NiColor &color ) = 0;
	// FUNCTION: STBC 0x7E3550 ?VirtualSlot25@NiRender@@UAEXI@Z
	virtual void			VirtualSlot25( unsigned int unknown ) {}
	// FUNCTION: STBC 0x7E3560 ?VirtualSlot26@NiRender@@UAEMXZ
	virtual float			VirtualSlot26() { return 1.0f; }
	// FUNCTION: STBC 0x7E3570 ?VirtualSlot27@NiRender@@UAEXM@Z
	virtual void			VirtualSlot27( float unknown ) {}
	virtual bool			VirtualSlot28( NiObject *object, bool unknown1, bool keep ) = 0;
	virtual void			VirtualSlot29( bool kept ) = 0;
	virtual void			VirtualSlot30( NiImage *image ) = 0;	// purges an image from the texture manager
	// FUNCTION: STBC 0x7E35F0 ?VirtualSlot31@NiRender@@UAE_NPAI0@Z
	virtual bool			VirtualSlot31( unsigned int *unknown1, unsigned int *unknown2 ) { return false; }
	// FUNCTION: STBC 0x7E3600 ?VirtualSlot32@NiRender@@UAE_NPAI0000@Z
	virtual bool			VirtualSlot32( unsigned int *unknown1, unsigned int *unknown2, unsigned int *unknown3, unsigned int *unknown4, unsigned int *unknown5 ) { return false; }
	// FUNCTION: STBC 0x7E3610 ?VirtualSlot33@NiRender@@UAEIXZ
	virtual unsigned int	VirtualSlot33() { return 0; }
	virtual bool			VirtualSlot34( unsigned int unknown ) = 0;
	virtual unsigned int	VirtualSlot35() = 0;
	// FUNCTION: STBC 0x7E3580 ?VirtualSlot36@NiRender@@UAEXI@Z
	virtual void			VirtualSlot36( unsigned int unknown ) {}
	// FUNCTION: STBC 0x7E3590 ?VirtualSlot37@NiRender@@UAEXPAI0@Z
	virtual void			VirtualSlot37( unsigned int *unknown1, unsigned int *unknown2 ) {}
	virtual void			VirtualSlot38( void *unknown );
	virtual void			VirtualSlot39();
	// the effect is being deleted
	// FUNCTION: STBC 0x7E35A0 ?PurgeEffect@NiRender@@UAEXPAVNiDynamicEffect@@@Z
	virtual void			PurgeEffect( NiDynamicEffect *effect ) {}
	// the camera: location, direction, up and right vectors, frustum and
	// view port
	virtual void			VirtualSlot41( const NiPoint3 &location, const NiPoint3 &direction, const NiPoint3 &up,
										   const NiPoint3 &right, const NiFrustum &frustum, const NiRect<float> &port ) = 0;
	// the world transform of the object drawn next
	virtual void			VirtualSlot42( const NiPoint3 &translate, const NiMatrix3 &rotate, float scale ) = 0;
	virtual void			VirtualSlot43( bool unknown );									// releases unknown10
	virtual void			VirtualSlot44() = 0;
	// the draw functions take the geometry data and its world bound
	virtual void			VirtualSlot45( NiTriShapeData *data, const NiBound &bound ) = 0;
	// TODO identity: the data of slots 46 and 47 are by their use the
	// NiTrianglesData and NiTriStripData the image names, not yet declared
	virtual void			VirtualSlot46( NiTriBasedGeomData *data, const NiBound &bound ) = 0;
	virtual void			VirtualSlot47( NiTriBasedGeomData *data, const NiBound &bound ) = 0;
	virtual void			VirtualSlot48( NiTriStripsData *data, const NiBound &bound ) = 0;
	virtual void			VirtualSlot49( NiParticlesData *data, const NiBound &bound ) = 0;	// draws particles
	virtual void			VirtualSlot50( NiLinesData *data, const NiBound &bound ) = 0;
	virtual void			VirtualSlot51( unsigned int vertexCount, NiPoint3 *vertices, NiPoint2 *textures, NiColorA *colors ) = 0;
	// the camera's active culling planes (NiCamera's call)
	virtual void			VirtualSlot52( NiPlane **planes, unsigned int count, unsigned int sideCount, unsigned int planeMask ) = 0;
	// FUNCTION: STBC 0x7E35B0 ?VirtualSlot53@NiRender@@UAE_NPAVNiObject@@@Z
	virtual bool			VirtualSlot53( NiObject *object ) { return true; }
	// the data is being deleted
	// FUNCTION: STBC 0x7E35C0 ?PurgeGeometryData@NiRender@@UAEXPAVNiGeometryData@@@Z
	virtual void			PurgeGeometryData( NiGeometryData *data ) {}

							NiRender();

	// tell every renderer that an effect or geometry data is being deleted,
	// so that it drops what it caches for it (names after the Gamebryo
	// lineage)
	static void				RemoveEffect( NiDynamicEffect *effect );
	static void				RemoveGeometryData( NiGeometryData *data );

	// the window point as fractions of the back buffer, y from the bottom;
	// false when the buffer has no size (Gamebryo 1.2.2:
	// NiRenderer::MapWindowPointToBufferPoint)
	bool					MapWindowPointToBufferPoint( unsigned int x, unsigned int y, float &bufferX, float &bufferY );

	static const NiRTTI		rtti;

	static NiTList<NiRender *>	renderers;	// every live renderer, added to by the constructor

	// the accumulator that sorts what a camera draws; NiCamera::Click
	// (0x7E7FA0) brackets the scene with its slots 12 and 13 (names after
	// the Gamebryo lineage)
	NiAccumulator *			GetSorter() const { return sorter; }
	// hands a geometry object to the sorter; false when there is none or it
	// does not take the object, and the caller draws it at once (name
	// invented after the accumulator slot it forwards to)
	bool					RegisterObject( NiAVObject *geometry );
	void					SetSorter( NiAccumulator *sorter_ ) { sorter = sorter_; }

	// the properties the next object is drawn with (Gamebryo lineage name)
	void					SetPropertyState( NiPropertyState *state ) { propertyState = state; }
	NiPropertyState *		GetPropertyState() const { return propertyState; }
	// the lights the next object is drawn with (Gamebryo lineage name)
	void					SetEffectState( NiDynamicEffectState *state ) { effectState = state; }
	NiDynamicEffectState *	GetEffectState() const { return effectState; }

	// a screen polygon drawn on its own, between slots 43 and 44 (name
	// invented); NiCamera draws its polygons through slot 51 between its
	// own calls
	void					DrawScreenPolygon( unsigned int vertices, NiPoint3 *vertex, NiPoint2 *texture, NiColorA *color ) {
								VirtualSlot43( false );
								VirtualSlot51( vertices, vertex, texture, color );
								VirtualSlot44();
							}

	// TODO identity: draws a cluster-sorted object with its own properties
	// and effects (0x7E2FD0); NiCamera's 0x7E8B90 calls it
	void					Unknown7E2FD0( NiCamera *camera, NiSortedObject *object );

protected:
	NiPointer<NiAccumulator>	sorter;
	NiPointer<NiPropertyState>	propertyState;	// the properties of the object being drawn
	NiPointer<NiDynamicEffectState>	effectState;	// its lights; released by slot 43
};

NiRegisterStream( NiRender );

#endif /* !__NIRENDER_H__ */
