/*
===============================================================================

	WarpFlash.h

	The flash of a ship entering or leaving warp: made at a ship, or in a
	set at a location and rotation with a radius, and shown for a time
	(1.5 seconds unless the script gives one).

	0x74 bytes. Evidence: the SWIG wrappers WarpFlash_Create ("O|f": the
	ship, the time) and WarpFlash_CreateWithoutShip ("OOOf|f": the set, the
	location, the rotation, the radius, the time) allocate 0x74 bytes from
	the pool and call the constructors 0x4D4850 and 0x4D48E0; the scripts
	name the arguments (pShip, fTime; pSet, pLocation, pRotation, fRadius,
	fTime). The constructors run ObjectClass's and store the vtable 0x88B048
	(ObjectClass's 67 slots: the destructor, GetObjType, IsTypeOf, the
	stream functions 0x4D4FF0 and 0x4D5030, the class strings and Update
	0x4D4FC0 overridden). The destructor (0x4D4A30) only stores the vtable
	before ObjectClass's. The stream loader's creator in UtopiaModule's unit
	(0x4417C0) allocates 0x74 bytes and calls the ID constructor (0x4417E0);
	the copies of the inline virtuals and of the deleting destructor
	(0x441810-0x4418B0) follow it. GetObjType returns 0x8013, the SWIG
	constant CT_WARP_FLASH. The constructors' common part (0x4D4950) keeps
	the game time, the time the flash ends and the scaled radius, names the
	object "Warp Flash %d" and plays the sound "Warp Flash" at its node.

===============================================================================
*/

#ifndef __WARPFLASH_H__
#define __WARPFLASH_H__

#include "ObjectClass.h"

class BaseObjectClass;
class NiNode;
class SetClass;
class TGMatrix3;
class TGPoint3;

// GetObjType's value for a WarpFlash (the SWIG constant CT_WARP_FLASH).
#define WARPFLASH_TYPE		0x8013

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004418b0 ??_GWarpFlash@@UAEPAXI@Z @game/UtopiaModule
class WarpFlash : public ObjectClass {
public:
	// FUNCTION: STBC 0x004417f0 ??0WarpFlash@@QAE@J@Z @game/UtopiaModule
					WarpFlash( long id = 0 ) : ObjectClass( id ) {}
					WarpFlash( BaseObjectClass *ship, float time );
					WarpFlash( SetClass *set, TGPoint3 *location, TGMatrix3 *rotation, float radius, float time );
	virtual			~WarpFlash( void );

	// TGObject
	// FUNCTION: STBC 0x00441810 ?GetObjType@WarpFlash@@UAEHXZ @game/UtopiaModule
	virtual int		GetObjType( void ) { return WARPFLASH_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00441880 ?GetClassName@WarpFlash@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetClassName( void ) { return "WarpFlash"; }
	// FUNCTION: STBC 0x00441890 ?GetSwigTypeName@WarpFlash@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetSwigTypeName( void ) { return "_p_WarpFlash"; }
	// FUNCTION: STBC 0x004418a0 ?GetPythonClassName@WarpFlash@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetPythonClassName( void ) { return "WarpFlashPtr"; }

	// BaseObjectClass
	virtual void	Update( float time );

	// Declared for the SWIG wrappers that call them.
	static WarpFlash *	Create( BaseObjectClass *ship, float time ) { return new WarpFlash( ship, time ); }	// the SWIG wrapper WarpFlash_Create expands it
	static WarpFlash *	CreateWithoutShip( SetClass *set, TGPoint3 *location, TGMatrix3 *rotation, float radius, float time ) { return new WarpFlash( set, location, rotation, radius, time ); }	// the SWIG wrapper WarpFlash_CreateWithoutShip expands it

	// The saved game's statics (UtopiaModule::ProcessSave and
	// ProcessInternalLoad; invented names).
	static bool		SaveStatics( TGStream *stream );
	static bool		LoadStatics( TGStream *stream );

protected:
	// the constructors' common part: the times, the scaled radius, the
	// flash's node, its place and name in the set and its sound; the
	// rotation is not used (invented name)
	void			Init( SetClass *set, const TGPoint3 *location, const TGMatrix3 *rotation, float radius, float time );
	// a billboard quad of the scaled radius that flips through the
	// "WarpFlashTextures" icons forward, then back, over the time (invented
	// name)
	NiNode *		CreateFlash( float time );

	float			startTime;				// 0x68 the game time at construction (invented name)
	float			endTime;				// 0x6C startTime plus the time (invented name)
	float			unknown70;				// 0x70 the radius times the constant at 0x88BD58

	static int		flashCount;				// the next flash's name number (invented name)
};

/*
================
WarpFlash::IsTypeOf
================
*/
// FUNCTION: STBC 0x00441820 ?IsTypeOf@WarpFlash@@UAE_NH@Z @game/UtopiaModule
inline bool WarpFlash::IsTypeOf( int type ) {
	if ( type == WARPFLASH_TYPE ) {
		return true;
	}
	return ObjectClass::IsTypeOf( type );
}

#endif /* !__WARPFLASH_H__ */
