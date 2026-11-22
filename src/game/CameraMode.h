/*
===============================================================================

	CameraMode.h

	A way of placing a camera: the modes the scripts make (CameraModes.py)
	are CameraModes with attributes, and the C++ modes (chase, target, map
	and the others) derive from it. A camera keeps a stack of modes and a
	map of them by name, and each holds a reference: the last release
	deletes the mode.

	0x6C bytes, the pool block the scalar deleting destructor (0x41EB40)
	frees. Evidence: the destructor (0x41EB70) stores the vtable 0x888DD8
	(34 slots: TGEventHandlerObject's 21 and 13 of CameraMode's own),
	deletes the objects of the list at 0x14 (an NiTList with two items per
	block), frees the name at 0x38 and destroys the three attribute maps
	(NiTStringMaps of 37 buckets) at 0x3C, 0x4C and 0x5C; GetObjType
	returns 0x80C2. The SWIG wrappers of the modes (classmap) name slots 21
	to 27; CameraObjectClass::Zoom (0x42C010) forwards to slot 33, and
	CameraObjectClass's slots 52 and 53 to slots 30 and 31. Slot 28 sets
	the flag at 0x34 and slot 29 clears it: the camera calls 28 on the
	mode it makes current and 29 on the one it leaves. The camera's
	PushCameraMode, AddNamedCameraMode and GetNamedCameraMode count the
	references at 0x30 and give the mode its name at 0x38 (0x41EDE0).
	The scripts pass SetAttrPoint a TGPoint3.

===============================================================================
*/

#ifndef __CAMERAMODE_H__
#define __CAMERAMODE_H__

#include "TGEventHandlerObject.h"
#include "NiTStringMap.h"
#include "NiPoint3.h"
#include "NiTList.h"
#include "NiQuaternion.h"
#include "NiMatrix3.h"
#include "TGPoint3.h"

class CameraObjectClass;
class ObjectClass;
class TGPoint3;
class BezierCurve;
class Waypoint;

// GetObjType's values of CameraMode and the modes derived from it (the
// CT_IDEAL_CONTROLLED_CAMERA_MODE constant of the App module, and the
// types the SWIG wrappers ChaseCameraMode_Cast and TargetCameraMode_Cast
// check).
#define CAMERAMODE_TYPE						0x80C2
#define IDEALCONTROLLEDCAMERAMODE_TYPE		0x80C3
#define CHASECAMERAMODE_TYPE				0x80C4
#define TARGETCAMERAMODE_TYPE				0x80C5
#define ZOOMTARGETMODE_TYPE					0x80C6
#define MAPCAMERAMODE_TYPE					0x80C7
#define DROPANDWATCHMODE_TYPE				0x80C8
#define PLACEMENTWATCHMODE_TYPE				0x80C9
#define LOCKEDPOSITIONMODE_TYPE				0x80CA
#define TORPCAMERAMODE_TYPE					0x80CB
#define PLACEBYDIRECTIONMODE_TYPE			0x80CC

class CameraLagTracker;
class TGStream;

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0041eb40 ??_GCameraMode@@UAEPAXI@Z
class CameraMode : public TGEventHandlerObject {
public:
	// The constructor (0x41E810) builds the list and the attribute maps,
	// stores the vtable, keeps the camera and sets the "Target" ID to none.
					CameraMode( CameraObjectClass *camera_ );
	// The loader's: UtopiaModule's creator expands it.
	// FUNCTION: STBC 0x0043dab0 ??0CameraMode@@QAE@J@Z @game/UtopiaModule
					CameraMode( long id ) : TGEventHandlerObject( id ), lagTrackers( 2 ) {}
	virtual			~CameraMode( void );

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

	// Defined in the class: the constructors call it directly and expand it, so
	// their nested SetAt gets a share of the budget (budget left / sites left)
	// and only the last attributes' SetAt is expanded.
	// FUNCTION: STBC 0x0041e9f0 ?SetAttrFloat@CameraMode@@UAEXPADM@Z @game/CameraMode
	virtual void	SetAttrFloat( char *attr, float value ) { floatAttrs.SetAt( attr, value ); }
	virtual void	SetAttrID( char *attr, long id );
	// Defined in the class like SetAttrFloat: the constructors pass it the
	// address of a TGPoint3 temporary, whose conversion they expand.
	// FUNCTION: STBC 0x0041ea80 ?SetAttrPoint@CameraMode@@UAEXPADPAVTGPoint3@@@Z @game/CameraMode
	virtual void	SetAttrPoint( char *attr, TGPoint3 *point ) { pointAttrs.SetAt( attr, *point ); }
	virtual void	SnapToIdealPosition( void );
	virtual void	Reset( void );
	virtual bool	IsValid( void );
	virtual void	Update( void );
	virtual void	VirtualSlot28( void );		// the camera made the mode current: sets the flag at 0x34
	virtual void	VirtualSlot29( void );		// the camera left the mode: clears the flag at 0x34
	virtual void	VirtualSlot30( BezierCurve *sweep );			// fills the camera sweep's control points
	virtual void	VirtualSlot31( NiPoint3 *forward, NiPoint3 *up );	// the direction the camera should face
	virtual void	VirtualSlot32( const NiMatrix3 &rotation );	// turns the camera by a world-space rotation
	virtual void	Zoom( float zoom );			// invented name: CameraObjectClass::Zoom forwards to it

	void			SetAttrIDObject( char *, ObjectClass * );
	ObjectClass *	GetAttrIDObject( char * );
	TGPoint3		GetAttrPoint( char *attr );
	// The float attribute; -1 when the mode has none.
	// FUNCTION: STBC 0x00423420 ?GetAttrFloat@CameraMode@@QAEMPAD@Z @game/CameraMode
	float			GetAttrFloat( char *attr ) { float value = -1.0f; floatAttrs.GetAt( attr, value ); return value; }
	// The ID attribute; 0 when the mode has none.
	long			GetAttrID( char *attr ) { long id = 0; idAttrs.GetAt( attr, id ); return id; }
	static CameraMode *	Create( char *, CameraObjectClass * );

	// How a mode is made: the function the registry holds under the mode's
	// class name (invented names; the registry is at 0x97ED78).
	typedef CameraMode *	( *CreateFunction )( CameraObjectClass *camera );
	// Files the function in the registry under the class name (invented name).
	static void		RegisterCreator( const char *className, CreateFunction create ) { creators.SetAt( className, create ); }

	char *			GetName( void ) { return name; }				// invented name
	void			SetName( char *name_ );							// invented name: keeps a new[] copy
	void			AddReference( void ) { referenceCount++; }		// invented name
	// Deletes the mode when the last reference goes (invented name).
	void			RemoveReference( void ) { if ( --referenceCount <= 0 ) delete this; }

	// Register the handler of the modes by name, and add the class handlers.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	// Saves every mode, and loads them back (invented names).
	static bool		SaveModes( TGStream *stream );
	static bool		LoadModes( TGStream *stream );

	// An object a mode watches became a hulk.
	void			ObjectChangedToHulk( TGEvent *event );

	// Records where each watched object is now; the camera calls it before
	// the mode's update (invented name).
	void			UpdateLagTrackers( void );

protected:
	// The lag records of the objects the mode watches, counted per object
	// (invented names).
	void			StartLagTracking( ObjectClass *object );
	void			StopLagTracking( long id );

	// The distance scaled by the zoom and kept between the limits (invented name).
	static float	GetZoomedDistance( float distance, float minimum, float maximum, float zoom );

	// Where the object was, and how it was turned, lag seconds ago by its lag
	// record; -1 takes a lag from the object's size, and the "LagPos" and
	// "LagRot" attributes override it (invented names, after the tracker's).
	NiPoint3		GetLaggedPosition( ObjectClass *object, float lag );
	NiMatrix3		GetLaggedRotation( ObjectClass *object, float lag );
	// Moves the camera position along its view of the point until it is
	// clear of the target and not inside the source (invented name).
	void			KeepClearOfObjects( NiPoint3 *position, const NiPoint3 *lookAt, ObjectClass *target, ObjectClass *source );

	NiTList<CameraLagTracker *>	lagTrackers;	// 0x14 one per watched object (invented name)
	CameraObjectClass *	camera;			// 0x2C the camera the mode places (invented name)
	int				referenceCount;		// 0x30 the camera's stack and name map hold references (invented name)
	bool			unknown34;			// 0x34 set while the mode is its camera's current one
	char *			name;				// 0x38 the name the camera files the mode under (invented name)
	NiTStringMap<float>		floatAttrs;		// 0x3C SetAttrFloat's values (invented name)
	NiTStringMap<long>		idAttrs;		// 0x4C SetAttrID's object IDs (invented name)
	NiTStringMap<NiPoint3>	pointAttrs;		// 0x5C SetAttrPoint's values (invented name)

	static NiTStringMap<CreateFunction>	creators;	// the functions that make each class of mode, by class name (invented name)
	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
CameraMode::IsTypeOf
================
*/
// FUNCTION: STBC 0x0041e970
inline bool CameraMode::IsTypeOf( int type ) {
	if ( type == CAMERAMODE_TYPE ) {
		return true;
	}
	return TGEventHandlerObject::IsTypeOf( type );
}

/*
================
CameraMode::VirtualSlot28

The camera made the mode current.
================
*/
// FUNCTION: STBC 0x0041eaf0
inline void CameraMode::VirtualSlot28( void ) {
	unknown34 = true;
}

/*
================
CameraMode::GetAttrPoint

The point attribute; the origin when the mode has none.
================
*/
// FUNCTION: STBC 0x00421290
inline TGPoint3 CameraMode::GetAttrPoint( char *attr ) {
	TGPoint3 point = TGPoint3::ZERO;
	pointAttrs.GetAt( attr, point );
	return point;
}

/*
	A watched object's placement at one time (0x20 bytes; invented names).
*/
struct CameraLagSample {
	float			time;				// 0x00 the game time of the sample
	NiPoint3		position;			// 0x04 the world location
	NiQuaternion	rotation;			// 0x10 the world rotation
};

/*
	One watched object's recent placements, kept for the modes that follow
	an object with a lag (the "LagPos" and "LagRot" attributes). 0x24 bytes:
	the mode's list holds one per object, counted by the modes that watch it.
	Invented names.
*/
class CameraLagTracker {
public:
					CameraLagTracker( void ) : samples( 2 ) {}
					CameraLagTracker( ObjectClass *object, float lagTime_ );
					~CameraLagTracker( void );

	bool			WriteToStream( TGStream *stream );
	bool			ReadFromStream( TGStream *stream );

	// Records where the object is now, and drops the placements older than
	// the lag time.
	void			AddSample( void );

	// Where the object was, and how it was turned, lag seconds ago.
	NiPoint3		GetLaggedPosition( float lag );
	NiMatrix3		GetLaggedRotation( float lag );

	static bool		IsRedundant( CameraLagSample *previous, CameraLagSample *sample, CameraLagSample *next );
	static NiPoint3	InterpolatePosition( CameraLagSample *from, CameraLagSample *to, float time );
	static NiQuaternion	InterpolateRotation( CameraLagSample *from, CameraLagSample *to, float time );

	// IsRedundant's limits (0x888DCC-0x888DD4)
	static const float	MAX_REDUNDANT_INTERVAL;
	static const float	MAX_REDUNDANT_DISTANCE_SQUARED;
	static const float	MIN_REDUNDANT_ROTATION_DOT;

	int				referenceCount;		// 0x00 the watches of the object
	long			objectID;			// 0x04 the object
	float			lagTime;			// 0x08 how long a placement is kept
	NiTList<CameraLagSample *>	samples;	// 0x0C oldest first
};

/*
	The modes the camera moves toward an ideal position: 0x70 bytes, vtable
	0x888EB8 (36 slots: CameraMode's 34 and two of its own), GetObjType
	0x80C3. The chase, target, zoom target, map and torpedo modes derive
	from it. Its constructor sets the "SweepTime", "PositionThreshold",
	"DotThreshold" and "MaxUpAngleChange" attributes. Slot 30 fills a
	camera sweep's control points from the mode swept from and this one;
	slot 31 gives the direction the camera should face.
*/
// FUNCTION: STBC 0x00421070 ??_GIdealControlledCameraMode@@UAEPAXI@Z
// FUNCTION: STBC 0x004210a0 ??1IdealControlledCameraMode@@UAE@XZ
class IdealControlledCameraMode : public CameraMode {
public:
					IdealControlledCameraMode( CameraObjectClass *camera_ );
	// The loader's: UtopiaModule's creator expands it.
	// FUNCTION: STBC 0x0043dc10 ??0IdealControlledCameraMode@@QAE@J@Z @game/UtopiaModule
					IdealControlledCameraMode( long id ) : CameraMode( id ) {}

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

	// CameraMode
	virtual void	SnapToIdealPosition( void );
	virtual void	Update( void );
	virtual void	VirtualSlot28( void );
	virtual void	VirtualSlot30( BezierCurve *sweep );
	virtual void	VirtualSlot31( NiPoint3 *forward, NiPoint3 *up );

	// Where the camera should be, and how it should face (invented names).
	virtual NiPoint3	GetIdealPosition( void );
	virtual void	GetIdealDirection( NiPoint3 *forward, NiPoint3 *up );

	// Moves the camera toward the ideal position when it is too far from
	// it: at once with no "SweepTime", else over that time. Slot 28 calls
	// it when the mode becomes current.
	void			Unknown421410( void );
	// TODO identity: called by Update when the mode was current already;
	// true when it placed the camera
	bool			Unknown421910( void );

protected:
	bool			unknown6C;			// 0x6C cleared by slot 28, set by Update

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
IdealControlledCameraMode::IsTypeOf
================
*/
// FUNCTION: STBC 0x00420fa0
inline bool IdealControlledCameraMode::IsTypeOf( int type ) {
	if ( type == IDEALCONTROLLEDCAMERAMODE_TYPE ) {
		return true;
	}
	return CameraMode::IsTypeOf( type );
}

/*
	The chase mode (0x7C bytes, vtable 0x888F48, GetObjType 0x80C4): the
	camera follows the "Target" object from behind ("Distance" between
	"MinimumDistance" and "MaximumDistance"). Each mode class has two Casts
	in retail: the one the SWIG wrapper calls (0x41DA60) and one for a
	CameraMode (0x41D8B0), which the editor calls with the camera's modes.
*/
// FUNCTION: STBC 0x00421fb0 ??_GChaseCameraMode@@UAEPAXI@Z
// FUNCTION: STBC 0x00421fe0 ??1ChaseCameraMode@@UAE@XZ
class ChaseCameraMode : public IdealControlledCameraMode {
public:
					ChaseCameraMode( CameraObjectClass *camera_ );
	// The loader's: UtopiaModule's creator expands it.
	// FUNCTION: STBC 0x0043dd70 ??0ChaseCameraMode@@QAE@J@Z @game/UtopiaModule
					ChaseCameraMode( long id ) : IdealControlledCameraMode( id ) {}
	// The object as a chase mode; NULL when it is none (the SWIG Cast).
	static ChaseCameraMode *	Cast( TGObject *object );

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

	// CameraMode
	virtual void	SetAttrPoint( char *attr, TGPoint3 *point );
	virtual void	Reset( void );
	virtual bool	IsValid( void );
	virtual void	VirtualSlot32( const NiMatrix3 &rotation );
	virtual void	Zoom( float zoom );

	// IdealControlledCameraMode
	virtual NiPoint3	GetIdealPosition( void );
	virtual void	GetIdealDirection( NiPoint3 *forward, NiPoint3 *up );

	static ChaseCameraMode *	Cast( CameraMode *mode );
	// The registry's function for "Chase" (invented name).
	static CameraMode *	Create( CameraObjectClass *camera_ );

protected:
	NiPoint3		unknown70;			// 0x70 saved and loaded with the mode

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
	The target mode (0x70 bytes, vtable 0x888FE0, GetObjType 0x80C5): the
	camera looks from the "Source" object at the "Target" object.
*/
// FUNCTION: STBC 0x00422b90 ??_GTargetCameraMode@@UAEPAXI@Z
// FUNCTION: STBC 0x00422bc0 ??1TargetCameraMode@@UAE@XZ
class TargetCameraMode : public IdealControlledCameraMode {
public:
					TargetCameraMode( CameraObjectClass *camera_ );
	// The loader's: UtopiaModule's creator expands it.
	// FUNCTION: STBC 0x0043de50 ??0TargetCameraMode@@QAE@J@Z @game/UtopiaModule
					TargetCameraMode( long id ) : IdealControlledCameraMode( id ) {}
	// The object as a target mode; NULL when it is none (the SWIG Cast).
	static TargetCameraMode *	Cast( TGObject *object );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// CameraMode
	virtual void	SetAttrID( char *attr, long id );
	virtual void	Reset( void );
	virtual bool	IsValid( void );
	virtual void	VirtualSlot28( void );
	virtual void	VirtualSlot32( const NiMatrix3 &rotation );
	virtual void	Zoom( float zoom );

	// IdealControlledCameraMode
	virtual NiPoint3	GetIdealPosition( void );
	virtual void	GetIdealDirection( NiPoint3 *forward, NiPoint3 *up );

	static TargetCameraMode *	Cast( CameraMode *mode );
	// The registry's function for "Target" (invented name).
	static CameraMode *	Create( CameraObjectClass *camera_ );

protected:
	// The target offset, eased in from the start of a between sweep (invented name).
	NiPoint3		GetTargetOffset( void );

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
	The zoom target mode (0x70 bytes, vtable 0x889070, GetObjType 0x80C6).
*/
// FUNCTION: STBC 0x00423c60 ??_GZoomTargetMode@@UAEPAXI@Z
// FUNCTION: STBC 0x00423c90 ??1ZoomTargetMode@@UAE@XZ
class ZoomTargetMode : public IdealControlledCameraMode {
public:
					ZoomTargetMode( CameraObjectClass *camera_ );
	// The loader's: UtopiaModule's creator expands it.
	// FUNCTION: STBC 0x0043dfb0 ??0ZoomTargetMode@@QAE@J@Z @game/UtopiaModule
					ZoomTargetMode( long id ) : IdealControlledCameraMode( id ) {}
	static ZoomTargetMode *	Cast( CameraMode *mode );
	// The object as a zoom target mode; NULL when it is none (the SWIG Cast).
	static ZoomTargetMode *	Cast( TGObject *object );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// CameraMode
	virtual bool	IsValid( void );
	virtual void	Zoom( float zoom );

	// IdealControlledCameraMode
	virtual NiPoint3	GetIdealPosition( void );
	virtual void	GetIdealDirection( NiPoint3 *forward, NiPoint3 *up );

	// The registry's function for "ZoomTarget" (invented name).
	static CameraMode *	Create( CameraObjectClass *camera_ );

protected:
	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
	The map mode (0x7C bytes, vtable 0x889100, GetObjType 0x80C7).
*/
// FUNCTION: STBC 0x004246c0 ??_GMapCameraMode@@UAEPAXI@Z
// FUNCTION: STBC 0x004246f0 ??1MapCameraMode@@UAE@XZ
class MapCameraMode : public IdealControlledCameraMode {
public:
					MapCameraMode( CameraObjectClass *camera_ );
	// The loader's: UtopiaModule's creator expands it.
	// FUNCTION: STBC 0x0043e110 ??0MapCameraMode@@QAE@J@Z @game/UtopiaModule
					MapCameraMode( long id ) : IdealControlledCameraMode( id ) {}
	static MapCameraMode *	Cast( CameraMode *mode );
	// The object as a map mode; NULL when it is none (the SWIG Cast).
	static MapCameraMode *	Cast( TGObject *object );

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

	// CameraMode
	virtual bool	IsValid( void );
	virtual void	VirtualSlot28( void );
	virtual void	VirtualSlot32( const NiMatrix3 &rotation );
	virtual void	Zoom( float zoom );

	// IdealControlledCameraMode
	virtual NiPoint3	GetIdealPosition( void );
	virtual void	GetIdealDirection( NiPoint3 *forward, NiPoint3 *up );

	// The registry's function for "Map" (invented name).
	static CameraMode *	Create( CameraObjectClass *camera_ );

protected:
	NiPoint3		unknown70;			// 0x70 saved and loaded with the mode

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
	The drop-and-watch mode (0x6C bytes, vtable 0x889194, GetObjType 0x80C8).
*/
// FUNCTION: STBC 0x00425130 ??_GDropAndWatchMode@@UAEPAXI@Z
// FUNCTION: STBC 0x00425160 ??1DropAndWatchMode@@UAE@XZ
class DropAndWatchMode : public CameraMode {
public:
					DropAndWatchMode( CameraObjectClass *camera_ );
	// The loader's: UtopiaModule's creator expands it.
	// FUNCTION: STBC 0x0043e1f0 ??0DropAndWatchMode@@QAE@J@Z @game/UtopiaModule
					DropAndWatchMode( long id ) : CameraMode( id ) {}
	static DropAndWatchMode *	Cast( CameraMode *mode );
	// The object as a drop-and-watch mode; NULL when it is none (the SWIG Cast).
	static DropAndWatchMode *	Cast( TGObject *object );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// CameraMode
	virtual bool	IsValid( void );
	virtual void	Update( void );
	virtual void	VirtualSlot28( void );
	virtual void	VirtualSlot32( const NiMatrix3 &rotation );

	// The registry's function for "DropAndWatch" (invented name).
	static CameraMode *	Create( CameraObjectClass *camera_ );

protected:
	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
	The placement watch mode (0x6C bytes, vtable 0x889224, GetObjType 0x80C9).
*/
// FUNCTION: STBC 0x004261d0 ??_GPlacementWatchMode@@UAEPAXI@Z
// FUNCTION: STBC 0x00426200 ??1PlacementWatchMode@@UAE@XZ
class PlacementWatchMode : public CameraMode {
public:
					PlacementWatchMode( CameraObjectClass *camera_ );
	// The loader's: UtopiaModule's creator expands it.
	// FUNCTION: STBC 0x0043e350 ??0PlacementWatchMode@@QAE@J@Z @game/UtopiaModule
					PlacementWatchMode( long id ) : CameraMode( id ) {}
	static PlacementWatchMode *	Cast( CameraMode *mode );
	// The object as a placement watch mode; NULL when it is none (the SWIG Cast).
	static PlacementWatchMode *	Cast( TGObject *object );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// CameraMode
	virtual void	SetAttrID( char *attr, long id );
	virtual bool	IsValid( void );
	virtual void	Update( void );
	virtual void	VirtualSlot32( const NiMatrix3 &rotation );

	// The registry's function for "PlacementWatch" (invented name).
	static CameraMode *	Create( CameraObjectClass *camera_ );

	// Moves the source along its waypoint path (invented name).
	void			MoveAlongPath( Waypoint *waypoint, Waypoint *next, NiPoint3 *position, NiPoint3 *up );

protected:
	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
	The locked position mode (0x6C bytes, vtable 0x8892AC, GetObjType 0x80CA).
*/
// FUNCTION: STBC 0x004269c0 ??_GLockedPositionMode@@UAEPAXI@Z
// FUNCTION: STBC 0x004269f0 ??1LockedPositionMode@@UAE@XZ
class LockedPositionMode : public CameraMode {
public:
					LockedPositionMode( CameraObjectClass *camera_ );
	// The loader's: UtopiaModule's creator expands it.
	// FUNCTION: STBC 0x0043e4b0 ??0LockedPositionMode@@QAE@J@Z @game/UtopiaModule
					LockedPositionMode( long id ) : CameraMode( id ) {}
	static LockedPositionMode *	Cast( CameraMode *mode );
	// The object as a locked position mode; NULL when it is none (the SWIG Cast).
	static LockedPositionMode *	Cast( TGObject *object );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// CameraMode
	virtual bool	IsValid( void );
	virtual void	Update( void );
	virtual void	VirtualSlot32( const NiMatrix3 &rotation );

	// The registry's function for "Locked" (invented name).
	static CameraMode *	Create( CameraObjectClass *camera_ );

protected:
	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
	The torpedo mode (0x70 bytes, vtable 0x889334, GetObjType 0x80CB): the
	camera follows a torpedo the player fired ("MainTorp"), and stays a
	while after it is gone. The vtable's last two slots override the ideal
	position (0x4272D0) and direction (0x427550).
*/
// FUNCTION: STBC 0x00426f60 ??_GTorpCameraMode@@UAEPAXI@Z
// FUNCTION: STBC 0x00426f90 ??1TorpCameraMode@@UAE@XZ
class TorpCameraMode : public IdealControlledCameraMode {
public:
					TorpCameraMode( CameraObjectClass *camera_ );
	// The loader's: UtopiaModule's creator calls it.
	// FUNCTION: STBC 0x0043e610 ??0TorpCameraMode@@QAE@J@Z @game/UtopiaModule
					TorpCameraMode( long id ) : IdealControlledCameraMode( id ) {}

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// CameraMode
	virtual bool	IsValid( void );
	virtual void	Update( void );
	virtual void	VirtualSlot32( const NiMatrix3 &rotation );

	// IdealControlledCameraMode
	virtual NiPoint3	GetIdealPosition( void );
	virtual void	GetIdealDirection( NiPoint3 *forward, NiPoint3 *up );

	// The registry's function for the mode (invented name).
	static CameraMode *	Create( CameraObjectClass *camera_ );

	static TorpCameraMode *	Cast( CameraMode *mode );
	// The object as a torpedo mode; NULL when it is none (the SWIG Cast).
	static TorpCameraMode *	Cast( TGObject *object );
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	// A torpedo was fired.
	void			TorpFired( TGEvent *event );

protected:
	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
PlaceByDirectionMode

Evidence: the vtable 0x8893C4 (34 slots, CameraMode's, overriding the
deleting destructor, GetObjType 0x427AA0 returning 0x80CC, IsTypeOf, the
three class strings, GetClassHandlers and Update); the SWIG wrapper
PlaceByDirectionMode_Cast (0x5BC880) calls the TGObject Cast. The base is
the one the vtable shows: App.pyc has no shadow class for it.
================
*/
// FUNCTION: STBC 0x00427b40 ??_GPlaceByDirectionMode@@UAEPAXI@Z
// FUNCTION: STBC 0x00427b70 ??1PlaceByDirectionMode@@UAE@XZ
class PlaceByDirectionMode : public CameraMode {
public:
	static PlaceByDirectionMode *	Cast( CameraMode *mode );
	// The object as a place-by-direction mode; NULL when it is none (the SWIG Cast).
	static PlaceByDirectionMode *	Cast( TGObject *object );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// CameraMode
	virtual void	Update( void );

	// TODO identity: the camera position the mode gives for looking at the
	// point (0x427E50); SetClass::Unknown414270 and CharacterClass::LookAtMe
	// call it
	NiPoint3		Unknown427E50( const NiPoint3 &target );

	// The registry's function for the mode (invented name).
	static CameraMode *	Create( CameraObjectClass *camera_ );

					PlaceByDirectionMode( CameraObjectClass *camera_ );
	// The loader's: UtopiaModule's creator expands it.
	// FUNCTION: STBC 0x0043e770 ??0PlaceByDirectionMode@@QAE@J@Z @game/UtopiaModule
					PlaceByDirectionMode( long id ) : CameraMode( id ) {}

protected:
	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__CAMERAMODE_H__ */
