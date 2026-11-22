/*
===============================================================================

	RepairSubsystemProperty.h

	0x54 bytes: the SWIG wrapper new_RepairSubsystemProperty allocates that much.

	Evidence: the SWIG wrappers RepairSubsystemProperty_* read and write the members inline
	(offsets at the members). The virtual functions are not declared yet.

===============================================================================
*/

#ifndef __REPAIRSUBSYSTEMPROPERTY_H__
#define __REPAIRSUBSYSTEMPROPERTY_H__

#include "PoweredSubsystemProperty.h"

class TGStream;

class TGModelProperty;

// The type RepairSubsystemProperty::Cast (0x68E5D0) checks.
#define REPAIRSUBSYSTEMPROPERTY_TYPE		0x813F

// FUNCTION: STBC 0x0068e800 ??_GRepairSubsystemProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class RepairSubsystemProperty : public PoweredSubsystemProperty {
public:
					RepairSubsystemProperty( long id = 0 );
	virtual			~RepairSubsystemProperty( void );

	// TGObject
	// FUNCTION: STBC 0x0068e680 ?GetObjType@RepairSubsystemProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return REPAIRSUBSYSTEMPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0068e6d0 ?GetClassName@RepairSubsystemProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "RepairSubsystemProperty"; }
	// FUNCTION: STBC 0x0068e6e0 ?GetSwigTypeName@RepairSubsystemProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_RepairSubsystemProperty"; }
	// FUNCTION: STBC 0x0068e6f0 ?GetPythonClassName@RepairSubsystemProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "RepairSubsystemPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual void	VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent );
	virtual bool	Copy( TGModelProperty *property );

	static RepairSubsystemProperty *	Cast( TGModelProperty * );
	static RepairSubsystemProperty *	Create( char * );

	float			GetMaxRepairPoints( void ) { return maxRepairPoints; }
	void			SetMaxRepairPoints( float maxRepairPoints_ ) { maxRepairPoints = maxRepairPoints_; }
	int				GetNumRepairTeams( void ) { return numRepairTeams; }
	void			SetNumRepairTeams( int numRepairTeams_ ) { numRepairTeams = numRepairTeams_; }

protected:
	float			maxRepairPoints;		// 0x4C
	int				numRepairTeams;			// 0x50
};

/*
================
RepairSubsystemProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x0068e690
inline bool RepairSubsystemProperty::IsTypeOf( int type ) {
	if ( type == REPAIRSUBSYSTEMPROPERTY_TYPE ) {
		return true;
	}
	return PoweredSubsystemProperty::IsTypeOf( type );
}

#endif /* !__REPAIRSUBSYSTEMPROPERTY_H__ */
