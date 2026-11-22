/*
===============================================================================

	DamageableObject.h

	A physics object that takes damage: it has a property set, a life
	time once it starts dying, splash damage, and the damage geometry the
	visible damage is drawn on.

	0x1E4 bytes, allocated from the pools (the scalar deleting destructor
	0x5911D0 frees through the pool of that size).

	Evidence: the vptr 0x893D88 (90 slots; 80-89 are new) is stored by the
	constructors (0x591033, 0x591372, 0x591580) and the destructor
	0x591620. GetObjType returns 0x8007, the type Cast checks. The SWIG
	wrappers name slots 85 (AddDamage) and 86 (SetupProperties) through
	the calls they make, and the members they read or write inline:
	GetClonedModelRadius and HasClonedModel 0x148, GetClonedModelCount
	0x1E0, GetPropertySet 0x144, Get/SetLifeTime and IsDying 0x14C, IsDead and
	SetDead 0x150, Get/SetSplashDamage 0x154 and GetSplashDamageRadius
	0x158, SetVisibleDamageRadiusModifier 0x1B8 and
	SetVisibleDamageStrengthModifier 0x1BC. SetSpecularKs stores 0x1C0.

===============================================================================
*/

#ifndef __DAMAGEABLEOBJECT_H__
#define __DAMAGEABLEOBJECT_H__

#include <float.h>
#include "PhysicsObjectClass.h"
#include "NiTList.h"
#include "OBBModel.h"
#include "TGModelPropertySet.h"
#include "TimeSliceProcess.h"
#include "VisibleDamage.h"

class CollisionRecorder;
class CollisionTest;
class DamageCollisionTest;
class MetaVolume;
class ObjectClass;
class WeaponPayload;
class WeaponHitEvent;
class NiAVObject;
class SetClass;
class TGObject;
class TGStringEvent;
class TGModelProperty;

// The life time of an object that is not dying (the SWIG variable
// INFINITE_LIFE).
extern float	INFINITE_LIFE;

// GetObjType's value for a DamageableObject.
#define DAMAGEABLEOBJECT_TYPE		0x8007

// Event types, as the SWIG constants of the App module give them.
#define ET_OBJECT_COLLISION				0x800050
#define ET_DEAD_OBJECT_REMOVAL_CHECK	0x800055
#define ET_OBJECT_CONVERTED_TO_HULK		0x800056
#define ET_HOST_OBJECT_COLLISION		0x8000FC

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class DamageableObject : public PhysicsObjectClass {
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
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	// PhysicsObjectClass
	virtual void	SetupModel( char *modelName );

	// Slots 80 and 81 test a hit along the segment from start to end, as
	// Torpedo::DetectCollision calls them (the last argument of slot 80 is
	// unproven). Slots 82 and 83 test this object's model against the
	// other's visible damage and back: slot 82 hands the other object this
	// object's side of the test (a DamageCollisionTest), and the other
	// object's slot 83 lets the two sides test each other. The parameter
	// types of slots 84 and 87 are unproven: only their counts are known,
	// from ret N; slot 84 returns nothing.
	virtual bool	VirtualSlot80( const NiPoint3 *start, const NiPoint3 *end, NiPoint3 *hitPoint, NiPoint3 *hitNormal, bool ignoreShields );
	// A WeaponHitEvent for a hit on this object (invented name).
	WeaponHitEvent *	CreateWeaponHitEvent( const NiPoint3 *point, const NiPoint3 *normal, float damage, float condition, WeaponPayload *payload, TGObject *source, ObjectClass *firingObject );
	virtual bool	VirtualSlot81( const NiPoint3 *start, const NiPoint3 *end, NiPoint3 *hitPoint, NiPoint3 *hitNormal, float *damage, WeaponPayload *payload, TGObject *source, ObjectClass *firingObject );
	virtual bool	VirtualSlot82( DamageableObject *other, CollisionRecorder *recorder );
	virtual bool	VirtualSlot83( CollisionTest *other, CollisionRecorder *recorder );
	// The first of the LOD levels' collision models that is set; NULL when
	// there is none (invented name).
	OBBModel *		GetFirstCollisionModel( void );
	// The first visible damage whose polygonizer is set up; NULL when there
	// is none (invented name).
	VisibleDamage *	GetFirstSetUpDamage( void ) {
		VisibleDamage *found = NULL;
		for ( unsigned int i = 0; i < visibleDamage.GetSize(); i++ ) {
			VisibleDamage *damage = visibleDamage.GetAt( i );
			if ( damage != NULL && damage->Unknown4B1B50() ) {
				found = damage;
				break;
			}
		}
		return found;
	}
	// Whether the point (in the model frame) lies in a filled cell of the
	// first set-up visible damage; false without one (ShipSubsystem::
	// UpdateDamagePoint asks it for the subsystem's position).
	bool			Unknown595440( const NiPoint3 *point );
	// Takes one hold on a copy of the model: the first shows a clone of the
	// model (of a LOD node's active child, else its first) lit by its vertex
	// colours, keeping the model itself in clonedModel (the warp engines
	// call it as a warp begins).
	void			Unknown593270( void );
	// Drops one hold on the cloned model; the last puts the object's own
	// model back (the warp engines call it as a warp ends).
	void			Unknown593550( void );
	// Makes the damage-refresh process of the priority, replacing one of another priority (invented name).
	void			Unknown593F30( int priority );
	// The damage-refresh process's function: refreshes the object's damage geometry (invented name).
	static void		Unknown5927E0( double time, void *data );
	// One step of the damage refresh, with the time the step must end by:
	// without visible damage the process goes; else the full model's
	// pieces break off (every other step), then the shown LOD level's
	// damage, the one shown last, and the levels next to the shown one
	// are rebuilt while time is left. True when a level's damage was
	// rebuilt; out of time, the process is queued again (priority 1 or 3)
	// (TODO identity).
	bool			Unknown592960( double time );
	// Breaks the pieces off the full model's damage: each grid other than
	// the largest with more than 9 cells, then keeps the largest (TODO
	// identity; the time is unused).
	void			Unknown592850( double time );
	// Narrows the damage to its largest grid when it has several (TODO
	// identity).
	static void		Unknown592910( VisibleDamage *damage );
	// Takes the collision the recorder holds with the other object; index
	// is this object's state in the recorder (0 or 1).
	virtual void	VirtualSlot84( CollisionRecorder *recorder, DamageableObject *other, int index );
	virtual void	AddDamage( NiAVObject *, float, float );	// the floats' meanings are open
	virtual void	SetupProperties( void );
	virtual void	VirtualSlot87( void );
	// Slot 88 takes each property of the property set in turn, and slot 89
	// follows the last one (SetupProperties); ShipClass's slot 88 builds a
	// subsystem from a subsystem property.
	// FUNCTION: STBC 0x00591190 ?VirtualSlot88@DamageableObject@@UAEXPAVTGModelProperty@@@Z
	virtual void	VirtualSlot88( TGModelProperty *property ) {}
	// FUNCTION: STBC 0x005911a0 ?VirtualSlot89@DamageableObject@@UAEXXZ
	virtual void	VirtualSlot89( void ) {}

					DamageableObject( long id );
					DamageableObject( NiAVObject *object );
					DamageableObject( char *modelName );
	static DamageableObject *	Create( char *modelName ) { return new DamageableObject( modelName ); }	// the SWIG wrapper DamageableObject_Create expands it
	virtual			~DamageableObject( void );

	static DamageableObject *	Cast( TGObject * );
	void	RemoveVisibleDamage( void );
	void	SetSpecularKs( float );
	void	DisableGlowAlphaMaps( void );
	// Puts the glow alpha maps of the model's LOD parts back (0x5941F0;
	// LODModelManager's glow switch calls it; invented name, after
	// DisableGlowAlphaMaps).
	void	EnableGlowAlphaMaps( void );
	bool	CanCollide( DamageableObject * );
	// Whether the other object touched this one at the last update (invented name).
	bool	WasTouching( DamageableObject *other ) { return unknown160.FindPos( other->GetObjID() ) != 0; }
	void	EnableCollisionsWith( DamageableObject *, bool );
	void	SetCollisionsOn( bool );
	void	AddObjectDamageVolume( float, float, float, float, float );
	void	DamageRefresh( bool, bool );
	// Makes a separate object of one of the damage's grids: the part of the
	// model the grid covers, thrown off at random or pushed out from the
	// object (0x591EE0; Unknown592580 calls it; invented name).
	void	BreakOffPiece( DamageVoxelGrid *grid, bool pushOut );
	// Rebuilds the damage; when that split the model into several grids,
	// keeps the largest and, with breakOff, breaks the others of ten cells
	// or more off. True when the model split (invented name).
	bool	Unknown592580( VisibleDamage *damage, bool breakOff, bool pushOut );
	void	AddMetaVolume( MetaVolume *volume );		// invented name
	// Hands the ships within the splash damage radius their share of the
	// splash damage, times fraction, through their shields first (invented
	// name).
	void	ApplySplashDamage( float fraction );
	// Sends the player a DAMAGE_VOLUME_MESSAGE for each of the object's
	// damage volumes (invented name).
	void	SendDamageVolumes( int playerID );
	// Adds a damage volume at the point, in the model's frame (invented name).
	void	AddDamageVolume( const NiPoint3 &center, float radius, float strength );
	void	Unknown5952D0( TGEvent *event );
	static DamageableObject *	GetObjectByID( SetClass *set, long id );
	static DamageableObject *	GetObject( SetClass *, char * );
	static DamageableObject **	GetClassObjects( SetClass *set, int *count );		// invented name
	static int	GetNumClassObjects( SetClass *set );

	float			GetClonedModelRadius( void ) {
		if ( clonedModel != NULL ) {
			return clonedModel->GetWorldBound().radius;
		}
		return 0.0f;
	}
	bool			HasClonedModel( void ) { return clonedModel != NULL; }
	// The LOD model's name; NULL until the model is set up (invented name).
	char *			GetLODModelName( void ) { return unknown108; }
	int				GetClonedModelCount( void ) { return clonedModelCount; }
	void			SetVisibleDamageRadiusModifier( float modifier ) { visibleDamageRadiusModifier = modifier; }
	void			SetVisibleDamageStrengthModifier( float modifier ) { visibleDamageStrengthModifier = modifier; }
	TGModelPropertySet *	GetPropertySet( void ) { return propertySet; }
	void			SetLifeTime( float lifeTime_ ) { lifeTime = lifeTime_; }
	float			GetLifeTime( void ) { return lifeTime; }
	bool			IsDying( void ) { return lifeTime < INFINITE_LIFE; }	// the life time is INFINITE_LIFE until the object starts dying
	bool			IsDead( void ) { return dead; }
	void			SetDead( void ) { dead = true; }
	void			SetSplashDamage( float damage, float radius ) { splashDamage = damage; splashDamageRadius = radius; }
	float			GetSplashDamage( void ) { return splashDamage; }
	float			GetSplashDamageRadius( void ) { return splashDamageRadius; }

	static void		SetDamageGeometryEnabled( bool enabled ) { damageGeometryEnabled = enabled; }
	static bool		IsDamageGeometryEnabled( void ) { return damageGeometryEnabled; }
	static void		SetVolumeDamageGeometryEnabled( bool enabled ) { volumeDamageGeometryEnabled = enabled; }
	static bool		IsVolumeDamageGeometryEnabled( void ) { return volumeDamageGeometryEnabled; }
	static void		SetBreakableComponentsEnabled( bool enabled ) { breakableComponentsEnabled = enabled; }
	static bool		IsBreakableComponentsEnabled( void ) { return breakableComponentsEnabled; }

	// Register the class's handler methods by name, and add them to its table.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			ObjectDestroyedHandler( TGEvent *event );
	void			CollisionEffectHandler( TGEvent *event );
	void			DeadObjectRemovalCheck( TGEvent *event );
	void			ConvertToDeadHulk( TGStringEvent *event );

protected:
	void			InitObject( void );		// invented name

	char *			unknown108;			// 0x108 owned; the name of the LOD model, released from g_kLODModelManager
	NiTArray< NiPointer<OBBModel> >	unknown10C;	// 0x10C the collision models of the LOD levels, built with the LOD model
	NiTArray<VisibleDamage *>	visibleDamage;	// 0x124 owned (invented name)
	NiPointer<MetaVolumePool>	unknown13C;	// 0x13C
	NiAVObject *	unknown140;			// 0x140 the model the damage volumes are expressed in
	TGModelPropertySet *	propertySet;	// 0x144 owned
	NiPointer<NiAVObject>	clonedModel;	// 0x148 NULL when there is none
	float			lifeTime;			// 0x14C
	bool			dead;				// 0x150
	float			splashDamage;		// 0x154
	float			splashDamageRadius;	// 0x158
	bool			unknown15C;			// 0x15C true at first
	// The list at 0x1C8 holds another type than the other three (its block
	// destructor is another instantiation). WriteToStream writes the
	// elements of the lists at 0x178 and 0x190 as IDs; Update moves those of
	// 0x178 to 0x160 (through NiTList<long>::NewItem, 0x434810).
	NiTList<long>	unknown160;			// 0x160 object IDs
	NiTList<long>	unknown178;			// 0x178 object IDs
	NiTList<long>	unknown190;			// 0x190 the IDs of the objects this one does not collide with (CanCollide)
	bool			collisionsOn;		// 0x1A8 true at first (SetCollisionsOn)
	long			unknown1AC;			// 0x1AC the ID of a timer of g_kTimerManager; 0 when there is none
	int				unknown1B0;			// 0x1B0
	int				unknown1B4;			// 0x1B4
	float			visibleDamageRadiusModifier;	// 0x1B8
	float			visibleDamageStrengthModifier;	// 0x1BC
	float			specularKs;			// 0x1C0 1 at first
	TimeSliceProcess *	unknown1C4;		// 0x1C4 owned
	NiTList<unsigned int>	unknown1C8;	// 0x1C8
	int				clonedModelCount;	// 0x1E0

	static TGEventHandlerTable	classHandlers;		// invented name
	static bool		damageGeometryEnabled;			// true at first
	static bool		volumeDamageGeometryEnabled;	// true at first
	static bool		breakableComponentsEnabled;		// true at first
};

/*
================
DamageableObject::IsTypeOf
================
*/
// FUNCTION: STBC 0x005910f0
inline bool DamageableObject::IsTypeOf( int type ) {
	if ( type == DAMAGEABLEOBJECT_TYPE ) {
		return true;
	}
	return PhysicsObjectClass::IsTypeOf( type );
}

#endif /* !__DAMAGEABLEOBJECT_H__ */
