/*
===============================================================================

	PhysicsObjectClass.h

	An object of a set that moves: it has a velocity, an acceleration and
	an angular motion, takes forces, and can be driven by an AI.

	0x108 bytes, allocated from the pools (the scalar deleting destructor
	0x59FF20 frees through the pool of that size).

	Evidence: the constructors (0x59FD60 by ID, 0x59FF50 from a NetImmerse
	object) call ObjectClass's, construct the 37-bucket maps at 0x68
	(vtable 0x89427C) and 0xF4 (vtable 0x894268), store the vtable
	0x894128 (80 slots; 67-79 are new) and call InitObject (0x5A0040). The
	destructor (0x5A0200) deletes the values of the map at 0x68. GetObjType
	returns 0x8006, the type Cast checks. The handler names are strings
	(0x8E5E84-0x8E5ED8). The SWIG wrappers name slots 75
	(TurnTowardOrientation), 76 (SetAcceleration), 77 (SetAI) and 78
	(ClearAI) through
	the calls they make. Slot 75 is an inline that returns 0.0f (kept at
	0x5910D0), slot 76 an inline that stores the point at 0xA8 (kept at
	0x578500);
	slots 24 and 49 only call ObjectClass's. InitObject sets the members
	from 0xA8 on; the SWIG wrappers name the ones they read or write
	(GetAcceleration 0xA8, GetAngularAcceleration 0xB4, GetAngularVelocity
	the axis 0xCC times the speed 0xE0, GetMass 0xD8, GetRotationalInertia
	0xDC, GetAngularDirectionType 0xE8, Get/SetNetType 0xEC,
	SetUsePhysics 0xF0, SetDoNetUpdate 0xF1, IsStatic 0x104; SetStatic also
	clears 0x105). The key and value types of the two maps are open (both
	hash their 4-byte key by value; the destructor deletes the values of
	the map at 0x68, objects with the non-virtual destructor 0x6A8CE0).

===============================================================================
*/

#ifndef __PHYSICSOBJECTCLASS_H__
#define __PHYSICSOBJECTCLASS_H__

#include "ObjectClass.h"
#include "NiTMap.h"
#include "TGPoint3.h"

class ArtificialIntelligence;
class MultiplayerPlayer;
class Force;
class NiAVObject;
class NiMatrix3;
class NiPoint3;
class SetClass;
class TGEvent;
class TGMessage;
class TGObject;
class TGPoint3;
class UnknownClass6A8C50;

// GetObjType's value for a PhysicsObjectClass.
#define PHYSICSOBJECTCLASS_TYPE		0x8006

// Event types, as the SWIG constants of the App module give them.
#define ET_AI_FINISHED_BUILDING		0x800024
#define ET_OBJECT_DESTROYED			0x80004F
#define ET_OBJECT_CREATED			0x8000C8
#define ET_OBJECT_CREATED_NOTIFY	0x8000C9

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class PhysicsObjectClass : public ObjectClass {
public:
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
	virtual void	SetContainingSet( SetClass *set );
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );
	virtual void	UpdateNode( float time );

	// The parameter types of slots 67-73 and 79 are unproven: only their
	// counts are known, from ret N. Slots 68-72 return the stream position
	// they end on (Torpedo's slot 72 returns 0, its slot 70 0 on failure).
	virtual int		VirtualSlot67( char *buffer, int size );	// writes slots 68 and 69 into the buffer; returns the length
	virtual int		VirtualSlot68( TGStream *stream );	// writes the type, the ID and the net type to a network message
	virtual int		VirtualSlot69( TGStream *stream );	// writes the object's state to a network message
	virtual int		VirtualSlot70( TGStream *stream );	// reads the net type from a network message
	virtual int		VirtualSlot71( TGStream *stream );	// reads the state VirtualSlot69 writes
	virtual TGMessage *	VirtualSlot72( MultiplayerPlayer *player );	// the object's state update for the player's place (MultiplayerGame::SendShipUpdates); NULL for none
	virtual void	VirtualSlot73( TGMessage *message );	// reads a state update VirtualSlot72 made (MultiplayerGame::ReceiveShipUpdateMessage)
	virtual void	SetupModel( char *modelName );	// clones the model by name and shows it
	// FUNCTION: STBC 0x005910d0 ?TurnTowardOrientation@PhysicsObjectClass@@UAEMPAVTGPoint3@@0@Z @game/DamageableObject
	virtual float	TurnTowardOrientation( TGPoint3 *forward, TGPoint3 *up ) { return 0.0f; }	// parameter names invented
	// FUNCTION: STBC 0x00578500 ?SetAcceleration@PhysicsObjectClass@@UAEXABVNiPoint3@@@Z @game/Torpedo
	virtual void	SetAcceleration( const NiPoint3 &acceleration_ ) { acceleration = acceleration_; }
	virtual int		SetAI( ArtificialIntelligence *ai, bool unknown1 = true, bool unknown2 = true );	// the bool meanings are open
	virtual void	ClearAI( bool unknown = true, ArtificialIntelligence *ai = NULL );	// the bool's meaning is open
	virtual bool	VirtualSlot79( MultiplayerPlayer *player );	// whether the object's state update goes to the player now; throttles by player count, connection and distance

					PhysicsObjectClass( long id = 0 );
					PhysicsObjectClass( NiAVObject *object );
	virtual			~PhysicsObjectClass( void );

	static PhysicsObjectClass *	Cast( TGObject * );
	static PhysicsObjectClass *	ConstCast( TGObject * );
	static PhysicsObjectClass *	Create( NiAVObject * );
	void	ApplyForce( Force * );
	// Takes an NiPoint3: ReadFromStream (0x5A1AF0) passes a local without a
	// destructor (no EH state), which a TGPoint3 would need.
	void	SetVelocity( const NiPoint3 * );
	void	SetAngularVelocity( const NiPoint3 *, int );
	void	SetAngularAcceleration( const NiPoint3 * );
	void	SetAngularAccelerationLinear( NiPoint3 *, NiPoint3 *, float );
	void	SetAngularDirectionType( int );
	static PhysicsObjectClass *	GetObject( SetClass *, char * );
	static PhysicsObjectClass *	GetObjectByID( SetClass *set, long id );
	static PhysicsObjectClass **	GetClassObjects( SetClass *set, int *count );		// invented name
	static int	GetNumClassObjects( SetClass *set );
	// The object a network message describes; NULL when its ID is taken (invented name).
	static PhysicsObjectClass *	CreateFromNetBuffer( char *buffer, int size );
	ArtificialIntelligence *	GetAI( void );
	bool	HasBuildingAIs( void );

	void			SetStatic( bool isStatic_ ) { isStatic = isStatic_; unknown105 = false; }
	bool			IsStatic( void ) { return isStatic; }
	float			GetMass( void ) { return mass; }
	void			SetMass( float mass_ ) { mass = mass_; }
	float			GetRotationalInertia( void ) { return rotationalInertia; }
	void			SetRotationalInertia( float inertia ) { rotationalInertia = inertia; }
	const NiPoint3 &	GetVelocity( void );		// the node's velocity; zero without a node
	const TGPoint3 &	GetVelocityTG( void ) { return static_cast<const TGPoint3 &>( GetVelocity() ); }
	const NiPoint3 &	GetAcceleration( void ) { return acceleration; }
	const NiPoint3 &	GetAngularAcceleration( void ) { return angularAcceleration; }
	// FUNCTION: STBC 0x00579a30 ?GetAngularVelocity@PhysicsObjectClass@@QAE?AVNiPoint3@@XZ @game/ShipClass
	NiPoint3		GetAngularVelocity( void ) { return angularAxis * angularSpeed; }
	// Where a body starting at a position moves in a time under a constant velocity and acceleration (invented name).
	NiPoint3		PredictPosition( const NiPoint3 &position, const NiPoint3 &velocity, const NiPoint3 &acceleration_, float time );
	// PredictPosition for the scripts (the SWIG wrapper PhysicsObjectClass_GetPredictedPosition, 0x607CC0).
	TGPoint3		GetPredictedPosition( const NiPoint3 *position, const NiPoint3 *velocity, const NiPoint3 *acceleration, float time );
	// Copies the motion state out: the node's translation, the velocity, the acceleration, the angular axis and
	// speed, the angular acceleration, the members at 0xC0 and 0xE4 and the rotation (invented name; the order
	// is the memory order of the members it reads).
	void			Unknown5A08F0( NiPoint3 *position, NiPoint3 *velocity, NiPoint3 *acceleration, NiPoint3 *angularAxis, float *angularSpeed, NiPoint3 *angularAcceleration, NiPoint3 *unknownC0, float *unknownE4, NiMatrix3 *rotation );
	// Advances such a state by a time (OptimizedAvoidObstacles steps the ships it looks ahead with it): the
	// position and velocity by PredictPosition's rule, then the angular members through 0x5A0D80 and 0x5A1240.
	void			Unknown5A09D0( float time, NiPoint3 *position, NiPoint3 *velocity, NiPoint3 *acceleration, NiPoint3 *angularAxis, float *angularSpeed, NiPoint3 *angularAcceleration, NiPoint3 *unknownC0, float *unknownE4, NiMatrix3 *rotation );
	// The rotation after a time (OptimizedFireScript, 0x484C8B, turns a subsystem's offset by it); the
	// SWIG wrapper PhysicsObjectClass_GetPredictedRotationTG returns it as a TGMatrix3.
	NiMatrix3		GetPredictedRotation( float time );
	// FUNCTION: STBC 0x00607e90 ?GetPredictedRotationTG@PhysicsObjectClass@@QAE?AVTGMatrix3@@M@Z @game/AppWrap
	TGMatrix3		GetPredictedRotationTG( float time ) { return GetPredictedRotation( time ); }
	// The angular velocity part of the step: the angular acceleration over the time left of its duration,
	// blended from start toward end (both written to; called by 0x5A0D80 and 0x5A1240; invented name).
	void			Unknown5A1150( NiPoint3 *startOut, NiPoint3 *blended, float time, float *duration, NiPoint3 *start, NiPoint3 *end );
	// Turns the rotation by the step's angular motion: about the world axes for angular direction type 0,
	// about the object's own axes for type 1 (invented name).
	void			Unknown5A0D80( float time, NiPoint3 *angularAxis_, float *angularSpeed_, NiPoint3 *angularAcceleration_, NiPoint3 *unknownC0_, float *unknownE4_, NiMatrix3 *rotation );
	// Turns the angular velocity (axis and speed) by the step's angular acceleration, then counts the
	// acceleration's duration down, clearing both accelerations once it is spent (invented name).
	void			Unknown5A1240( float time, NiPoint3 *angularAxis_, float *angularSpeed_, NiPoint3 *angularAcceleration_, NiPoint3 *unknownC0_, float *unknownE4_ );
	const TGPoint3 &	GetAccelerationTG( void ) { return static_cast<const TGPoint3 &>( acceleration ); }
	const TGPoint3 &	GetAngularAccelerationTG( void ) { return static_cast<const TGPoint3 &>( angularAcceleration ); }
	NiPoint3		GetAngularVelocityTG( void ) { return GetAngularVelocity(); }
	int				GetAngularDirectionType( void ) { return angularDirectionType; }
	void			SetNetType( int netType_ ) { netType = netType_; }
	int				GetNetType( void ) { return netType; }
	void			SetDoNetUpdate( bool doNetUpdate_ ) { doNetUpdate = doNetUpdate_; }
	bool			IsDoingNetUpdate( void ) { return doNetUpdate; }
	void			SetUsePhysics( bool usePhysics_ ) { usePhysics = usePhysics_; }
	bool			IsUsingPhysics( void ) { return usePhysics; }

	// Register the class's handler methods by name, and add them to its table.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			DeleteMeStage1( TGEvent *event );
	void			ObjectDestroyedHandler( TGEvent *event );
	void			AIFinishedBuilding( TGEvent *event );

protected:
	void			InitObject( long id );		// invented name

	NiTMap<unsigned int, UnknownClass6A8C50 *>	unknown68;	// 0x68 owns its values; the key type is unproven
	float			unknown78;			// 0x78 the time of the last position part read (VirtualSlot73)
	float			unknown7C;			// 0x7C the time of the last forward axis read
	float			unknown80;			// 0x80 the time of the last up axis read
	float			unknown84;			// 0x84 the time of the last speed read
	float			unknown88;			// 0x88 the time of the last whole position read
	float			unknown8C;			// 0x8C the time of the last extrapolation
	NiPoint3		unknown90;			// 0x90 set to the position by VirtualSlot71
	NiPoint3		unknown9C;			// 0x9C set to the position by VirtualSlot71
	NiPoint3		acceleration;		// 0xA8
	NiPoint3		angularAcceleration;	// 0xB4
	NiPoint3		unknownC0;			// 0xC0
	NiPoint3		angularAxis;		// 0xCC the axis of the angular velocity (invented name)
	float			mass;				// 0xD8 1 at first
	float			rotationalInertia;	// 0xDC 1 at first
	float			angularSpeed;		// 0xE0 the speed about angularAxis (invented name)
	float			unknownE4;			// 0xE4
	int				angularDirectionType;	// 0xE8
	int				netType;			// 0xEC
	bool			usePhysics;			// 0xF0 true at first
	bool			doNetUpdate;		// 0xF1 true at first
	// The key and value types are unproven: SetAI (0x5A15A0) keys it by an
	// ID; it is another instantiation than the map at 0x68 (vtable 0x894268).
	NiTMap<long, long>	unknownF4;		// 0xF4
	bool			isStatic;			// 0x104
	bool			unknown105;			// 0x105

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
PhysicsObjectClass::IsTypeOf
================
*/
// FUNCTION: STBC 0x0059fe60
inline bool PhysicsObjectClass::IsTypeOf( int type ) {
	if ( type == PHYSICSOBJECTCLASS_TYPE ) {
		return true;
	}
	return ObjectClass::IsTypeOf( type );
}

#endif /* !__PHYSICSOBJECTCLASS_H__ */
