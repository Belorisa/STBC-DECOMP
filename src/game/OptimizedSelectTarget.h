/*
===============================================================================

	OptimizedSelectTarget.h

	The optimized version of the script preprocessor SelectTarget
	(AI.Preprocessors): it picks the ship's target from a target group,
	rating each candidate by the script's factors, and calls the SetTarget
	functions of the AI trees under it when the target changes. Its event
	handler, an OSTEventHandler, forwards the group, set, cloak and damage
	events it listens for.

	0xE0 bytes: the create functions (0x488F60, 0x48A8B0) allocate that
	much. Evidence: the static initialiser 0x488320 files "SelectTarget"
	with 0x488F60 in PreprocessingAI's list of optimized versions; the
	constructors (0x4888A0, 0x48A340) store the vtable 0x88BFB0
	(PreprocessingAI's 23 slots; 0, 15, 16, 17, 18, 19, 20, 21 and 22 are
	overridden). The constructor reads the script attributes into the
	members after their names ("pTargetGroup", "sCurrentTarget",
	"lAdditionalSetTargetAITrees", "fUpdateTime", "bSetShipTarget", ...,
	"sReplacementSetTargetFunction"). AddSetTargetTree is the SWIG
	wrapper's. Slot 19 returns 10.

	OSTEventHandler: "OSTEventHandler", "_p_OSTEventHandler" and
	"OSTEventHandlerPtr" (slots 9 to 11 of the vtable 0x88BF5C, 21 slots),
	GetObjType 0x803A, 0x18 bytes from the object pool; its handler
	methods are registered under "OSTEventHandler::TargetListChanged" and
	the rest. Its module (0x9825F8, vtable 0x88BF3C, priority 0x20000000)
	files its creator and its handler table and registers the names at
	startup. The module's name is invented.

===============================================================================
*/

#ifndef __OPTIMIZEDSELECTTARGET_H__
#define __OPTIMIZEDSELECTTARGET_H__

#include "PreprocessingAI.h"
#include "TGEventHandlerObject.h"
#include "TGModule.h"
#include "NiTArray.h"
#include "NiTList.h"
#include "NiTMap.h"

class ArtificialIntelligence;
class ObjectClass;
class ObjectGroup;
class OptimizedSelectTarget;
class TGEvent;
class WeaponHitEvent;

// The AI type of the class (slot 19).
#define AI_TYPE_OPTIMIZED_SELECT_TARGET		10

// GetObjType's value for an OSTEventHandler (the SWIG constant).
#define CT_OST_EVENT_HANDLER		0x803A

/*
================
OSTEventHandler

Hands the events of an OptimizedSelectTarget to it.
================
*/
// FUNCTION: STBC 0x00488760 ??_GOSTEventHandler@@UAEPAXI@Z @game/OptimizedSelectTarget
// FUNCTION: STBC 0x00488790 ??1OSTEventHandler@@UAE@XZ @game/OptimizedSelectTarget
class OSTEventHandler : public TGEventHandlerObject {
public:
					// FUNCTION: STBC 0x00488650 ??0OSTEventHandler@@QAE@J@Z @game/OptimizedSelectTarget
					OSTEventHandler( long id = 0 ) : TGEventHandlerObject( id ) {}
					OSTEventHandler( OptimizedSelectTarget *ai_ ) { ai = ai_; }

	// TGObject
	// FUNCTION: STBC 0x00488670 ?GetObjType@OSTEventHandler@@UAEHXZ @game/OptimizedSelectTarget
	virtual int		GetObjType( void ) { return CT_OST_EVENT_HANDLER; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x004886c0 ?GetClassName@OSTEventHandler@@UAEPADXZ @game/OptimizedSelectTarget
	virtual char *	GetClassName( void ) { return "OSTEventHandler"; }
	// FUNCTION: STBC 0x004886d0 ?GetSwigTypeName@OSTEventHandler@@UAEPADXZ @game/OptimizedSelectTarget
	virtual char *	GetSwigTypeName( void ) { return "_p_OSTEventHandler"; }
	// FUNCTION: STBC 0x004886e0 ?GetPythonClassName@OSTEventHandler@@UAEPADXZ @game/OptimizedSelectTarget
	virtual char *	GetPythonClassName( void ) { return "OSTEventHandlerPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x004886f0 ?GetClassHandlers@OSTEventHandler@@UAEPAVTGEventHandlerTable@@XZ @game/OptimizedSelectTarget
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// The handler methods.
	void			TargetListChanged( TGEvent *event );
	void			TargetEnteredSet( TGEvent *event );
	void			OurShipEnteredSet( TGEvent *event );
	void			TargetGone( TGEvent *event );
	void			ObjectDecloaked( TGEvent *event );
	void			DamageEvent( TGEvent *event );

	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	// The AI it works for; its ID between a load and the fix-up.
	OptimizedSelectTarget *		ai;			// 0x14

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
OSTEventHandler::IsTypeOf
================
*/
// FUNCTION: STBC 0x00488680 ?IsTypeOf@OSTEventHandler@@UAE_NH@Z @game/OptimizedSelectTarget
inline bool OSTEventHandler::IsTypeOf( int type ) {
	if ( type == CT_OST_EVENT_HANDLER ) {
		return true;
	}
	return TGEventHandlerObject::IsTypeOf( type );
}

/*
================
OSTEventHandlerModule

Files OSTEventHandler at startup. The name is invented.
================
*/
// FUNCTION: STBC 0x004887d0 ??1OSTEventHandlerModule@@QAE@XZ @game/OptimizedSelectTarget
class OSTEventHandlerModule : public TGModule {
public:
					// FUNCTION: STBC 0x00488450 ??0OSTEventHandlerModule@@QAE@XZ @game/OptimizedSelectTarget
					OSTEventHandlerModule( void ) : TGModule( 0x20000000 ) {}

	// TGModule
	virtual void	VirtualSlot00( void );		// the startup
	virtual void	VirtualSlot05( void );

	static TGObject *	CreateOSTEventHandler( long id );
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class OptimizedSelectTarget : public PreprocessingAI {
public:
	// Takes over the scripted AI's contained AI and its settings.
							OptimizedSelectTarget( PreprocessingAI *ai );
							OptimizedSelectTarget( TGStream *stream, bool *ok );
	virtual					~OptimizedSelectTarget( void );

	// ArtificialIntelligence
	virtual PyObject *		VirtualSlot15( void );
	virtual bool			WriteToStream( TGStream *stream );
	virtual bool			FixUpPointers( void );
	virtual void			PostFixUp( void );
	virtual int				GetAIType( void );
	virtual bool			VirtualSlot20( int type );

	// PreprocessingAI
	virtual PyObject *		GetPreprocessingInstance( void );
	virtual int				CallPreprocessingMethod( double time );

	// Adds an AI tree whose SetTarget functions are called too.
	void					AddSetTargetTree( ArtificialIntelligence *ai );

	// The event handler's calls (the script's method names).
	void					ForceTargetUpdate( void ) { forceUpdate = true; }		// invented name
	void					ObjectDecloaked( TGEvent *event );
	void					DamageEvent( WeaponHitEvent *event );

	// The optimized version of the scripted AI, which is deleted (invented
	// name).
	static ArtificialIntelligence *	CreateFromScript( PreprocessingAI *ai );
	// The stream constructor, for CreateFromStream.
	static ArtificialIntelligence *	Create( TGStream *stream );

protected:
	// The script's methods of the same names.
	void					AddEventHandlers( void );		// invented name
	void					UpdateTargetInfo( double endTime );
	void					RemoveOldTargetHandlers( void );
	void					CallSetTargetFunctions( double endTime );
	// The best rated object of the group, by name; NULL for none.
	char *					FindGoodTarget( void );
	// The script's GetTargetRating: how good a target the object is for the
	// ship, from the rating factors (0x489BD0).
	float					GetTargetRating( ShipClass *ship, ObjectClass *target );

	ObjectGroup *			targetGroup;				// 0x4C the script's pTargetGroup
	PyObject *				targetGroupInstance;		// 0x50 its shadow instance; a reference held
	char *					currentTarget;				// 0x54 a new[] copy (sCurrentTarget)
	long					currentTargetID;			// 0x58 the target the handlers watch
	NiTArray<long *>		setTargetTrees;				// 0x5C the IDs of the added AI trees, owned (lAdditionalSetTargetAITrees)
	NiTList<long *>			pendingSetTargetAIs;		// 0x74 the IDs of the AIs whose SetTarget is still to be called, owned (invented name)
	float					updateTime;					// 0x8C fUpdateTime
	bool					setShipTarget;				// 0x90 bSetShipTarget
	bool					callSetTargetFuncsWithNoTarget;	// 0x91
	bool					callSetTargetFunctions;		// 0x92
	bool					updateInfoOnCodeAISet;		// 0x93
	bool					updatingTargetInfo;			// 0x94
	bool					ignoreSensors;				// 0x95
	int						noTargetPreprocessStatus;	// 0x98 a preprocessStatus_t
	int						noTargetNoShipTargetPreprocessStatus;	// 0x9C
	float					distanceFactor;				// 0xA0 the rating's factors
	float					inFrontFactor;				// 0xA4
	float					isTargetFactor;				// 0xA8
	float					shieldFactor;				// 0xAC
	float					weaponsFactor;				// 0xB0
	float					hullFactor;					// 0xB4
	float					damageFactor;				// 0xB8
	float					priorityFactor;				// 0xBC
	float					popularityFactor;			// 0xC0
	char *					replacementSetTargetModule;		// 0xC4 a new[] copy, or NULL
	char *					replacementSetTargetFunction;	// 0xC8 a new[] copy, or NULL
	NiTMap<long, float>		damageTaken;				// 0xCC by attacker ID, the hull fraction it took (invented name)
	OSTEventHandler *		eventHandler;				// 0xDC owned
};

#endif /* !__OPTIMIZEDSELECTTARGET_H__ */
