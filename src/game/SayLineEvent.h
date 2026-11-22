/*
===============================================================================

	SayLineEvent.h

	The event that has a character say a line: the line's name and one flag.
	CT_SAY_LINE_EVENT is its type constant in the SWIG interface.

	0x30 bytes. Evidence: the constructor (0x675550, called from the class
	factory's unit at 0x441E05) calls TGEvent's, stores the vtable 0x894AC4
	and clears the string at 0x28 and the byte at 0x2C; GetObjType returns
	0x8123 (CT_SAY_LINE_EVENT) and IsTypeOf accepts TGEvent's 0x101; the
	class strings are "SayLineEvent", "_p_SayLineEvent" and
	"SayLineEventPtr". Copy (0x675670), WriteToStream (0x675710) and
	ReadFromStream (0x675790) carry the string and the byte. No SWIG
	wrapper names the members; their names are invented.

===============================================================================
*/

#ifndef __SAYLINEEVENT_H__
#define __SAYLINEEVENT_H__

#include "TGEvent.h"

// GetObjType's value for a SayLineEvent (CT_SAY_LINE_EVENT).
#define SAYLINEEVENT_TYPE		0x8123

// FUNCTION: STBC 0x006755f0 ??_GSayLineEvent@@UAEPAXI@Z
class SayLineEvent : public TGEvent {
public:
					SayLineEvent( long id );
					SayLineEvent( void );
	virtual			~SayLineEvent( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEvent
	virtual bool	Copy( TGEvent *event );

	void			SetLine( char *line_ );			// invented name

protected:
	char *			line;					// 0x28 owned (invented name)
	char			unknown2C;				// 0x2C
};

#endif /* !__SAYLINEEVENT_H__ */
