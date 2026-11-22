/*
===============================================================================

	OptimizedFireScript.h

	The optimized version of the script preprocessor FireScript
	(AI.Preprocessors): about every tenth of a second it takes one step of
	firing the ship's listed weapon systems at the named target. A step
	checks that the target is in range and no friend is in the line of
	fire, chooses the target's subsystem to aim at, or sets up and fires
	one weapon system.

	Evidence: the static initialiser 0x481B60 files the script class name
	"FireScript" with 0x482510 in PreprocessingAI's list of optimized
	versions, and 0x481B00 files the stream create function 0x483090 for
	the AI type 8. 0x482510 allocates 0xC8 bytes and calls the constructor
	0x481CC0 with the scripted AI, which it then deletes; the constructor,
	the stream constructor (0x482B50) and the destructor (0x4822E0) store
	the vtable 0x88BE34 (PreprocessingAI's 23 slots; the destructor,
	CallExternalFunction 0x4827A0, LostFocus 0x482780, VirtualSlot15
	0x482220, WriteToStream 0x4828D0, FixUpPointers 0x483110, PostFixUp
	0x483150, GetAIType 0x482230, VirtualSlot20 0x482240,
	GetPreprocessingInstance 0x482210 and the preprocessing call 0x483330
	are overridden). The class name is the SWIG interface's
	("OptimizedFireScript_AddWeaponSystem" and the like) and the string the
	shadow instance is made with. The constructor reads the script's
	attributes by name ("sTarget", "fMaxFiringRange", ...): the members are
	named after them, and the protected functions after the script methods
	they replace, where the script has one.

	The module at 0x982350 (vtable 0x88BE14, built by 0x481C40) adds the
	Python method "GetWeapons" (0x482830) to the shadow class at startup.

===============================================================================
*/

#ifndef __OPTIMIZEDFIRESCRIPT_H__
#define __OPTIMIZEDFIRESCRIPT_H__

#include "PreprocessingAI.h"
#include "NiTList.h"
#include "NiTMap.h"
#include "TGModule.h"
#include "TGPoint3.h"

class ObjectClass;
class ObjectGroup;
class ShipSubsystem;
class TorpedoSystem;
class WeaponSystem;

// The AI type of the class (slot 19).
#define AI_TYPE_FIRE_SCRIPT		8

// A kind of subsystem to aim at, with its priority: an entry of the script's
// lTargetSubsystems (invented name).
struct FireTargetSubsystem {
	long				type;		// the subsystem's class type
	float				priority;	// lower priorities are attacked first
};

// How a subsystem of the target was rated, and at which round: a value of the
// script's dTargetSubsystemRating (invented name).
struct FireSubsystemRating {
	int					updateNum;	// the round of ratings it is from
	float				rating;
};

// A torpedo type's fitness for a shot: an entry of ChooseTorpType's
// lTorpAmmoRatings (invented name).
struct FireTorpRating {
	int					ammoType;				// 0x00
	float				damage;					// 0x04
	float				idealDistMin;			// 0x08
	float				idealDistMax;			// 0x0C
	float				idealTargetSpeedMin;	// 0x10
	float				idealTargetSpeedMax;	// 0x14
	float				rating;					// 0x18 the damage less the power cost
};

/*
================
OptimizedFireScriptModule

Adds the Python methods of the shadow class at startup. The name is invented.
================
*/
// FUNCTION: STBC 0x00481cb0 ??1OptimizedFireScriptModule@@QAE@XZ @game/OptimizedFireScript
class OptimizedFireScriptModule : public TGModule {
public:
					// FUNCTION: STBC 0x00481c40 ??0OptimizedFireScriptModule@@QAE@XZ @game/OptimizedFireScript
					OptimizedFireScriptModule( void ) : TGModule( 0x20000000 ) {}

	// TGModule
	virtual void	VirtualSlot00( void );		// the startup
	virtual void	VirtualSlot05( void );
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class OptimizedFireScript : public PreprocessingAI {
public:
	// Takes over the scripted AI's contained AI and its settings.
							OptimizedFireScript( PreprocessingAI *ai );
							OptimizedFireScript( TGStream *stream, bool *ok );
	virtual					~OptimizedFireScript( void );

	// ArtificialIntelligence
	// "SetTarget" with a name or None is handled here.
	virtual bool			CallExternalFunction( char *functionName, PyObject *arguments );
	virtual void			LostFocus( void );
	virtual PyObject *		VirtualSlot15( void );
	virtual bool			WriteToStream( TGStream *stream );
	virtual bool			FixUpPointers( void );
	virtual void			PostFixUp( void );
	virtual int				GetAIType( void );
	virtual bool			VirtualSlot20( int type );

	// PreprocessingAI
	virtual PyObject *		GetPreprocessingInstance( void );
	virtual int				CallPreprocessingMethod( double time );

	void					AddWeaponSystem( WeaponSystem * );
	void					RemoveAllWeaponSystems( void );
	void					SetEnabled( bool );
	bool					HasSubsystemTargets( void );
	void					IgnoreSubsystemTargets( void );
	void					RestoreSubsystemTargets( void );
	void					SetTarget( char * );

	NiTList<WeaponSystem *> &	GetWeapons( void ) { return weapons; }

	// The optimized version of the scripted AI, which is deleted (invented
	// name).
	static ArtificialIntelligence *	CreateFromScript( PreprocessingAI *ai );
	// The stream constructor, for CreateFromStream.
	static ArtificialIntelligence *	Create( TGStream *stream );

protected:
	bool					IsKindListed( WeaponSystem *system );

	// The script's methods of the same names.
	ObjectClass *			GetTarget( void );
	void					StopFiring( void );
	void					FireSystemAtTarget( WeaponSystem *system, ObjectClass *target, ShipSubsystem *subsystem );
	bool					ConfigureWeaponSystem( WeaponSystem *system, ObjectClass *target, ShipSubsystem *subsystem );
	bool					WeaponTooDangerous( WeaponSystem *system, ObjectClass *target, ShipSubsystem *subsystem );
	void					ChooseTorpType( TorpedoSystem *torpSystem, NiPoint3 *targetLocation, float targetSpeed );
	bool					CheckGoodShot( WeaponSystem *system, ObjectClass *target, ShipSubsystem *subsystem );
	NiPoint3				PredictTargetLocation( ShipClass *ship, ObjectClass *target, ShipSubsystem *subsystem, float speed );
	// Whether the target is in range with no friend in the line of fire.
	bool					TargetVisible( ShipClass *ship, ObjectClass *target );
	void					ChooseTargetSubsystem( ShipClass *ship, ObjectClass *target );
	// Adds the subsystem's targetable descendants; returns how many are
	// disabled.
	int						GetChildTargets( ShipSubsystem *subsystem, NiTList<ShipSubsystem *> *targets, bool attackDisabled );
	ShipSubsystem *			PickTargetSubsystemFromList( NiTList<ShipSubsystem *> *subsystems );
	void					GetTargetableSubsystems( ShipSubsystem *subsystem, NiTList<ShipSubsystem *> *targetable );
	float					RateSubsystemForTargeting( ShipClass *ship, ShipClass *targetShip, ShipSubsystem *subsystem );

	// Parts of the script's methods (invented names).
	// Calls "UsingWeaponType" with the weapon systems on the AIs of the tree,
	// when the systems changed.
	void					CallUsingWeaponTypeFunc( void );
	// Whether a ship of the group is in the line from the ship to the point.
	bool					IsLineOfFireBlocked( ShipClass *ship, const NiPoint3 &point, ObjectGroup *group, ObjectClass *target );
	// Whether the phasers are worth firing at this distance and shield.
	bool					ShouldFirePhasers( WeaponSystem *system, ObjectClass *target, ShipSubsystem *subsystem );
	// ChooseTargetSubsystem by the kinds of targetSubsystems.
	ShipSubsystem *			ChooseListedSubsystem( ShipClass *ship, ShipClass *targetShip );
	// ChooseTargetSubsystem by the ratings.
	ShipSubsystem *			ChooseRatedSubsystem( ShipClass *ship, ShipClass *targetShip );

	char *					target;						// 0x4C a new[] copy (sTarget)
	float					maxFiringRange;				// 0x50
	bool					highPower;					// 0x54
	bool					changePowerSetting;			// 0x55
	bool					hullSelectedChooseAlternate;	// 0x56
	NiTList<FireTargetSubsystem *>	targetSubsystems;	// 0x58 owned (lTargetSubsystems)
	bool					ignoreSubsystemTargets;		// 0x70 (invented name: the script moves the list away)
	bool					chooseSubsystemTargets;		// 0x71
	bool					chooseTorpsWisely;			// 0x72
	bool					smartPhasers;				// 0x73
	NiTList<WeaponSystem *>	weapons;					// 0x74 lWeapons; the systems' IDs between a load and the fix-up
	bool					enabled;					// 0x8C
	bool					attackDisabledSubsystems;	// 0x8D
	bool					attackIfNoSubsystemsLeft;	// 0x8E
	bool					callUsingWeaponTypeFunc;	// 0x8F
	float					fireDotThreshold;			// 0x90
	float					phaserUse;					// 0x94 0 to 1: rises while the phasers fire, falls while they rest (invented name)
	float					lastPhaserTime;				// 0x98 game time of the last change of phaserUse (invented name)
	bool					dumbFireTorps;				// 0x9C
	bool					disableFirst;				// 0x9D
	int						tractorBeamMode;			// 0xA0 eTractorBeamMode; 0 for none
	bool					torpsTooDangerous;			// 0xA4 the torpedo systems are left out of UsingWeaponType (invented name)
	long					targetedSubsystemID;		// 0xA8 idTargetedSubsystem; 0 for none
	int						lastUpdate;					// 0xAC iLastUpdate: -2 checks the target, -1 chooses the subsystem, others fire
	NiTMap<long, FireSubsystemRating *>	targetSubsystemRating;	// 0xB0 by subsystem ID, owned (dTargetSubsystemRating)
	int						targetSubsystemUpdateNum;	// 0xC0
	bool					targetVisible;				// 0xC4
};

// The Python method of the shadow class.
PyObject *	OptimizedFireScript_GetWeapons( PyObject *self, PyObject *args );

#endif /* !__OPTIMIZEDFIRESCRIPT_H__ */
