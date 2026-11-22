/*
===============================================================================

	EngineProperty.h

	0x4C bytes: the SWIG wrapper new_EngineProperty allocates that much.

	Evidence: the SWIG wrappers EngineProperty_* read and write the members inline
	(offsets at the members). The virtual functions are not declared yet.

===============================================================================
*/

#ifndef __ENGINEPROPERTY_H__
#define __ENGINEPROPERTY_H__

#include "SubsystemProperty.h"

class TGStream;

class TGModelProperty;

// The type EngineProperty::Cast (0x6821C0) checks.
#define ENGINEPROPERTY_TYPE		0x813D

// FUNCTION: STBC 0x006823e0 ??_GEngineProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class EngineProperty : public SubsystemProperty {
public:
					EngineProperty( long id = 0 );
	virtual			~EngineProperty( void );

	// TGObject
	// FUNCTION: STBC 0x00682260 ?GetObjType@EngineProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return ENGINEPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x006822b0 ?GetClassName@EngineProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "EngineProperty"; }
	// FUNCTION: STBC 0x006822c0 ?GetSwigTypeName@EngineProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_EngineProperty"; }
	// FUNCTION: STBC 0x006822d0 ?GetPythonClassName@EngineProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "EnginePropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual void	VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent );
	virtual bool	Copy( TGModelProperty *property );

	static EngineProperty *	Cast( TGModelProperty * );
	static EngineProperty *	Create( char * );

	// Which engine the property describes (the SWIG constants
	// EngineProperty.EP_*, in value order).
	enum engineType_t {
		EP_IMPULSE,
		EP_WARP
	};

	int				GetEngineType( void ) { return engineType; }
	void			SetEngineType( int engineType_ ) { engineType = engineType_; }

protected:
	int				engineType;				// 0x48 an engineType_t
};

/*
================
EngineProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x00682270
inline bool EngineProperty::IsTypeOf( int type ) {
	if ( type == ENGINEPROPERTY_TYPE ) {
		return true;
	}
	return SubsystemProperty::IsTypeOf( type );
}

#endif /* !__ENGINEPROPERTY_H__ */
