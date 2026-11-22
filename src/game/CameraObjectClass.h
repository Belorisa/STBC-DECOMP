/*
===============================================================================

	CameraObjectClass.h

	The members of CameraObjectClass that the SWIG wrappers CameraObjectClass_* call. Of the
	virtual functions only the two set overrides are declared yet.

	The data members up to 0xE8 are those the constructor (0x429A60)
	initialises: BaseObjectClass's constructor, then (in this order) a list
	of two items per block at 0xF4, two maps of 37 buckets at 0x10C and
	0x11C, the vtable 0x88947C, and the members below. The set's camera
	queries read the active flag at 0xE1 (SetClass::GetActiveCamera
	0x412FD0) and the NetImmerse camera at 0xE8, which the set renders
	through (0x410730) and picks with (0x413950).

	The modes: the mode current at 0xEC, which PushCameraMode,
	GetCurrentCameraMode and PopCameraMode leave through its slot 29 and
	enter through its slot 28, the ID of the mode last left at 0xF0, the
	stack of modes at 0xF4 (pushed at the head, which is the top), the
	modes by name at 0x10C (GetNamedCameraMode, AddNamedCameraMode) and
	the hierarchy at 0x11C, the name of each mode's fallback by the mode's
	name (AddModeHierarchy): GetCurrentCameraMode walks it while a mode is
	not valid. The size, 0x12C, is the pool block Create allocates.

	The vtable (0x88947C) has 54 slots: BaseObjectClass's 50 and four of
	CameraObjectClass's own; the maps' vtables follow it. Slot 50 is
	PopCameraMode (the SWIG wrapper); slot 51 gives the NetImmerse camera a
	renderer; slots 52 and 53 pass their arguments to the current mode's
	slots 30 and 31.

===============================================================================
*/

#ifndef __CAMERAOBJECTCLASS_H__
#define __CAMERAOBJECTCLASS_H__

#include "BaseObjectClass.h"
#include "NiTList.h"
#include "NiTStringMap.h"

class CameraMode;
class NiCamera;
class BezierCurve;
class NiFrustum;
class NiRender;
class SetClass;
class TGObject;

// GetObjType's value for a CameraObjectClass.
#define CAMERAOBJECTCLASS_TYPE		0x80BE

// Event types, as the SWIG constants of the App module give them.
#define ET_CAMERA_ANIMATION_DONE		0x80004D

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class TGPoint3;
// FUNCTION: STBC 0x00429e00 ??_GCameraObjectClass@@UAEPAXI@Z @game/CameraObjectClass
class CameraObjectClass : public BaseObjectClass {
public:
	virtual			~CameraObjectClass( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }
	// The class's handler methods by name, and in the class's table (invented names).
	static void		RegisterHandlerNames( void );
	static void		RegisterHandlers( void );
	// ET_CAMERA_ANIMATION_DONE: the camera's animation has played out, so
	// its controllers stop.
	void			AnimationDoneHandler( TGEvent *event );

	// BaseObjectClass: the camera's transform is its NetImmerse camera's
	virtual void	Update( float time );
	virtual void	UpdateNodeOnly( void );
	// FUNCTION: STBC 0x00429de0 ?Rotate@CameraObjectClass@@UAEXPAVTGMatrix3@@@Z @game/CameraObjectClass
	virtual void	Rotate( TGMatrix3 *matrix ) { Rotate( *matrix ); }
	virtual void	Rotate( const NiMatrix3 &matrix );
	virtual void	SetTranslateXYZ( float x, float y, float z );
	virtual void	SetAngleAxisRotation( float angle, float x, float y, float z );
	// FUNCTION: STBC 0x00429df0 ?SetMatrixRotation@CameraObjectClass@@UAEXPAVTGMatrix3@@@Z @game/CameraObjectClass
	virtual void	SetMatrixRotation( TGMatrix3 *matrix ) { SetMatrixRotation( *matrix ); }
	virtual void	SetMatrixRotation( const NiMatrix3 &matrix );
	virtual const TGPoint3 &	GetWorldLocation( void );
	virtual const TGPoint3 &	GetTranslate( void );
	virtual const TGMatrix3 &	GetRotation( void );
	virtual NiPoint3	GetWorldForward( void );
	virtual NiPoint3	GetWorldUp( void );
	virtual NiPoint3	GetWorldRight( void );
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	virtual void	PopCameraMode( char *name_ );
	virtual void	SetRenderer( NiRender *renderer );		// invented name
	virtual void	VirtualSlot52( BezierCurve *sweep );	// the current mode's slot 30
	virtual void	VirtualSlot53( NiPoint3 *forward, NiPoint3 *up );	// the current mode's slot 31

					CameraObjectClass( long id = 0 );

	static CameraObjectClass *	Create( float x, float y, float z, float unknown4, float unknown5, float unknown6, float unknown7, char *name_ );
	static CameraObjectClass *	CreateFromNiCamera( NiCamera *niCamera, char *name_ );
	// TODO identity: Create's set-up of a new camera (0x42A290): makes its
	// NetImmerse camera with the system renderer and the name, places it
	// at x, y, z and aims it by the other four values, puts it under a new
	// node and gives it the default frustum (SetFrustumValues)
	void	Unknown42A290( float x, float y, float z, float unknown4, float unknown5, float unknown6, float unknown7, char *name_ );
	static CameraObjectClass *	Cast( TGObject * );
	void	SetFrustumValues( float halfWidth, float nearDistance, float farDistance );
	void	SetFrustum( float left, float right, float top, float bottom, float nearDistance, float farDistance );
	void	SetNearAndFarDistance( float, float );
	// The NetImmerse camera's view frustum; an unset one without a camera
	// (0x42AB70; invented name).
	NiFrustum	GetNiFrustum( void );
	void	SetNiFrustum( NiFrustum *frustum );
	// Turns the camera by the two angles about its up and right axes, as
	// one rotation; nothing in retail calls it (0x42BE90).
	void	Unknown42BE90( float upAngle, float rightAngle );
	// The rotation with its columns taken from the rows of the matrix,
	// in the orders 2, 0, 1 and 1, 2, 0; nothing in retail calls them
	// (0x42C030 and 0x42C0B0).
	static NiMatrix3	Unknown42C030( const NiMatrix3 &rotation );
	static NiMatrix3	Unknown42C0B0( const NiMatrix3 &rotation );
	// Projects the world point onto the screen through the NetImmerse
	// camera; false without one (0x42C130; invented name, after NiCamera's).
	bool	WorldPtToScreenPt( const NiPoint3 &point, float &x, float &y );
	void	PushCameraMode( CameraMode * );
	CameraMode *	GetCurrentCameraMode( bool );
	void	AddModeHierarchy( char *, char * );
	CameraMode *	GetNamedCameraMode( char * );
	void	AddNamedCameraMode( char *, CameraMode * );
	void	Zoom( float );
	static CameraObjectClass *	GetObject( SetClass *set, char *name_ );
	static int	GetNumClassObjects( SetClass * );
	static CameraObjectClass **	GetClassObjects( SetClass *set, int *count );		// invented name
	static CameraObjectClass *	GetObjectByID( SetClass *set, long id );			// invented name
	// Gives every camera the renderer: each CameraObjectClass (SetRenderer),
	// the module's default camera, the application's and the icon
	// manager's, then resets the glow for it (invented name).
	static void	SetAllRenderers( NiRender *renderer );

	bool			IsActive( void ) { return active; }		// invented name
	void			SetActive( bool active_ ) { active = active_; }	// invented name
	NiCamera *		GetNiCamera( void ) { return camera; }	// invented name
	// TODO identity: the object of the ID at 0x4C while 0x48 is set (a
	// mode's sweep reads its start from it); NULL otherwise
	TGObject *		Unknown42BDF0( void );
	// True while an animation blender of the camera's animation node is
	// playing (invented name; 0x42AA00)
	bool			IsAnimating( void );
	// The sweep and the turn (invented names).
	void			StartSweep( int count, NiPoint3 *points, float time, bool loop, long objectID );
	void			UpdateSweep( float elapsed );
	void			StopSweep( void );
	void			LookAt( NiPoint3 *point, NiPoint3 *up );
	// Starts a turn from the direction and up vector to the others, from the
	// rotation, the elapsed time into the duration (0x42B540).
	void			StartTurn( const NiPoint3 *fromForward, const NiPoint3 *fromUp, const NiPoint3 *forward, const NiPoint3 *up, const NiMatrix3 *rotation, float elapsed, float duration );
	void			UpdateTurn( float elapsed );
	void			StopTurn( void );
	void			Reorthogonalize( void );

	// The sweep: the camera moves along a curve over a time (invented names).
	float			sweepTime;			// 0x3C the time into the sweep
	float			sweepDuration;		// 0x40 the sweep's length in time
	bool			sweepLoops;			// 0x44 starts over at the end
	BezierCurve *	sweep;				// 0x48 the curve; NULL for none
	long			sweepObjectID;		// 0x4C the ID of the object the sweep starts from
	// The turn: the camera turns to a direction and up vector over a time,
	// eased in and out (invented names).
	NiPoint3		turnFromForward;	// 0x50
	NiPoint3		turnFromUp;			// 0x5C
	NiPoint3		turnToForward;		// 0x68 compared with the current mode's direction (slot 53)
	NiPoint3		turnToUp;			// 0x74 compared with the current mode's up vector (slot 53)
	NiPoint3		turnAxis;			// 0x80
	NiMatrix3		turnStart;			// 0x8C the rotation the turn starts from; the identity at first
	float			turnAngle;			// 0xB0 about the axis
	float			rollAngle;			// 0xB4 about the X axis
	float			turnTime;			// 0xB8 the time into the turn
	float			turnDuration;		// 0xBC the turn's length in time
	int				unknownC0;			// 0xC0 the rotation is reorthogonalised while it is a multiple of 8
	bool			turning;			// 0xC4 a turn is under way
	// The point the camera looks at, while lookingAt is set (invented names).
	NiPoint3		lookAtPoint;		// 0xC8
	NiPoint3		lookAtUp;			// 0xD4
	bool			lookingAt;			// 0xE0
	bool			active;				// 0xE1 the set's active camera (invented name)
	int				unknownE4;			// 0xE4 5 at first
	NiCamera *		camera;				// 0xE8 the NetImmerse camera (invented name)
	CameraMode *	currentMode;		// 0xEC the mode the camera is in; NULL for none (invented name)
	long			lastModeID;			// 0xF0 the ID of the mode the camera last left (invented name)
	NiTList<CameraMode *>	modeStack;	// 0xF4 the pushed modes, the top at the head (invented name)
	NiTStringMap<CameraMode *>	namedModes;	// 0x10C the modes by name, each holding a reference (invented name)
	NiTStringMap<char *>	modeHierarchy;	// 0x11C the name of each mode's fallback, a new[] copy, by the mode's name (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name
	static NiTList<CameraObjectClass *>	cameras;	// every camera there is (invented name)

	// Declared for the SWIG wrappers that call them.
public:
	// retail 0x42BCB0: the placement editor's overhead view passes plain
	// NiPoint3 locals (no EH frame), so the C++ parameters are NiPoint3s
	void	LookToward( NiPoint3 *, NiPoint3 *, float );
	// the scripts' form, which the SWIG wrapper CameraObjectClass_LookToward
	// calls (TGPoint3 is an NiPoint3; the casts pick the NiPoint3 form)
	void	LookToward( TGPoint3 *forward, TGPoint3 *up, float time ) { LookToward( (NiPoint3 *)forward, (NiPoint3 *)up, time ); }
	// TODO identity: moves the camera from where its NetImmerse camera is
	// to the position over the time, through 0x42B2E0 (0x42B360)
	void	SweepTo( NiPoint3 *position, float time );
};

/*
================
CameraObjectClass::IsTypeOf

Inline: ZoomCameraObjectClass's IsTypeOf expands the chain.
================
*/
// FUNCTION: STBC 0x00429d50 ?IsTypeOf@CameraObjectClass@@UAE_NH@Z @game/CameraObjectClass
inline bool CameraObjectClass::IsTypeOf( int type ) {
	if ( type == CAMERAOBJECTCLASS_TYPE ) {
		return true;
	}
	return BaseObjectClass::IsTypeOf( type );
}

#endif /* !__CAMERAOBJECTCLASS_H__ */
