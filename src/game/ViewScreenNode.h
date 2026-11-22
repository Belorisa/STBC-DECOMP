/*
===============================================================================

	ViewScreenNode.h

	The node of a bridge view screen: it switches the screen between the
	"off" texture and the static, and flickers the static's alpha. No
	string names the class (its GetRTTI is NiNode's); the name is invented.

	0x150 bytes: ViewScreenObject's model setup (0x6787A0) allocates that
	much and calls the constructor 0x676220, which calls NiNode's, builds
	the four property pointers at 0xE8-0xF4 and the rect at 0x134, clears the
	two switches at 0x144 and 0x145, stores the vtable 0x894B00 and turns
	the screen off. ViewScreenObject reaches it through its node
	(0x678900, the node at 0x18) to forward SetIsOn, IsOn, SetStaticIsOn,
	IsStaticOn, SetStaticVariation, SetOffTexture and
	SetStaticTextureIconGroup. The vtable has NiNode's 44 slots and one of
	its own, slot 44, which flickers the static and updates the node.

===============================================================================
*/

#ifndef __VIEWSCREENNODE_H__
#define __VIEWSCREENNODE_H__

#include "NiNode.h"
#include "NiPointer.h"
#include "NiColor.h"
#include "NiRect.h"

class NiTextureProperty;
class NiTriShape;
class NiAVObject;
class NiAlphaProperty;
class NiMaterialProperty;
class NiCamera;
class CameraObjectClass;

// The destructor is the compiler's (it stores no vtable).
// FUNCTION: STBC 0x006762b0 ??_GViewScreenNode@@UAEPAXI@Z
// FUNCTION: STBC 0x006762d0 ??1ViewScreenNode@@UAE@XZ
class ViewScreenNode : public NiNode {
public:
					ViewScreenNode( void );

	// NiNode
	virtual void	AttachChild( NiAVObject *child, bool firstAvail );

	// Flickers the static's alpha while it is on, then updates the node.
	virtual void	VirtualSlot44( float time );

	void			SetIsOn( bool on );
	bool			IsOn( void ) { return isOn; }
	void			SetStaticIsOn( bool on );
	bool			IsStaticOn( void ) { return staticIsOn; }
	void			SetStaticVariation( float low, float high );
	void			SetOffTexture( char *name );
	void			SetStaticTextureIconGroup( char *name );
	void			Unknown676E50( void );
	// Whether the screen is in the camera's view: its bound is not behind a culling plane (invented name).
	bool			IsInView( CameraObjectClass *camera );
	// The screen's rect in the view camera's port; false when it is wholly outside (invented name).
	bool			FindScreenPort( NiCamera *viewCamera, NiCamera *screenCamera );
	// Fits the screen camera's frustum and port to the screen's rect (invented name).
	bool			FitScreenCamera( NiCamera *viewCamera, NiCamera *screenCamera );
	// How far the screen's rect ran past the left, right, top and bottom
	// edges of the view port, and the rect (invented name).
	void			GetScreenRect( float &left, float &right, float &top, float &bottom, NiRect<float> &rect ) {
						left = unknown11C[0];
						right = unknown11C[1];
						top = unknown11C[2];
						bottom = unknown11C[3];
						rect = port;
					}
	// The view port position of a world point, unclipped (invented name).
	void			ProjectPoint( NiCamera *camera, const NiPoint3 &point, float &x, float &y );
	// The first triangle shape in the tree under the object, or the object itself (invented name).
	NiTriShape *	FindTriShape( NiAVObject *object );

protected:
	NiPointer<NiTextureProperty>	offTexture;		// 0xE8 shown while the static is off (invented name)
	NiPointer<NiTextureProperty>	staticTexture;	// 0xEC shown while the static is on (invented name)
	NiPointer<NiAlphaProperty>	alpha;				// 0xF0 (invented name)
	NiPointer<NiMaterialProperty>	material;		// 0xF4 its alpha flickers with the static (invented name)
	NiPoint3		screenNormal;			// 0xF8 the facing of the screen's first triangle, unit length, reversed (invented name)
	NiPoint3		screenCenter;			// 0x104 the middle of the screen's world-space bounding box (invented name)
	NiPoint3		screenHalfExtent;		// 0x110 half the box's size (invented name)
	// How far the screen's rect ran past the left, right, top and bottom of
	// the view [0-3], and its full width and height [4-5], in view port units
	// (FindScreenPort).
	float			unknown11C[6];			// 0x11C
	NiRect<float>	port;					// 0x134 the screen's rect in the view port, clipped to it; zero at first
	bool			isOn;					// 0x144
	bool			staticIsOn;				// 0x145
	float			staticAlphaMin;			// 0x148 the static's lowest alpha, 0 to 1 (invented name)
	float			staticAlphaRange;		// 0x14C how far above the lowest it flickers (invented name)
};

#endif /* !__VIEWSCREENNODE_H__ */
