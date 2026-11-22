/*
===============================================================================

	PowerProperty.h

	0x5C bytes: the SWIG wrapper new_PowerProperty allocates that much.

	Evidence: the SWIG wrappers PowerProperty_* read and write the members inline
	(offsets at the members). The virtual functions are not declared yet.

===============================================================================
*/

#ifndef __POWERPROPERTY_H__
#define __POWERPROPERTY_H__

#include "SubsystemProperty.h"

class TGStream;

class TGModelProperty;

// The type PowerProperty::Cast (0x68B290) checks.
#define POWERPROPERTY_TYPE		0x813E

// FUNCTION: STBC 0x0068b4d0 ??_GPowerProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class PowerProperty : public SubsystemProperty {
public:
					PowerProperty( long id = 0 );
	virtual			~PowerProperty( void );

	// TGObject
	// FUNCTION: STBC 0x0068b350 ?GetObjType@PowerProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return POWERPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0068b3a0 ?GetClassName@PowerProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "PowerProperty"; }
	// FUNCTION: STBC 0x0068b3b0 ?GetSwigTypeName@PowerProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_PowerProperty"; }
	// FUNCTION: STBC 0x0068b3c0 ?GetPythonClassName@PowerProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "PowerPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual void	VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent );
	virtual bool	Copy( TGModelProperty *property );

	static PowerProperty *	Cast( TGModelProperty * );
	static PowerProperty *	Create( char * );

	float			GetMainBatteryLimit( void ) { return mainBatteryLimit; }
	void			SetMainBatteryLimit( float mainBatteryLimit_ ) { mainBatteryLimit = mainBatteryLimit_; }
	float			GetBackupBatteryLimit( void ) { return backupBatteryLimit; }
	void			SetBackupBatteryLimit( float backupBatteryLimit_ ) { backupBatteryLimit = backupBatteryLimit_; }
	float			GetMainConduitCapacity( void ) { return mainConduitCapacity; }
	void			SetMainConduitCapacity( float mainConduitCapacity_ ) { mainConduitCapacity = mainConduitCapacity_; }
	float			GetBackupConduitCapacity( void ) { return backupConduitCapacity; }
	void			SetBackupConduitCapacity( float backupConduitCapacity_ ) { backupConduitCapacity = backupConduitCapacity_; }
	float			GetPowerOutput( void ) { return powerOutput; }
	void			SetPowerOutput( float powerOutput_ ) { powerOutput = powerOutput_; }

protected:
	float			mainBatteryLimit;		// 0x48
	float			backupBatteryLimit;		// 0x4C
	float			mainConduitCapacity;	// 0x50
	float			backupConduitCapacity;	// 0x54
	float			powerOutput;			// 0x58
};

/*
================
PowerProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x0068b360
inline bool PowerProperty::IsTypeOf( int type ) {
	if ( type == POWERPROPERTY_TYPE ) {
		return true;
	}
	return SubsystemProperty::IsTypeOf( type );
}

#endif /* !__POWERPROPERTY_H__ */
