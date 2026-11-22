/*
===============================================================================

	SensorProperty.h

	0x54 bytes: the SWIG wrapper new_SensorProperty allocates that much.

	Evidence: the SWIG wrappers SensorProperty_* read and write the members inline
	(offsets at the members). The virtual functions are not declared yet.

===============================================================================
*/

#ifndef __SENSORPROPERTY_H__
#define __SENSORPROPERTY_H__

#include "PoweredSubsystemProperty.h"

class TGStream;

class TGModelProperty;

// The type SensorProperty::Cast (0x68F360) checks.
#define SENSORPROPERTY_TYPE		0x8139

// FUNCTION: STBC 0x0068f590 ??_GSensorProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class SensorProperty : public PoweredSubsystemProperty {
public:
					SensorProperty( long id = 0 );
	virtual			~SensorProperty( void );

	// TGObject
	// FUNCTION: STBC 0x0068f410 ?GetObjType@SensorProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return SENSORPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0068f460 ?GetClassName@SensorProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "SensorProperty"; }
	// FUNCTION: STBC 0x0068f470 ?GetSwigTypeName@SensorProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_SensorProperty"; }
	// FUNCTION: STBC 0x0068f480 ?GetPythonClassName@SensorProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "SensorPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual void	VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent );
	virtual bool	Copy( TGModelProperty *property );

	static SensorProperty *	Cast( TGModelProperty * );
	static SensorProperty *	Create( char * );

	float			GetBaseSensorRange( void ) { return baseSensorRange; }
	void			SetBaseSensorRange( float baseSensorRange_ ) { baseSensorRange = baseSensorRange_; }
	int				GetMaxProbes( void ) { return maxProbes; }
	void			SetMaxProbes( int maxProbes_ ) { maxProbes = maxProbes_; }

protected:
	float			baseSensorRange;		// 0x4C
	int				maxProbes;				// 0x50
};

/*
================
SensorProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x0068f420
inline bool SensorProperty::IsTypeOf( int type ) {
	if ( type == SENSORPROPERTY_TYPE ) {
		return true;
	}
	return PoweredSubsystemProperty::IsTypeOf( type );
}

#endif /* !__SENSORPROPERTY_H__ */
