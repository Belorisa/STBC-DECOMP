/*
===============================================================================

	TGMouseEvent.h

	A mouse event (ET_MOUSE): the pointer's position and, in TGEvent's
	flags word, which button went down or up.

	Evidence: the SWIG wrappers TGMouseEvent_GetX and GetY read 0x28 and
	0x2C; GetFlags reads the input bits of the flags word (0x18 & 0xFF00).
	The MEF_* values are those
	of the SWIG constants TGMouseEvent_MEF_*. The load constructor
	(0x6E3D50, built by TGInputModule's creator 0x6E3D20 in 0x30 bytes)
	stores the vtable 0x8945D0; it overrides Copy (0x6E8630), WriteToStream
	(0x6E86C0) and ReadFromStream (0x6E8700), which copy, write and read the
	position. The destructor is implicit.

===============================================================================
*/

#ifndef __TGMOUSEEVENT_H__
#define __TGMOUSEEVENT_H__

#include "TGIEvent.h"
#include "NiPoint2.h"

class NiCamera;
class NiPoint3;

// GetObjType's value for a TGMouseEvent (its slot 1, 0x5C97F0).
#define TGMOUSEEVENT_TYPE		0x405

// The event type of mouse events (the SWIG constant ET_MOUSE).
#define ET_MOUSE				0x30001

// The button bits of the flags word (the SWIG constants TGMouseEvent_MEF_*).
#define MEF_BUTTON_DOWN			0x0200
#define MEF_BUTTON_UP			0x0400
#define MEF_BUTTON_MASK			0x0600
#define MEF_BUTTON_LEFT			0x0800
#define MEF_BUTTON_RIGHT		0x1000
#define MEF_BUTTON_MIDDLE		0x1800
#define MEF_BUTTON_NUM_MASK		0x1800

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x005c9870 ??_GTGMouseEvent@@UAEPAXI@Z @game/AppWrap
// FUNCTION: STBC 0x005c98a0 ??1TGMouseEvent@@UAE@XZ @game/AppWrap
class TGMouseEvent : public TGIEvent {
public:
	// FUNCTION: STBC 0x006e3d50 ??0TGMouseEvent@@QAE@J@Z @game/TGInputModule
					TGMouseEvent( long id ) : TGIEvent( id ) {}
	// FUNCTION: STBC 0x005c97c0 ??0TGMouseEvent@@QAE@XZ @game/AppWrap
					TGMouseEvent( void ) { SetEventType( ET_MOUSE ); }
	// FUNCTION: STBC 0x005c9a00 ??0TGMouseEvent@@QAE@ABVNiPoint2@@H@Z @game/AppWrap
	// DRAFT: schedule: retail ors the input flags into the flags word in memory after the position stores; this build reloads the word right after the base constructor's mask
					TGMouseEvent( const NiPoint2 &position, int inputFlags ) { SetEventType( ET_MOUSE ); x = position.x; y = position.y; AddInputFlags( inputFlags ); }
	// The script-side constructor (the SWIG wrapper TGMouseEvent_Create expands it).
	static TGMouseEvent *	Create( float x, float y, int inputFlags ) { return new TGMouseEvent( NiPoint2( x, y ), inputFlags ); }

	// TGObject
	// FUNCTION: STBC 0x005c97f0 ?GetObjType@TGMouseEvent@@UAEHXZ @game/AppWrap
	virtual int		GetObjType( void ) { return TGMOUSEEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x005c9840 ?GetClassName@TGMouseEvent@@UAEPADXZ @game/AppWrap
	virtual char *	GetClassName( void ) { return "TGMouseEvent"; }
	// FUNCTION: STBC 0x005c9850 ?GetSwigTypeName@TGMouseEvent@@UAEPADXZ @game/AppWrap
	virtual char *	GetSwigTypeName( void ) { return "_p_TGMouseEvent"; }
	// FUNCTION: STBC 0x005c9860 ?GetPythonClassName@TGMouseEvent@@UAEPADXZ @game/AppWrap
	virtual char *	GetPythonClassName( void ) { return "TGMouseEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );

	// The input flags only: the SWIG wrapper TGMouseEvent_GetFlags masks them.
	int				GetFlags( void ) { return flags & INPUT_EVENT_FLAGS; }
	void			SetFlags( int inputFlags ) { AddInputFlags( inputFlags ); }
	int				GetButtonNum( void ) { return flags & MEF_BUTTON_NUM_MASK; }
	bool			IsButtonEvent( void ) { return ( flags & MEF_BUTTON_MASK ) != 0; }
	// The ray through the event's position from the camera (0x6E8660;
	// invented name): the pick ray the editors select with.
	bool			GetPickRay( NiPoint3 *origin, NiPoint3 *direction, NiCamera *camera );	// false when the point maps outside the view
	float			GetX( void ) { return x; }
	float			GetY( void ) { return y; }
	// The position (invented name).
	void			GetPosition( NiPoint2 *position ) { position->x = x; position->y = y; }

	float			x;						// 0x28 in screen fractions
	float			y;						// 0x2C
};

/*
================
TGMouseEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x005c9800 ?IsTypeOf@TGMouseEvent@@UAE_NH@Z @game/AppWrap
inline bool TGMouseEvent::IsTypeOf( int type ) {
	if ( type == TGMOUSEEVENT_TYPE ) {
		return true;
	}
	return TGIEvent::IsTypeOf( type );
}

// Where the input code last saw the mouse, in screen fractions (invented
// name; written with each mouse event at 0x4A0277, read by
// TGRootPane::SetMouseCursor).
extern NiPoint2	lastMousePosition;

// The mouse's movement the editors turn the camera and the objects by
// (invented name). Retail reads it (0x497F69, 0x4A019E) but no code writes
// it, so it stays zero.
extern NiPoint2	mouseMovement;

#endif /* !__TGMOUSEEVENT_H__ */
