/*
===============================================================================

	TorpedoSystemProperty.h

	The members of TorpedoSystemProperty that the SWIG wrappers TorpedoSystemProperty_* call. Only what the
	wrappers prove is declared: no data members yet.

===============================================================================
*/

#ifndef __TORPEDOSYSTEMPROPERTY_H__
#define __TORPEDOSYSTEMPROPERTY_H__

#include "WeaponSystemProperty.h"
#include "TorpedoAmmoType.h"

class TGStream;

class TGModelProperty;

// The type TorpedoSystemProperty::Cast (0x693ED0) checks.
#define TORPEDOSYSTEMPROPERTY_TYPE		0x8133

// FUNCTION: STBC 0x00694170 ??_GTorpedoSystemProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class TorpedoSystemProperty : public WeaponSystemProperty {
public:
					TorpedoSystemProperty( long id = 0 );
	virtual			~TorpedoSystemProperty( void );

	// TGObject
	// FUNCTION: STBC 0x00693fe0 ?GetObjType@TorpedoSystemProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return TORPEDOSYSTEMPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00694040 ?GetClassName@TorpedoSystemProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TorpedoSystemProperty"; }
	// FUNCTION: STBC 0x00694050 ?GetSwigTypeName@TorpedoSystemProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TorpedoSystemProperty"; }
	// FUNCTION: STBC 0x00694060 ?GetPythonClassName@TorpedoSystemProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TorpedoSystemPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual void	VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent );
	virtual bool	Copy( TGModelProperty *property );

	static TorpedoSystemProperty *	Cast( TGModelProperty * );
	static TorpedoSystemProperty *	Create( char * );
	void	SetMaxTorpedoes( int, int );
	int	GetMaxTorpedoes( int );
	void	SetTorpedoScript( int, char * );
	char *	GetTorpedoScript( int );
	TorpedoAmmoType *	GetAmmoType( int index );		// invented name

	int				GetNumAmmoTypes( void ) { return numAmmoTypes; }
	void			SetNumAmmoTypes( int numAmmoTypes_ ) { numAmmoTypes = numAmmoTypes_; }

protected:
	TorpedoAmmoType	ammoTypes[MAX_AMMO_TYPES];	// 0x64
	int				numAmmoTypes;			// 0xA4
};

/*
================
TorpedoSystemProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x00693ff0
inline bool TorpedoSystemProperty::IsTypeOf( int type ) {
	if ( type == TORPEDOSYSTEMPROPERTY_TYPE ) {
		return true;
	}
	return WeaponSystemProperty::IsTypeOf( type );
}

#endif /* !__TORPEDOSYSTEMPROPERTY_H__ */
