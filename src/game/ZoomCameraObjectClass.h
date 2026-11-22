/*
===============================================================================

	ZoomCameraObjectClass.h

	The members of ZoomCameraObjectClass that the SWIG wrappers ZoomCameraObjectClass_* call, and its
	data members. No virtual functions are declared yet.

	0x164 bytes: Create (0x679FC0) allocates them from the pools. The
	constructor (0x679E50) runs CameraObjectClass's, stores the vtable
	0x894CC0 and initialises the members below. CharacterClass's menu
	handler (0x66D450) toggles the zoom off when it is on (0x160), and sets
	the zoomed scale (0x144) from the character's position before toggling it on.

===============================================================================
*/

#ifndef __ZOOMCAMERAOBJECTCLASS_H__
#define __ZOOMCAMERAOBJECTCLASS_H__

#include "CameraObjectClass.h"

class SetClass;
class TGEventHandlerTable;
class TGObject;
class TGStream;

// GetObjType's value for a ZoomCameraObjectClass (the type Cast checks, 0x679E20).
#define ZOOMCAMERAOBJECTCLASS_TYPE		0x80C0

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// The destructor is the implicit one (a bare jump to CameraObjectClass's).
// FUNCTION: STBC 0x00679f80 ??_GZoomCameraObjectClass@@UAEPAXI@Z @game/ZoomCameraObjectClass
// FUNCTION: STBC 0x00679fb0 ??1ZoomCameraObjectClass@@UAE@XZ @game/ZoomCameraObjectClass
class ZoomCameraObjectClass : public CameraObjectClass {
public:
					ZoomCameraObjectClass( long id = 0 );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// BaseObjectClass
	virtual void	Update( float time );
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	static ZoomCameraObjectClass **	GetClassObjects( SetClass *set, int *count );		// invented name

	void	UpdateViewFrustum( void );
	static ZoomCameraObjectClass *	Create( float, float, float, float, float, float, float, char * );
	static ZoomCameraObjectClass *	Cast( TGObject * );
	void	ToggleZoom( float );
	void	SetShake( float, float );
	void	LookForward( void );
	static ZoomCameraObjectClass *	GetObject( SetClass *, char * );
	static ZoomCameraObjectClass *	GetObjectByID( SetClass *, long );		// invented name
	static int	GetNumClassObjects( SetClass * );

	bool	IsZoomed( void ) { return zoomed; }					// the SWIG wrapper ZoomCameraObjectClass_IsZoomed
	void	SetZoom( float zoom_ ) { minZoom = zoom_; }			// invented name

	float	GetMaxZoom( void ) { return maxZoom; }
	float	GetShake( void ) { return shake; }
	float	GetZoomTime( void ) { return zoomTime; }
	float	GetMinZoom( void ) { return minZoom; }
	void	SetZoomTime( float zoomTime_ ) { zoomTime = zoomTime_; }
protected:
	// The names of the members below without a SWIG accessor are invented.
	NiPoint3	forward;		// 0x12C (0, -1, 0) at first: LookForward's direction, LookToward's forward (name invented)
	NiPoint3	up;				// 0x138 (0, 0, 1) at first: LookForward's up vector, LookToward's up (name invented)
	float	minZoom;			// 0x144 0.7 at first: the frustum scale while zoomed (SetMinZoom)
	float	maxZoom;			// 0x148 1 at first: the frustum scale while not zoomed (SetMaxZoom)
	float	zoomTime;			// 0x14C 1 at first
	float	zoomStartTime;		// 0x150 -1 at first: the time ToggleZoom started the zoom; Update scales by the share of zoomTime since (name invented)
	float	shake;			// 0x154 0 at first: SetShake's amount
	float	shakeTime;			// 0x158 SetShake's time, run down by 0.1 at each new offset (name invented)
	float	timeToNextShake;	// 0x15C 0 at first: Update's countdown to the next random offset, 0.1 apart (name invented)
	bool	zoomed;				// 0x160 false at first: IsZoomed's
	bool	zooming;			// 0x161 false at first: set by ToggleZoom, cleared when Update reaches the zoom's end (name invented)

	static TGEventHandlerTable	classHandlers;		// invented name

	// Declared for the SWIG wrappers that call them.
public:
	void	SetMaxZoom( float maxZoom_ ) { maxZoom = maxZoom_; UpdateViewFrustum(); }
	void	SetMinZoom( float minZoom_ ) { minZoom = minZoom_; UpdateViewFrustum(); }
};

#endif /* !__ZOOMCAMERAOBJECTCLASS_H__ */
