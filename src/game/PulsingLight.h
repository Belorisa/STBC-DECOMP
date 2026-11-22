/*
===============================================================================

	PulsingLight.h

	A light object whose light pulses (App's CT_PULSING_LIGHT, 0x80BD):
	NetImmerse lights that fade one into the next, each in turn, over a
	period, lighting the nodes of the objects the light shines on.

	Evidence: GetObjType (0x4416A0) returns 0x80BD, CT_PULSING_LIGHT in the
	SWIG constant table; IsTypeOf (0x4416B0) also accepts 0x80BC
	(CT_LIGHT_OBJECT) and the bases'; GetClassName, GetSwigTypeName and
	GetPythonClassName (0x441710-0x441730) return "PulsingLight",
	"_p_PulsingLight" and "PulsingLightPtr". The loading constructor
	(0x441660), the inline virtuals and the scalar deleting destructor
	(0x441750, which returns 0x88 bytes to the pool) are kept from
	UtopiaModule's unit, whose creator (0x441620) builds the class. The
	vtable 0x88AF6C (55 slots) replaces slot 21 (Update), 46 and 47 (AddToSet,
	RemoveFromSet) and 51-54 (the illuminated objects).

	Layout (0x88 bytes): LightObjectClass is 0x54 bytes (its list of IDs at
	0x3C is 0x18). The default constructor (0x4E8590) sets 0x54 to 1.0,
	clears 0x58, 0x5C, the list at 0x6C, the flag at 0x84 and the two light
	pointers 0x60 and 0x64. Create (0x4E86D0) fills the period (0x54), the
	number of lights (0x58) and the array of NiLights (0x5C), and copies each
	light's dimmer (0x120) into a new float array (0x68). Update (0x4E8760)
	fades two consecutive lights of the array through SetDimmer (0x120,
	revision 0xD4). The list at 0x6C holds the nodes (BaseObjectClass 0x18)
	of the objects AddIlluminatedObject is given: its items' destructor
	(0x4E91D0) releases a NiPointer; 0x60 and 0x64 are the lights whose
	affected nodes DetachLights (0x4E89C0) clears. The destructor (0x4E85E0)
	deletes the lights and their array.

	Every member and function name is invented, after its use, except the
	slots PulsingLight replaces.

===============================================================================
*/

#ifndef __PULSINGLIGHT_H__
#define __PULSINGLIGHT_H__

#include "LightObjectClass.h"
#include "NiPointer.h"

class NiLight;
class NiNode;
class TGEventHandlerTable;

// IsTypeOf's value for a pulsing light (App's CT_PULSING_LIGHT).
#define PULSINGLIGHT_TYPE		0x80BD

// FUNCTION: STBC 0x00441750 ??_GPulsingLight@@UAEPAXI@Z @game/UtopiaModule
class PulsingLight : public LightObjectClass {
public:
	// FUNCTION: STBC 0x00441660 ??0PulsingLight@@QAE@J@Z @game/UtopiaModule
					PulsingLight( long id ) : LightObjectClass( id ), affectedNodes( 2 ) {}	// the loader's
					PulsingLight( void );
	virtual			~PulsingLight( void );

	// the pulsing light of the lights, numLights of them, which it then owns,
	// fading each into the next over period seconds (unreferenced)
	static PulsingLight *	Create( int numLights, NiLight **lights, float period, char *name );

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

	// BaseObjectClass
	virtual void	Update( float time );
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	// LightObjectClass
	virtual void	AddIlluminatedObject( BaseObjectClass *object );
	virtual void	RemoveIlluminatedObject( BaseObjectClass *object );
	virtual void	IlluminateEntireSet( void );
	virtual void	UnilluminateEntireSet( void );

	// the set's pulsing lights, by type, ID and name, as LightObjectClass's
	// (unreferenced)
	static PulsingLight **	GetClassObjects( SetClass *set, int *count );
	static int		GetNumClassObjects( SetClass *set );
	static PulsingLight *	GetObjectByID( SetClass *set, long id );
	static PulsingLight *	GetObject( SetClass *set, char *name_ );

	// the light of the array at the index (unreferenced)
	NiLight *		GetLight( int index );

protected:
	// takes the affected nodes off the two lights Update last lit
	void			DetachLights( void );
	// lights the two lights' affected nodes (see the BUG in the body)
	void			AttachLights( NiLight *fading_, NiLight *rising_ );

	float			period;				// 0x54 seconds for one light to fade into the next; 1.0 at first
	int				numLights;			// 0x58
	NiLight **		lights;				// 0x5C owned, numLights of them, each owned
	NiLight *		fading;				// 0x60 the light Update fades out
	NiLight *		rising;				// 0x64 the light Update fades in
	float *			dimmers;			// 0x68 each light's full dimmer
	NiTList<NiPointer<NiNode> >	affectedNodes;	// 0x6C the nodes of the illuminated objects
	bool			entireSet;			// 0x84 IlluminateEntireSet's

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
PulsingLight::GetObjType
================
*/
// FUNCTION: STBC 0x004416a0 @game/UtopiaModule
inline int PulsingLight::GetObjType( void ) {
	return PULSINGLIGHT_TYPE;
}

/*
================
PulsingLight::IsTypeOf
================
*/
// FUNCTION: STBC 0x004416b0 @game/UtopiaModule
inline bool PulsingLight::IsTypeOf( int type ) {
	if ( type == PULSINGLIGHT_TYPE ) {
		return true;
	}
	return LightObjectClass::IsTypeOf( type );
}

/*
================
PulsingLight::GetClassName
================
*/
// FUNCTION: STBC 0x00441710 @game/UtopiaModule
inline char *PulsingLight::GetClassName( void ) {
	return "PulsingLight";
}

/*
================
PulsingLight::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00441720 @game/UtopiaModule
inline char *PulsingLight::GetSwigTypeName( void ) {
	return "_p_PulsingLight";
}

/*
================
PulsingLight::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00441730 @game/UtopiaModule
inline char *PulsingLight::GetPythonClassName( void ) {
	return "PulsingLightPtr";
}

/*
================
PulsingLight::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00441740 @game/UtopiaModule
inline TGEventHandlerTable *PulsingLight::GetClassHandlers( void ) {
	return &classHandlers;
}

#endif /* !__PULSINGLIGHT_H__ */
