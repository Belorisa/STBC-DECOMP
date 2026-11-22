/*
===============================================================================

	Planet.h

	A planet: a model scaled to its radius, and an atmosphere around it that
	damages the objects inside it. 0x7C bytes, allocated from the pools.

	Evidence: Create (0x5A3DC0, the SWIG wrapper Planet_Create) allocates
	0x7C bytes and calls the constructor (0x5A3E00), which stores the vtable
	0x88A4C0 after ObjectClass's constructor: ObjectClass's 67 slots and two
	added (67, 68), up to Sun's vtable at 0x88A5D4. GetObjType (0x4409B0)
	returns 0x8004, the type Cast checks. UtopiaModule's loader creator
	(0x440960) expands the loading constructor (0x440990), so its unit keeps
	the inline virtuals and both destructors. WriteToStream's and
	ReadFromStream's log strings ("Saving planet %s") name the class.

===============================================================================
*/

#ifndef __PLANET_H__
#define __PLANET_H__

#include "ObjectClass.h"

class SetClass;
class TGObject;
class ProximityEvent;

// GetObjType's value for a Planet (the type its Cast, 0x5A3D90, checks).
#define PLANET_TYPE		0x8004

// The event type the proximity manager files the atmosphere handler under.
#define ET_PROXIMITY_PLANET		0x800063

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00440a70 ??_GPlanet@@UAEPAXI@Z @game/UtopiaModule
class Planet : public ObjectClass {
public:
	// The planet of the radius, with its model scaled to it; the atmosphere
	// reaches atmosphereRadius_ beyond it.
					Planet( float radius_, float atmosphereRadius_, char *modelName_ );
	// FUNCTION: STBC 0x00440990 ??0Planet@@QAE@J@Z @game/UtopiaModule
					Planet( long id ) : ObjectClass( id ), modelName( NULL ) {}	// the loader's: the stream sets the rest
	// FUNCTION: STBC 0x00440aa0 ??1Planet@@UAE@XZ @game/UtopiaModule
	virtual			~Planet( void ) { delete [] modelName; }

	// TGObject
	// FUNCTION: STBC 0x004409b0 ?GetObjType@Planet@@UAEHXZ @game/UtopiaModule
	virtual int		GetObjType( void ) { return PLANET_TYPE; }
	// FUNCTION: STBC 0x004409c0 ?IsTypeOf@Planet@@UAE_NH@Z @game/UtopiaModule
	virtual bool	IsTypeOf( int type ) {
		if ( type == PLANET_TYPE ) {
			return true;
		}
		return ObjectClass::IsTypeOf( type );
	}
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00440a20 ?GetClassName@Planet@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetClassName( void ) { return "Planet"; }
	// FUNCTION: STBC 0x00440a30 ?GetSwigTypeName@Planet@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetSwigTypeName( void ) { return "_p_Planet"; }
	// FUNCTION: STBC 0x00440a40 ?GetPythonClassName@Planet@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetPythonClassName( void ) { return "PlanetPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00440a50 ?GetClassHandlers@Planet@@UAEPAVTGEventHandlerTable@@XZ @game/UtopiaModule
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// BaseObjectClass
	// Puts the atmosphere's proximity check in the new set: it sends
	// ET_PROXIMITY_PLANET for the damageable objects that come within the
	// planet and its atmosphere.
	virtual void	SetContainingSet( SetClass *set );
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	// ObjectClass
	// FUNCTION: STBC 0x00440a60 ?IsTargetable@Planet@@UAE_NXZ @game/UtopiaModule
	virtual bool	IsTargetable( void ) { return false; }

	// Planet
	// The hull damage, then the shield damage, the object takes this frame
	// (before the frame step): the environmental damage scaled by how deep in
	// the atmosphere it is, and that depth's factor in depth (0 outside the
	// atmosphere and below a speed of 0.5). direction is the way the
	// atmosphere pushes against it (ShipClass::EnvironmentDamageHandler hands
	// it to the atmosphere effect).
	virtual float	VirtualSlot67( ObjectClass *object, float *depth, NiPoint3 *direction );	// with environmentalHullDamage
	virtual float	VirtualSlot68( ObjectClass *object, float *depth, NiPoint3 *direction );	// with environmentalShieldDamage

	// ET_PROXIMITY_PLANET: an object that comes inside the planet's atmosphere.
	void			AtmosphereProximityHandler( ProximityEvent *event );
	// The class's handler methods by name, and in the class's table (invented names).
	static void		RegisterHandlerNames( void );
	static void		RegisterHandlers( void );

	float	GetAtmosphereRadius( void ) { return atmosphereRadius; }
	void	SetEnvironmentalHullDamage( float environmentalHullDamage_ ) { environmentalHullDamage = environmentalHullDamage_; }
	void	SetEnvironmentalShieldDamage( float environmentalShieldDamage_ ) { environmentalShieldDamage = environmentalShieldDamage_; }
	void	SetAtmosphereRadius( float atmosphereRadius_ ) { atmosphereRadius = atmosphereRadius_; }

	static Planet *	Cast( TGObject * );
	static Planet *	Create( float radius, char *modelName );
	static Planet *	GetObject( SetClass *, char * );
	static Planet *	GetObjectByID( SetClass *set, long id );
	static Planet **	GetClassObjects( SetClass *set, int *count );		// invented name
	static int	GetNumClassObjects( SetClass * );

protected:
	// Clones the model of modelName, loading it first when it is not, and
	// scales it to the planet's radius (invented name).
	void			CreateModel( void );
	// The environmental damage given, scaled by the object's depth in the
	// atmosphere; the depth and the push's direction go to depth and
	// direction (invented name).
	float			GetEnvironmentalDamage( float damage, ObjectClass *object, float *depth, NiPoint3 *direction );

	// The SWIG wrappers Planet_Set/GetAtmosphereRadius and Planet_SetEnvironmental* read these.
	char *			modelName;					// 0x68 owned (invented name)
	float			atmosphereRadius;			// 0x6C
	float			radius;						// 0x70 the planet's own: its model is scaled to it (invented name)
	float			environmentalHullDamage;	// 0x74
	float			environmentalShieldDamage;	// 0x78
	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__PLANET_H__ */
