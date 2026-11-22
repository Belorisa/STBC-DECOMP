/*
===============================================================================

	DamageIcon.h

	One subsystem's icon in a DamageDisplay: coloured by the subsystem's
	condition. A damaged subsystem blinks, once a second, until a six second
	timer expires.

	0x64 bytes (the scalar deleting destructor frees through the pool of that
	size). The vtable is 0x891A28; it overrides TGIcon's destructor,
	GetObjType, IsTypeOf, WriteToStream, ReadFromStream, FixUpPointers,
	PostFixUp, the three class-name getters and GetClassHandlers.

===============================================================================
*/

#ifndef __DAMAGEICON_H__
#define __DAMAGEICON_H__

#include "TGIcon.h"

class TGEvent;
class TGObject;
class TGStream;
class ShipSubsystem;

// GetObjType's value for a DamageIcon (the type its Cast checks).
#define DAMAGEICON_TYPE		0x8101

// The events the icon's timers send it (invented names); "DamageIcon::HandleExpire" and "DamageIcon::HandleBlink" handle them.
#define ET_DAMAGE_ICON_EXPIRE	0x8000B2
#define ET_DAMAGE_ICON_BLINK	0x8000B3

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00540700 ??_GDamageIcon@@UAEPAXI@Z
class DamageIcon : public TGIcon {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558a30 ??0DamageIcon@@QAE@J@Z @game/InterfaceModule
					DamageIcon( long id ) : TGIcon( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					DamageIcon( char *groupName, int iconNum, ShipSubsystem *subsystem, NiColorA *color );
	virtual			~DamageIcon( void );

	// TGObject
	// FUNCTION: STBC 0x00540650 ?GetObjType@DamageIcon@@UAEHXZ
	virtual int		GetObjType( void ) { return DAMAGEICON_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x005406c0 ?GetClassName@DamageIcon@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "DamageIcon"; }
	// FUNCTION: STBC 0x005406d0 ?GetSwigTypeName@DamageIcon@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_DamageIcon"; }
	// FUNCTION: STBC 0x005406e0 ?GetPythonClassName@DamageIcon@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "DamageIconPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x005406f0 ?GetClassHandlers@DamageIcon@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	ShipSubsystem *	GetSubsystem( void ) { return subsystem; }
	static DamageIcon *	Cast( TGObject * );
	void	ResetPosition( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void	HandleExpire( TGEvent *event );				// handler "DamageIcon::HandleExpire"
	void	HandleBlink( TGEvent *event );				// handler "DamageIcon::HandleBlink"
	void	UpdateColor( bool startTimers );			// invented name

protected:
	void	StopBlinking( void );						// invented name
	void	StopExpireTimer( void );					// invented name

	static TGEventHandlerTable	classHandlers;			// invented name
	ShipSubsystem *	subsystem;				// 0x50 (DamageIcon_GetSubsystem)
	long			blinkTimerID;			// 0x54 the timer that sends ET_DAMAGE_ICON_BLINK, 0 for none (invented name)
	long			expireTimerID;			// 0x58 the timer that sends ET_DAMAGE_ICON_EXPIRE, 0 for none (invented name)
	long			expireEventID;			// 0x5C that timer's event (invented name)
	bool			blinking;				// 0x60 the blink timer runs (invented name)
};

/*
================
DamageIcon::IsTypeOf
================
*/
// FUNCTION: STBC 0x00540660 ?IsTypeOf@DamageIcon@@UAE_NH@Z
inline bool DamageIcon::IsTypeOf( int type ) {
	if ( type == DAMAGEICON_TYPE ) {
		return true;
	}
	return TGIcon::IsTypeOf( type );
}

#endif /* !__DAMAGEICON_H__ */
